/*=============================================================================
 * Product        : 
 * File           : pmAlgoDynamic
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef SL_ALGO_DYN_GS_H
#define SL_ALGO_DYN_GS_H

#include <pilgrim/general/scores/DecomposableScore.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoGS_T.h>
#include <pilgrim/general/algorithms/AlgoMMHC.h>
#include <pilgrim/general/algorithms/AlgoMMHC_T.h>
#include <pilgrim/general/pmCSVdynDataSet.h>
#include <pilgrim/general/pmDynamicBayesianNetwork.h>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <stdexcept>

namespace PILGRIM{


template <typename rowDataType>
class  pmAlgoDynamic
{
public:

/*
=====CONSTRUCTOR=====
*/
  pmAlgoDynamic(pmAlgorithm<rowDataType>* initialAlgo,
                pmAlgorithm_T<rowDataType>* transitionAlgo);

/*
=====DESTRUCTOR=====
*/ 
  virtual ~pmAlgoDynamic();

/*
=====STATIC ATTRIBUTE=====
*/
  static int totalNbrs;

/*
=====INIT=====
*/
  void initDBN();

/*
=====METHOD=====
*/
  // run the two algos, create the DBN
  void run(pmDynamicBayesianNetwork* dynamicBN);


/*
=====PRINTER=====
*/
void summary();

/*
=====Getter======
*/
pmDynamicBayesianNetwork* getDynamicInitBN();


protected:

  /* Initial bayesian network */
  pmDynamicBayesianNetwork* dynamicBN;
  
  // My algos
  pmAlgorithm<rowDataType>* algo_t0;
  pmAlgorithm_T<rowDataType>* algo_T;
};
};

#endif

/**
 * Creation: 2019/06/04
 * Last update: 2019/06/07
 * _t0 means initial and _T means transition
 * Simple super structure with initial and transition algorithm
 * Setting debug mode printers
 * Modif : Côme Vitis
 */