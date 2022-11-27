/**
 * \file prm::algo::PMMixed.cpp
 */

#include <random>
#include <time.h>
#include <string>
#include <iostream>
#include <vector>

#include <boost/graph/connected_components.hpp>
#include <boost/graph/dag_shortest_paths.hpp>
#include <pilgrim/general/pmBayesianNetwork.h>
#include "pilgrim/general/random/PMMixed.h"

using namespace utils;
using namespace PILGRIM;

prm::algo::PMMixed::PMMixed()
{
}

prm::algo::PMMixed::~PMMixed()
{
}

DirectedGraph prm::algo::PMMixed::generateDAG(unsigned int nbNodes, unsigned int maxInducedWidth, unsigned int nbIterations, unsigned int maxNodeDegree, unsigned int maxNbEdges)
{
	this->graph.clear();

	//1-Create the directed and undirected graphs :
	if (nbNodes == 1)
	{
		boost::add_vertex(this->graph);
	}
	else
	{
		this->nbNodes = nbNodes;
		this->maxNodeDegree = maxNodeDegree;
		this->maxInducedWidth = maxInducedWidth;
		this->maxNbEdges = maxNbEdges;

		//1. The directed graph: all nodes have just one parent except the first one (a tree):
		for (unsigned int i = 0; i < nbNodes; i++)
		{
			boost::add_vertex(this->graph);
			if (i > 0)
			{
				boost::add_edge(i, i - 1, this->graph);
			}
		}

		//2
		std::random_device r;
		std::mt19937 eng(r());

		// a distribution that takes randomness and produces values in specified range
		std::uniform_real_distribution<> dist(0, 1);

		float p, q, pq;
		do
		{
			p = dist(eng);
			q = dist(eng);
			pq = p + q;

		} while (p == 0 || q == 0 || pq >= 1);

		for (size_t i = 0; (i < nbIterations || detectedCycle()); i++)
		{
			// Pick a random value
			float v = dist(eng);

			if (isPolytree()) //if the current graph is a polytree
			{
				if (v <= p)
				{
					this->AddOrRemove();
				}
				else if (v > p && v <= pq)
				{
					this->AddAndRemove();
				}
				else if (v > pq && v <= 1)
				{
					this->sequenceOfAOrR();
				}
			}
			else //the graph is a multi-connected
			{
				if (v <= pq)
				{
					this->AddOrRemove();
				}
				else
				{
					this->sequenceOfAOrR();
				}
			}
		}
	}

	return this->graph;
}

void prm::algo::PMMixed::generateRandomNodePair(unsigned int &i, unsigned int &j)
{
	std::random_device r;
	std::mt19937 eng(r());
	unsigned int max = boost::num_vertices(this->graph) - 1;
	std::uniform_int_distribution<> dist(0, max);

	do
	{
		i = dist(eng);
		j = dist(eng);
	} while (i == j);
}

bool prm::algo::PMMixed::isPolytree()
{
	return isSingleComponent() && !detectedCycle();
}

void prm::algo::PMMixed::sequenceOfAOrR()
{
	unsigned int i, j;
	this->generateRandomNodePair(i, j);

	bool exist = boost::edge(i, j, this->graph).second;

	if (isPolytree())
	{
		if (!exist)
		{
			boost::add_edge(i, j, this->graph);
			if (detectedCycle() || !satisfiesConstraints()) // If constraints are not satisfied, revert the changes
			{
				boost::remove_edge(i, j, this->graph);
			}
		}
	}
	else
	{
		if (exist)
		{
			boost::remove_edge(i, j, this->graph);
			// No need to check for constraints (max induced width, max node degree, and max number of edges) because removing an arc
			// from a graph satisfying constraints does not affect constraints satisfaction (Lemma 1 (Ide et al. 2004))
		}
	}
}

void prm::algo::PMMixed::AddAndRemove()
{
	unsigned int i, j;
	this->generateRandomNodePair(i, j);

	bool exist = boost::edge(i, j, this->graph).second;

	if (!exist)
	{
		typedef boost::graph_traits<utils::DirectedGraph>::vertex_descriptor Vertex;

		// Find predecessor of j on the path to i
		std::vector<Vertex> predecessors(nbNodes);
		boost::dag_shortest_paths(graph, i, boost::predecessor_map(&predecessors[0]));
		Vertex k = predecessors[j];
		// Remove the predecessor
		bool removePredecessor = (k != j);
		if (removePredecessor)
		{
			boost::remove_edge(k, j, graph);
		}

		int choice = rand() % 2;
		if (choice == 1)
		{
			// Swap i and j
			unsigned int temp = i;
			i = j;
			j = temp;
		}
		

		if (!boost::edge(i, j, this->graph).second)
		{
			boost::add_edge(i, j, graph);
		}

		if (detectedCycle() || !satisfiesConstraints()) // If constraints are not satisfied, revert the changes
		{
			if (boost::edge(i, j, this->graph).second)
			{
				boost::remove_edge(i, j, graph);
			}

			if (removePredecessor)
			{
				boost::add_edge(k, j, graph);
			}
		}
	}
}

void prm::algo::PMMixed::AddOrRemove()
{
	unsigned int i, j;
	this->generateRandomNodePair(i, j);

	bool exist = boost::edge(i, j, this->graph).second;

	if (!exist)
	{
		boost::add_edge(i, j, this->graph);
		if (detectedCycle() || !satisfiesConstraints())
		{
			boost::remove_edge(i, j, this->graph);
		}
	}
	else
	{
		boost::remove_edge(i, j, this->graph);
		if (!remainsConnected())
		{
			boost::add_edge(i, j, this->graph);
			// No need to check for constraints (max induced width, max node degree, and max number of edges) because removing an arc
			// from a graph satisfying constraints does not affect constraints satisfaction (Lemma 1 (Ide et al. 2004))
		}
	}
}

bool prm::algo::PMMixed::remainsConnected()
{
	utils::UndirectedGraph ug;

	unsigned int nbNodes = boost::num_vertices(this->graph);

	for (unsigned int i = 0; i < nbNodes; i++)
	{
		boost::add_vertex(ug);
	}

	boost::graph_traits<utils::DirectedGraph>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
	{
		auto s = source(*ei, graph);
		auto t = target(*ei, graph);

		boost::add_edge(s, t, ug);
	}

	std::vector<int> component(nbNodes);
	int num = connected_components(ug, &component[0]);

	return (num == 1);
}

bool prm::algo::PMMixed::detectedCycle()
{

	bool has_cycle = false;
	cycle_detector vis(has_cycle);
	boost::depth_first_search(this->graph, boost::visitor(vis));

	return has_cycle;
}

bool prm::algo::PMMixed::satisfiesConstraints()
{

	unsigned int nbEdges = boost::num_edges(this->graph);
	// Check constraint 1
	bool satisfies = (nbEdges <= maxNbEdges);

	// Check constraint 2
	if (satisfies)
	{
		unsigned int nbNodes(boost::num_vertices(this->graph));
		// check maxNodeDegree
		for (unsigned int i = 0; i < nbNodes; i++)
		{
			unsigned int degree = boost::degree(i, this->graph);
			if (maxNodeDegree < degree)
			{
				satisfies = false;
				break;
			}
		}
	}

	// Check constraint 3
	if (satisfies)
	{
		unsigned int inducedWidth = getInducedWidth();
		satisfies = (inducedWidth <= maxInducedWidth);
	}

	return satisfies;
}

unsigned int prm::algo::PMMixed::getInducedWidth()
{
	// Heuristic (Ide et al. ): if the DAG is a polytree, then the heuristic width is equal to the induced width.
	// Even if a given heuristic does not satisfy this property, the heuristic can be easily modified to do so:
	// 		test whether the DAG is a polytree and, if so, return the induced width of the polytree (the maximum number of parents amongst all nodes).

	unsigned int inducedWidth = 0;
	if (isPolytree())
	{
		inducedWidth = getMaxInDegree(this->graph);
	}
	else
	{
		// UndirectedGraph ug;
		// getMoralGraph(ug, this->graph);
		// inducedWidth = getMaxDegree(ug);
	}
	return inducedWidth;
}

bool prm::algo::PMMixed::isSingleComponent()
{
	return (this->remainsConnected());
}

utils::DirectedGraph prm::algo::PMMixed::connectSubDAGs(const utils::DirectedGraph &subDAGs, const std::map<unsigned int, std::string> &nodeToClassMap, unsigned int maxInducedWidth, unsigned int maxNodeDegree, unsigned int maxNbEdges)
{
	this->graph = subDAGs;
	this->maxInducedWidth = maxInducedWidth;
	this->maxNbEdges = maxNbEdges;
	this->maxNodeDegree = maxNodeDegree;

	std::set<std::string> classes;
	// Extract classes
	for (auto it(nodeToClassMap.begin()), it_end(nodeToClassMap.end()); it != it_end; ++it)
	{
		classes.insert(it->second);
	}

	unsigned int nbNodes = boost::num_vertices(subDAGs);

	std::random_device r;
	std::mt19937 eng(r());

	// a distribution that takes randomness and produces values in specified range
	std::uniform_real_distribution<> dist(0, 1);

	// Probability of adding an interclass edge such that the graph remains acyclic; 1-p is the probability of adding an interclass edge, and removing an edge if a cycle is created
	double p;

	do
	{
		p = dist(eng);
	} while (p == 1);

	typedef boost::graph_traits<utils::DirectedGraph>::vertex_descriptor Vertex;
	bool singleComponent = isSingleComponent();

	while (!singleComponent)
	{

		unsigned int i, j;
		this->generateRandomNodePair(i, j);

		// If there are more than one class, and i and j are in the same class, regenerate i and j
		if ((classes.size() > 1 && nodeToClassMap.at(i) == nodeToClassMap.at(j)))
		{
			continue;
		}

		// A random value to determine whether to add i -> j or j <- i.
		double e = dist(eng);

		if (e > 0.5)
		{
			// swap i, j
			unsigned int temp = i;
			i = j;
			j = temp;
		}
		//  if an edge exists between i and j, re-start
		if (boost::edge(i, j, this->graph).second)
		{
			continue;
		}

		// A random value to determine whether to add or addAndRemove
		double v = dist(eng);

		if (v <= p) // Add
		{

			boost::add_edge(i, j, graph);

			if (detectedCycle() || !satisfiesConstraints())
			{
				boost::remove_edge(i, j, graph);
			}
		}
		else // Add and remove
		{
			boost::add_edge(i, j, graph);
			if (!detectedCycle() && !satisfiesConstraints())
			{
				// revert
				boost::remove_edge(i, j, graph);
			}
			else
			{
				while (detectedCycle())
				{
					boost::remove_edge(i, j, graph);

					// Find predecessor of i on the path to j (to avoid cycle)
					std::vector<Vertex> predecessors(nbNodes);
					boost::dag_shortest_paths(graph, j, boost::predecessor_map(&predecessors[0]));
					Vertex k = predecessors[i];

					// Remove the predecessor
					if (k != i)
					{
						boost::remove_edge(k, i, graph);
						boost::add_edge(i, j, graph);
					}
				}
			}
		}

		singleComponent = isSingleComponent();
	}

	return this->graph;
}

// pmGraph prm::algo::PMMixed::GetEquivalentPmGraph(){
	// pmGraph g;

	// unsigned int nbNodes = boost::num_vertices(this->graph);

	// for (unsigned int i = 0; i < nbNodes; i++ )
	// {
	// 	boost::add_vertex(g);
	// }

	// boost::graph_traits<utils::DirectedGraph>::edge_iterator ei, ei_end;
	// for (boost::tie(ei, ei_end) = boost::edges(graph); ei != ei_end; ++ei)
	// {
	// 	auto s = source(*ei, graph);
	// 	auto t = target(*ei, graph);

	// 	boost::add_edge(s, t, ug);
	// }

	// for (j = 0; j < n; j++)
	// 	{
	// 		if (j == i)
	// 			continue;
	// 		boost::tie(e, exists) = edge(vertex(i, graph), vertex(j, graph), graph);
	// 		if (exists)
	// 			cout << this->getVariables()[j].name() << " ";
	// 	}
	// 	add_edge(vertex(i, graph), vertex(j, graph), graph);


	// return g;
// 	return g;
// }

void prm::algo::PMMixed::printGraph()
{
	// typedef boost::graph_traits<DirectedGraph>::vertex_descriptor Vertex;
	// unsigned int nbNodes = boost::num_vertices(this->graph);

	// for (Traits::vertices_size_type v_index = 0;
	// 	 v_index < num_vertices(graph); ++v_index)
	// {

	// 	// The two indices should always be equal
	// 	std::cout << "Index of vertex " << v_index << " is " << get(boost::vertex_index, graph, vertex(v_index, graph)) << std::endl;
	// }


	// boost::graph_traits<utils::DirectedGraph>::vertex_iterator vi, vi_end;
	// for (boost::tie(vi, vi_end) = boost::edges(this->graph); vi != vi_end; ++vi)
	// {
	// 	auto s = source(*vi, this->graph);
	// 	auto t = target(*vi, this->graph);

	// 	boost::add_edge(s, t, ug);
	// }

	// unsigned int i, j, n;
	// bool exists;
	// pmEdge e;
	// n = (unsigned int)boost::num_vertices(graph);
	// for (i = 0; i < n; i++)
	// {
	// 	cout << this->getVariables()[i].name() << " -->  ";
	// 	for (j = 0; j < n; j++)
	// 	{
	// 		if (j == i)
	// 			continue;
	// 		boost::tie(e, exists) = edge(vertex(i, graph), vertex(j, graph), graph);
	// 		if (exists)
	// 			cout << this->getVariables()[j].name() << " ";
	// 	}
	// 	cout << endl;
	// }
}
