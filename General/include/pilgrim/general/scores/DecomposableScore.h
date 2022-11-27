/*=============================================================================
 * Product        : 
 * File           : DecomposableScore.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef SL_DECOMPOSABLE_SCORE_H
#define SL_DECOMPOSABLE_SCORE_H


#include <pilgrim/general/scores/Score.h>
#include <vector>

#include "pilgrim/general/scores/cache.h"

struct pmDCValueIterator
{
  /*
    naming convention: 
    After  = all the parents that have the index greater than the node;
    Before = all the parents that have the index lower than the node;
    loop Parents Values == loop Parents Before { loop parents After }
  */

  unsigned int parentBeforeValueIdx;
  unsigned int parentAfterValueIdx;
  unsigned int nodeIdx;
  
  unsigned int dimBefore;
  unsigned int dimAfter;
  unsigned int cardBefore;
  unsigned int cardNode;
  
  unsigned int baseBefore;
};

namespace PILGRIM{

/**
   Abstract base-class for score functions depomposable at the node level. This
   type of score can be decompose as the sum of the local score of every node.
*/
template <typename rowDataType>
class  pmDecomposableScore : public pmScore<rowDataType> 
{
 public:

	 pmCache *cache;//added by Billy Rousseaux 11/01/2011
  /**
     Constructs a decomposable score object which will evaluate the score of
     the given joint distribution with respect to data referenced by the
     frequency counter objet.
  */
  pmDecomposableScore(plJointDistribution& jd,
                      pmFrequencyCounter* pFrequencyCounter, pmCache* cache=NULL);

  /**
     Constructs a decomposable score object which will evaluate the score of
     the given bayesian network with respect to data referenced by the
     frequency counter objet.
  */
  pmDecomposableScore(pmBayesianNetwork* pBN=NULL,
                      pmFrequencyCounter* pFrequencyCounter=NULL, pmCache* cache=NULL);

  /** Destructor */
  virtual ~pmDecomposableScore();

  /**
     Compute the score by summing on the local score of evry node. 
  */
  pmScoreValueType compute();

  /**
     Compute the local score of the given variable.
  */	
  pmScoreValueType computeNode(plVariable& var);

  /**
     Virtual function which compute the local score of the given node.
     This function must be implemented by the derived classes. 
  */	
  virtual pmScoreValueType computeNode(pmNode& node) = 0;

  /**
     This function computes the log of the likelihood of the given structure 
     with respect to data, by using maximum likelihood parameters.
  */	
  pmScoreValueType computeMaxLikelihood();

  /**
     This function computes the log of the likelihood of the given node from
     the structure with respect to data, by using maximum likelihood
     parameters.
  */	
  pmScoreValueType computeNodeMaxLikelihood(pmNode& node);

  /**
     Sets the value which is used to compute the log value when the parameter
     is 0.  The default is 1e-15.
  */	
  void setEpsilon(pmScoreValueType epsilon);

  /**
     Gets the value which is used to compute the log value when the parameter
     is 0.  The default is 1e-15.
  */	
  pmScoreValueType getEpsilon();

 protected:
 	/**
	* \brief  Run MMHC algorithm and generate graph
	* \details 
	* \param variables : 
	*/
  void iteratorInit(pmDCValueIterator& iter, const pmNode& node,
                    std::vector<bool>& searchedVars);
	/**
	* \brief  Run MMHC algorithm and generate graph
	* \details 
	* \param variables : 
	*/
  bool iteratorNextParentValue(pmDCValueIterator& iter);
	/**
	* \brief  Run MMHC algorithm and generate graph
	* \details 
	* \param variables : 
	*/
  double getNodeCounts(const pmDCValueIterator& iter,
                       const std::vector<plFloat>& freq,
                       std::vector<plFloat>& nodeCounts);

  pmScoreValueType epsilon;

};
	};

#endif //__SL_DECOMPOSABLE_SCORE_H
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 79
 * End:
 */
