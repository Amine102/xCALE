#include "pilgrim/general/pmWGraph.h"

using namespace std;
using namespace PILGRIM;
using namespace boost;
pmWGraph::pmWGraph(pmGraph g):graph(g) {}

void pmWGraph::summary() {
  boost::property_map<pmGraph, boost::vertex_index_t>::type vertex_id;
  boost::property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);

  pmEdgeIterator ei, ei_end;
  for (boost::tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei) {
    // cout<<edgeLabel[*ei]<<" w: "<<weightmap[*ei]<<"  ";
    std::cout << vertex_id[source(*ei, graph)] << " --" << weightMap[*ei]
              << "--> " << vertex_id[target(*ei, graph)] << " " << std::endl;
  }
}

void pmWGraph::drawGraph() {}

pmGraph pmWGraph::getGraph() {
  return graph;
}

