/*=============================================================================
 * Product        : 
 * File           : AlgoMMPC
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef _ALGOMMPC_H
#define _ALGOMMPC_H

#include <iostream>
#include "pilgrim/general/algorithms/AlgoMMPCbar.h"

using namespace boost;
using namespace std;

namespace PILGRIM{

//! 
class AlgoMMPC : public AlgoMMPCbar 
{
public:

	//! Constructor
	AlgoMMPC(pmCSVDataSet* _dataSet, // Dataset
		const unsigned int &_condVarSize = 10, // maximum size of conditional variables for ind test.
		const unsigned int &_kBestSize = 20, // 
		const unsigned int &_TOCOsize = 3, // 
		bool _debugMod=false
		);

	//! Destructor
	~AlgoMMPC(void);

	void computeMMPC(unsigned int&, /*!  Target variable */
		                CPC_t & /*!  Populate CPC matrix as a return */
					 );     // Target variable 
	void computeCPCBar(unsigned int&, /*!  Target variable */
						std::vector<unsigned int>& /*! Return list of cpcs */
		               );
	void incrementCounters();
	static void resetCounters();
	static int totalFunctionCalls, TotalTOCOnonFired, TotalTOCOFired;
	static std::map <unsigned int, std::vector<unsigned int>> cpcMapList;  ///< To temporarily store learned cpc from MMPCbar 
	static float MItime, MIsearchTime;
	static int cachesize;
protected:
	pmCSVDataSet* ds;               /*!  Details. */
	bool debugMode;	
};

	};
#endif 

/**
 * Creation: ????
 * Last update: 2019/06/17
 * Cleaning little of the code
 * Modif : Côme Vitis
 */