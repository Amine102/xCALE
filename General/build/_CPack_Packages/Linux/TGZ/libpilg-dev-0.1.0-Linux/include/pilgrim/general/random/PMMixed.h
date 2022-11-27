#ifndef PMMIXED_H
#define PMMIXED_H

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <pilgrim/general/random/GraphUtility.h>
#include <pilgrim/general/pmBayesianNetwork.h>

using namespace PILGRIM;

namespace prm
{
namespace algo
{
class PMMixed
{
public:
  PMMixed();
  virtual ~PMMixed();

  utils::DirectedGraph generateDAG(unsigned int nbNodes, unsigned int maxInducedWidth, unsigned int nbIterations, unsigned int maxNodeDegree, unsigned int maxNbEdges);
  utils::DirectedGraph connectSubDAGs(const utils::DirectedGraph &subDAGs, const std::map<unsigned int, std::string> &nodeToClassMap, unsigned int maxInducedWidth, unsigned int maxNodeDegree, unsigned int maxNbEdges);
  void printGraph();
  // pmGraph GetEquivalentPmGraph();

  utils::DirectedGraph graph;

private:
  void generateRandomNodePair(unsigned int &i, unsigned int &j);

  void AddAndRemove();
  void AddOrRemove();
  void sequenceOfAOrR();

  bool isPolytree();
  bool isSingleComponent();
  bool detectedCycle();
  bool remainsConnected();

  bool satisfiesConstraints();
  unsigned int getInducedWidth();

  unsigned int nbNodes;
  unsigned int maxInducedWidth;
  unsigned int maxNodeDegree;
  unsigned int maxNbEdges;
};
} // namespace algo
} // namespace prm
#endif