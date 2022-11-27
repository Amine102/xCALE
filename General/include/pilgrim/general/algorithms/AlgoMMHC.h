/*=============================================================================
 * Product        :
 * File           : AlgoMMHC
 * Author         :
 * Creation       :

 *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef _ALGOMMHC_H
#define _ALGOMMHC_H

#include <iostream>
#include <ios>
#include <pl.h>
#include "pilgrim/general/pmCSVDataSet.h"
#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/algorithms/AlgoMMPC.h"
#include "pilgrim/general/algorithms/AlgoMMPCbar.h"
#include "pilgrim/general/algorithms/AlgoGS.h"

#include "pilgrim/general/pmFrequencyCounter.h"

#include "pilgrim/general/scores/computeMI.h"

#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "plStructureLearner.h"


namespace PILGRIM {
/*Class to calculate Max-Min Hill Climbing algorithm*/

template <typename rowDataType>
class pmAlgoMMHC : public pmAlgorithm <rowDataType> {
public:
/*
=====CONSTRUCTOR=====
*/
	pmAlgoMMHC(	pmCSVDataSet* ds,
							pmDecomposableScore<rowDataType>* pScore,
							pmCache* cache = NULL,
							bool debugVerbose = false
							);

/*
=====DESTRUCTOR=====
*/
	~pmAlgoMMHC(void);

/*
=====GETTERS=====
*/
	// Get initialBN (virtual from abstract class)
	pmBayesianNetwork* getInitialBN();

	// Get debugVerbose
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

	// Set debug mode
	void setDebugVerbose(bool debugVerbose);

/*
=====METHODS=====
*/
	// run the MMHC algorithm (virtual from abstract class)
	void run(pmBayesianNetwork*  bnMMHC);

	// Compute CPC 
	void computeCPC(CPC_t &CPCs,  ///< Return calculated matrix of CPC.
									const int &targetVariable = -1  ///< Optional Target variable, by default -1 to learn CPC for all variables.
									);

	// Compute the cpc and return it, check for symetry
	CPC_t extractCPC();

	// Create a CPC Graph
	pmBayesianNetwork* createConstraintsGraph(CPC_t& cpc_v, 
																						CPC_t* PC_set = NULL
																						);

	// Extract Markov Blanket
	void extractMarkovBlanket(pmBayesianNetwork* BN, 
														CPC_t& CPClist
														);

/*
=====PROTECTED=====
*/
private:
	pmCSVDataSet* ds;  ///< Data set matrix.
	unsigned int condVarSize;  // = 10
	float alpha; // Significance Level for MMPC
	pmBayesianNetwork* initialBN; // For GS
	pmCache *cache; // cache for GS
	pmDecomposableScore<rowDataType>* pScore; // Score for GS

	bool debugVerbose;
};
};
#endif

/**
 * Creation: ????
 * Last update: 2019/06/17
 * Adding getInitialBN() from Algorithm abstract class
 * Hard cleaning of the code and adding comment
 * Suppressing useless debugMode boolean (there is one in parent class)
 * Shortening the run code with extractCPC function
 * Adding getDebug and setDebug from abstract
 * Modif : Côme Vitis
 */