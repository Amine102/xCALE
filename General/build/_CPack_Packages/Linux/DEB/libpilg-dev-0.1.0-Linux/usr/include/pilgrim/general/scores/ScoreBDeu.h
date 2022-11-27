/*=============================================================================
 * Product        : 
 * File           : ScoreBDeu.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef SL_SCORE_BDEU_H
#define SL_SCORE_BDEU_H


#include <pl.h>
#include "pilgrim/general/scores/DecomposableScore.h"
#include "pilgrim/general/scores/cache.h"

namespace PILGRIM{

template <typename rowDataType>
class  pmScoreBDeu : public pmDecomposableScore<rowDataType>
{
public:
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreBDeu(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter, 
              unsigned long int Nprime = 1, pmCache* cache=NULL);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreBDeu(pmBayesianNetwork* pBN=NULL, pmFrequencyCounter* pFrequencyCounter= NULL, 
              unsigned long int Nprime = 1, pmCache* cache=NULL);

  /**
  * \brief  Run the MI test
  * \details
  * \param variables :
  **/
  pmScoreBDeu(unsigned long int Nprime, pmBayesianNetwork* pBN = NULL, pmFrequencyCounter* pFrequencyCounter = NULL,
	  pmCache* cache = NULL);

  /**
  * \brief  Run the MI test
  * \details
  * \param variables :
  **/
  ~pmScoreBDeu();
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreValueType computeNode(pmNode& node);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  void setPriorWeight(unsigned long int Nprime);
  /**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  unsigned long int getPriorWeight();

  static unsigned int functionCalls;
protected:
  unsigned long int Nprime;
};
	};
#endif //__SL_SCORE_BDEU_H
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 79
 * End:
 */
