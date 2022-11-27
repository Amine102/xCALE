/*=============================================================================
 * Product        : 
 * File           : ScoreAIC.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */


#ifndef SL_SCORE_AIC_H
#define SL_SCORE_AIC_H


#include <pl.h>
#include "pilgrim/general/scores/DecomposableScore.h"
#include "pilgrim/general/scores/cache.h"

namespace PILGRIM{

template <typename rowDataType>
class  pmScoreAIC : public pmDecomposableScore<rowDataType>
{
public:
	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
  pmScoreAIC(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter, pmCache* cache=NULL);
  	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
  pmScoreAIC(pmBayesianNetwork* pBN = NULL, pmFrequencyCounter* pFrequencyCounter = NULL, pmCache* cache=NULL);
  /**
  * \brief
  * \details
  * \param variables :
  */


  virtual ~pmScoreAIC();
	/**
	* \brief  
	* \details 
	* \param variables : 
	*/
  pmScoreValueType computeNode(pmNode& node);
  static unsigned int functionCalls;
};
	};
#endif //SL_SCORE_AIC_H
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 79
 * End:
 */
