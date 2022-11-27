/* ###################################################################
* Project :                                                          *
* Function : Algorithme d'apprentissage TAN Classifieur              *
* Author : Soline PONSARD                                            *
* Copyright juillet 2020                                             *
#################################################################### */

#include "pilgrim/general/algorithms/AlgoTAN.h"
#include <vector>
#include <string>
#include <plDebug.h>
#include <sstream>

using namespace std;
using namespace boost;
using namespace PILGRIM;

//=============================================================================
template <typename rowDataType>
pmAlgoTAN<rowDataType>::pmAlgoTAN(pmCSVDataSet *ds, pmDecomposableScore<rowDataType> *pScore,int classC, pmCache *cache, bool debugVerbose)
: pmAlgorithm<rowDataType>(ds)
{
	this->setScore(pScore);
	this->pScore->cache = cache;
	this->setDebugVerbose(debugVerbose);
	this->setRandomRoot();
	this->setClassVariable(classC);
}

//=============================================================================
template <typename rowDataType>
pmAlgoTAN<rowDataType>::~pmAlgoTAN()
{
}

//=============================================================================
template <typename rowDataType>
void pmAlgoTAN<rowDataType>::run(pmBayesianNetwork* compBN)
{

	/* Initial bayesian network */
	// pmBayesianNetwork *compBN = new pmBayesianNetwork(this->variables);
	compBN->naiveBayes(this->classVariable);

	/* Number of nodes */
	int n = compBN->num_vertices();

	/* Fill the score with BN and FC*/
	this->pScore->setBayesianNetwork(compBN);
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
			if(col != this->classVariable && row != this->classVariable){
				/*Get the current node pair*/
				pmNode A = compBN->getNode(compBN->getVariable(row));
				pmNode B = compBN->getNode(compBN->getVariable(col));

				/* Calculate the Score with parents = class */
				pmScoreValueType sc0 = this->pScore->computeNode(A);
				// std::cout << "WIHOUT parents : "<< sc0 << "  ";

				/* add the edge to the network */
				compBN->add_arrow(B, A);

				/*Calculate the Score with one parent + class */
				pmScoreValueType sc1 = this->pScore->computeNode(A);

				if (this->debugVerbose)
				{
					std::cout << "Weight for : " << row << " <--> " << col << "  =  " << sc0 - sc1 << "\n";
				}
				cout << endl;

				/* remove edge for next iteration*/
				compBN->remove_arrow(B, A);

				/* Add the edge to the undirected Graph with weight (inverse since MINIMIMUM Spanning Tree)*/
				boost::add_edge(row, col, sc0 - sc1, ug);
			}
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

	/*Run DFS on TAN with given root and store predecessors */
	boost::depth_first_search(mst,
		boost::visitor(boost::make_dfs_visitor((boost::record_predecessors(&p[0], boost::on_tree_edge()))))
		.root_vertex(root));

		/* Summary of TAN */
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
			if(i!=this->classVariable){
				// not if root
				if (p[i] != i)
				{

					pmNode A = compBN->getNode(compBN->getVariable(i));
					pmNode B = compBN->getNode(compBN->getVariable(p[i]));

					/* add edge to directed graph ! */
					compBN->add_arrow(B, A);

				}
			}
		}
	}

	//=============================================================================
	template <typename rowDataType>
	void pmAlgoTAN<rowDataType>::setScore(pmDecomposableScore<rowDataType> *pScore)
	{
		/* Setting a particular score */
		this->pScore = pScore;
		this->setEvaluatingScore(pScore);
	}

	//=============================================================================
	template <typename rowDataType>
	pmDecomposableScore<rowDataType> *pmAlgoTAN<rowDataType>::getScore()
	{
		/* Getting the used score */
		return this->pScore;
	}

	//=============================================================================
	template <typename rowDataType>
	void pmAlgoTAN<rowDataType>::setRoot(size_t rootIdx)
	{
		/* Setting a particular root */
		size_t root = rootIdx % this->variables.size();
		if (root != this->classVariable){
			this->rootIdx = rootIdx % this->variables.size();
		} else {
			this->setRandomRoot();
		}
	}

	//=============================================================================
	template <typename rowDataType>
	void pmAlgoTAN<rowDataType>::setClassVariable(int classC)
	{
		this->classVariable = classC;
	}
	//=============================================================================
	template <typename rowDataType>
	bool pmAlgoTAN<rowDataType>::getDebugVerbose()
	{
		return this->debugVerbose;
	};
	//=============================================================================
	template <typename rowDataType>
	pmBayesianNetwork *pmAlgoTAN<rowDataType>::getInitialBN()
	{
		//EMPTY FUNCTION. NO INITBN IN MWST!!!!!!!!!!
	}
	//=============================================================================

	template <typename rowDataType>
	void pmAlgoTAN<rowDataType>::setInitialNetwork(pmBayesianNetwork *initialBN)
	{
		//EMPTY FUNCTION. NO INITBN IN MWST!!!!!!!!!!
	}
	//=============================================================================
	template <typename rowDataType>
	void pmAlgoTAN<rowDataType>::setCPCGraph(pmGraph *graphCPC)
	{
		/* Setting a CPC Graph */
		//this->graphCPC = graphCPC;
	}
	//=============================================================================
	template <typename rowDataType>
	void pmAlgoTAN<rowDataType>::setDebugVerbose(bool debugVerbose)
	{
		this->debugVerbose = debugVerbose;
	}
	//=============================================================================
	template <typename rowDataType>
	void pmAlgoTAN<rowDataType>::setRandomRoot()
	{
		/* Setting a random root */
		random_device rd;
		mt19937 rng(rd());
		uniform_int_distribution<int> uni(0, this->variables.size() - 1);
		auto rand_int = uni(rng);
		while (rand_int % this->variables.size() == this->classVariable) {
			auto rand_int = uni(rng);
		}
		this->rootIdx = rand_int;
	}
	//=============================================================================
	template <typename rowDataType>
	plVariablesConjunction pmAlgoTAN<rowDataType>::getVariables(){
		return this->variables;
	}


//=============================================================================

namespace PILGRIM {
    template class pmAlgoTAN<std::vector<unsigned int>>;
    template class pmAlgoTAN<std::vector<int>>;
    template class pmAlgoTAN<std::vector<float>>;
    template class pmAlgoTAN<std::vector<double>>;
    template class pmAlgoTAN<std::vector<long double>>;

    template class pmAlgoTAN<unsigned int*>;
    template class pmAlgoTAN<int*>;
    template class pmAlgoTAN<float*>;
    template class pmAlgoTAN<double*>;
    template class pmAlgoTAN<long double*>;
}; // namespace PILGRIM
