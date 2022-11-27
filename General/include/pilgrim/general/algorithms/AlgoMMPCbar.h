/*=============================================================================
 * Product        :
 * File           : AlgoMMPCbar
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef _AlgoMMPCbar_H
#define _AlgoMMPCbar_H

#include <pl.h>
#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <cstdlib>

#include "pilgrim/general/pmCSVDataSet.h"


using namespace boost;
using namespace std;


// Map iterator to store MaxMin variables along with assoc
typedef std::multimap<float, unsigned int> MaxMinVarMap;
typedef std::vector< vector <unsigned int>> SubSets;
typedef std::pair<bool, float> pair_t;

namespace PILGRIM{

class AlgoMMPCbar 
{
public:

	//! A constructor.
    AlgoMMPCbar(pmCSVDataSet* _dataSet,          /*! Data set matrix */
		const unsigned int &_condVarSize = 10,    /*! Optional: Fix the maximum number of conditional variables in independence test. Default are 10  */
		const unsigned int &_KBestSize = 8,
		const unsigned int &_TOCOSize = 0,
		bool _debugMode = true
		);

	//! Destructor
	~AlgoMMPCbar(void);

	//! Compute CPC for a target variable
    /*!
      \param 1 Target variable as a const unsigned integer.
      \return  a vector of CPCs.
    */	
	void computeMMPCbar( unsigned int& ,                          ///< Target variable
		                           std::vector<unsigned int>&         ///<  Return learned cpc 
								   );

	
	//! 
	void Phase1(unsigned int&); /// Target variable
									

	void Phase2(unsigned int& vTarget);

	//SubSets computeSubSets( std::vector <int>& );
	//! To calculate sub sets of size n.
	/*!
	\return  It will return list of subsets by using function computeSubsets().
	*/
	//void subs(const std::vector<unsigned int>& pre, size_t k, size_t n, SubSets& c);
	void subs(const std::vector<unsigned int>& org, const std::vector<unsigned int>& pre, size_t k, size_t n, SubSets& c);

	//! To calculate sub sets of size n.
	/*!
	\return  It will return list of subsets by using function subs().
	*/
	//void computeSubSets(SubSets& c, unsigned int& subSetSize);
	void computeSubSets(const std::vector<unsigned int>& org, unsigned int n, SubSets& c);

	bool computeMaxMinHeuristic( unsigned int& vTarget, unsigned int& varF, float& assocF, MaxMinVarMap& mapMinAssoc );

	bool computeMinAssoc (const unsigned int& vTarget, const unsigned int& vMax,  pair_t& minAssoc );
		//( MaxMinVarMap& mapMaxMin, int& varF, float& assocF );

	//bool removeFromList( unsigned int& vX, std::vector<unsigned int>& vList);
	//bool removeFromList( std::vector<unsigned int>::iterator& position, std::vector<unsigned int>& vList);
	//bool removeFromList( unsigned int& vX, std::vector<unsigned int>& vList, std::vector<unsigned int>::iterator& itr);
	bool isInList(const unsigned int& var, std::vector<unsigned int>& vList);
	bool isInList(const unsigned int& var, std::vector<unsigned int>& vList, std::vector<unsigned int>::iterator& position);
	bool isInList(const unsigned int& var, std::map <unsigned int, CPC_t>& mapList);
//	static void resetIncrementalLearningCounters();

	// to store k best variables for a target variable 
    // inline void copyKBestVar(MaxMinVarMap&, CPCSteps&);

	//! To retieve the list of k best variables
	/*!
	\param Target variable
	\param Step number
	\return  It will return a pointer to the list of k best variables .
	*/
	bool getKbest(unsigned int&, int&);
	bool isKBestExist(unsigned int&, int&);
	bool getKbest(unsigned int&, int, std::vector<unsigned int>&);
	//static void setKbestSize(unsigned int);
	void printKbest();
	static void printFinalCPCPath();
	bool compareCPCPath(unsigned int& vTarget, unsigned int& varF, int& step, unsigned int & oldCPC);
	//static void resetStaticParam(); // reset stored cashe of already computed CPCS for next iteration in incremental learning
	//void clearIncrementalParamForBatchLearing(); // clear stored k best and path to not use in batch learning
	//void setLogFileName(vector<string> fnamelist);
	//void writeSummaryLog();
	static unsigned int varCounter;
	static unsigned int numVars;                  ///< Number of total vars
	static plVariablesConjunction all_variable;  
	static bool incrementLearning;                ///< Flag for incremental learning by default is off
	static std::map <unsigned int, CPC_t> bestKvariables;  ///< To store top k best variables for each step
	static std::map <unsigned int, std::vector<unsigned int> > cpcPath; ///<  To store learning path for final CPC 		
	
protected:
	float subsetTime;
	float alpha;
	float p;
	float MI;
	unsigned int  df;
	unsigned int conditionalVariableSize;	///< conditional variables limit for independence test.
	unsigned int KBestSize, TOCOsize;      ///< k best and TOCO size
	std::vector<unsigned int> varList; //= new std::vector<unsigned int>();    ///< To store variable indexes
	std::vector<unsigned int> fullVarList; // = new std::vector<unsigned int>();
	pmCSVDataSet* ds;                      ///< data set
	bool debugMode;                        ///< Debug mode by default off	
	std::vector<unsigned int> *CPCBar;  ///< to store cpc of target variable
	std::vector<unsigned int>::iterator cpcItr; ///< iterator for cpcbar
	int nFunctionCalls, nTOCOnonFired, nTOCOFired;

};

};
#endif
