/* =============================================================================
* Product        : ProBT
* File           : AlgoDMMPCbar.cpp
* Author         : Ghada Trabelsi
* Creation       : January 01 2012
*
*=============================================================================
*        (c) Copyright 2007, ProBayes SAS - all rights reserved
*=============================================================================
*
*------------------------- Description ---------------------------------------
*     It contains the functions related to DMMHC algorithm.
*
*-----------------------------------------------------------------------------
*/
#ifndef _ALGOMMHC_T_H
#define _ALGOMMHC_T_H


#include "plStructureLearner.h"
#include <iostream>
#include <pl.h>
#include <ios>
#include <string>

#include "pilgrim/general/pmCSVDataSet.h"
#include "pilgrim/general/algorithms/AlgoGS_T.h"
#include "pilgrim/general/algorithms/AlgoMMPC_T.h"
#include "pilgrim/general/log.h"

#include "pilgrim/general/scores/computeMI.h"
#include "pilgrim/general/scores/ScoreAIC.h"
#include "pilgrim/general/scores/ScoreMDL.h"
#include "pilgrim/general/scores/ScoreBIC.h"
#include "pilgrim/general/scores/ScoreBDeu.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>


namespace PILGRIM {
//! Class to calculate Max-Min Hill Climbing algorithm

template <typename rowDataType>
class pmAlgoMMHC_T : public pmAlgorithm_T<rowDataType> {
public:
/*
=====CONSTRUCTORS=====
*/
	pmAlgoMMHC_T( pmCSVDataSet* ds,
			          pmDecomposableScore<rowDataType>* pScore,
			          pmCache* cache = NULL,
			          bool debugVerbose = false
			          );

/*
=====DESTRUCTOR=====
*/
  ~pmAlgoMMHC_T(void);


/*
=====GETTER=====
*/
  pmBayesianNetwork* getTransitionBN();

  bool getDebugVerbose();

/*
=====SETTERS=====
*/
	// Set significance level for MMPC 
	void setAlpha(float alpha);

	// Sets the initial Network from which the GS starts 
	void setCondVarSize(unsigned int condVarSize);

	// Sets the initial Network from which the GS starts 
	void setInitialNetwork(pmBayesianNetwork* initialBN);

	// Set the Scoring function for the GS
	void setScore(pmDecomposableScore<rowDataType>* pScore);

  // Set debug bool
  void setDebugVerbose(bool debugVerbose);

/*
=====METHODS======
*/
  // run the MMHC algorithm (virtual from abstract class)
  pmBayesianNetwork* run(pmBayesianNetwork* bn);

  // compute CPC
  void computeCPC_T(  CPC_t& CPCs,
                      CPC_t& CCs,
                      CPC_t& PCs,  ///< Return calculated matrix of CPC.
                      const int& targetVariable = -1  ///< Optional Target variable, by default -1 to learn CPC for all variables.
                      );
  
  // Create a CPC Graph
  pmBayesianNetwork* createConstraintsGraph_T(CPC_t& cpc_v, CPC_t& cp_v);

/*
=====STATIC ATTRIBUTES======
*/
  static std::string orignalNetFileName;
  static bool logFileHeaders;
  bool initializeGSwithEmptyNet;
  static int t;  // ghada
  static int t1;  // ghada
  static int t2;  // ghada

/*
=====PRIVATE ATTRIBUTES=====
*/
private:
  pmCSVDataSet* ds;  ///< Data set matrix.
  unsigned int condVarSize;  //= 10
  float alpha;  // Significance level of MMPC
  pmBayesianNetwork* transitionBN; // initialBN for GS
  pmCache* cache; // Cache for GS
  pmDecomposableScore<rowDataType>* pScore; // for GS
  bool debugVerbose;
};
};
#endif

/**
 * Creation: ????
 * Last update: 2019/06/11
 * Rewriting as a template class
 * Adding getTransitionBN() as pure virtual from abstract Algo
 * Adding run as pure virtual from abstract Algo
 * New constructor implemented
 * Changing old constructor uses of AlgoGS_T, now setters are available in AlgoGS_T
 * Deleting all old methods and useless attributes
 * Modif : Côme Vitis
 */