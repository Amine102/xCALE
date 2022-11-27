
#include "pilgrim/general/scores/Score.h"

using namespace PILGRIM;

//=============================================================================
template <typename rowDataType>
pmScore<rowDataType>::pmScore(plJointDistribution& jd,
                              pmFrequencyCounter* pFrequencyCounter) {
  this->freeBN = false;
  this->setJointDistribution(jd);
  this->setFrequencyCounter(pFrequencyCounter);
}

//=============================================================================
template <typename rowDataType>
pmScore<rowDataType>::pmScore(pmBayesianNetwork* pBN,
                              pmFrequencyCounter* pFrequencyCounter) {
  this->freeBN = false;
  this->setBayesianNetwork(pBN);
  this->setFrequencyCounter(pFrequencyCounter);
}


//=============================================================================
template <typename rowDataType>
pmScore<rowDataType>::~pmScore() {
  this->setBayesianNetwork(NULL, false);
}

//=============================================================================
template <typename rowDataType>
void pmScore<rowDataType>::setJointDistribution(plJointDistribution& jd) {
  pmBayesianNetwork* pBN = new pmBayesianNetwork(jd);
  this->setBayesianNetwork(pBN, true);
}

//=============================================================================
template <typename rowDataType>
plJointDistribution pmScore<rowDataType>::getJointDistribution() {
  if (this->pBN)
    return pBN->getJointDistribution();
  return plJointDistribution();
}

//=============================================================================
template <typename rowDataType>
void pmScore<rowDataType>::setBayesianNetwork(pmBayesianNetwork* pBN
                                              , bool autoDelete) {
  if (this->freeBN && this->pBN)
    delete this->pBN;
  this->freeBN = autoDelete;
  this->pBN = pBN;
}

//=============================================================================
template <typename rowDataType>
pmBayesianNetwork* pmScore<rowDataType>::getBayesianNetwork() {
  return this->pBN;
}

//=============================================================================
template <typename rowDataType>
void pmScore<rowDataType>::
setFrequencyCounter(pmFrequencyCounter* pFrequencyCounter) {
  this->pFrequencyCounter = pFrequencyCounter;
}

//=============================================================================
template <typename rowDataType>
pmFrequencyCounter* pmScore<rowDataType>::getFrequencyCounter() {
  return this->pFrequencyCounter;
}


//=============================================================================

namespace PILGRIM {
    template class pmScore<std::vector<unsigned int>>;
    template class pmScore<std::vector<int>>;
    template class pmScore<std::vector<float>>;
    template class pmScore<std::vector<double>>;
    template class pmScore<std::vector<long double>>;

    template class pmScore<unsigned int*>;
    template class pmScore<int*>;
    template class pmScore<float*>;
    template class pmScore<double*>;
    template class pmScore<long double*>;
}; // namespace PILGRIM
