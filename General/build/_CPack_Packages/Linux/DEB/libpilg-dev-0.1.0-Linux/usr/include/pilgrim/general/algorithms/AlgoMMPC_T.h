/*=============================================================================
 * Product        : 
 * File           : AlgoMMPC_T.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */


#ifndef _ALGOMMPC_T_H
#define _ALGOMMPC_T_H

#include <iostream>
#include "pilgrim/general/algorithms/AlgoMMPCbar_T.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include "pilgrim/general/log.h"
using namespace boost;
using namespace std;

namespace PILGRIM{

//! 
class AlgoMMPC_T : public AlgoMMPCbar_T
{
public:

	//! Constructor
	AlgoMMPC_T(	pmCSVDataSet* _dataSet, // Dataset
							const unsigned int &_condVarSize = 10, // maximum size of conditional variables for ind test.
							bool _debugMod=false
							);

	//! Destructor
	~AlgoMMPC_T(void);
	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
	void computeMMPC_T(	unsigned int&, /*!  Target variable */
		                	CPC_t &, /*!  Populate CPC_t matrix as a return */
					 						CPC_t &,CPC_t &					 
											);     // Target variable 
					 /**
	* \brief  
	* \details 
	* \param variables : 
	*/
	void computeCPCbar_T(	unsigned int& vTarget, /*!  Target variable */
												std::vector<unsigned int>& cpcOfTargetVariable/*! Return list of cpcs */
		               			);
					   /**
	* \brief  
	* \details 
	* \param variables : 
	*/
	void incrementCounters();
	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
	static void resetCounters();
	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
	static int totalFunctionCalls;
	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
	static std::map <unsigned int, std::vector<unsigned int>> cpcMapList;  ///< To temporarily store learned cpc from MMPCbar 
	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
	static std::map <unsigned int, std::map<unsigned int, std::vector<unsigned int>>> cpc_tMapList;///< To temporarily store learned cpc_t from DMMPCbar with time slice
	
	static float MItime, MIsearchTime;
	static int cachesize;
protected:
	pmCSVDataSet* ds; /*!  Data discriptor */
};
	};
#endif 

/**
 * Creation: ????
 * Last update: 2019/06/17
 * suppressing debugMode useless attribute because inherited from MMPCbar_T
 * cleaning the code
 * Modif : Côme Vitis
 */