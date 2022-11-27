/*=============================================================================
* Product        : ProBT 
* File           : slidingWindow.h
* Author         : Amanullah YASIN
* Creation       : July 02 2012
*
*=============================================================================
*        (c) Copyright 2007, ProBayes SAS - all rights reserved
*=============================================================================
*
*------------------------- Description ---------------------------------------
*    
*   
*-----------------------------------------------------------------------------
*/

#ifndef _slidingWindow_H
#define _slidingWindow_H

#include "pl.h"
#include <fstream>
#include <strstream>
#include <string>

#include "pmBayesianNetwork.h"
#include <plDataDescriptor.h>
// #include "commonTypesForMMPC.h"
#include "pmUtility.h"
#include "pmFrequencyCounter.h"
#include <boost/graph/adjacency_list.hpp>
#include "pmCSVDataSet.h"

using namespace std;

typedef pmGraph::vertex_descriptor pmNode;
typedef pmGraph::vertex_iterator pmNodeIterator;

class pmSlidingWindow
{
public:
	// pmSlidingWindow(pmBayesianNetwork *, pmCSVDataSet *);
	// pmSlidingWindow(pmCSVDataSet *);
	pmSlidingWindow();
	~pmSlidingWindow();

	static void initSlidingWindow(pmCSVDataSet *data,float fc);

	static void save_CurrentBN(pmBayesianNetwork *currBN); // save current BN for next window

	static void setForgettingCoefficient(float);

	static bool getFreqFromCache(const plVariablesConjunction &varABC, vector<plFloat> &freq);
	static bool getFreqFromCache(string &key, vector<plFloat> &freq);
	static bool getOldFreqFromCache(const plVariablesConjunction &varABC, vector<plFloat> &freq);
	static void saveFreqInCache(plVariablesConjunction &varABC, vector<plFloat> &freq);
	static void saveFreqInCache(string &key, vector<plFloat> &freq);
	static void computeAndSaveFreqInCache(std::vector<unsigned int> &varSet, string key);
	static void generateKey(const plVariablesConjunction &varABC, string &key);
	static void generateKey(const std::vector<unsigned int> &varSet, string &key);
	static std::vector<unsigned int> parseKey(const std::string &s);

	/**
    It adds one learn object for every node of the network in the given list.
	It initialize the learn object histogram with given frequencies.
    The learn objects are pushed at the end of the list.
    */
	static void setLearnObjects(pmBayesianNetwork *temppmBN);

	// PlvariableConjunction
	static bool getOldFrequencyCount(const plVariablesConjunction &varAB, vector<plFloat> &freq); // P(A,B)

	static void updateCache();
	static void MoveFreqToFreqOld();
	static void updatePreviousBN(pmBayesianNetwork bn);
	static void UpdateN(pmCSVDataSet* ds);
	static void UpdateN(plDataDescriptor* pDataDesc);
	static void UpdateALL(pmCSVDataSet *ds, pmBayesianNetwork previousBN);
	static void UpdateCache_N(pmCSVDataSet *ds);
	static void UpdateCacheAndPrevBN(pmBayesianNetwork bn);

	// static void initializeSlidingWindow();
	
	static pmBayesianNetwork *previousBN;
	static unsigned int N;
	static unsigned int N_old;
	static unsigned int freqFoundInOldCache;
	static unsigned int freqNotFoundInOldCache;
	static std::map<string, std::vector<plFloat>> freqCache;
	static std::map<string, std::vector<plFloat>> freqCacheOld;
	static float forgettingCoefficient;

	static void freqCacheSummary();
	static void freqOldCacheSummary();
	static void Summary();

	static pmBayesianNetwork *BN;
	static pmCSVDataSet *ds; ///< Data set matrix.
	static plVariablesConjunction variables;

protected:
private:
	
};

#endif //