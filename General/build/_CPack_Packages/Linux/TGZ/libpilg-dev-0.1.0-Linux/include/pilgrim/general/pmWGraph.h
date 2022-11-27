#ifndef PMWGRAPH_H
#define PMWGRAPH_H

#include <pl.h>
#include "pilgrim/general/pmUtility.h"
#include "pilgrim/general/pmBayesianNetwork.h"

namespace PILGRIM {
/**
 * \brief Weighted graphs class
 * \details This class represents weighted graphs to be used in bayesian networks
 */
class pmWGraph {
 protected:
    /**
     * \brief Property map for weights
     */
    boost::property_map<pmGraph, boost::edge_weight_t>::type weightMap;
    /**
     * \brief Property map for vertices ids
     */
    boost::property_map<pmGraph, boost::vertex_index_t>::type vertex_id;
    /**
     * \brief Internal graph
     */
    pmGraph graph;

 public:
    /**
     * \brief Default constructor
     * \param g Graph
     */
    pmWGraph(pmGraph g);
    /**
     * \brief Prints a summary of the weighted graph
     */
    void summary();
    void drawGraph();
    /**
     * \brief Returns the internal graph
     */
    pmGraph getGraph();
};
};  // namespace PILGRIM

#endif
