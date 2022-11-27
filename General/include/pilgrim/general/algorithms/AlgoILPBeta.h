/*=============================================================================
 * Product        : 
 * File           : AlgoILP
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef SL_ALGO_ILP_H
#define SL_ALGO_ILP_H


#include <pilgrim/general/algorithms/Algorithm.h>
#include <pilgrim/general/scores/DecomposableScore.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <stdexcept>



//=============================================================================

namespace PILGRIM{



/**
 *  Algo container for ILP for exact structure learning- This class has no implemenation for run(), but is meant 
 *  as a template and provides only a method to calculate each local score 
 */
template <typename rowDataType>
class  pmAlgoILP: public pmAlgorithm<rowDataType>
{
public:
  /**
   *  Constructor.
   */
	pmAlgoILP(pmCSVDataSet* ds, pmDecomposableScore<rowDataType>* pScore, pmCache* cache = NULL, bool debugVerbose = false);

  /**
   *  Destructor.
   */
  virtual ~pmAlgoILP();

  /**
   *  This method is a placeholder for the real implementation of the ILP - returns empty network
   */
  void run(pmBayesianNetwork* bn);

  /**
  *  Compute all possible local scores for each variable and write to file in GOBNILP format
  */
  void computeAllLocalScores(const std::string &file_name);

  /**
  * TODO: Make as optional parameter for computeAllLocalScores method or "under the hood"
  *  Compute all possible local scores constrained by the CPC for each variable and write to file in GOBNILP format
  */
  void computeAllLocalScoresUseCPC(const std::string &file_name, CPC_t CPCs);


  /**
   *  Sets the score to be used by the computational method for structure
   *  learning.
   */
  void setScore(pmDecomposableScore<rowDataType>* pScore);

  /**
   *  Gets the score used by the computational method for structure learning.
   *  \return .
   */
  pmDecomposableScore<rowDataType>* getScore();


protected:

  /* Score used for structure learning */
  pmDecomposableScore<rowDataType>* pScore;

   /* Score stored to adjust epsilon value */
  pmScoreValueType oldBestDeltaScore;

  /* to the frequency of same best delta score occurrences, to adjust epsilon value */
  size_t freqCounterForBestDeltaScore;

  pmScoreValueType epsilon;
};
	};
//=============================================================================
#endif //SL_ALGO_ILP_H
// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:

