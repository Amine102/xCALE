/*=============================================================================
 * Product        : 
 * File           : pmMI.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */


#ifndef _pmMI_H
#define _pmMI_H
#pragma once

#include <pl.h>
#include <vector>
#include <math.h>

#include <plStructureLearner.h>

#include <boost/math/distributions/chi_squared.hpp>

//#include "pilgrim/general/pmCommonTypeDefinitions.h"
#include "pilgrim/general/pmUtility.h"
#include "pilgrim/general/pmFrequencyCounter.h"
#include <pilgrim/general/pmCSVDataSet.h>

using namespace std;

namespace PILGRIM{

//Mutual Infomation class
class mutualInformation
{
public:
	/**! Constructor. **/
	mutualInformation(pmCSVDataSet * _pCompDataSet, plVariablesConjunction* _variables=0, bool _usingSlidingWindow = false,
    float *_alpha = NULL, float *_p = NULL);

	/**! Run the MI test. Intputs varA, varB, condVAr **/
	bool computeMI(const unsigned int&, const unsigned int&, std::vector <unsigned int>);
	
	bool computeMI(const unsigned int&, const unsigned int&);

	/**! Run the MI test for pmiding windows. It will store previous values too.
	   * Intputs varA, varB, condVAr **/
	bool computeMI_SW(const unsigned int&, const unsigned int&, std::vector <unsigned int>);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	bool computeMI_SW(const unsigned int&, const unsigned int&);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	plFloat compute_cnd_mi(const std::vector<plFloat> &Nabc,
		unsigned int cardA, unsigned int cardB, unsigned int cardC,
		unsigned int N);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	plFloat compute_mi(const std::vector<plFloat> &Nab,
		unsigned int cardA, unsigned int cardB,
		unsigned int N);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	void frequencyCount(const std::vector<unsigned int> & SearchedVariablesIndices, vector<plFloat>& freq);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	float chi_square_dist(bool& ind, unsigned int& df);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	bool isDataSufficientToProceedIndTest(const unsigned int&, const unsigned int&, std::vector<unsigned int>);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
	void computeDegreeOfFreedom(std::vector<unsigned int>&);

	/**! Get the Mutual Information value. **/
	float getMI();                      //** TO DO INLINE
	/**! Get the p-value. **/
	float getPValue();                //** TO DO INLINE

	float getQuantileValue();  
	/**! Get the degree of freedom. **/
	unsigned int getDF();            //** TO DO INLINE
	

	 

protected:
	float alpha;
	float p;
	float MI;
	float Quantile_value;
	unsigned int  df;
	
	unsigned int numOfRecords;
	pmCSVDataSet *pCompDataSet;
	plVariablesConjunction variables;
	bool usingSlidingWindow;
	
};
	};

#endif //
