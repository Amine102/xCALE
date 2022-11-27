#include "pilgrim/general/algorithms/Algorithm_T.h"

using namespace boost;
using namespace std;
using namespace PILGRIM;

/*
=====CONSTRUCTORS=====
*/
//=============================================================================
template <typename rowDataType>
pmAlgorithm_T<rowDataType>::
pmAlgorithm_T(pmFrequencyCounter* pFrequencyCounter) {
  this->variables = pFrequencyCounter->getVariables();
  this->setFrequencyCounter(pFrequencyCounter);
  this->setEvaluatingScore(NULL);
  // this->debugVerbose = false;
}

//=============================================================================
template <typename rowDataType>
pmAlgorithm_T<rowDataType>::
pmAlgorithm_T(pmCSVDataSet* ds) {
	this->variables = ds->observed_variables();
	this->setFrequencyCounterFromCSV(ds);
	this->setEvaluatingScore(NULL);
  // this->debugVerbose = false;
}

/*
=====DESTRUCTOR=====
*/
//=============================================================================
template <typename rowDataType>
pmAlgorithm_T<rowDataType>::~pmAlgorithm_T() {
}

/*
=====GETTER=====
*/
//=============================================================================
template <typename rowDataType>
pmFrequencyCounter* pmAlgorithm_T<rowDataType>::
getFrequencyCounter() {
  return this->pFrequencyCounter;
}

//=============================================================================
template <typename rowDataType>
const vector< pair<unsigned int, unsigned int> >&
pmAlgorithm_T<rowDataType>::getIncludedEdges() {
  return this->includedEdges;
}

//=============================================================================
template <typename rowDataType>
const vector< pair<unsigned int, unsigned int> >& 
pmAlgorithm_T<rowDataType>::getExcludedEdges() {
  return this->excludedEdges;
}

//=============================================================================
template <typename rowDataType>
pmScore<rowDataType>* pmAlgorithm_T<rowDataType>::getEvaluatingScore() {
  return this->pEvaluatingScore;
}

/*
=====SETTERS=====
*/
//=============================================================================
template <typename rowDataType>
void pmAlgorithm_T<rowDataType>::
setFrequencyCounter(pmFrequencyCounter* pFrequencyCounter) {
  this->pFrequencyCounter = pFrequencyCounter; 
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm_T<rowDataType>::
setFrequencyCounterFromCSV(pmCSVDataSet* ds) {
	
	PILGRIM::pmFrequencyCounter *fc = new PILGRIM::pmFrequencyCounter(ds->get_data_descriptor(), this->variables);
	fc->computeFrequencies();
	this->setFrequencyCounter(fc);
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm_T<rowDataType>::
setIncludedEdges(vector< pair<unsigned int, unsigned int> >& includedEdges) {
  this->includedEdges = includedEdges;
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm_T<rowDataType>::
setExcludedEdges(vector< pair<unsigned int, unsigned int> >& excludedEdges) {
  this->excludedEdges = excludedEdges;
}

/*
=====METHODS=====
*/
//=============================================================================
// template <typename rowDataType>
// plJointDistribution pmAlgorithm_T<rowDataType>::compute() {
//   pmBayesianNetwork* pBN = this->run();
//   return pBN->getJointDistribution();
// }

/*
=====PROTECTED METHODS & SETTERS=====
*/
//=============================================================================
template <typename rowDataType>
void pmAlgorithm_T<rowDataType>::
setEvaluatingScore(pmScore<rowDataType>* pEvaluatingScore) {
  this->pEvaluatingScore = pEvaluatingScore;
}

//=============================================================================
template <typename rowDataType>
void pmAlgorithm_T<rowDataType>::
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
bool pmAlgorithm_T<rowDataType>::
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

    template class pmAlgorithm_T<std::vector<unsigned int>>;
    template class pmAlgorithm_T<std::vector<int>>;
    template class pmAlgorithm_T<std::vector<float>>;
    template class pmAlgorithm_T<std::vector<double>>;
    template class pmAlgorithm_T<std::vector<long double>>;

    template class pmAlgorithm_T<unsigned int*>;
    template class pmAlgorithm_T<int*>;
    template class pmAlgorithm_T<float*>;
    template class pmAlgorithm_T<double*>;
    template class pmAlgorithm_T<long double*>;
}; // namespace PILGRIM
