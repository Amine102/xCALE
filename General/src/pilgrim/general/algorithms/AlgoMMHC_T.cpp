#include "pilgrim/general/algorithms/AlgoMMHC_T.h"
#include <plDebug.h>


using namespace boost;
using namespace PILGRIM;
using namespace std;

template <typename rowDataType>
string pmAlgoMMHC_T<rowDataType>::orignalNetFileName = "";
template <typename rowDataType>
bool pmAlgoMMHC_T<rowDataType>::logFileHeaders = false;  // to put only once header in the log file
template <typename rowDataType>
int pmAlgoMMHC_T<rowDataType>::t = 1;  // learning kind (naive -1 or optimized 0 (initial model) or 1 (transition model)) add ghada
template <typename rowDataType>
int pmAlgoMMHC_T<rowDataType>::t1 = 1;  // naive DMMHC 1 or MMHC 0 //add ghada
template <typename rowDataType>
int pmAlgoMMHC_T<rowDataType>::t2 = 1;

/*
=====CONSTRUCTORS======
*/
//==========================================================================
template <typename rowDataType>
pmAlgoMMHC_T<rowDataType>::
pmAlgoMMHC_T( pmCSVDataSet* ds,
              pmDecomposableScore<rowDataType>* pScore,
              pmCache* cache,
              bool debugVerbose
              ) : pmAlgorithm_T<rowDataType>(ds) {
  this->transitionBN = new pmBayesianNetwork(this->variables);
	this->condVarSize = 10;
	this->setDebugVerbose(debugVerbose);
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
pmAlgoMMHC_T<rowDataType>::
~pmAlgoMMHC_T() {}

/*
=====GETTER=====
*/
//=============================================================================
template <typename rowDataType>
pmBayesianNetwork* pmAlgoMMHC_T<rowDataType>::
getTransitionBN() {
  return this->transitionBN;
}

//=============================================================================
template <typename rowDataType>
bool pmAlgoMMHC_T<rowDataType>::
getDebugVerbose() {
  return this->debugVerbose;
}

/*
=====SETTERS=====
*/
//=============================================================================
template<typename rowDataType>
void pmAlgoMMHC_T<rowDataType>::
setAlpha(float alpha) {

	if (alpha >= 0 && alpha <= 1){
		this->alpha = alpha;
	}
	else{
		this->alpha = 0.05;
		std::cout << "\n Alpha must be a float [0,1] - set to 0.05 \n";
	}
}

//=============================================================================
template<typename rowDataType>
void pmAlgoMMHC_T<rowDataType>::
setCondVarSize(unsigned int condVarSize) {
	this->condVarSize = condVarSize;
}

//=============================================================================
template<typename rowDataType>
void pmAlgoMMHC_T<rowDataType>::
setInitialNetwork(pmBayesianNetwork* initialBN) {
  this->transitionBN = initialBN;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC_T<rowDataType>::
setScore(pmDecomposableScore<rowDataType>* pScore) {
  this->pScore = pScore;
	this->pEvaluatingScore = pScore;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC_T<rowDataType>::
setDebugVerbose(bool debugVerbose) {
  this->debugVerbose = debugVerbose;
}

/*
=====METHODS=====
*/
//=============================================================================
template <typename rowDataType>
pmBayesianNetwork* pmAlgoMMHC_T<rowDataType>::
run(pmBayesianNetwork* bnMMHC_T) {

  if (debugVerbose) {
    std::cout << endl << " ---------------------- Start Learning Transition MMHC ---------------------- " << endl;
  }

  // initialize MI cache
  pmMI::clearCache();

  // Creating my parent/children graphs (transition one)
  CPC_t cpc_v_final;
  CPC_t cpc_v;
  CPC_t cc_v;
  CPC_t pc_v;

  if (debugVerbose) {
    std::cout << endl << " ---------------------- 1: Create transition CPC Graph ---------------------- " << endl;
  }

  // compute CPC
  this->computeCPC_T(cpc_v, cc_v, pc_v);

  // create Bayesian network for CPC graph
	pmBayesianNetwork* bnCPC_T = NULL;

  // Create CPC
  bnCPC_T = createConstraintsGraph_T(cpc_v, pc_v);
  cout << "out of createConstrainsGraph_T" << endl;

  // Instantiate GS  and set CPC and Initial BN
  PILGRIM::pmAlgoGS_T<rowDataType> algoGS_T(this->ds, this->pScore, this->cache);
  algoGS_T.setCPCGraph(&bnCPC_T->graph);
	algoGS_T.setTransitionNetwork(this->transitionBN);

  if (debugVerbose) {
	  std::cout << endl << " ---------------------- 2: Run GS Transition Algorithm ---------------------- " << endl;
  }
  // Run GS with CPC
	bnMMHC_T = algoGS_T.run(this->transitionBN);

  return bnMMHC_T;
}

//=============================================================================
template <typename rowDataType>
void pmAlgoMMHC_T<rowDataType>::
computeCPC_T(CPC_t &CPCs, CPC_t &CCs, CPC_t &PCs, const int &targetVariable) {

  // copy to change data type from int to unsigned int
	unsigned int vTarget = static_cast<unsigned int>(targetVariable); 
  size_t nVars = ds->get_num_fields();


  clock_t tStart;

  AlgoMMPCbar_T::numVars = nVars;
  AlgoMMPCbar_T::all_variable = this->ds->observed_variables();
  AlgoMMPC_T::resetCounters();
  AlgoMMPCbar_T::varCounter = 0;

  AlgoMMPCbar_T::t = t;

  tStart = clock();

  if (targetVariable== -1) { // means learn all variables

    // Learn for all variables (nVars always pair)
    for (unsigned int i(nVars / 2); i < nVars; i++) {
      if (this->debugVerbose) {
        cout << endl<<"----------ComputeDMMPC iteration: "<< i << endl;
      }
      AlgoMMPC_T *mmpc_T = new AlgoMMPC_T(this->ds, this->condVarSize, this->debugVerbose);
      mmpc_T->setAlpha(this->alpha);
      mmpc_T->computeMMPC_T(i, CPCs, CCs, PCs);
      if (this->debugVerbose) {
        cout << endl;
      }
      delete mmpc_T;
    }
  }
  else {  // learn only target variable
    AlgoMMPC_T *mmpc_T = new AlgoMMPC_T(this->ds, this->condVarSize, this->debugVerbose);
    mmpc_T->computeMMPC_T(vTarget, CPCs, CCs, PCs);  // Calculate Children and Parents of target variable
    delete mmpc_T;
  }

  AlgoMMPC_T::cpcMapList.clear();
  AlgoMMPC_T::cpc_tMapList.clear();  // Reset static variable for next iteration.
}

//=============================================================================
template <typename rowDataType>
pmBayesianNetwork* pmAlgoMMHC_T<rowDataType>::
createConstraintsGraph_T(CPC_t& cpc_v , CPC_t& cp_v) {

  // create a undirected graph with calculated set of CPCs
  pmBayesianNetwork* resultedBN = new pmBayesianNetwork(this->variables);
  pmGraph& graphCPC = resultedBN->get_graph();

  int nA, nB;
  pmEdge e, e1, e2,e3;
  bool edgeAdded, edgeAdded2, edgeAdded3;

  CPC_t::iterator nodeItr;
  std::vector<unsigned int>::iterator CPCItr;

  unsigned int i = 0;
  unsigned int j = 0;

  // A bidirectional graph assume as undirected graph
  for (nodeItr = cpc_v.begin(); nodeItr != cpc_v.end(); ++nodeItr) {
    
    nA = nodeItr->first;

    for (CPCItr = nodeItr->second.begin(); CPCItr != nodeItr->second.end(); ++CPCItr) {
      nB = *CPCItr;

      // check whether edge exist in other direction
      pmNode nodeA, nodeB;
      nodeA = vertex(nA, graphCPC);
      nodeB = vertex(nB, graphCPC);

      // Creating the edges between nodes
      boost::tie(e2, edgeAdded) = add_edge(nodeA, nodeB, graphCPC);
    }  // end for
  }  // end for


  for (nodeItr = cp_v.begin(); nodeItr != cp_v.end(); ++nodeItr) {
    nA = nodeItr->first;
    for (CPCItr = nodeItr->second.begin(); CPCItr != nodeItr->second.end(); ++CPCItr) {
      nB = *CPCItr;

      // check whether edge exist in other direction
      pmNode nodeA, nodeB;
      nodeA = vertex(nA, graphCPC);
      nodeB = vertex(nB, graphCPC);

      boost::tie(e3, edgeAdded3) = add_edge(nodeB, nodeA, graphCPC);
    }  // end for
  }  // end for
  return resultedBN;
}

namespace PILGRIM {
    template class pmAlgoMMHC_T<std::vector<unsigned int>>;
    template class pmAlgoMMHC_T<std::vector<int>>;
    template class pmAlgoMMHC_T<std::vector<float>>;
    template class pmAlgoMMHC_T<std::vector<double>>;
    template class pmAlgoMMHC_T<std::vector<long double>>;

    template class pmAlgoMMHC_T<unsigned int*>;
    template class pmAlgoMMHC_T<int*>;
    template class pmAlgoMMHC_T<float*>;
    template class pmAlgoMMHC_T<double*>;
    template class pmAlgoMMHC_T<long double*>;
}; // namespace PILGRIM
