#include "pilgrim/general/algorithms/AlgoDynamic.h"
#include <vector>
#include <string>
#include <plDebug.h>
#include <sstream>

using namespace std;
using namespace boost;
using namespace PILGRIM;

template <typename rowDataType>
int pmAlgoDynamic<rowDataType>::totalNbrs = 0;

/*
=====CONSTRUCTOR=====
*/

//=============================================================================
template <typename rowDataType>
pmAlgoDynamic<rowDataType>::pmAlgoDynamic(pmAlgorithm<rowDataType>* initialAlgo,
                													pmAlgorithm_T<rowDataType>* transitionAlgo) {
	this->algo_t0 = initialAlgo;
	this->algo_T = transitionAlgo;
	initDBN();
}

/*
=====DESTRUCTOR=====
*/
//=============================================================================
template <typename rowDataType>
pmAlgoDynamic<rowDataType>::~pmAlgoDynamic() {
	delete algo_t0;
	delete algo_T;
}

/*
=====INIT=====
*/
//=============================================================================
template <typename rowDataType>
void pmAlgoDynamic<rowDataType>::initDBN() {
	// Using getInitialBN and getTransitionBN pure virtual getters in Algorithm and Algorithm_T
	this->dynamicBN = new pmDynamicBayesianNetwork(this->algo_t0->getInitialBN(), this->algo_T->getTransitionBN());
}


/*
=====METHOD=====
*/

//=============================================================================
template <typename rowDataType>
void pmAlgoDynamic<rowDataType>::run(pmDynamicBayesianNetwork* dynBN) {
	// Want to access again because the two algos migth have been set
	pmDynamicBayesianNetwork* computedDBN = dynBN;
	algo_t0->run(this->algo_t0->getInitialBN());
	algo_T->run( this->algo_T->getTransitionBN());
}
/*
=====PRINTER=====
*/
//=============================================================================
template <typename rowDataType>
void pmAlgoDynamic<rowDataType>::summary() {
	this->dynamicBN->summary();
}

template <typename rowDataType>
pmDynamicBayesianNetwork* pmAlgoDynamic<rowDataType>::getDynamicInitBN(){
	return this->dynamicBN;
}

//=============================================================================

namespace PILGRIM {
    template class pmAlgoDynamic<std::vector<unsigned int>>;
    template class pmAlgoDynamic<std::vector<int>>;
    template class pmAlgoDynamic<std::vector<float>>;
    template class pmAlgoDynamic<std::vector<double>>;
    template class pmAlgoDynamic<std::vector<long double>>;

    template class pmAlgoDynamic<unsigned int*>;
    template class pmAlgoDynamic<int*>;
    template class pmAlgoDynamic<float*>;
    template class pmAlgoDynamic<double*>;
    template class pmAlgoDynamic<long double*>;
}; // namespace PILGRIM