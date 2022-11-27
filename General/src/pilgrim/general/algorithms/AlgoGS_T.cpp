#include "pilgrim/general/algorithms/AlgoGS_T.h"
#include <plDebug.h>


using namespace std;
using namespace boost;
using namespace PILGRIM;

template <typename rowDataType>
int pmAlgoGS_T<rowDataType>::totalNbrs = 0;

/*
=====CONSTRUCTOR=====
*/
//=============================================================================
//NEW CONSTRUCTOR
template <typename rowDataType>
pmAlgoGS_T<rowDataType>::pmAlgoGS_T(pmCSVDataSet* ds, 
								                    pmDecomposableScore<rowDataType>* pScore, 
								                    pmCache* cache, 
								                    bool debugVerbose
                                    ) : pmAlgorithm_T<rowDataType>(ds) {
  this->transitionBN = new pmBayesianNetwork(this->variables);
  this->setScore(pScore);
  this->pScore->cache = cache;
  this->setDebugVerbose(debugVerbose);
  this->graphCPC = NULL;
}

/*
=====DESTRUCTOR=====
*/
//=============================================================================
template <typename rowDataType>
pmAlgoGS_T<rowDataType>::~pmAlgoGS_T() {
}

/*
=====GETTERS=====
*/
//=============================================================================
template <typename rowDataType>
pmDecomposableScore<rowDataType> *pmAlgoGS_T<rowDataType>::getScore() {
  /* Getting the used score */
  return this->pScore;
}

//=============================================================================
template <typename rowDataType>
pmBayesianNetwork* pmAlgoGS_T<rowDataType>::getTransitionBN() {
  return transitionBN;
}

//=============================================================================
template <typename rowDataType>
bool pmAlgoGS_T<rowDataType>::
getDebugVerbose() {
  return this->debugVerbose;
}

/*
=====SETTERS=====
*/
//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::setScore(pmDecomposableScore<rowDataType>* pScore) {
  /* Setting a particular score */
  this->pScore = pScore;
  this->setEvaluatingScore(pScore);
}

//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::setTransitionNetwork(pmBayesianNetwork* transitionBN) {
  this->transitionBN = transitionBN;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::setCPCGraph(pmGraph* graphCPC) {
  this->graphCPC = graphCPC;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::
setDebugVerbose(bool debugVerbose) {
  this->debugVerbose = debugVerbose;
}
/*
=====PROTECTED METHODS AND ATTRIBUTES=====
*/
//=============================================================================
struct pmGraphOp {
  pmNode nodeA;
  pmNode nodeB;
  enum op_type { SL_EDGE_INVALID = 0, SL_EDGE_ADD, SL_EDGE_REM, SL_EDGE_REV } op;

  pmGraphOp(pmNode nodeA_, pmNode nodeB_, op_type op_)
    : nodeA(nodeA_), nodeB(nodeB_), op(op_) {}

  pmGraphOp() : nodeA(), nodeB(), op(SL_EDGE_INVALID) {}

  void apply(pmGraph& graph) const {
    // added by Yasin
    pmEdge e;
    bool edgeAdded;
    property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);
    //
    switch (op) {
      case SL_EDGE_ADD:
        boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
        put(weightMap, e, 2);  // added by YASIN
        break;
      case SL_EDGE_REV:
        remove_edge(nodeA, nodeB, graph);
        boost::tie(e, edgeAdded) = add_edge(nodeB, nodeA, graph);
        put(weightMap, e, 2);  // added by YASIN
        break;
      case SL_EDGE_REM:
        remove_edge(nodeA, nodeB, graph);
        break;
      default:
        throw std::runtime_error("invalid operation in Glutton Search");
    }
  }

  void unapply(pmGraph& graph) const {
    // added by Yasin
    pmEdge e;
    bool edgeAdded;
    property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);
    //
    switch (op) {
      case SL_EDGE_ADD:
        remove_edge(nodeA, nodeB, graph);
        break;
      case SL_EDGE_REV:
        remove_edge(nodeB, nodeA, graph);
        boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
        put(weightMap, e, 2);   // added by YASIN
        break;
      case SL_EDGE_REM:
        boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
        put(weightMap, e, 2);   // added by YASIN
        break;
      default:
        throw std::runtime_error("invalid operation in Glutton Search");
    }
  }

  ostream& print(ostream& out, const pmGraph& g) const {
    switch (op) {
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
bool pmAlgoGS_T<rowDataType>::remainsADAG(const pmNode& nodeInit,
                                        const pmNode& node,
                                        const pmGraph& graph) {
  graph_traits<pmGraph>::adjacency_iterator vi, vi_end;
  boost::tie(vi, vi_end) = adjacent_vertices(node, graph);
  for (; vi != vi_end; ++vi) {
    if ((*vi == nodeInit) || (!remainsADAG(nodeInit, *vi, graph)))
      return false;
  }

  return true;
}

//=============================================================================
template <typename rowDataType>
bool pmAlgoGS_T<rowDataType>::remainsADAG(pmGraph& graph,
                                        const pmGraphOp& op) {
  bool result = true;
  switch (op.op) {
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

//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::generateDAG2TBNNeighbours(pmGraph& graph,
                                           std::vector<pmGraphOp>& operations,int time_slices) {
#ifdef DEBUG
  plTracer tracer("generateDAG2TBNNeighbours");
#endif

  /* Number of nodes within the initial bayesian network */
  int numberOfNodes = num_vertices(graph);
  /* Variables representing two nodes into the following loops */
  vector<pmNode> leftVars;
  vector<pmNode> rightVars;
  for (int i = 0; i < numberOfNodes; i++){
	  pmNode nodeX= vertex(i, graph);
	  if (0 == this->pScore->getBayesianNetwork()->getVariable(nodeX).name().compare(this->pScore->getBayesianNetwork()->getVariable(nodeX).name().length() - 2, 2, "_t")){
		  rightVars.push_back(nodeX);
	  }
	  else{ leftVars.push_back(nodeX); }
  }
  /* start with an empty operation list */
  operations.clear();

  // In this part we trait the dependecy between nodes in the same time slice t
  /* Scrolling through the nodes in time slice t*/
  for (int i = numberOfNodes/ 2; i < numberOfNodes - 1; i++) {
	  pmNode nodeA = vertex(i, graph);

    /* Scrolling through the remaining nodes in the t time slice*/
	  for (int j = i + 1; j < numberOfNodes; j++) {
		  pmNode nodeB = vertex(j, graph);
      pmEdge edgeA, edgeB;
      bool testA, testB;

      /* Variables association to desired edges */
      boost::tie(edgeA, testA) = edge(nodeA, nodeB, graph);
      boost::tie(edgeB, testB) = edge(nodeB, nodeA, graph);

      /* Testing adding the current edge in the two possible ways in the same t time slice */
      if ((testA == false) && (testB == false)) {
        if (graphCPC != NULL) {  // for DMMHC algorithm
          this->addingAnEdgeUsingMMHCconstraint(graph, operations, nodeA, nodeB, i, j);
        } else {
          this->addingAnEdge(graph, operations, nodeA, nodeB);
        }
      /* Testing reversing and removing the current edge in the way nodeA->nodeB
         in the same t time slice */
      } else if (testA == true) {
        this->reversingAnEdge(graph, operations, nodeA, nodeB);
      /* Testing reversing and removing the current edge in the way nodeB->nodeA
         in the same t time slice*/
      } else if (testB == true) {
        this->reversingAnEdge(graph, operations, nodeA, nodeB);
      }
      /* End of the first for loop */
    }
    /* End of the second for loop */
  }
/* In this part we trait the dependency between nodes in two different 
   time slices t-1 and t */

  /* Scrolling through the nodes in the t-1 time slice */
  for (int i = 0; i < numberOfNodes / 2; i++) {
	  pmNode nodeA = vertex(i, graph);

    /* Scrolling through the all nodes in the t time slice*/
	  for (int j = numberOfNodes / 2; j < numberOfNodes; j++) {
		  pmNode nodeB = vertex(j, graph);
      pmEdge edgeA;
      bool testA = false;

      /* Variables association to desired edges */
      boost::tie(edgeA, testA) = edge(nodeA, nodeB, graph);

      // Testing adding the current temporal edge in the one possible way
      // nodeA-->nodeB where nodeA in t-1 time slice and nodeB in t time slice
      if ((testA == false)) {
        if (graphCPC != NULL) { // for DMMHC algorithm
          this->addingATemporalEdgeUsingDMMHCconstraint(graph, operations, nodeA, nodeB, i, j);
        } else {
         this->addingATemporalEdge(graph, operations, nodeA, nodeB);
        }
      // Testing only removing the current temporal edge in the way nodeA->nodeB
      } else if (testA == true) {
        this->removingATemporalEdge(graph, operations, nodeA, nodeB);
      }
      /* End of the first for loop */
    }
    /* End of the second for loop */
  }
  /* Creating the list of available operations */
  /* Returning the list containing the DAG of G-> neighbors */
}
//=============================================================================

template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::addingAnEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB) {
  /* Testing adding the current edge in the two possible ways */
  /* First way */
  pmGraphOp op(nodeA, nodeB, pmGraphOp::SL_EDGE_ADD);
  if (remainsADAG(graph, op))
    operations.push_back(op);

  /* Second way */
  op = pmGraphOp(nodeB, nodeA, pmGraphOp::SL_EDGE_ADD);
  if (remainsADAG(graph, op))
    operations.push_back(op);
}
//=============================================================================

template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::addingATemporalEdge(pmGraph& graph, std::vector<pmGraphOp>& operations,pmNode& nodeA, pmNode& nodeB) {
  /* Testing adding the current edge in the one possible way nodeA-->nodeB
     where nodeA in t-1 time slice and nodeB in t time slice  */
  /* First way */
  pmGraphOp op(nodeA, nodeB, pmGraphOp::SL_EDGE_ADD);
  if (remainsADAG(graph, op))
    operations.push_back(op);
}
//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::addingAnEdgeUsingMMHCconstraint(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB, int i, int j) {
  bool CPCtestA = true, CPCtestB = true;
  pmEdge CPCedgeA, CPCedgeB;

  /* Variables association to desired edges in CPC graph*/
  boost::tie(CPCedgeA, CPCtestA) = edge(vertex(i, *graphCPC), vertex(j, *graphCPC), *graphCPC);
  boost::tie(CPCedgeB, CPCtestB) = edge(vertex(j, *graphCPC), vertex(i, *graphCPC), *graphCPC);

  if ((CPCtestA == true) || (CPCtestB == true))  // by default it will be true
    this->addingAnEdge(graph, operations, nodeA, nodeB);
}
//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::addingATemporalEdgeUsingDMMHCconstraint(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB, int i, int j) {
  bool CPCtestA = true;
  pmEdge CPCedgeA;

  /* Variables association to desired edges in CPC, CP and CC of G-> graph*/
  boost::tie(CPCedgeA, CPCtestA) = edge(vertex(i, *graphCPC), vertex(j, *graphCPC), *graphCPC);

  if (CPCtestA == true)  // by default it will be true
    this->addingATemporalEdge(graph, operations, nodeA, nodeB);
}
//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::reversingAnEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB) {
  /* Testing reversing and removing the current edge in the way nodeA->nodeB */

  /* Reversing */
  pmGraphOp op(nodeA, nodeB, pmGraphOp::SL_EDGE_REV);
  if (remainsADAG(graph, op))
    operations.push_back(op);

  /* Removing */
  op = pmGraphOp(nodeA, nodeB, pmGraphOp::SL_EDGE_REM);
  operations.push_back(op);
}
//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::removingAnEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB) {
  /* Testing reversing and removing the current edge in the way nodeB->nodeA */
  /* Reversing */
  pmGraphOp op(nodeB, nodeA, pmGraphOp::SL_EDGE_REV);
  if (remainsADAG(graph, op))
    operations.push_back(op);

  /* Removing */
  op = pmGraphOp(nodeB, nodeA, pmGraphOp::SL_EDGE_REM);
  operations.push_back(op);
}
//=============================================================================
template <typename rowDataType>
void pmAlgoGS_T<rowDataType>::removingATemporalEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB) {
  /* Testing removing the current temporal edge in the way nodeA->nodeB */
  /* Removing */
  pmGraphOp op = pmGraphOp(nodeA, nodeB, pmGraphOp::SL_EDGE_REM);
  operations.push_back(op);
}

/*
=====METHOD=====
*/
//=============================================================================
template <typename rowDataType>
pmBayesianNetwork* pmAlgoGS_T<rowDataType>::run(pmBayesianNetwork* transBN) {
  /* Initial bayesian network */
  //TransBN should be "this->transBN"
  pmBayesianNetwork* computedBN = transBN;
  pmGraph& graph = computedBN->get_graph();

  /* Configuring the used score */
  this->pScore->setBayesianNetwork(computedBN);
  this->pScore->setFrequencyCounter(this->pFrequencyCounter);

  vector<pmGraphOp> neighbourhood;

  epsilon = 0;
  this->oldBestDeltaScore = 0;
  this->freqCounterForBestDeltaScore = 0;
  pmScoreValueType bestDeltaScore = epsilon;

  /* Loop continuing until no better optimum is found by
     adding, reversing or removing an edge at a time */
  do {
    /* Initializing the best delta score */

    /* if 50 times same delta score occurred then consider it as error and 
       readjust epsilon value otherwise it will continue unlimited times. */
    if (this->freqCounterForBestDeltaScore == 50)
      epsilon = bestDeltaScore;

    bestDeltaScore = epsilon;

#ifdef DEBUG
    std::cerr << "NEXT ITERATION......" << std::endl;
#endif
/****************************used for algorithm GS*****************************/
    /* List containing the DAG neighbours */
     //generateDAGNeighbours1(graph, neighbourhood);
/******************************************************************************/

/***************************used for algorithm Dynamic GS**********************/

  /* List containing the DAG neighbours with Candidate Parents children CPC */
  //  generateDAGNeighbours(graph, neighbourhood);
  // used for algorithm MMHC and for G0 (initial model) in algorithm DMMHC
  // generateDAGNeighbourstest(graph, neighbourhood);

  /* List containing the DAG neighbours with Candidate CPC (Parents and
     children), CP (Parents) and CC (children) */
  // used for G-> (transition model) in DMMHC algorithm
    generateDAG2TBNNeighbours(graph, neighbourhood,2);

/******************************************************************************/
#ifdef DEBUG
    cout << "nbre des neighbourhood  " << neighbourhood.size() << endl;
#endif
    /* Variables used to keep information about the operation to make */
    pmGraphOp operationHeld;

    totalNbrs += neighbourhood.size();
#ifdef DEBUG
    for (size_t i = 0; i < neighbourhood.size(); ++i) {
      std::cerr << i << ": ";
      neighbourhood[i].print(cerr, graph);
      std::cerr << std::endl;
    }
#endif

    /* Reading the content of the list */
    for (size_t i = 0; i < neighbourhood.size(); i++) {
      /* Variables representing the operation studied */
      pmGraphOp& currentOperation = neighbourhood[i];
      pmScoreValueType deltaScore = 0;
      /* Temporary scores needed to compute the delta score */
      pmScoreValueType score1, score2, score3, score4;

      /* Processing depending on the type of operation */
      /* Evaluating by adding an edge */
      switch (currentOperation.op) {
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
      }  // end switch op

      /* An operation represents a gain in the score computation */
      if (deltaScore > bestDeltaScore) {
        /* Updating the best delta score */
        bestDeltaScore = deltaScore;
        operationHeld = currentOperation;
      }
      #ifdef DEBUG
      cout << "\n deltaScore " << deltaScore
           << " bestDeltaScore " << bestDeltaScore;
      #endif
    } /* End of the for loop */

    if (bestDeltaScore != epsilon) {
      #ifdef DEBUG
      std::cout << "\n BEFORE OPERATION BestDelta Score: "<<bestDeltaScore<<" ";
      operationHeld.print(cerr, graph);
      #endif
      /* Adding an edge */
      operationHeld.apply(graph);
    }
    this->freqCounterForBestDeltaScore += (this->oldBestDeltaScore == bestDeltaScore? 1 : 0);
    this->oldBestDeltaScore = bestDeltaScore;
    // record bestDelta value at each iteration.

  } while (bestDeltaScore != epsilon);

#ifdef DEBUG
  /* Printing the computed bayesian network */
  print_graph(graph);
#endif
  /* Returning the computed bayesian network found by the GS algorithm */
  return computedBN;
}

//=============================================================================

namespace PILGRIM {
    template class pmAlgoGS_T<std::vector<unsigned int>>;
    template class pmAlgoGS_T<std::vector<int>>;
    template class pmAlgoGS_T<std::vector<float>>;
    template class pmAlgoGS_T<std::vector<double>>;
    template class pmAlgoGS_T<std::vector<long double>>;

    template class pmAlgoGS_T<unsigned int*>;
    template class pmAlgoGS_T<int*>;
    template class pmAlgoGS_T<float*>;
    template class pmAlgoGS_T<double*>;
    template class pmAlgoGS_T<long double*>;
}; // namespace PILGRIM
