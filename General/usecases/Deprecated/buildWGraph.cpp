#include "buildWGraph.h"
#include <vector>
#include <string>


using namespace std;
using namespace PILGRIM;

void buildWGraph() {
  // Construct Asia bayesian network
  plJointDistribution jd;
  plJointDistribution jd2;
  jd.load_from_genie("benchmarks/networks/Asia.xdsl");
  pmBayesianNetwork bnAsia(jd);

  std::cout << "==================" << std::endl
            << "bnAsia summary" << std::endl;
  bnAsia.summary();
  pmGraph asiaGraph = bnAsia.get_graph();
  pmWGraph asiaWeightedGraph = pmWGraph(asiaGraph);

  std::cout << "==================" << std::endl
            <<  "asiaWG summary" << std::endl;
  asiaWeightedGraph.summary();
  pmPDAG asiaPDAG = pmPDAG(asiaGraph);

  std::cout << "==================" << std::endl
            <<  "asiaPDAG summary" << std::endl;
  asiaPDAG.summary();

  std::cout << "==================" << std::endl
            << "asiaPDAG compare score" << std::endl
            << "asiaPDAG modified summary" << endl;
  jd2.load_from_genie("benchmarks/networks/Asia_modified.xdsl");
  pmBayesianNetwork bnAsia2(jd2);
  pmPDAG asiaPDAG2 = pmPDAG(bnAsia2.get_graph());
  asiaPDAG2.summary();
  cout << "Distance : "<< asiaPDAG.compare(asiaPDAG2)["shd"] << endl;
  // asiaWeightedGraph.draw_graph("benchmark/networks/Asia.dot");
}

