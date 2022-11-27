/*=============================================================================
 * Product        :
 * File           : AlgoTAN
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2020, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef SL_ALGO_TAN_H
#define SL_ALGO_TAN_H

#include <pilgrim/general/algorithms/Algorithm.h>
#include <pilgrim/general/scores/DecomposableScore.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <stdexcept>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/undirected_graph.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/cstdlib.hpp>

//=============================================================================

namespace PILGRIM
{

typedef boost::property<boost::edge_weight_t, double> EdgeWeightProperty;
typedef boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property, EdgeWeightProperty,
    boost::property<boost::vertex_color_t, boost::default_color_type,
                    boost::property<boost::vertex_degree_t, int,
                                    boost::property<boost::vertex_in_degree_t, int,
                                                    boost::property<boost::vertex_out_degree_t, int>>>>>
    UGraph;
typedef UGraph::edge_descriptor Edge;
typedef UGraph::vertex_descriptor Vertex;

/**
 *  Implementation of the Tree Augmented BN (TAN) algorithm
 */
template <typename rowDataType>
class pmAlgoTAN : public pmAlgorithm<rowDataType>
{
public:
  /**
   *  Constructor.
   */
  pmAlgoTAN(pmCSVDataSet *ds, pmDecomposableScore<rowDataType> *pScore, int classC, pmCache *cache = NULL, bool debugVerbose = false);

  /**
   *  Destructor.
   */
  virtual ~pmAlgoTAN();

  /**
   *  Computes the bayesian network structure by using the TAN algorithm
   */
  void run(pmBayesianNetwork *bn);

  /**
   *  Sets the score to be used by the computational method for structure
   *  learning.
   */
  void setScore(pmDecomposableScore<rowDataType> *pScore);

  /**
   *  Sets the class to be used by the computational method for structure
   *  learning.
   */
  void setClassVariable(int classC);

  /**
   *  Gets the score used by the computational method for structure learning.
   *  \return .
   */
  pmDecomposableScore<rowDataType> *getScore();

  /**
  *  Sets the root of the Depth First Search by index. Exceeding index will handled with modulo
  */
  void setRoot(size_t rootIdx);

  /**
   *  NO INITBN IN TAN, NEVER USE IT
   */
  pmBayesianNetwork *getInitialBN();

  void setInitialNetwork(pmBayesianNetwork *initialBN);

  /**
  *  Sets CPC Graph
  */
  void setCPCGraph(pmGraph *graphCPC);

  // Get debugVerbose
  bool getDebugVerbose();

  void setDebugVerbose(bool debugVerbose);

  plVariablesConjunction getVariables();

protected:
  /**
	*  Sets a random root of the Depth First Search by inde
	*/
  void setRandomRoot();

  /* Score used for structure learning */
  pmDecomposableScore<rowDataType> *pScore;

  /* Score stored to adjust epsilon value */
  pmScoreValueType oldBestDeltaScore;

  /* to the frequency of same best delta score occurrences, to adjust epsilon value */
  size_t freqCounterForBestDeltaScore;

  /* root index */
  size_t rootIdx;

  pmScoreValueType epsilon;

  int classVariable;

};
}; // namespace PILGRIM
//=============================================================================
#endif //SL_ALGO_TAN_H
// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
