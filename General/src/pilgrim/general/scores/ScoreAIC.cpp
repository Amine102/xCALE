
#include "pilgrim/general/scores/ScoreAIC.h"

using namespace PILGRIM;

template <typename rowDataType>
unsigned int pmScoreAIC<rowDataType>::functionCalls = 0;

//=============================================================================
template <typename rowDataType>
pmScoreAIC<rowDataType>::pmScoreAIC(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
  : pmDecomposableScore<rowDataType>(jd, pFrequencyCounter, cache) {
}

//=============================================================================
template <typename rowDataType>
pmScoreAIC<rowDataType>::pmScoreAIC(pmBayesianNetwork* pBN, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
  : pmDecomposableScore<rowDataType>(pBN, pFrequencyCounter, cache) {
}

//=============================================================================
template <typename rowDataType>
pmScoreAIC<rowDataType>::~pmScoreAIC() {
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreAIC<rowDataType>::computeNode(pmNode& node) {
 ++functionCalls;

  if (!this->cache) {  // same as the old function if there is no cache
    return this->computeNodeMaxLikelihood(node)
      - (pmScoreValueType)this->pBN->computeNodeDim(node);
  } else {
    plVariablesConjunction parents;
    parents = this->pBN->getParents(node);
    int neu = this->pBN->getVariableIndex(node);

    pmScoreValueType s = this->cache->isPresent(neu, parents);
    if (s != -1) {  // lire le score
      return s;
    }

    pmScoreValueType sc = this->computeNodeMaxLikelihood(node)
      - (pmScoreValueType)this->pBN->computeNodeDim(node);

    this->cache->insert(neu, parents, sc);  // insertion du score en cache
    return sc;
  }
}

//=============================================================================

namespace PILGRIM {
    template class pmScoreAIC<std::vector<unsigned int>>;
    template class pmScoreAIC<std::vector<int>>;
    template class pmScoreAIC<std::vector<float>>;
    template class pmScoreAIC<std::vector<double>>;
    template class pmScoreAIC<std::vector<long double>>;

    template class pmScoreAIC<unsigned int*>;
    template class pmScoreAIC<int*>;
    template class pmScoreAIC<float*>;
    template class pmScoreAIC<double*>;
    template class pmScoreAIC<long double*>;
}; // namespace PILGRIM
