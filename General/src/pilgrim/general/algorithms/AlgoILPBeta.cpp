
#include "pilgrim/general/algorithms/AlgoILPBeta.h"
#include "pilgrim/general/algorithms/AlgoMMHC.h"
#include <vector>
#include <string>
#include <iostream>
#include <numeric>
#include <algorithm>
#include <plDebug.h>
#include <sstream>

using namespace std;
using namespace boost;
using namespace PILGRIM;

//=============================================================================
template <typename rowDataType>
pmAlgoILP<rowDataType>::pmAlgoILP(pmCSVDataSet* ds, pmDecomposableScore<rowDataType>* pScore, pmCache* cache, bool debugVerbose)
	: pmAlgorithm<rowDataType>(ds) {
	this->setScore(pScore);
	this->pScore->cache = cache;
	this->setDebugVerbose(debugVerbose);

}

//=============================================================================
template <typename rowDataType>
pmAlgoILP<rowDataType>::~pmAlgoILP() {
}

//=============================================================================
template <typename rowDataType>
void pmAlgoILP<rowDataType>::run(pmBayesianNetwork* compBN) {

	/* Initial bayesian network */
	pmBayesianNetwork*  computedBN = compBN;

	/* Returning the computed bayesian network */
}

//=============================================================================
template <typename rowDataType>
void pmAlgoILP<rowDataType>::computeAllLocalScores(const std::string &file_name) {

	/* Initial bayesian network */
	pmBayesianNetwork* computedBN = new pmBayesianNetwork(this->variables);

	/* Number of nodes */
	int n = computedBN->num_vertices();

	/* Fill the score with BN and FC*/
	this->pScore->setBayesianNetwork(computedBN);
	this->pScore->setFrequencyCounter(this->getFrequencyCounter());

	if (this->debugVerbose){
		std::cout << "\n =================== Calculation of every possible local score  =================== \n\n\n";
		std::cout << "\nOutput in GOBNILP format: \nnode idx - no. of calculated parentset  \nscore - no. of parents - parent(s) idx\n\n\n";
	}

	/* Set up int vector for each Variable */
	std::vector<int> v(n);
	std::iota(std::begin(v), std::end(v), 0);

	/* File to store results */
	std::ofstream outfile;
	outfile.open(file_name, std::ios_base::app);

	/* Output node index and following GOBNILP format */
	outfile << v.size()  << endl;

	/* Iterate over each variable */
	for (std::size_t i = 0; i < v.size(); ++i){

		if (this->debugVerbose){
			std::cout << "Check for variable " << i << "\n";
		}

		/* Output node index and following GOBNILP format */
		outfile << i << " " << pow(2, n - 1) << "\n";

		/* Possible parent set */
		vector<int>parents;

		/* Fill with only possible parents , i.e. remove variable of interest */
		std::copy_if(v.begin(), v.end(), std::back_inserter(parents), [&](int j){return j != i; });

		/* Get the node of interest */
		pmNode A = computedBN->getNode(computedBN->getVariable(i));

		/* Iterate trough  2^n-1 possible parent sets*/
		for (auto k = 0; k < (1 << parents.size()); ++k){

			/* Bitset for powerset */
			bitset<32>b(k);

			/* Number of parents */
			int n_parents = b.count();

			/* Iterate over parent set and add edges */
			for (auto j = 0; j < parents.size(); j++){

				/* If parent exists*/
				if (b[j]){

					/* add edges*/
					pmNode B = computedBN->getNode(computedBN->getVariable(parents[j]));
					computedBN->add_arrow(B, A);
				}
			}

			/* Calculate the local score */
			pmScoreValueType local_score = this->pScore->computeNode(A);

			/* Remove all edges for next power set configuration*/
			computedBN->clear_all_edges();

			/* Write to file */
			outfile << local_score << " " << n_parents << " ";

			/* Iterate over current parentset and write to file  - improvement possible */
			for (auto j = 0; j < parents.size(); j++){
				if (b[j]){
					outfile << parents[j] << " ";
				}
			}

			/* End line for current configuration*/
			outfile << endl;
		}

	}
}

//=============================================================================
template <typename rowDataType>
void pmAlgoILP<rowDataType>::computeAllLocalScoresUseCPC(const std::string &file_name, CPC_t CPCs) {

	/* Initial bayesian network */
	pmBayesianNetwork* computedBN = new pmBayesianNetwork(this->variables);

	/* Number of nodes */
	int n = computedBN->num_vertices();

	/* Fill the score with BN and FC*/
	this->pScore->setBayesianNetwork(computedBN);
	this->pScore->setFrequencyCounter(this->getFrequencyCounter());

	if (this->debugVerbose){
		std::cout << "\n =================== Calculation of every possible local score  =================== \n\n\n";
		std::cout << "\nOutput in GOBNILP format: \nnode idx - no. of calculated parentset  \nscore - no. of parents - parent(s) idx\n\n\n";
	}

	/* Set up int vector for each Variable */
	std::vector<int> v(n);
	std::iota(std::begin(v), std::end(v), 0);

	/* File to store results */
	std::ofstream outfile;
	outfile.open(file_name, std::ios_base::app);

	/* Output node index and following GOBNILP format */
	outfile << v.size() << endl;

	/* Iterate over each variable */
	for (std::size_t i = 0; i < v.size(); ++i){

		if (this->debugVerbose){
			std::cout << "Check for variable " << i << "\n";
		}

		/* Possible parent set by CPC  */
		std::vector<unsigned int>parents = CPCs.at(i);

		/* Sort in ascending order */
		sort(parents.begin(), parents.end());
		
		/* Output node index and following GOBNILP format */
		outfile << i << " " << pow(2, parents.size()) << "\n";

		/* Get the node of interest */
		pmNode A = computedBN->getNode(computedBN->getVariable(i));
		
		/* Iterate trough  2^n-1 possible parent sets*/
		for (auto k = 0; k < (1 << parents.size()); ++k){

			/* Bitset for powerset */
			bitset<32>b(k);

			/* Number of parents */
			int n_parents = b.count();

			/* Iterate over parent set and add edges */
			for (auto j = 0; j < parents.size(); j++){

				/* If parent exists*/
				if (b[j]){

					/* add edges*/
					pmNode B = computedBN->getNode(computedBN->getVariable(parents[j]));
					computedBN->add_arrow(B, A);
				}
			}

			/* Calculate the local score */
			pmScoreValueType local_score = this->pScore->computeNode(A);

			/* Remove all edges for next power set configuration*/
			computedBN->clear_all_edges();

			/* Write to file */
			outfile << local_score << " " << n_parents << " ";

			/* Iterate over current parentset and write to file  - improvement possible */
			for (auto j = 0; j < parents.size(); j++){
				if (b[j]){
					outfile << parents[j] << " ";
				}
			}

			/* End line for current configuration*/
			outfile << endl;
		}

	}
}

//=============================================================================
template <typename rowDataType>
void pmAlgoILP<rowDataType>::setScore(pmDecomposableScore<rowDataType>* pScore) {
	/* Setting a particular score */
	this->pScore = pScore;
	this->setEvaluatingScore(pScore);
}

//=============================================================================
template <typename rowDataType>
pmDecomposableScore<rowDataType> *pmAlgoILP<rowDataType>::getScore() {
	/* Getting the used score */
	return this->pScore;
}

namespace PILGRIM {
   template class pmAlgoILP<std::vector<unsigned int>>;
    template class pmAlgoILP<std::vector<int>>;
    template class pmAlgoILP<std::vector<float>>;
    template class pmAlgoILP<std::vector<double>>;
    template class pmAlgoILP<std::vector<long double>>;

    template class pmAlgoILP<unsigned int*>;
    template class pmAlgoILP<int*>;
    template class pmAlgoILP<float*>;
    template class pmAlgoILP<double*>;
    template class pmAlgoILP<long double*>;
}; // namespace PILGRIM
