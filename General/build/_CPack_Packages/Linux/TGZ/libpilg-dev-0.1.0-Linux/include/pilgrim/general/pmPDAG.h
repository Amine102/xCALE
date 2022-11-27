#ifndef PMPDAG_H
#define PMPDAG_H


#include <string>
#include <map>
#include "pilgrim/general/pmUtility.h"
#include "pilgrim/general/pmWGraph.h"


namespace PILGRIM {
/**
 * \brief PDAG class
 * \details Implements PDAGs using weights on edges
 *          Meaning of weights:
 *             A --2--> B : A is the parent of B
 *             A --1--> B : A and B are related (equivalent to A <--1-- B)
 */
class pmPDAG : public pmWGraph {
 public:
    /**
     * \brief Constructor from a pmGraph
     * \details Takes a pmGraph and compute the direction (i.e. weights) of edges
     */
    pmPDAG(pmGraph);
    /**
     * \brief Constructor from a pmWGraph
     * \details Takes a pmWGraph and compute the direction (i.e. weights) of edges
     */
    pmPDAG(pmWGraph);
    /**
    * \brief Compare two DAG
    * \details Compare two DAG and return a map containing five parameters :
    * -shd : structural hamming distance, see below
    * -tp : true positive, if an edge is shared between the two graph regardless of the direction
    * -fp : false positive, edge present in the compared graph but not in the original graph
    * -fn : false negative,  edge present in the original graph but not in the compared graph
    * -tn : true negative, number of edge that not exist into both graph
    *
    * the shd is caculated  following those rules :
    * -distance does not change if an edge is in the two graph, in regard of the type and the direction
    * -distance +1 if graphs shared a edge but one is oriented and not the other
    * -distance +1 if graphs shared a common oriented edge but the direction is different
    * -distance +2 if an edge exists in only one graph
    *http://dx.doi.org/10.1007/s10994-006-6889-7
    **/
    std::map<std::string, unsigned int> compare(pmPDAG graphCpm);
    pmGraph DAGtoCPDAG(pmGraph);

 private:
    bool isExistZYedgeAndZnotParentOfX(pmNode& x,
                                       pmNode& y,
                                       pmEdge* xyEdge,
                                       pmGraph& graphArg);
    bool isParent(pmNode& w,
                  pmNode& y,
                  pmGraph& g,
                  pmEdge* wyEdge);
    void orderEdges(pmGraph& g, std::vector<pmNode>& ndList);
    void labelEdges(pmEdge* xyEdge,
                    pmNode& y,
                    pmGraph& g,
                    std::string label,
                    bool onlyUnknownEdges,
                    bool incidentEdgesToo);
};
};  // namespace PILGRIM

#endif
