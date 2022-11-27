/*=============================================================================
 * Product        : 
 * File           : ScoreBIC.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef SL_SCORE_BIC_H
#define SL_SCORE_BIC_H


#include <pl.h>
#include <pilgrim/general/scores/DecomposableScore.h>
#include <vector>

#include "pilgrim/general/scores/cache.h"

namespace PILGRIM{

template <typename rowDataType>
class  pmScoreBIC: public pmDecomposableScore<rowDataType>
{
public:
/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreBIC(plJointDistribution& jd,
             pmFrequencyCounter* pFrequencyCounter, pmCache* cache=NULL);
  pmScoreBIC(pmBayesianNetwork* pBN= NULL,
             pmFrequencyCounter* pFrequencyCounter = NULL, pmCache* cache=NULL);


  virtual ~pmScoreBIC();
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
  static pmScoreValueType computeBICMatrix(pmBayesianNetwork* pBN,
                                           pmBayesianNetwork* pFullBN, 
                                           std::vector<plDistribution>& P_Xi,
                                           std::vector<plCndDistribution>& P_Xj_given_Xi,
                                           unsigned long int exempleCount);
	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  pmScoreValueType computeDLnet(pmBayesianNetwork* pBN);

	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  static pmScoreValueType
    computeLocalBICScoreOneParent(unsigned int j,
                                  plVariable& Xj,
                                  unsigned int cardinalityJ,
                                  unsigned int i, plVariable& Xi,
                                  unsigned int cardinalityI,
                                  plValues& values,
                                  std::vector<plDistribution>& P_Xi,
                                  std::vector<plCndDistribution>& P_Xj_given_Xi,
                                  unsigned long int exempleCount);


	/**
	* \brief  Run the MI test
	* \details 
	* \param variables : 
	**/
  static pmScoreValueType 
    computeLocalBICScoreNoParent(unsigned int j,
                                 plVariable& Xj,
                                 unsigned int cardinalityJ,
                                 plValues& values,
                                 std::vector<plDistribution>& P_Xi,
                                 unsigned long int exempleCount);

  static unsigned int functionCalls;

};
	};


#endif //SL_SCORE_BIC_H
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 79
 * End:
 */
