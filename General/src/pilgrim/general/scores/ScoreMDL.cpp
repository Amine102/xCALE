
#include "pilgrim/general/scores/ScoreMDL.h"
#include <iostream>

using namespace PILGRIM;

template <typename rowDataType>
unsigned int pmScoreMDL<rowDataType>::functionCalls = 0;

//=============================================================================
template <typename rowDataType>
pmScoreMDL<rowDataType>::pmScoreMDL(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
  : pmDecomposableScore<rowDataType>(jd, pFrequencyCounter, cache) {
}

//=============================================================================
template <typename rowDataType>
pmScoreMDL<rowDataType>::pmScoreMDL(pmBayesianNetwork* pBN, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
  : pmDecomposableScore<rowDataType>(pBN, pFrequencyCounter, cache) {
}


//=============================================================================
template <typename rowDataType>
pmScoreMDL<rowDataType>::~pmScoreMDL() {
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreMDL<rowDataType>::computeNode(pmNode& node) {
  ++functionCalls;

  if (!this->cache) {  //same as the old function if there is no cache
    int c = (int)ceil(log((double)this->pBN->getMaxCardinality()) / log((double)2));  // log2(a)
    return (this->computeNodeMaxLikelihood(node)
      - c * this->pBN->computeNodeDim(node)
      - in_degree(node, this->pBN->graph) * log((double)this->pFrequencyCounter->getExempleCount()));
  } else {
    plVariablesConjunction parents;
    parents = this->pBN->getParents(node);
    int neu = this->pBN->getVariableIndex(node);

    pmScoreValueType s = this->cache->isPresent(neu, parents);
    if (s != -1) {  // lire le score
      return s;
    }

    int c = (int)ceil(log((double)this->pBN->getMaxCardinality()) / log((double)2));  // log2(a)

    pmScoreValueType sc = this->computeNodeMaxLikelihood(node)
              - c * this->pBN->computeNodeDim(node)
              - in_degree(node, this->pBN->graph) * log((double)this->pFrequencyCounter->getExempleCount());

    this->cache->insert(neu, parents, sc);  // insertion du score en cache

    return sc;
  }
}

//=============================================================================

namespace PILGRIM {
    template class pmScoreMDL<std::vector<unsigned int>>;
    template class pmScoreMDL<std::vector<int>>;
    template class pmScoreMDL<std::vector<float>>;
    template class pmScoreMDL<std::vector<double>>;
    template class pmScoreMDL<std::vector<long double>>;

    template class pmScoreMDL<unsigned int*>;
    template class pmScoreMDL<int*>;
    template class pmScoreMDL<float*>;
    template class pmScoreMDL<double*>;
    template class pmScoreMDL<long double*>;
}; // namespace PILGRIM
