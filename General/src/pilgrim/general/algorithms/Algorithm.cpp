#include "pilgrim/general/algorithms/Algorithm.h"

using namespace boost;
using namespace std;
using namespace PILGRIM;

//=============================================================================
template <typename rowDataType>
pmAlgorithm<rowDataType>::
pmAlgorithm(pmFrequencyCounter* pFrequencyCounter) {
  this->variables = pFrequencyCounter->getVariables();
  this->setFrequencyCounter(pFrequencyCounter);
  this->setEvaluatingScore(NULL);
}

//=============================================================================
template <typename rowDataType>
pmAlgorithm<rowDataType>::
pmAlgorithm(pmCSVDataSet* ds) {
	this->variables = ds->observed_variables();
	this->setFrequencyCounterFromCSV(ds);
	this->setEvaluatingScore(NULL);
}


//=============================================================================
template <typename rowDataType>
pmAlgorithm<rowDataType>::~pmAlgorithm() {
}

//=============================================================================
/*
DEPRICATED
*/
// template <typename rowDataType>
// plJointDistribution pmAlgorithm<rowDataType>::compute() {
//   pmBayesianNetwork* pBN = this->run();
//   return pBN->getJointDistribution();
// }

//=============================================================================
template <typename rowDataType>
pmFrequencyCounter* pmAlgorithm<rowDataType>::
getFrequencyCounter() {
  return this->pFrequencyCounter;
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm<rowDataType>::
setFrequencyCounter(pmFrequencyCounter* pFrequencyCounter) {
  this->pFrequencyCounter = pFrequencyCounter; 
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm<rowDataType>::
setFrequencyCounterFromCSV(pmCSVDataSet* ds) {
	
	PILGRIM::pmFrequencyCounter *fc = new PILGRIM::pmFrequencyCounter(ds->get_data_descriptor(), this->variables);
	fc->computeFrequencies();
	this->setFrequencyCounter(fc);
}


//=============================================================================
template <typename rowDataType>
const vector< pair<unsigned int, unsigned int> >&
pmAlgorithm<rowDataType>::getIncludedEdges() {
  return this->includedEdges;
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm<rowDataType>::
setIncludedEdges(vector< pair<unsigned int, unsigned int> >& includedEdges) {
  this->includedEdges = includedEdges;
}

//=============================================================================
template <typename rowDataType>
const vector< pair<unsigned int, unsigned int> >& 
pmAlgorithm<rowDataType>::getExcludedEdges() {
  return this->excludedEdges;
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm<rowDataType>::
setExcludedEdges(vector< pair<unsigned int, unsigned int> >& excludedEdges) {
  this->excludedEdges = excludedEdges;
}

//=============================================================================
template <typename rowDataType>
pmScore<rowDataType>* pmAlgorithm<rowDataType>::getEvaluatingScore() {
  return this->pEvaluatingScore;
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm<rowDataType>::
setEvaluatingScore(pmScore<rowDataType>* pEvaluatingScore) {
  this->pEvaluatingScore = pEvaluatingScore;
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm<rowDataType>::
applyIncludedAndExcludedEdges(pmBayesianNetwork* pBN, 
                              bool applyIncludedEdges, 
                              bool applyExcludedEdges) {
  register unsigned int i, size;
  pair<unsigned int, unsigned int> edge;

  if (applyIncludedEdges) {
    size = this->includedEdges.size();
    for (i = 0; i < size; i++) {
      edge = this->includedEdges[i];
      add_edge(vertex(edge.second, pBN->graph),
               vertex(edge.first, pBN->graph), pBN->graph);
    }
  }

  if (applyExcludedEdges) {
    size = this->excludedEdges.size();
    for (i = 0; i < size; i++) {
      edge = this->excludedEdges[i];
      remove_edge(vertex(edge.second, pBN->graph), 
                  vertex(edge.first, pBN->graph), pBN->graph);
    }
  }
}

//=============================================================================
template <typename rowDataType>
bool pmAlgorithm<rowDataType>::
isEdgeInList(unsigned int nodeIdx, unsigned int parentIdx, bool includeList) {
  vector< pair<unsigned int, unsigned int> >& edgeList = (includeList) ? 
                                                            this->includedEdges : 
                                                            this->excludedEdges;
  pair<unsigned int, unsigned int> edge;
  unsigned int i, size = edgeList.size();
  for (i = 0; i < size; i++) {
    edge = edgeList[i];
    if (edge.first == nodeIdx && edge.second == parentIdx)
      return true;
  }
  return false;
}


//=============================================================================

namespace PILGRIM {
    template class pmAlgorithm<std::vector<unsigned int>>;
    template class pmAlgorithm<std::vector<int>>;
    template class pmAlgorithm<std::vector<float>>;
    template class pmAlgorithm<std::vector<double>>;
    template class pmAlgorithm<std::vector<long double>>;

    template class pmAlgorithm<unsigned int*>;
    template class pmAlgorithm<int*>;
    template class pmAlgorithm<float*>;
    template class pmAlgorithm<double*>;
    template class pmAlgorithm<long double*>;
}; // namespace PILGRIM
