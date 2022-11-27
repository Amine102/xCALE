#include "pilgrim/general/algorithms/AlgoMWST.h"
#include <vector>
#include <string>
#include <plDebug.h>
#include <sstream>

using namespace std;
using namespace boost;
using namespace PILGRIM;

//=============================================================================
template <typename rowDataType>
pmAlgoMWST<rowDataType>::pmAlgoMWST(pmCSVDataSet *ds, pmDecomposableScore<rowDataType> *pScore, pmCache *cache, bool debugVerbose)
	: pmAlgorithm<rowDataType>(ds)
{
	this->setScore(pScore);
	this->pScore->cache = cache;
	this->setDebugVerbose(debugVerbose);
	this->setRandomRoot();
}

//=============================================================================
template <typename rowDataType>
pmAlgoMWST<rowDataType>::~pmAlgoMWST()
{
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMWST<rowDataType>::run(pmBayesianNetwork* computedBN)
{

	/* Initial bayesian network */
	// pmBayesianNetwork *computedBN = new pmBayesianNetwork(this->variables);

	/* Number of nodes */
	int n = computedBN->num_vertices();

	/* Fill the score with BN and FC*/
	this->pScore->setBayesianNetwork(computedBN);
	this->pScore->setFrequencyCounter(this->getFrequencyCounter());

	/* set up and empty undirected graph for Kruskal */
	UGraph ug(n);

	if (this->debugVerbose)
	{
		std::cout << "\nCalculation of weights for each edge: \n";
	}

	/* traverse upper half of matrix*/
	for (int row = 0; row < n - 1; ++row)
	{
		for (int col = row + 1; col < n; ++col)
		{

			/*Get the current node pair*/
			pmNode A = computedBN->getNode(computedBN->getVariable(row));
			pmNode B = computedBN->getNode(computedBN->getVariable(col));
			//std::cout << "Node no. A: " << row << "\t Node no. B: " << col << endl;

			/* Calculate the Score without parents */
			pmScoreValueType sc0 = this->pScore->computeNode(A);
			//std::cout << "WIHOUT parents : "<< sc0 << "  ";

			/* add the edge to the network */
			computedBN->add_arrow(B, A);

			/*Calculate the Score with one parent */
			pmScoreValueType sc1 = this->pScore->computeNode(A);

			if (this->debugVerbose)
			{
				std::cout << "Weight for : " << row << " <--> " << col << "  =  " << sc0 - sc1 << "\n";
			}

			/* remove edge for next iteration*/
			computedBN->remove_arrow(B, A);

			/* Add the edge to the undirected Graph with weight (inverse since MINIMIMUM Spanning Tree)*/
			boost::add_edge(row, col, sc0 - sc1, ug);
		}
	}

	/* store Edge weight*/
	boost::property_map<UGraph, boost::edge_weight_t>::type EdgeWeightMap = get(boost::edge_weight_t(), ug);

	struct InSpanning
	{
		std::set<Edge> edges;
		bool operator()(Edge e) const { return edges.count(e); }
	} spanning;

	/* run Kruskal*/
	boost::kruskal_minimum_spanning_tree(ug, std::inserter(spanning.edges, spanning.edges.end()));

	/* Select only edges from spanning tree */
	boost::filtered_graph<UGraph, InSpanning, boost::keep_all> mst(ug, spanning, {});

	/* Array to store predecessor (parent) of each vertex */
	std::vector<Vertex> p(n);

	/*Get root */
	Vertex root = this->rootIdx;

	/* set roots parent to itself*/
	p[root] = root;

	/*Run DFS on MWST with given root and store predecessors */
	boost::depth_first_search(mst,
							  boost::visitor(boost::make_dfs_visitor((boost::record_predecessors(&p[0], boost::on_tree_edge()))))
								  .root_vertex(root));

	/* Summary of MWST */
	if (this->debugVerbose)
	{
		std::cout << "\nStart Depth First Search from root : " << root << "\n";
		for (std::size_t i = 0; i < p.size(); ++i)
		{
			cout << i << "  child ----- of " << p[i] << "\n";
		}
	}

	/* Add to BN */
	for (std::size_t i = 0; i < p.size(); ++i)
	{

		// not if root
		if (p[i] != i)
		{
			pmNode A = computedBN->getNode(computedBN->getVariable(i));
			pmNode B = computedBN->getNode(computedBN->getVariable(p[i]));

			/* add edge to directed graph ! */
			computedBN->add_arrow(B, A);
		}
	}

	/* Returning the computed bayesian network */
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMWST<rowDataType>::setScore(pmDecomposableScore<rowDataType> *pScore)
{
	/* Setting a particular score */
	this->pScore = pScore;
	this->setEvaluatingScore(pScore);
}

//=============================================================================
template <typename rowDataType>
pmDecomposableScore<rowDataType> *pmAlgoMWST<rowDataType>::getScore()
{
	/* Getting the used score */
	return this->pScore;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMWST<rowDataType>::setRoot(size_t rootIdx)
{
	/* Setting a particular root */
	this->rootIdx = rootIdx % this->variables.size();
}
//=============================================================================
template <typename rowDataType>
bool pmAlgoMWST<rowDataType>::getDebugVerbose()
{
  return this->debugVerbose;
};
//=============================================================================
template <typename rowDataType>
pmBayesianNetwork *pmAlgoMWST<rowDataType>::getInitialBN()
{ 
	//EMPTY FUNCTION. NO INITBN IN MWST!!!!!!!!!!
}
//=============================================================================

template <typename rowDataType>
void pmAlgoMWST<rowDataType>::setInitialNetwork(pmBayesianNetwork *initialBN)
{
	//EMPTY FUNCTION. NO INITBN IN MWST!!!!!!!!!!
}
//=============================================================================
template <typename rowDataType>
void pmAlgoMWST<rowDataType>::setCPCGraph(pmGraph *graphCPC)
{
	/* Setting a CPC Graph */
	//this->graphCPC = graphCPC;
}
//=============================================================================
template <typename rowDataType>
void pmAlgoMWST<rowDataType>::setDebugVerbose(bool debugVerbose)
{
  this->debugVerbose = debugVerbose;
}
//=============================================================================
template <typename rowDataType>
void pmAlgoMWST<rowDataType>::setRandomRoot()
{
	/* Setting a random root */
	random_device rd;
	mt19937 rng(rd());
	uniform_int_distribution<int> uni(0, this->variables.size() - 1);
	auto rand_int = uni(rng);
	this->rootIdx = rand_int;
}
//=============================================================================
template <typename rowDataType>
plVariablesConjunction pmAlgoMWST<rowDataType>::getVariables(){
	return this->variables;
}

//=============================================================================

namespace PILGRIM {
    template class pmAlgoMWST<std::vector<unsigned int>>;
    template class pmAlgoMWST<std::vector<int>>;
    template class pmAlgoMWST<std::vector<float>>;
    template class pmAlgoMWST<std::vector<double>>;
    template class pmAlgoMWST<std::vector<long double>>;

    template class pmAlgoMWST<unsigned int*>;
    template class pmAlgoMWST<int*>;
    template class pmAlgoMWST<float*>;
    template class pmAlgoMWST<double*>;
    template class pmAlgoMWST<long double*>;
}; // namespace PILGRIM
