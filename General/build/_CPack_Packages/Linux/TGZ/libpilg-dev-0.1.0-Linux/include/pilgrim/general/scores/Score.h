/*=============================================================================
 * Product        : 
 * File           : Score.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */


#ifndef SL_SCORE_H
#define SL_SCORE_H


#include <pl.h>
#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/pmFrequencyCounter.h"

namespace PILGRIM{

/**
  Abstract base-class for score functions. A score computes a value for a given 
  bayesian network structure. The score should not take into account the probability 
  laws presented in the network, as it evaluates the structure only (ie. the 
  existance of dependancies between the variables). 
*/
template <typename rowDataType>
class  pmScore
{
public:
  /**
    Constructs a score object which will evaluate the score of the given joint distribution
    with respect to data referenced by the frequency counter objet. 
  */
  pmScore(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter);

  /**
    Constructs a score object which will evaluate the score of the given bayesian network
    with respect to data referenced by the frequency counter objet. 
  */
  pmScore(pmBayesianNetwork* pBN, pmFrequencyCounter* pFrequencyCounter);

  /**
  Constructs an empty score object which will evaluate the score of the given bayesian network
  with respect to data referenced by the frequency counter object.
  */
  pmScore();

  /**
    Destroys the current score object. If the bayesian network is associated with the score, 
    than it will be also destroyed.
  */
  virtual ~pmScore();
    
  /**
    This function computes the score of the given bayesian network with respect
    to data presented in the frequency counter object. 
    The frequency counter object must be initialised properly, by calling 
    computeFrequencies() on it before the execution of this function.
  */  
  virtual pmScoreValueType compute() = 0;

  /**
    Sets the joint distribution which will be evaluated. Internally, this object is 
    converted to a bayesian network object.
  */
  virtual void setJointDistribution(plJointDistribution& jd);

  /**
    Gets the joint distribution which will be evaluated. 
  */
  virtual plJointDistribution getJointDistribution();

  /**
    Sets the bayesian network which will be evaluated. If the autoDelete is set to true,
    the user is no longer in charge with de destruction of this object.
  */
  virtual void setBayesianNetwork(pmBayesianNetwork* pBN , bool autoDelete = false);

  /**
    Gets the bayesian network which will be evaluated. 
  */
  virtual pmBayesianNetwork* getBayesianNetwork();

  /**
    Sets the frequency counter object used for evaluation. 
  */
  virtual void setFrequencyCounter(pmFrequencyCounter* pFrequencyCounter);

  /**
    Gets the frequency counter object used for evaluation. 
  */
  virtual pmFrequencyCounter* getFrequencyCounter();

protected:
  pmBayesianNetwork *pBN;
  pmFrequencyCounter *pFrequencyCounter;
  bool freeBN;

};
	};
#endif //SL_SCORE_H
/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 79
 * End:
 */
