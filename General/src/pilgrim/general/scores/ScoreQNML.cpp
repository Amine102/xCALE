
#include "pilgrim/general/scores/ScoreQNML.h"

using namespace PILGRIM;



template <typename rowDataType>
unsigned int pmScoreQNML<rowDataType>::functionCalls = 0;
double regret(size_t N, size_t r);

//=============================================================================
template <typename rowDataType>
pmScoreQNML<rowDataType>::pmScoreQNML(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
	: pmDecomposableScore<rowDataType>(jd, pFrequencyCounter, cache) {
}

//=============================================================================
template <typename rowDataType>
pmScoreQNML<rowDataType>::pmScoreQNML(pmBayesianNetwork* pBN, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
	: pmDecomposableScore<rowDataType>(pBN, pFrequencyCounter, cache) {
}

//=============================================================================
template <typename rowDataType>
pmScoreQNML<rowDataType>::~pmScoreQNML() {
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreQNML<rowDataType>::computeNode(pmNode& node) {
	++functionCalls;

	if (!this->cache) {  // same as the old function if there is no cache
		return this->computeNodeMaxLikelihood(node)  //ML 
			- (pmScoreValueType)regret((size_t)this->pFrequencyCounter->getExempleCount(), (size_t) this->pBN->computeParentDim(node) *
			this->pBN->getCardinality(this->pBN->getVariableIndex(node))) // - regret (N, qi*r)
			+ (pmScoreValueType)regret((size_t)this->pFrequencyCounter->getExempleCount(), (size_t) this->pBN->computeParentDim(node)); // + regret(N,qi)
	}
	else {
		plVariablesConjunction parents;
		parents = this->pBN->getParents(node);
		int neu = this->pBN->getVariableIndex(node);

		pmScoreValueType s = this->cache->isPresent(neu, parents);
		if (s != -1) {  // lire le score
			return s;
		}

		pmScoreValueType sc = this->computeNodeMaxLikelihood(node) //ML 
			- (pmScoreValueType)regret((size_t)this->pFrequencyCounter->getExempleCount(), (size_t) this->pBN->computeParentDim(node) *
			this->pBN->getCardinality(this->pBN->getVariableIndex(node))) // - regret (N, qi*r)
			+ (pmScoreValueType)regret((size_t)this->pFrequencyCounter->getExempleCount(), (size_t) this->pBN->computeParentDim(node)); // + regret(N,qi);

		this->cache->insert(neu, parents, sc);  // insertion du score en cache
		return sc;
	}
}

double regret(size_t N, size_t r){

	double regret;

	double alpha = double(r) / N;
	double calpha = 0.5 + (0.5* sqrt(1 + 4 / alpha));

	regret = N * (log(alpha) + (alpha + 2) * log(calpha) - 1 / calpha) - 0.5 * log(calpha + 2 / alpha);

	return regret;
}

//=============================================================================

namespace PILGRIM {
    template class pmScoreQNML<std::vector<unsigned int>>;
    template class pmScoreQNML<std::vector<int>>;
    template class pmScoreQNML<std::vector<float>>;
    template class pmScoreQNML<std::vector<double>>;
    template class pmScoreQNML<std::vector<long double>>;

    template class pmScoreQNML<unsigned int*>;
    template class pmScoreQNML<int*>;
    template class pmScoreQNML<float*>;
    template class pmScoreQNML<double*>;
    template class pmScoreQNML<long double*>;
}; // namespace PILGRIM
