
#include "pilgrim/general/scores/DecomposableScore.h"
#include "pilgrim/general/pmUtility.h"
#include <iostream>

using namespace boost;
using namespace std;
using namespace PILGRIM;

//=============================================================================
template <typename rowDataType>
pmDecomposableScore<rowDataType>::
pmDecomposableScore(plJointDistribution& jd,
                    pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
  : pmScore<rowDataType>(jd, pFrequencyCounter) {
  this->epsilon = (pmScoreValueType)1e-15;
  this->cache = cache;  // added bt Billy Rousseaux 11/01/2011
}

//=============================================================================
template <typename rowDataType>
pmDecomposableScore<rowDataType>::
pmDecomposableScore(pmBayesianNetwork* pBN,
                    pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
  : pmScore<rowDataType>(pBN, pFrequencyCounter) {
  this->epsilon = (pmScoreValueType)1e-15;
  this->cache=cache;//added bt Billy Rousseaux 11/01/2011
}

//=============================================================================
template <typename rowDataType>
pmDecomposableScore<rowDataType>::~pmDecomposableScore() {
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmDecomposableScore<rowDataType>::compute() {
  pmScoreValueType totalScore = 0;
  pmNodeIterator vi, vi_end;
  for (boost::tie(vi, vi_end) = vertices(this->pBN->graph); vi != vi_end; ++vi)
    totalScore += this->computeNode(*vi);
  return totalScore;
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmDecomposableScore<rowDataType>::computeNode(plVariable& var) {
  unsigned int nodeIdx = this->pBN->getVariableIndex(var);
  pmNode node = vertex(nodeIdx, this->pBN->graph);
  return this->computeNode(node);
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmDecomposableScore<rowDataType>::computeMaxLikelihood() {
  pmScoreValueType likelyhood = 0;
  pmNodeIterator vi, vi_end;

  for (boost::tie(vi, vi_end) = vertices(this->pBN->graph); vi != vi_end; ++vi) {
    likelyhood += this->computeNodeMaxLikelihood(*vi);
  }
  return likelyhood;
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmDecomposableScore<rowDataType>::
computeNodeMaxLikelihood(pmNode& node) {
  pmScoreValueType likelihood = 0;
  unsigned int k, nodeCount = (unsigned int)num_vertices(this->pBN->graph);

  vector< bool > searchedVars(nodeCount, false);
  vector< plFloat > freq, Nijk;
  double Nij = 0;
  pmDCValueIterator iter;

  this->iteratorInit(iter, node, searchedVars);
  Nijk.resize(iter.cardNode);

  this->pFrequencyCounter->frequencyCount(searchedVars, freq);

  do {
    Nij = this->getNodeCounts(iter, freq, Nijk);

    if (Nij != 0)
      for (k = 0; k < iter.cardNode; k++)
        likelihood += Nijk[k] * log(Nijk[k] / Nij);
  } while (this->iteratorNextParentValue(iter));

  return likelihood;
}

//=============================================================================
template <typename rowDataType>
void pmDecomposableScore<rowDataType>::setEpsilon(pmScoreValueType epsilon) {
  this->epsilon = epsilon;
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmDecomposableScore<rowDataType>::getEpsilon() {
  return this->epsilon;
}

//=============================================================================
template <typename rowDataType>
void pmDecomposableScore<rowDataType>::
iteratorInit(pmDCValueIterator& iter, const pmNode& node,
             vector< bool >& searchedVars) {
  pmInEdgeIterator ei, ei_end;
  pmNode parent;
  unsigned int parentIdx, cardParent;

  iter.dimAfter = iter.cardBefore = 1;
  iter.nodeIdx = this->pBN->getVariableIndex(node);
  iter.cardNode = this->pBN->getCardinality(iter.nodeIdx);
  searchedVars[iter.nodeIdx] = true;
  for (boost::tie(ei, ei_end) = in_edges(node, this->pBN->graph); ei != ei_end; ++ei) {
    parent = source(*ei, this->pBN->graph);
    parentIdx = this->pBN->getVariableIndex(parent);
    cardParent = this->pBN->getCardinality(parentIdx);
    searchedVars[parentIdx] = true;
    if (parentIdx > iter.nodeIdx)
      iter.dimAfter *= cardParent;
    else
      iter.cardBefore *= cardParent;  // we suppose that we allways have: parentIdx != nodeIdx
  }
  iter.dimBefore = iter.cardNode * iter.dimAfter;
  iter.parentBeforeValueIdx = 0;
  iter.parentAfterValueIdx = 0;
  iter.baseBefore = 0;
}


//=============================================================================
template <typename rowDataType>
bool pmDecomposableScore<rowDataType>::
iteratorNextParentValue(pmDCValueIterator& iter) {
  iter.parentAfterValueIdx++;
  if (iter.parentAfterValueIdx >= iter.dimAfter) {
    iter.parentAfterValueIdx = 0;
    iter.parentBeforeValueIdx++;
    iter.baseBefore += iter.dimBefore;
    if (iter.parentBeforeValueIdx >= iter.cardBefore)
      return false;
  }
  return true;
}

//=============================================================================
template <typename rowDataType>
double pmDecomposableScore<rowDataType>::
getNodeCounts(const pmDCValueIterator& iter,
              const vector< plFloat >& freq,
              vector< plFloat >& nodeCounts) {
  unsigned int nodeValIdx = 0, idx = 0;
  double freqCount, sumOfNodeCounts = 0;

  idx = iter.baseBefore + iter.parentAfterValueIdx;

  for (nodeValIdx = 0;
       nodeValIdx < iter.cardNode;
       nodeValIdx++, idx += iter.dimAfter) {
    freqCount = (idx < freq.size()) ? freq[idx] : 0;  // freq[idx];
    sumOfNodeCounts += freqCount;
    nodeCounts[nodeValIdx] = (freqCount == 0) ? this->epsilon : freqCount;
  }
  return sumOfNodeCounts;
}


//=============================================================================

namespace PILGRIM {
    template class pmDecomposableScore<std::vector<unsigned int>>;
    template class pmDecomposableScore<std::vector<int>>;
    template class pmDecomposableScore<std::vector<float>>;
    template class pmDecomposableScore<std::vector<double>>;
    template class pmDecomposableScore<std::vector<long double>>;

    template class pmDecomposableScore<unsigned int*>;
    template class pmDecomposableScore<int*>;
    template class pmDecomposableScore<float*>;
    template class pmDecomposableScore<double*>;
    template class pmDecomposableScore<long double*>;
}; // namespace PILGRIM
