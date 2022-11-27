
#include <math.h>
#include <plGamma.h>

#include "pilgrim/general/pmUtility.h"
#include "pilgrim/general/scores/ScoreBDeu.h"

using namespace boost;
using namespace std;
using namespace PILGRIM;

template <typename rowDataType>
unsigned int pmScoreBDeu<rowDataType>::functionCalls = 0;

//=============================================================================
inline double logGamma(double x) {
  return plLgamma(x);
}

//=============================================================================
template <typename rowDataType>
pmScoreBDeu<rowDataType>::pmScoreBDeu(plJointDistribution& jd,
                                      pmFrequencyCounter* pFrequencyCounter,
                                      unsigned long int Nprime, pmCache* cache)
  : pmDecomposableScore<rowDataType>(jd, pFrequencyCounter, cache) {
  this->setPriorWeight(Nprime);
}

//=============================================================================
template <typename rowDataType>
pmScoreBDeu<rowDataType>::pmScoreBDeu(pmBayesianNetwork* pBN,
                                      pmFrequencyCounter* pFrequencyCounter,
                                      unsigned long int Nprime, pmCache* cache)
  : pmDecomposableScore<rowDataType>(pBN, pFrequencyCounter, cache) {
  this->setPriorWeight(Nprime);
}

//=============================================================================
template <typename rowDataType>
pmScoreBDeu<rowDataType>::pmScoreBDeu(unsigned long int Nprime, 
	pmBayesianNetwork* pBN,
	pmFrequencyCounter* pFrequencyCounter,
	pmCache* cache)
	: pmDecomposableScore<rowDataType>(pBN, pFrequencyCounter, cache) {
	this->setPriorWeight(Nprime);
}


//=============================================================================
template <typename rowDataType>
pmScoreBDeu<rowDataType>::~pmScoreBDeu() {
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreBDeu<rowDataType>::computeNode(pmNode& node) {
  ++functionCalls;

  if(!this->cache) {  // same as the old function if there is no cache
    pmScoreValueType score = 0;
    unsigned int k, nodeCount = (unsigned int)num_vertices(this->pBN->graph);
    vector< bool > searchedVars(nodeCount, false);
    vector< plFloat > freq, Nijk;
    double Nij;
    pmScoreValueType alpha_ij, alpha_ijk;
    pmDCValueIterator iter;

    this->iteratorInit(iter, node, searchedVars);
    alpha_ij = (pmScoreValueType)this->Nprime
      / (pmScoreValueType)this->pBN->computeParentDim(node);
    alpha_ijk = alpha_ij / (pmScoreValueType)iter.cardNode;
    Nijk.resize(iter.cardNode);
    this->pFrequencyCounter->frequencyCount(searchedVars, freq);
    do {
      Nij = this->getNodeCounts(iter, freq, Nijk);
      for (k = 0; k < iter.cardNode; k++)
        score += logGamma(Nijk[k] + alpha_ijk) - logGamma(alpha_ijk);
      score += logGamma(alpha_ij) - logGamma(Nij + alpha_ij);
    } while (this->iteratorNextParentValue(iter));

    return score;
  } else {
    plVariablesConjunction parents;
    parents = this->pBN->getParents(node);
    int neu = this->pBN->getVariableIndex(node);

    pmScoreValueType s = this->cache->isPresent(neu, parents);
    if (s != -1) {  // lire le score
      return s;
    }
    pmScoreValueType score = 0;
    unsigned int k, nodeCount = (unsigned int)num_vertices(this->pBN->graph);
    vector< bool > searchedVars(nodeCount, false);
    vector< plFloat > freq, Nijk;
    double Nij;
    pmScoreValueType alpha_ij, alpha_ijk;
    pmDCValueIterator iter;

    this->iteratorInit(iter, node, searchedVars);
    alpha_ij = (pmScoreValueType)this->Nprime
      / (pmScoreValueType)this->pBN->computeParentDim(node);
    alpha_ijk = alpha_ij / (pmScoreValueType)iter.cardNode;
    Nijk.resize(iter.cardNode);
    this->pFrequencyCounter->frequencyCount(searchedVars, freq);
    do {
      Nij = this->getNodeCounts(iter, freq, Nijk);
      for (k = 0; k < iter.cardNode; k++)
        score += logGamma(Nijk[k] + alpha_ijk) - logGamma(alpha_ijk);
      score += logGamma(alpha_ij) - logGamma(Nij + alpha_ij);
    } while (this->iteratorNextParentValue(iter));

  this->cache->insert(neu, parents, score);  // insertion du score en cache
  return score;
  }
}

//=============================================================================
template <typename rowDataType>
void pmScoreBDeu<rowDataType>::setPriorWeight(unsigned long int Nprime) {
  this->Nprime = Nprime;
}

//=============================================================================
template <typename rowDataType>
unsigned long int pmScoreBDeu<rowDataType>::getPriorWeight() {
  return Nprime;
}

//=============================================================================

namespace PILGRIM {
    template class pmScoreBDeu<std::vector<unsigned int>>;
    template class pmScoreBDeu<std::vector<int>>;
    template class pmScoreBDeu<std::vector<float>>;
    template class pmScoreBDeu<std::vector<double>>;
    template class pmScoreBDeu<std::vector<long double>>;

    template class pmScoreBDeu<unsigned int*>;
    template class pmScoreBDeu<int*>;
    template class pmScoreBDeu<float*>;
    template class pmScoreBDeu<double*>;
    template class pmScoreBDeu<long double*>;
}; // namespace PILGRIM
