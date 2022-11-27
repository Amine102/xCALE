/*=============================================================================
 * Product        : 
 * File           : computeMI.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef _COMPUTEMI_H
#define _COMPUTEMI_H

#include <pl.h>

#include <vector>
#include <map>

#include "pilgrim/general/pmCommonTypeDefinitions.h"

#include "pilgrim/general/algorithms/AlgoMMPC.h"  // only for test purpose it can be removed if it generates error.

#include "pilgrim/general/scores/pmMI.h"


using namespace std;

struct MIResultBackup
{
    bool test;
	float MIValue, pValue, quantileValue;
};


namespace PILGRIM{

class pmMI: public mutualInformation
{
	
public:

	static std::map<std::string, MIResultBackup> testValuesCache;
	bool cache;

	/**! Constructor. **/
	pmMI(pmCSVDataSet * const _pCompDataSet, plVariablesConjunction *_variables = 0, bool _cache=false, bool _usingSlidingWindow = false,
		float *_alpha = NULL, float *_p = NULL );


	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
    bool computeMI(const unsigned int&, const unsigned int&, std::vector<unsigned int>);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	bool computeMI(const unsigned int&, const unsigned int&);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	static void clearCache();
	static unsigned int functionCalls;
	bool usingSlidingWindow;
	bool debugMode;
 
};
	};

#endif //
