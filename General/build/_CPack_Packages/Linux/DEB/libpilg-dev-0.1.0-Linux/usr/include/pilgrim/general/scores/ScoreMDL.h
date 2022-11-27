/*=============================================================================
 * Product        : 
 * File           : ScoreMDL.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef SL_SCORE_MDL_H
#define SL_SCORE_MDL_H


#include <pl.h>
#include "pilgrim/general/scores/DecomposableScore.h"
#include "pilgrim/general/scores/cache.h"

namespace PILGRIM{

template <typename rowDataType>
class  pmScoreMDL : public pmDecomposableScore<rowDataType>
{
public:
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreMDL(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter, pmCache* cache=NULL);
  /**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreMDL(pmBayesianNetwork* pBN=NULL, pmFrequencyCounter* pFrequencyCounter=NULL, pmCache* cache=NULL);

  ~pmScoreMDL();
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreValueType computeNode(pmNode& node);

   static unsigned int functionCalls;
};
	};
#endif //SL_SCORE_MDL_H
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 79
 * End:
 */
