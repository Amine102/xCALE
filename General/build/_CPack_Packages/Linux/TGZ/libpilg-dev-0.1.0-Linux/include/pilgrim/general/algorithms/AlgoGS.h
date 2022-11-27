/*=============================================================================
 * Product        : 
 * File           : AlgoGS
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef SL_ALGO_GS_H
#define SL_ALGO_GS_H


#include <pilgrim/general/algorithms/Algorithm.h>
#include <pilgrim/general/scores/DecomposableScore.h>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <stdexcept>

//=============================================================================
struct pmGraphOp;

//=============================================================================

namespace PILGRIM{

/**
 *  Implementation of the Greedy Search (GS) algorithm
 */
template <typename rowDataType>
class  pmAlgoGS: public pmAlgorithm<rowDataType>
{
public:
/*
=====CONSTRUCTORS=====
*/
	pmAlgoGS( pmCSVDataSet* ds,
		        pmDecomposableScore<rowDataType>* pScore, 
		        pmCache* cache = NULL, 
		        bool debugVerbose = false
		      );

/*
=====DESTRUCTOR=====
*/
  virtual ~pmAlgoGS();

/*
=====GETTERS=====
*/
  /**
   *  Gets the score used by the computational method for structure learning.
   *  \return .
   */
  pmDecomposableScore<rowDataType>* getScore();

  /**
   *  Gets the initialBN
   */
  pmBayesianNetwork* getInitialBN();

	// Get debugVerbose
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
  void setInitialNetwork(pmBayesianNetwork* initialBN);

  /**
  *  Sets CPC Graph 
  */
  void setCPCGraph(pmGraph* graphCPC);

  // Set debug mode
	void setDebugVerbose(bool debugVerbose);

/*
=====METHOD=====
*/
  /**
   *  Computes the bayesian network structure by using the GS algorithm.
   *  \param bnCPC: Learn using Bayesian network generated from CPCs of the variable. Optional and by default NULL.
   *  \return .
   */
  void run(pmBayesianNetwork* initComputedBN);

/*
=====STATIC ATTRIBUTE=====
*/
  static int totalNbrs;

 
/*
=====PROTECTED ATTRIBUTES AND METHODS=====
*/
protected:
  /**
   *  Generates a list of the Directed Acyclic Graph (DAG) neighbours.
   *  \param computedBN: bayesian network used for the computations.
   *  \return .
   */
  void generateDAGNeighbours(pmGraph& graph,
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

  /* Initial bayesian network */
  pmBayesianNetwork* initialBN;
    
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
 * _t0 means initial and _T means transition
 * Adding getter for initialBN to simplify in the dynamic encapsulation
 * Delete old constructor
 * Setting debug mode printers
 * Adding getDebug and setDebug from abstract
 * Modif : Côme Vitis
 */