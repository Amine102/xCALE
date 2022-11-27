#include "pilgrim/general/algorithms/AlgoMMHC.h"
#include <plDebug.h>

using namespace boost;
using namespace PILGRIM;
using namespace std;

/*
=====CONSTRUCTOR=====
*/
//=============================================================================
template <typename rowDataType>
pmAlgoMMHC<rowDataType>::
	pmAlgoMMHC(pmCSVDataSet *ds,
			   pmDecomposableScore<rowDataType> *pScore,
			   pmCache *cache,
			   bool debugVerbose) : pmAlgorithm<rowDataType>(ds)
{
	this->initialBN = new pmBayesianNetwork(this->variables);
	this->condVarSize = 10;
	this->debugVerbose = debugVerbose;
	this->ds = ds;
	this->setScore(pScore);
	this->cache = cache;
	this->alpha = 0.05;
}

/*
=====DESTRUCTOR=====
*/
//=============================================================================
template <typename rowDataType>
pmAlgoMMHC<rowDataType>::~pmAlgoMMHC(){};

/*
=====GETTER=====
*/
//=============================================================================
template <typename rowDataType>
pmBayesianNetwork *pmAlgoMMHC<rowDataType>::getInitialBN()
{
	return this->initialBN;
};

//=============================================================================
template <typename rowDataType>
bool pmAlgoMMHC<rowDataType>::getDebugVerbose()
{
	return this->debugVerbose;
};

/*
=====SETTERS=====
*/
//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::setAlpha(float alpha)
{
	if (alpha >= 0 && alpha <= 1)
	{
		this->alpha = alpha;
	}
	else
	{
		this->alpha = 0.05;
		std::cout << "\n Alpha must be a float [0,1] - set to 0.05 \n";
	}
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::setCondVarSize(unsigned int condVarSize)
{
	this->condVarSize = condVarSize;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::setInitialNetwork(pmBayesianNetwork *initialBN)
{
	// Setting a particular initial BN for the greedy search
	this->initialBN = initialBN;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::setScore(pmDecomposableScore<rowDataType> *pScore)
{
	/* Setting a particular score */
	this->pScore = pScore;
	this->pEvaluatingScore = pScore;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::setDebugVerbose(bool debugVerbose)
{
	this->debugVerbose = debugVerbose;
}

/*
=====METHODS=====
*/
//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::
	run(pmBayesianNetwork *bnMMHC)
{
	cout<<"MMHC begins"<<endl;
	if (this->debugVerbose)
	{
		std::cout << endl
				  << " ---------------------- Start Learning Initial MMHC ---------------------- " << endl;
		std::cout << endl
				  << " ---------------------- 1: Create initial CPC Graph ---------------------- " << endl;
	}

	// create Bayesian network for CPC graph
	pmBayesianNetwork *bnCPC = NULL;

	// Creating my parent/children graph and checking symetry
	CPC_t cpc_v = extractCPC();

	// Create CPC
	bnCPC = createConstraintsGraph(cpc_v);
	// AlgoMMPCbar::printFinalCPCPath();
	
	cout<<"\n============>bnCPC is\n"<<endl;
	bnCPC->print_graph();
	cout<<"\n============>Finished printing bnCPC\n"<<endl;
	// Instantiate GS  and set CPC and Initial BN
	PILGRIM::pmAlgoGS<rowDataType> algoGS(this->ds, this->pScore, this->cache);
	algoGS.setCPCGraph(&bnCPC->graph);
	algoGS.setInitialNetwork(this->initialBN);

	if (this->debugVerbose)
	{
		std::cout << endl
				  << " ---------------------- 2: Run GS Initial Algorithm ---------------------- " << endl;
	}

	// Run GS with CPC

	algoGS.run(bnMMHC);
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::computeCPC(CPC_t &CPCs, const int &targetVariable)
{
	// cout<<"AlgoMMHC::ComputeCPC begins"<<endl;	
	// copy to change data type from int to unsigned int
	unsigned int vTarget = static_cast<unsigned int>(targetVariable);
	size_t nVars = ds->get_num_fields();

	AlgoMMPCbar::numVars = nVars;
	AlgoMMPCbar::all_variable = this->ds->observed_variables();
	AlgoMMPC::resetCounters();
	AlgoMMPCbar::varCounter = 0;
	if (targetVariable == -1)
	{ // means learn all variables
		// Learn for all variables
		for (unsigned int i = 0; i < nVars; i++)
		{
			// cout<<"In computeCPC, variable number "<<i<<endl;
			AlgoMMPC *mmpc = new AlgoMMPC(this->ds, this->condVarSize,3);
			// mmpc->setAlpha(this->alpha);
			mmpc->computeMMPC(i, CPCs);
			delete mmpc;
		} // end for
	}
	else
	{ // learn only target variable
		AlgoMMPC *mmpc = new AlgoMMPC(this->ds, this->condVarSize,3);
		// mmpc->setAlpha(this->alpha);
		mmpc->computeMMPC(vTarget, CPCs);
		delete mmpc;
	}

	AlgoMMPC::cpcMapList.clear(); // Reset static variable for next iteration.

	if (this->debugVerbose)
	{
		cout << endl
			 << endl
			 << "CPC List without Symmetrie Test = { ";
		unsigned int totalCPC = 0;
		CPC_t::const_iterator itrMap;
		for (itrMap = CPCs.begin(); itrMap != CPCs.end(); ++itrMap)
		{
			cout << endl
				 << itrMap->first << " = ";
			for (size_t j = 0; j < itrMap->second.size(); j++)
			{
				totalCPC++;
				cout << itrMap->second.at(j) << " ; ";
			} // end for
		}	  // end for
		cout << endl;
	}
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC<rowDataType>::extractMarkovBlanket(pmBayesianNetwork *BN, CPC_t &CPClist)
{
	pmGraph &graph = BN->get_graph();

	pmNode node, parent;
	unsigned int i, j, n, targetVar;

	n = this->variables.dim();

	for (targetVar = 0; targetVar < n; ++targetVar)
	{
		graph_traits<pmGraph>::in_edge_iterator inEdgeItr, inEdgeItrEnd;
		boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(vertex(targetVar, graph), graph);

		graph_traits<pmGraph>::out_edge_iterator outEdgeItr, outEdgeItrEnd;
		boost::tie(outEdgeItr, outEdgeItrEnd) = out_edges(vertex(targetVar, graph), graph);

		// Extracting the parents of the targetVariable
		for (; inEdgeItr != inEdgeItrEnd; ++inEdgeItr)
		{
			parent = source(*inEdgeItr, graph);
			j = BN->getVariableIndex(parent);
			CPClist[targetVar].push_back(j);
		}

		// Extracting the children of the targetVariable
		for (; outEdgeItr != outEdgeItrEnd; ++outEdgeItr)
		{
			node = target(*outEdgeItr, graph);
			i = BN->getVariableIndex(node);
			CPClist[targetVar].push_back(i);

			// Extracting the parents of the children
			boost::tie(inEdgeItr, inEdgeItrEnd) = in_edges(node, graph);
			for (; inEdgeItr != inEdgeItrEnd; ++inEdgeItr)
			{
				parent = source(*inEdgeItr, graph);
				j = BN->getVariableIndex(parent);
				if (j != targetVar)
					CPClist[targetVar].push_back(j);
			} // end for
		}	  // end for
	}		  // end for
}

//=============================================================================
template <typename rowDataType>
pmBayesianNetwork *pmAlgoMMHC<rowDataType>::createConstraintsGraph(CPC_t &cpc_v, CPC_t *PC_set)
{
	cout<<"CreateConstraintsGraph begins"<<endl;
	// create a undirected graph with calculated set of CPCs
	pmBayesianNetwork *resultedBN = new pmBayesianNetwork(this->variables);
	pmGraph &graphCPC = resultedBN->get_graph();

	int nA, nB;
	pmEdge e, e1, e2;
	bool edgeAdded, edgeAdded2;

	CPC_t::iterator nodeItr;
	std::vector<unsigned int>::iterator CPCItr;

	// A bidirectional graph assume as undirected graph
	for (nodeItr = cpc_v.begin(); nodeItr != cpc_v.end(); ++nodeItr)
	{
		nA = nodeItr->first;
		for (CPCItr = nodeItr->second.begin(); CPCItr != nodeItr->second.end(); ++CPCItr)
		{
			nB = *CPCItr;
			// check whether edge exist in other direction
			pmNode nodeA, nodeB;
			nodeA = vertex(nA, graphCPC);
			nodeB = vertex(nB, graphCPC);

			boost::tie(e2, edgeAdded) = add_edge(nodeA, nodeB, graphCPC);
			boost::tie(e1, edgeAdded2) = add_edge(nodeB, nodeA, graphCPC);
		} // end for
	}	  // end for
	return resultedBN;
}

//=============================================================================
template <typename rowDataType>
CPC_t pmAlgoMMHC<rowDataType>::extractCPC()
{
	// cout<<"extractCPC begins"<<endl;
	// initialize MI cache
	pmMI::clearCache();

	CPC_t cpc_v;

	// compute CPC
	this->computeCPC(cpc_v);

	// for symmetry test
	unsigned int nodeA, nodeB;
	CPC_t::iterator nodeItr;
	std::vector<unsigned int>::iterator CPCItr, cpcTempItr;
	for (nodeItr = cpc_v.begin(); nodeItr != cpc_v.end(); ++nodeItr)
	{
		nodeA = nodeItr->first;

		CPCItr = nodeItr->second.begin();
		while (CPCItr != nodeItr->second.end())
		{
			nodeB = *CPCItr;
			cpcTempItr = find(cpc_v[nodeB].begin(), cpc_v[nodeB].end(), nodeA);
			if (cpcTempItr == cpc_v[nodeB].end())
				CPCItr = cpc_v[nodeA].erase(CPCItr); // if element not found, remove it - and adjust ITERATOR !
			else
				++CPCItr;
		}
	}

	return cpc_v;
}

namespace PILGRIM {
    template class pmAlgoMMHC<std::vector<unsigned int>>;
    template class pmAlgoMMHC<std::vector<int>>;
    template class pmAlgoMMHC<std::vector<float>>;
    template class pmAlgoMMHC<std::vector<double>>;
    template class pmAlgoMMHC<std::vector<long double>>;

    template class pmAlgoMMHC<unsigned int*>;
    template class pmAlgoMMHC<int*>;
    template class pmAlgoMMHC<float*>;
    template class pmAlgoMMHC<double*>;
    template class pmAlgoMMHC<long double*>;
}; // namespace PILGRIM
