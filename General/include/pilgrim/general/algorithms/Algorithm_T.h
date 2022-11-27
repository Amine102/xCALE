/*=============================================================================
 * Product        : 
 * File           : Algorithm_T
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2019, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef SL_ALGORITHM_T_H
#define SL_ALGORITHM_T_H

#include <pl.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <pilgrim/general/scores/Score.h>
#include <vector>

namespace PILGRIM{
/** Abstract base-class for structure learning algorithms of transition data. */
template <typename rowDataType>
class  pmAlgorithm_T
{
public:
/*
=====CONSTRUCTORS=====
*/
  /** Instanciate an algorithm for learning the structure between
      the given variables according to the Frequency counter. */
  pmAlgorithm_T(pmFrequencyCounter* pFrequencyCounter);

  /** Instanciate an algorithm for learning the structure between
  the given variables according to the given dataset. */
  pmAlgorithm_T(pmCSVDataSet* dataset);
	
/*
=====DESTRUCTOR=====
*/
	virtual ~pmAlgorithm_T();

/*
=====GETTERS=====
*/
  /** Return the current frequency counter. */
  virtual pmFrequencyCounter* getFrequencyCounter();

 /** Returns the included edge set. */
  const std::vector< std::pair< unsigned int, unsigned int > >& getIncludedEdges();

  /** Returns the excluded edge set. */
  const std::vector< std::pair< unsigned int, unsigned int > >& getExcludedEdges();

 /** Gets the score which will be used in evaluating a bayesian network. It can
     be NULL. */
  pmScore<rowDataType>* getEvaluatingScore();

  /** Gets the debug verbose flag. */
  virtual bool getDebugVerbose() = 0;

  /** getTransitionBN */
  virtual pmBayesianNetwork* getTransitionBN() = 0;

/*
=====SETTERS=====
*/

  /** Changes the frequency counter. */
  virtual void setFrequencyCounter(pmFrequencyCounter* pFrequencyCounter);

	/** Changes the frequency counter from CSV. */
  virtual void setFrequencyCounterFromCSV(pmCSVDataSet* ds);

  /** 
    Sets the included edge set. Each edge in this set will be contained in the
    final joint distribution. An edge is represented as a pair of the node
    index and its parent index.
  */
  void setIncludedEdges(std::vector< std::pair< unsigned int, unsigned int > >& includedEdges);

  /** 
    Sets the excluded edge set. None of the edges present in this set will be
    contained in the final joint distribution. An edge is represented as a pair
    of the node index and its parent index.
  */
  void setExcludedEdges(std::vector< std::pair< unsigned int, unsigned int > >& excludedEdges);
  
  /** Sets the debug verbose flag. */
  virtual void setDebugVerbose(bool debugVerbose) = 0;


/*
=====METHODS=====
*/
  /** Run the algorithm and return the computed joint distribution. */
  /*DEPRICATED*/
  // virtual plJointDistribution compute();

  /** Run the algorithm and return the computed bayesian network.
      Must be implemented by derived classes. */
  virtual pmBayesianNetwork *run(pmBayesianNetwork* bn) = 0;

/*
=====PROTECTED ATTRIBUTES AND METHODS=====
*/
protected:
  /** 
    Sets the score which will be used in evaluating a bayesian network. 
    This can be set to NULL. 
  */
  void setEvaluatingScore(pmScore<rowDataType>* pEvaluatingScore);

   /** 
    Apply included and excluded edge set on a bayesian network. All included
    edges are added to the network (if @em applyIncludedEdges is set to @em
    true). Then all excluded edges are removed from the network(if @em
    applyExcludedEdges is set to @em true).  So, if an edge is present in both
    of the sets, it won't be present in the final network (in the case where
    both @em applyIncludedEdges and @em applyExcludedEdges are set to true).
  */
  void applyIncludedAndExcludedEdges(pmBayesianNetwork* pBN, 
                                     bool applyIncludedEdges = true, 
                                     bool applyExcludedEdges = true);

  /** 
    Returns true if the edge (parentIdx, nodeIdx) is in the list of included or
    excluded edges.  The edge list is selected via @em includeList parameter.
    If @em includeList is set to true, the included edge list is checked for
    the specified edge, otherwise is the excluded edge list.
   */
  bool isEdgeInList(unsigned int nodeIdx, unsigned int parentIdx, bool includeList = true);

/*
=====ATTRIBUTES=====
*/
  plVariablesConjunction variables;
  pmFrequencyCounter* pFrequencyCounter;
  std::vector< std::pair< unsigned int, unsigned int > > includedEdges;
  std::vector< std::pair< unsigned int, unsigned int > > excludedEdges;
  pmScore<rowDataType>* pEvaluatingScore;
  // bool debugVerbose;
};
};

#endif

/**
 * Creation: ????
 * Last update: 2019/06/11
 * Adding getTransitionlBN() as pure virtual
 * Modif : Côme Vitis
 */