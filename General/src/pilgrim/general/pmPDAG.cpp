#include "pilgrim/general/pmPDAG.h"

using namespace boost;
using namespace std;
using namespace PILGRIM;

pmPDAG::pmPDAG(pmGraph g): pmWGraph(DAGtoCPDAG(g)) {}

pmPDAG::pmPDAG(pmWGraph wg): pmWGraph(DAGtoCPDAG(wg.getGraph())) {}

bool pmPDAG::isExistZYedgeAndZnotParentOfX(pmNode& x,
                                           pmNode& y,
                                           pmEdge* xyEdge,
                                           pmGraph& graphArg) {
  pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
  for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(y, graph);
       inEdgeItr != inEdgeItrEnd;
       ++inEdgeItr) {
    pmNode w = boost::source(*inEdgeItr, graphArg);
    if (*inEdgeItr != *xyEdge && !isParent(w, x, graph, NULL)) {
      // if z != x(means it is not the same edge) and z is not a parent of x
      return true;
    }
  }  // endfor
  return false;
}

bool pmPDAG::isParent(pmNode& w, pmNode& y, pmGraph& g, pmEdge* wyEdge) {
  pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
  for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(y, g);
       inEdgeItr != inEdgeItrEnd;
       ++inEdgeItr) {
    if (boost::source(*inEdgeItr, g) == w) {  // if w is a parent of y
      pmEdge wyE = *inEdgeItr;
      wyEdge = &wyE;
      return true;
    } else {
      wyEdge = NULL;
      return false;
    }
  }
}

void pmPDAG::orderEdges(pmGraph& g, std::vector<pmNode>& ndList) {
  // topological sort will gives the order from lowest to highest
  // we need the highest node toward lowest so, we will reverse the order
  // and then we will check the first(highest) node towards lowest
  // by using out_edges_list.
  ndList.clear();

  try {
    topological_sort(g, std::back_inserter(ndList));
  } catch (not_a_dag &e) {
    cout << " \n Cycle found" <<endl;
  }
  std::reverse(ndList.begin(), ndList.end());
}

void pmPDAG::labelEdges(pmEdge* xyEdge,
                        pmNode& y,
                        pmGraph& g,
                        std::string label,
                        bool onlyUnknownEdges,
                        bool incidentEdgesToo) {
  boost::property_map<pmGraph, edge_name_t>::type edgeNameMap = get(edge_name, g);
  boost::property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, g);

  int w;

  if (label.compare("COMPELLED") == 0)
    w = 2;
  else
    w = 1;

  if (xyEdge!= NULL) {
    edgeNameMap[*xyEdge] = label;
    weightmap[*xyEdge] = w;
  }

  if (incidentEdgesToo) {
    // find edges incident of y
    pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
    for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(y, g);
         inEdgeItr != inEdgeItrEnd;
         ++inEdgeItr) {
      if (onlyUnknownEdges) {
        if (edgeNameMap[*inEdgeItr].compare("UNKNOWN") == 0) {
          edgeNameMap[*inEdgeItr] = label;
          weightmap[*inEdgeItr] = w;
        }
      } else {
        edgeNameMap[*inEdgeItr] = label;
        weightmap[*inEdgeItr] = w;
      }
    }  // endfor
  }
}


/* Algorithm DAG-to-CPDAG ,chickering(1995)
 * Input: DAG graph
 * OutPut: DAG graph with each edge labeled either "COMPELLED" or "REVERSIBLE"
 *
 * Order the edges in Graph
 * Label every edge in Graph as "UNKNOWN"
 * While there are edges labeled "UNKNOWN" in Graph
 *      let x->y be the lowest ordered edge that is labeled "UNKNOWN"
 *      For every edge w->x labeled "COMPELED"
 *          IF w is not a parent of y then
 *             label x->y and every edge  incident into y with "COMPELLED"
 *             GO TO While;
 *          ELSE
 *             Label w-> with "COMPELLED"
 *
 *  IF there exists an edge z->y such that z!=x and z is not a parent of x then
 *      Label x->y and all "UNKNOWN" edges incident into y with "COMPELLED"
 *  ELSE
 *      Label x->y and all "UNKNOWN" edges incident into y with "REVERSIBLE"
 **/
pmGraph pmPDAG::DAGtoCPDAG(pmGraph g) {
  boost::property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, g);
  boost::property_map<pmGraph, edge_name_t>::type name = get(edge_name, g);
  std::vector<pmNode> nodeList;

  // 1-order nodes using topological sort
  this->orderEdges(g, nodeList);

  // 2-label every edge of g as "UNKNOWN"
  pmEdgeIterator ei, ei_end;
  for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
    name[*ei] = "UNKNOWN";
    weightmap[*ei] = 1;
    }  // end for

  // 3-
  std::vector<pmNode>::iterator nodeListItr;
  pmNode x, y, w, z;
  pmEdge *xyEdge = NULL;

  for (nodeListItr = nodeList.begin();
       nodeListItr != nodeList.end();
       ++nodeListItr) {
    bool flag = false;
    x = *nodeListItr;
    pmOutEdgeIterator outEdgeItr, outEdgeItrEnd;
    for (boost::tie(outEdgeItr, outEdgeItrEnd) = out_edges(x, g);
         outEdgeItr!= outEdgeItrEnd;
         ++outEdgeItr) {  // to get x-->y edges
      y = boost::target(*outEdgeItr, g);
      pmEdge xyE = *outEdgeItr;
      xyEdge = &xyE;  // out_edges, it is already sorted from lowest to highest

      name = get(edge_name, g);
      if (name[*outEdgeItr].compare("UNKNOWN") == 0) {
        // for every edge w-->x labeled "compelled"
        pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
        for (boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(x, g);
             inEdgeItr != inEdgeItrEnd;
             ++inEdgeItr) {
          if (name[*inEdgeItr].compare("COMPELLED") == 0) {
            w = boost::source(*inEdgeItr, g);
            pmEdge* wyEdge = NULL;
            if (!isParent(w, y, g, wyEdge)) {
              labelEdges(xyEdge, y, g, "COMPELLED", false, true);
              flag = true;
              break;
            } else {
              labelEdges(wyEdge, y, g, "COMPELLED", false, false);
            }
          }  // endif
        }  // endfor
        if (!flag) {
          if (isExistZYedgeAndZnotParentOfX(x, y, xyEdge, g))
            labelEdges(xyEdge, y, g, "COMPELLED", true, true);
          else
            labelEdges(xyEdge, y, g, "REVERSIBLE", true, true);
        }
      }  // endif
    }  // end for
  name = get(edge_name, g);
  }  // end for

  return g;
}

map<string, unsigned int> pmPDAG::compare(pmPDAG pdagCpm) {
  // Return parameter
  map<string, unsigned int> hamming = map<string, unsigned int>();
  hamming["shd"] = 0;
  hamming["tp"] = 0;
  hamming["fp"] = 0;
  hamming["tn"] = 0;
  hamming["fn"] = 0;
  pmEdgeIterator ei, ei_end;
  pmGraph graphCmp = pdagCpm.getGraph();
  for (boost::tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei) {
    bool equi = false;
    pmEdge toRem;
    pmNode s = vertex(vertex_id[source(*ei, graph)], graphCmp);
    pmNode t = vertex(vertex_id[target(*ei, graph)], graphCmp);
    int w = weightMap[*ei];
    pmEdge ec;
    // Finding s->t
    ec = boost::edge(s, t, graphCmp).first;
    if (boost::edge(s, t, graphCmp).second) {
      equi = true;
      toRem = ec;
      pmNode sC = source(ec, graphCmp);
      pmNode tC = target(ec, graphCmp);
      int wC = weightMap[ec];
      if (w != wC)
        hamming["shd"] += 1;
    }

    // Finding t->s
    ec = boost::edge(t, s, graphCmp).first;
    if (boost::edge(t, s, graphCmp).second) {
      equi = true;
      toRem = ec;
      pmNode sC = source(ec, graphCmp);
      pmNode tC = target(ec, graphCmp);
      int wC = weightMap[ec];
      if (w != wC || w == 2)
        hamming["shd"] += 1;
    }
    if (!equi) {
      hamming["shd"] += 2;
      hamming["fn"] += 1;
    } else {
      remove_edge(toRem, graphCmp);
    }
    hamming["tp"] += 1;
  }

  for (boost::tie(ei, ei_end) = edges(graphCmp); ei != ei_end; ++ei) {
    hamming["shd"] += 2;
    hamming["fp"] += 1;
  }

  int n = num_vertices(graph);
  hamming["tn"] = (n*n-n)/2 - hamming["tp"] - hamming["fp"] - hamming["fn"];
  return hamming;
}

