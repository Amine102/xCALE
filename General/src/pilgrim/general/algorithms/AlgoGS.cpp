#include "pilgrim/general/algorithms/AlgoGS.h"
#include <vector>
#include <string>
#include <plDebug.h>
#include <sstream>

using namespace std;
using namespace boost;
using namespace PILGRIM;

template <typename rowDataType>
int pmAlgoGS<rowDataType>::totalNbrs = 0;
//=============================================================================
template <typename rowDataType>
pmAlgoGS<rowDataType>::pmAlgoGS(pmCSVDataSet *ds,
                                pmDecomposableScore<rowDataType> *pScore,
                                pmCache *cache,
                                bool debugVerbose) : pmAlgorithm<rowDataType>(ds)
{
  this->initialBN = new pmBayesianNetwork(this->variables); // can be changed by setters later
  this->setScore(pScore);
  this->pScore->cache = cache;
  this->setDebugVerbose(debugVerbose);
  this->graphCPC = NULL; // can be changed by setters later
}

//=============================================================================
template <typename rowDataType>
pmAlgoGS<rowDataType>::~pmAlgoGS()
{
}

//=============================================================================
struct pmGraphOp
{
  pmNode nodeA;
  pmNode nodeB;
  enum op_type
  {
    SL_EDGE_INVALID = 0,
    SL_EDGE_ADD,
    SL_EDGE_REM,
    SL_EDGE_REV
  } op;

  pmGraphOp(pmNode nodeA_, pmNode nodeB_, op_type op_)
      : nodeA(nodeA_), nodeB(nodeB_), op(op_) {}

  pmGraphOp() : nodeA(), nodeB(), op(SL_EDGE_INVALID) {}

  void apply(pmGraph &graph) const
  {
    // added by Yasin
    pmEdge e;
    bool edgeAdded;
    property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);

    switch (op)
    {
    case SL_EDGE_ADD:
      boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
      put(weightMap, e, 2); // added by YASIN
      break;
    case SL_EDGE_REV:
      remove_edge(nodeA, nodeB, graph);
      boost::tie(e, edgeAdded) = add_edge(nodeB, nodeA, graph);
      put(weightMap, e, 2); // added by YASIN
      break;
    case SL_EDGE_REM:
      remove_edge(nodeA, nodeB, graph);
      break;
    default:
      throw std::runtime_error("invalid operation in Glutton Search");
    }
  }

  void unapply(pmGraph &graph) const
  {
    // added by Yasin
    pmEdge e;
    bool edgeAdded;
    property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);

    switch (op)
    {
    case SL_EDGE_ADD:
      remove_edge(nodeA, nodeB, graph);
      break;
    case SL_EDGE_REV:
      remove_edge(nodeB, nodeA, graph);
      boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
      put(weightMap, e, 2); // added by YASIN
      break;
    case SL_EDGE_REM:
      boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
      put(weightMap, e, 2); // added by YASIN
      break;
    default:
      throw std::runtime_error("invalid operation in Glutton Search");
    }
  }

  ostream &print(ostream &out, const pmGraph &g) const
  {
    switch (op)
    {
    case SL_EDGE_ADD:
      out << "ADD";
      break;
    case SL_EDGE_REM:
      out << "REM";
      break;
    case SL_EDGE_REV:
      out << "REV";
      break;
    default:
      throw std::runtime_error("invalid operation in Glutton Search");
    }
    out << "(" << get(vertex_index, g)[nodeA]
        << "->" << get(vertex_index, g)[nodeB]
        << ")";
    return out;
  }
};

//=============================================================================
template <typename rowDataType>
void pmAlgoGS<rowDataType>::run(pmBayesianNetwork *computedBN)
{
  /* Initial bayesian network */
  pmBayesianNetwork *initBN = getInitialBN();
  computedBN->copy_structure(*initBN);
  
  pmGraph &graph = computedBN->get_graph();

  /* Configuring the used score */
  this->pScore->setBayesianNetwork(computedBN);
  this->pScore->setFrequencyCounter(this->getFrequencyCounter());

  vector<pmGraphOp> neighbourhood;

  epsilon = 0;
  this->oldBestDeltaScore = 0;
  this->freqCounterForBestDeltaScore = 0;
  pmScoreValueType bestDeltaScore = epsilon;

  /* Loop continuing until no better optimum is found by
     adding, reversing or removing an edge at a time */
  do
  {
    /* Initializing the best delta score */
    /* if 50 times same delta score occurred then consider it as an error and re-adjust epsilon value otherwise it will continue unlimited times. */
    if (this->freqCounterForBestDeltaScore == 50)
      epsilon = bestDeltaScore;

    bestDeltaScore = epsilon;

#ifdef DEBUG
    std::cerr << "NEXT ITERATION......" << std::endl;
#endif
    /* List containing the DAG neighbors */
    generateDAGNeighbours(graph, neighbourhood);
    /* Variables used to keep information about the operation to make */
    pmGraphOp operationHeld;

    totalNbrs += neighbourhood.size();
#ifdef DEBUG
    for (size_t i = 0; i < neighbourhood.size(); ++i)
    {
      std::cerr << i << ": ";
      neighbourhood[i].print(cerr, graph);
      std::cerr << std::endl;
    }
#endif

    /* Reading the content of the list */
    for (size_t i = 0; i < neighbourhood.size(); i++)
    {
      /* Variables representing the operation studied */
      pmGraphOp &currentOperation = neighbourhood[i];
      pmScoreValueType deltaScore = 0;
      /* Temporary scores needed to compute the delta score */
      pmScoreValueType score1, score2, score3, score4;

      /* Processing depending on the type of operation */
      /* Evaluating by adding an edge */
      switch (currentOperation.op)
      {
      case pmGraphOp::SL_EDGE_ADD:
        score2 = this->pScore->computeNode(currentOperation.nodeB);
        currentOperation.apply(graph);
        score1 = this->pScore->computeNode(currentOperation.nodeB);
        currentOperation.unapply(graph);
        /* Computation of the delta score */
        deltaScore = score1 - score2;
        break;

      /* Evaluating by reversing an edge */
      case pmGraphOp::SL_EDGE_REV:
        score2 = this->pScore->computeNode(currentOperation.nodeB);
        score4 = this->pScore->computeNode(currentOperation.nodeA);

        remove_edge(currentOperation.nodeA, currentOperation.nodeB, graph);
        score1 = this->pScore->computeNode(currentOperation.nodeB);
        add_edge(currentOperation.nodeB, currentOperation.nodeA, graph);
        score3 = this->pScore->computeNode(currentOperation.nodeA);

        currentOperation.unapply(graph);
        /* Computation of the delta score */
        deltaScore = score1 - score2 + score3 - score4;
        break;

      /* Evaluating by removing an edge */
      case pmGraphOp::SL_EDGE_REM:
        score2 = this->pScore->computeNode(currentOperation.nodeB);
        currentOperation.apply(graph);
        score1 = this->pScore->computeNode(currentOperation.nodeB);
        currentOperation.unapply(graph);
        /* Computation of the delta score */
        deltaScore = score1 - score2;
        break;

      default:
        throw std::runtime_error("invalid operation in Glutton Search");
      } // end switch op
      /* An operation represents a gain in the score computation */
      if (deltaScore > bestDeltaScore)
      {
        /* Updating the best delta score */
        bestDeltaScore = deltaScore;
        operationHeld = currentOperation;
      }
    } /* End of the for loop */

    if (bestDeltaScore != epsilon)
    {
#ifdef DEBUG
      std::cerr << "\n BEFORE OPERATION BestDelta Score: " << bestDeltaScore << " ";
      operationHeld.print(cerr, graph);
#endif

      /* Adding an edge */
      operationHeld.apply(graph);
    }

    // for error detection
    this->freqCounterForBestDeltaScore += (this->oldBestDeltaScore == bestDeltaScore ? 1 : 0);
    this->oldBestDeltaScore = bestDeltaScore; // record bestDelta value at each iteration.

  } while (bestDeltaScore != epsilon);

#ifdef DEBUG
  /* Printing the computed bayesian network */
  print_graph(graph);
#endif
  /* Returning the computed bayesian network found by the GS algorithm */
  // cout << "computedBN printGraph" << endl;
  // computedBN->print_graph();
  // cout << "===============================" << endl;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoGS<rowDataType>::generateDAGNeighbours(pmGraph &graph,
                                                  std::vector<pmGraphOp> &operations)
{
  ////////// added for MMHC algorithm /////////////////////////
  /* For learning using CPC graph (MMHC algorithm) */
  bool learnUsingCPCgraph = false;

  if (graphCPC != NULL)
  {
    learnUsingCPCgraph = true;
  }
  /////////////////////////////////////////////////////////////

  /* Number of nodes within the initial bayesian network */
  int numberOfNodes = num_vertices(graph);
  /* Variables representing two nodes into the following loops */

  /* start with an empty operation list */
  operations.clear();

  /* Scrolling through the nodes */
  for (int i = 0; i < numberOfNodes - 1; i++)
  {
    pmNode nodeA = vertex(i, graph);

    /* Scrolling through the remaining nodes */
    for (int j = i + 1; j < numberOfNodes; j++)
    {
      pmNode nodeB = vertex(j, graph);
      pmEdge edgeA, edgeB;
      bool testA, testB, CPCtestA = true, CPCtestB = true;

      /* Variables association to desired edges */
      boost::tie(edgeA, testA) = edge(nodeA, nodeB, graph);
      boost::tie(edgeB, testB) = edge(nodeB, nodeA, graph);

      ////////// added for MMHC algorithm /////////////////////////

      if (learnUsingCPCgraph)
      {
        pmEdge CPCedgeA, CPCedgeB;
        /* Variables association to desired edges in CPC graph*/

        boost::tie(CPCedgeA, CPCtestA) = edge(vertex(i, *graphCPC), vertex(j, *graphCPC), *graphCPC);
        boost::tie(CPCedgeB, CPCtestB) = edge(vertex(j, *graphCPC), vertex(i, *graphCPC), *graphCPC);
      }
      ///////////////////////////////////////////////////////////////

      /* Testing adding the current edge in the two possible ways */
      if ((testA == false) && (testB == false))
      {
        /* Testing if edge exist in CPC graph. Default value is true*/
        if ((CPCtestA == true) || (CPCtestB == true))
        { // by default it will be true

          /* First way */
          pmGraphOp op(nodeA, nodeB, pmGraphOp::SL_EDGE_ADD);
          if (remainsADAG(graph, op))
            operations.push_back(op);

          /* Second way */
          op = pmGraphOp(nodeB, nodeA, pmGraphOp::SL_EDGE_ADD);
          if (remainsADAG(graph, op))
            operations.push_back(op);
        }
        /* Testing reversing and removing the current edge in the way nodeA->nodeB */
      }
      else if (testA == true)
      {
        /* Reversing */
        pmGraphOp op(nodeA, nodeB, pmGraphOp::SL_EDGE_REV);
        if (remainsADAG(graph, op))
          operations.push_back(op);

        /* Removing */
        op = pmGraphOp(nodeA, nodeB, pmGraphOp::SL_EDGE_REM);
        operations.push_back(op);
        /* Testing reversing and removing the current edge in the way nodeB->nodeA */
      }
      else if (testB == true)
      {
        /* Reversing */
        pmGraphOp op(nodeB, nodeA, pmGraphOp::SL_EDGE_REV);
        if (remainsADAG(graph, op))
          operations.push_back(op);

        /* Removing */
        op = pmGraphOp(nodeB, nodeA, pmGraphOp::SL_EDGE_REM);
        operations.push_back(op);
      }
      /* End of the first for loop */
    }
    /* End of the second for loop */
  }
  /* Creating the list of available operations */
  /* Returning the list containing the DAG neighbours */
}

//=============================================================================
template <typename rowDataType>
bool pmAlgoGS<rowDataType>::getDebugVerbose()
{
  return this->debugVerbose;
};

//=============================================================================
template <typename rowDataType>
void pmAlgoGS<rowDataType>::setDebugVerbose(bool debugVerbose)
{
  this->debugVerbose = debugVerbose;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoGS<rowDataType>::setScore(pmDecomposableScore<rowDataType> *pScore)
{
  /* Setting a particular score */
  this->pScore = pScore;
  this->setEvaluatingScore(pScore);
}

//=============================================================================
template <typename rowDataType>
void pmAlgoGS<rowDataType>::setInitialNetwork(pmBayesianNetwork *initialBN)
{
  /* Setting a particular initial BN*/
  this->initialBN = initialBN;
}
//=============================================================================
template <typename rowDataType>
void pmAlgoGS<rowDataType>::setCPCGraph(pmGraph *graphCPC)
{
  /* Setting a CPC Graph */
  this->graphCPC = graphCPC;
}

//=============================================================================
template <typename rowDataType>
pmDecomposableScore<rowDataType> *pmAlgoGS<rowDataType>::getScore()
{
  /* Getting the used score */
  return this->pScore;
}

//=============================================================================
template <typename rowDataType>
pmBayesianNetwork *pmAlgoGS<rowDataType>::getInitialBN()
{
  return this->initialBN;
}

//=============================================================================
template <typename rowDataType>
bool pmAlgoGS<rowDataType>::remainsADAG(const pmNode &nodeInit,
                                        const pmNode &node,
                                        const pmGraph &graph)
{
  graph_traits<pmGraph>::adjacency_iterator vi, vi_end;
  boost::tie(vi, vi_end) = adjacent_vertices(node, graph);
  for (; vi != vi_end; ++vi)
  {
    if ((*vi == nodeInit) || (!remainsADAG(nodeInit, *vi, graph)))
      return false;
  }

  return true;
}

//=============================================================================
template <typename rowDataType>
bool pmAlgoGS<rowDataType>::remainsADAG(pmGraph &graph,
                                        const pmGraphOp &op)
{
  bool result = true;
  switch (op.op)
  {
  case pmGraphOp::SL_EDGE_ADD:
    add_edge(op.nodeA, op.nodeB, graph);
    result = remainsADAG(op.nodeA, op.nodeB, graph);
    remove_edge(op.nodeA, op.nodeB, graph);
    break;

  case pmGraphOp::SL_EDGE_REM:
    break;

  case pmGraphOp::SL_EDGE_REV:
    remove_edge(op.nodeA, op.nodeB, graph);
    add_edge(op.nodeB, op.nodeA, graph);
    result = remainsADAG(op.nodeB, op.nodeA, graph);
    remove_edge(op.nodeB, op.nodeA, graph);
    add_edge(op.nodeA, op.nodeB, graph);
    break;

  default:
    throw std::runtime_error("invalid operation in Glutton Search");
  }

  return result;
}

namespace PILGRIM {
    template class pmAlgoGS<std::vector<unsigned int>>;
    template class pmAlgoGS<std::vector<int>>;
    template class pmAlgoGS<std::vector<float>>;
    template class pmAlgoGS<std::vector<double>>;
    template class pmAlgoGS<std::vector<long double>>;

    template class pmAlgoGS<unsigned int*>;
    template class pmAlgoGS<int*>;
    template class pmAlgoGS<float*>;
    template class pmAlgoGS<double*>;
    template class pmAlgoGS<long double*>;
}; // namespace PILGRIM
