/*=============================================================================
 * Product        : 
 * File           : Algorithm
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef SL_ALGORITHM_H
#define SL_ALGORITHM_H

#include <pl.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <pilgrim/general/scores/Score.h>
#include <vector>

namespace PILGRIM{

/** Abstract base-class for structure learning algorithms of initial data. */
template <typename rowDataType>
class  pmAlgorithm
{
public:
/*
=====CONSTRUCTOR=====
*/
  /** Instanciate an algorithm for learning the structure between
      the given variables according to the Frequency counter. */
  pmAlgorithm(pmFrequencyCounter* pFrequencyCounter);

  /** Instanciate an algorithm for learning the structure between
  the given variables according to the given dataset. */
  pmAlgorithm(pmCSVDataSet* dataset);

/*
=====DESTRUCTOR=====
*/
  /** Destructor. */
  virtual ~pmAlgorithm();

/*
=====GETTER=====
*/
  /* getInitialBN */
  virtual pmBayesianNetwork* getInitialBN() = 0;
  
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

/*
=====SETTER=====
*/
  /** Changes the frequency counter. */
  virtual void setFrequencyCounter(pmFrequencyCounter* pFrequencyCounter);

  /** Changes the frequency counter from CSV. */
  virtual void setFrequencyCounterFromCSV(pmCSVDataSet* ds);

  /** Sets the included edge set. Each edge in this set will be contained in the
      final joint distribution. An edge is represented as a pair of the node
      index and its parent index.
  */
  void setIncludedEdges(std::vector< std::pair< unsigned int, unsigned int > >& includedEdges);

  /** Sets the excluded edge set. None of the edges present in this set will be
      contained in the final joint distribution. An edge is represented as a pair
      of the node index and its parent index.
  */
  void setExcludedEdges(std::vector< std::pair< unsigned int, unsigned int > >& excludedEdges);

  virtual void setDebugVerbose(bool) = 0;

/*
=====METHODS=====
*/
  /** Run the algorithm and return the computed joint distribution. */
  /*DEPRICATED*/
  // virtual plJointDistribution compute();

  /** Run the algorithm and return the computed bayesian network.
      Must be implemented by derived classes. */
  virtual void run(pmBayesianNetwork* bn) = 0;

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
  void applyIncludedAndExcludedEdges( pmBayesianNetwork* pBN, 
                                      bool applyIncludedEdges = true, 
                                      bool applyExcludedEdges = true);

  /** 
    Returns true if the edge (parentIdx, nodeIdx) is in the list of included or
    excluded edges.  The edge list is selected via @em includeList parameter.
    If @em includeList is set to true, the included edge list is checked for
    the specified edge, otherwise is the excluded edge list.
   */
  bool isEdgeInList(unsigned int nodeIdx, 
                    unsigned int parentIdx, 
                    bool includeList = true);


  plVariablesConjunction variables;
  pmFrequencyCounter* pFrequencyCounter;
  std::vector< std::pair< unsigned int, unsigned int > > includedEdges;
  std::vector< std::pair< unsigned int, unsigned int > > excludedEdges;
  pmScore<rowDataType>* pEvaluatingScore;
  bool debugVerbose;
};
	};

#endif //SL_ALGORITHM_H

/*
 * Local Variables:
 * mode: c++
 * indent-tabs-mode: nil
 * c-basic-offset: 2
 * fill-column: 79
 * End:
 */


/**
 * Creation: ????
 * Last update: 2019/06/11
 * Adding getInitialBN() as pure virtual
 * Adding getDebug and setDebug as pure virtual
 * Modif : Côme Vitis
 */