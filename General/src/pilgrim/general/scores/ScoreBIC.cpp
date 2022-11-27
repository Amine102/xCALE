
#include "pilgrim/general/scores/ScoreBIC.h"

using namespace boost;
using namespace std;
using namespace PILGRIM;

template <typename rowDataType>
unsigned int pmScoreBIC<rowDataType>::functionCalls = 0;

//=============================================================================
template <typename rowDataType>
pmScoreBIC<rowDataType>::pmScoreBIC(plJointDistribution& jd, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
: pmDecomposableScore<rowDataType>(jd, pFrequencyCounter,cache) {
}

//=============================================================================
template <typename rowDataType>
pmScoreBIC<rowDataType>::pmScoreBIC(pmBayesianNetwork* pBN, pmFrequencyCounter* pFrequencyCounter, pmCache* cache)
: pmDecomposableScore<rowDataType>(pBN, pFrequencyCounter,cache) {
}

//=============================================================================
template <typename rowDataType>
pmScoreBIC<rowDataType>::~pmScoreBIC() {
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreBIC<rowDataType>::computeNode(pmNode& node) {
  ++functionCalls;

  if (!this->cache) {  // same as the old function if there is no cache
    return this->computeNodeMaxLikelihood(node)
        -0.5 * (pmScoreValueType)this->pBN->computeNodeDim(node)
        * log((double)this->pFrequencyCounter->getExempleCount());  // + computeDLnet(pBN);
  } else {
    plVariablesConjunction parents;
    parents = this->pBN->getParents(node);
    int neu = this->pBN->getVariableIndex(node);

    pmScoreValueType s = this->cache->isPresent(neu, parents);
    if (s != -1) {  // lire le score
      return s;
    }
    pmScoreValueType sc = this->computeNodeMaxLikelihood(node)
                -0.5 * (pmScoreValueType)this->pBN->computeNodeDim(node)
                * log((double)this->pFrequencyCounter->getExempleCount());  // + computeDLnet(pBN);
    this->cache->insert(neu, parents, sc);  // insertion du score en cache
    return sc/*/this->pFrequencyCounter->getExempleCount()*/;
  }
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreBIC<rowDataType>::computeDLnet( pmBayesianNetwork* pBN) {
  const plVariablesConjunction& variables = pBN->getVariables();
  unsigned int j, nmbParents, n = variables.dim();

  unsigned int cardinalityJ;
  pmScoreValueType totalBIC, term1, term2;

  pmNode node; // parent;

  totalBIC = 0;
  term1 = term2 = 0;

  // To use later in the formula
  double logNby2 = 0.5*log((double)this->pFrequencyCounter->getExempleCount());
  double logn = log(double(n));

  for (j = 0; j < n; j++) {
    cardinalityJ = pBN->getCardinality(j);
    node = vertex(j, pBN->graph);
    nmbParents = in_degree(node, pBN->graph);
    term1 += (log((double)cardinalityJ) + (1 + nmbParents)*logn);
    term2 += (nmbParents * (cardinalityJ-1));
  }
  term2 *= logNby2;

  return term1 + term2;
}

template <typename rowDataType>
pmScoreValueType pmScoreBIC<rowDataType>::computeBICMatrix(pmBayesianNetwork* pBN,
                               pmBayesianNetwork* pFullBN,
                               vector<plDistribution>& P_Xi,
                               vector<plCndDistribution>& P_Xj_given_Xi,
                               unsigned long int exempleCount) {
  const plVariablesConjunction& variables = pFullBN->getVariables();
  plVariable Xj, Xi;
  plValues values(variables);
  property_map<pmGraph, edge_weight_t>::type weightMap
    = get(edge_weight, pFullBN->graph);
  unsigned int i, j, n = variables.dim();
  unsigned int cardinalityJ, cardinalityI;
  pmScoreValueType bicOneParent, bicNoParent, totalBIC, weight;

  pmNode node, parent;
  pmNode nodeOnFullBN, parentOnFullBN;
  pmEdge e;
  bool foundEdge, lookEdge;

  totalBIC = 0;
  for (j = 0; j < n; j++) {
    Xj = variables[j];
    cardinalityJ = pFullBN->getCardinality(j);
    bicNoParent = pmScoreBIC::computeLocalBICScoreNoParent(j, Xj, cardinalityJ,
      values, P_Xi,
      exempleCount);
    nodeOnFullBN = vertex(j, pFullBN->graph);
    node = vertex(j, pBN->graph);
    if(in_degree(node, pBN->graph) == 0) {
      totalBIC += bicNoParent;
      lookEdge = false;
    } else {
      lookEdge = true;
    }

    for (i = 0; i < n; i++) {
      if (i == j)
        continue;
      Xi = variables[i];
      cardinalityI = pFullBN->getCardinality(i);
      parentOnFullBN = vertex(i, pFullBN->graph);

      bicOneParent = pmScoreBIC::computeLocalBICScoreOneParent(j, Xj,
        cardinalityJ, i,
        Xi, cardinalityI,
        values, P_Xi,
        P_Xj_given_Xi,
        exempleCount);
      weight = bicNoParent - bicOneParent;

      boost::tie(e, foundEdge) = edge(parentOnFullBN, nodeOnFullBN, pFullBN->graph);
      put(weightMap, e, weight);

      if (lookEdge) {
        parent = vertex(i, pBN->graph);
        boost::tie(e, foundEdge) = edge(parent, node, pBN->graph);
        if (foundEdge) {
          totalBIC += bicOneParent;
          lookEdge = false;
        }
      }
    }
  }
  return totalBIC;
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreBIC<rowDataType>::
computeLocalBICScoreOneParent(unsigned int j,
                plVariable& Xj,
                unsigned int cardinalityJ,
                unsigned int i,
                plVariable& Xi,
                unsigned int cardinalityI,
                plValues& values,
                vector<plDistribution>& P_Xi,
                vector<plCndDistribution>& P_Xj_given_Xi,
                unsigned long int exempleCount) {
  pmScoreValueType LL, bic;
  plProbValue theta_ijk, theta_ij;
  pmScoreValueType Nijk;
  plDistribution Distribution;
  unsigned int n = P_Xi.size();

  LL = 0;
  values.reset(Xi);
  do {
    P_Xj_given_Xi[j*n+i].instantiate(Distribution, values);
    values.reset(Xj);
    do {
      theta_ijk = Distribution.compute(values);
      theta_ij = P_Xi[i].compute(values);
      Nijk = (pmScoreValueType)exempleCount * theta_ijk * theta_ij;
      LL += Nijk * log(theta_ijk);
    } while (values.next(Xj));
  } while (values.next(Xi));

  bic = LL;  // - log(exempleCount) * (cardinalityJ - 1) * cardinalityI / 2;
  return bic;
}

//=============================================================================
template <typename rowDataType>
pmScoreValueType pmScoreBIC<rowDataType>::
computeLocalBICScoreNoParent(unsigned int j, plVariable& Xj,
               unsigned int cardinalityJ,
               plValues& values,
               vector<plDistribution>& P_Xi,
               unsigned long int exempleCount) {
  plProbValue theta_ijk;
  pmScoreValueType LL, bic;
  pmScoreValueType Nijk;

  Nijk = 0;
  LL = 0;
  values.reset(Xj);
  do {
    theta_ijk = P_Xi[j].compute(values);
    Nijk = theta_ijk * (pmScoreValueType)exempleCount;
    LL += Nijk * log(theta_ijk);
  } while (values.next(Xj));
  bic = LL;   // - log(exempleCount) * (cardinalityJ - 1) / 2;
  return bic;
}

//=============================================================================

namespace PILGRIM {
    template class pmScoreBIC<std::vector<unsigned int>>;
    template class pmScoreBIC<std::vector<int>>;
    template class pmScoreBIC<std::vector<float>>;
    template class pmScoreBIC<std::vector<double>>;
    template class pmScoreBIC<std::vector<long double>>;

    template class pmScoreBIC<unsigned int*>;
    template class pmScoreBIC<int*>;
    template class pmScoreBIC<float*>;
    template class pmScoreBIC<double*>;
    template class pmScoreBIC<long double*>;
}; // namespace PILGRIM
