/*=============================================================================
 * Product        : 
 * File           : pmAlgoGS_T
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef _ALGO_GS_T_H
#define _ALGO_GS_T_H


#include <pilgrim/general/algorithms/Algorithm_T.h>
#include <pilgrim/general/scores/DecomposableScore.h>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>

//=============================================================================
struct pmGraphOp;

//=============================================================================

namespace PILGRIM{

/**
 *  Implementation of the Greedy Search (GS) algorithm
 */
template <typename rowDataType>
class  pmAlgoGS_T: public pmAlgorithm_T<rowDataType>
{
public:

/*
=====CONSTRUCTOR=====
*/
  pmAlgoGS_T( pmCSVDataSet* ds,
		          pmDecomposableScore<rowDataType>* pScore, 
		          pmCache* cache = NULL, 
		          bool debugVerbose = false
		        );

/*
=====DESTRUCTOR=====
*/
  virtual ~pmAlgoGS_T();

/*
=====GETTERS=====
*/
  /**
   *  Gets the score used by the computational method for structure learning.
   *  \return .
   */
  pmDecomposableScore<rowDataType>* getScore();

  /**
   *  Gets transition BN.
   */
  pmBayesianNetwork* getTransitionBN();

  bool getDebugVerbose();

/*
=====SETTERS=====
*/
  /**
   *  Sets the score to be used by the computational method for structure
   *  learning.
   */
  void setScore(pmDecomposableScore<rowDataType>* pScore);

  /**
  *  Sets the initial Network from which the GS starts 
  */
  void setTransitionNetwork(pmBayesianNetwork* transitionBN);

  /**
  *  Sets CPC Graph 
  */
  void setCPCGraph(pmGraph* graphCPC);

  void setDebugVerbose(bool debugVerbose);

/*
=====METHOD=====
*/
  /**
   *  Computes the bayesian network structure by using the GS algorithm.
   *  \param bnCPC: Learn using Bayesian network generated from CPCs of the variable. Optional and by default NULL.
   *  \return .
   */
  pmBayesianNetwork* run(pmBayesianNetwork* transBN);


/*
=====STATIC ATTRIBUTE=====
*/
  static int totalNbrs;

/*
=====PROTECTED METHODS AND ATTRIBUTES=====
*/
protected:
  /**
   *  Generates a list of the Directed Acyclic Graph (DAG) neighbours.
   *  \param computedBN: bayesian network used for the computations.
   *  \return .
   */
 /* List containing the DAG neighbours with Candidate Parents children CPC */
	/*used for algorithm MMHC and for G0 (initial model) in algorithm DMMHC*/
  void generateDAGNeighbours(pmGraph& graph,
                             std::vector<pmGraphOp>& result);
  
  /* List containing the DAG neighbours */
  void generateDAGNeighbours1(pmGraph& graph,
                             std::vector<pmGraphOp>& result);
  /**
   *  Makes sure that the bayesian network remains valid, i.e. a DAG.
   *  \param nodeInit: .
   *  \param node: .
   *  \param computedBN: .
   *  \return true if the graph remains a DAG.
   */
  bool remainsADAG(const pmNode& nodeInit,
                   const pmNode& node, const pmGraph& graph);

  bool remainsADAG(pmGraph& graph, const pmGraphOp& op);
  /*****************************************************************************/
 // protected:
  /**
   *  Generates a list of the Directed Acyclic Graph (DAG) neighbours.
   *  \param computedBN: bayesian network used for the computations.
   *  \return .
   */
  void generateDAGNeighbourstest(pmGraph& graph,
                             std::vector<pmGraphOp>& result);
   /**
   *  Generates a list of the Directed Acyclic Graph (DAG) for transition model of (2-TBN)neighbours.
   *  \param computedBN: bayesian network used for the computations.
   *  \return .
   */
  /* List containing the DAG neighbours with Candidate CPC (Parents and children), CP (Parents) and CC (children) */
  /*used for G-> (transition model) in DMMHC algorithm*/
  
  void generateDAG2TBNNeighbours(pmGraph& graph,std::vector<pmGraphOp>& result,int time_slices);
   /**
   *  Testing by adding an edge
   *  \param 
   *  \return .
   */
  void addingAnEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB);
 /**
   *  Testing by adding a temporal edge
   *  \param 
   *  \return .
   */
  void addingATemporalEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB);

   /**
   *  Testing by adding a temporal edge in DMMHC
   *  \param 
   *  \return .
   */
  void addingATemporalEdgeUsingDMMHCconstraint(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB, int i, int j);
  /**
   *  Testing by adding an edge
   *  \param 
   *  \return .
   */
  void addingAnEdgeUsingMMHCconstraint(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB, int i, int j);

  /**
   *  Testing by reversing an edge
   *  \param
   *  \return .
   */
  void reversingAnEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB);

    /**
   *  Testing by removing an edge
   *  \param 
   *  \return .
   */
  void removingAnEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB);

    /**
   *  Testing by removing a temporal edge
   *  \param 
   *  \return .
   */
  void removingATemporalEdge(pmGraph& graph, std::vector<pmGraphOp>& operations, pmNode& nodeA, pmNode& nodeB);


  /*****************************************************************************/
  /* Initial bayesian network */
  pmBayesianNetwork* transitionBN;
    
  /* CPC Bayesian network to for constraint in add_edge operation */
  pmGraph* graphCPC;

  /* Score used for structure learning */
  pmDecomposableScore<rowDataType>* pScore;

   /* Score stored to adjust epsilon value */
  pmScoreValueType oldBestDeltaScore;

  /* to the frequency of same best delta score occurrences, to adjust epsilon value */
  size_t freqCounterForBestDeltaScore;

  pmScoreValueType epsilon;

  bool debugVerbose;
};
};
//=============================================================================
#endif //SL_ALGO_GS_H
// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:

/**
 * Creation: ????
 * Last update: 2019/06/07
 * class name was changed from AlgoDynGS to AlgoGS_T
 * _t0 means initial and _T means transition
 * Adding getter for transitionBN to simplify in the dynamic encapsulation
 * Adding setCPCGraph for AlgoMMHC_T
 * Adding setTransitionBN for AlgoMMHC_T
 * Delete all use of old constructor
 * Modif : Côme Vitis
 */