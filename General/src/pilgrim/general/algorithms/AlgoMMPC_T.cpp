
#include "pilgrim/general/algorithms/AlgoMMPC_T.h"

using namespace PILGRIM;

//to temporarily store learned cpc from MMPCbar
std::map <unsigned int, std::vector<unsigned int>> AlgoMMPC_T::cpcMapList;
std::map <unsigned int,std::map<unsigned int,std::vector<unsigned int>>> AlgoMMPC_T::cpc_tMapList;
float AlgoMMPC_T::MItime = 0, AlgoMMPC_T::MIsearchTime = 0;
int AlgoMMPC_T::cachesize;

int AlgoMMPC_T::totalFunctionCalls = 0;

//==========================================================================
AlgoMMPC_T::AlgoMMPC_T( pmCSVDataSet* _dataSet,  
                        const unsigned int &_condVarSize, // = 10
                        bool _debugMod // = false
                        ) : AlgoMMPCbar_T(_dataSet, _condVarSize, _debugMod), 
                            ds(_dataSet) {}  // initialize class members

//==========================================================================
AlgoMMPC_T::~AlgoMMPC_T(){}

//==========================================================================
void AlgoMMPC_T::computeMMPC_T(unsigned int &vTarget, CPC_t &CPCs, CPC_t &CCs, CPC_t &PCs) {
  std::vector< unsigned int > cpcOfVariableX, cpcOfTargetVariable, cpcOfTargetVariable1, ccOfTargetVariable, pcOfTargetVariable;
  cpcOfTargetVariable.reserve(numVars);
  cpcOfVariableX.reserve(numVars);
  std::vector< unsigned int >::iterator itr;

    //- find in the cache if already calculated then retrieve
    // Compute the vector of CPC from the vTarget
    this->computeCPCbar_T(vTarget, cpcOfTargetVariable);  // call for MMPC bar algorithm

    // For all the cpc of target variable found
    for (int i = 0; i < cpcOfTargetVariable.size(); i++) {
      // If my variable is a PC one :
      if (cpcOfTargetVariable.at(i) < numVars/2) {
        pcOfTargetVariable.push_back(cpcOfTargetVariable.at(i));
      }
      // else, my variable is a CPC one :
      else {
        // Compute the CPC for my vTarget's CPC variable
        this->computeCPCbar_T(cpcOfTargetVariable.at(i), cpcOfVariableX);

        // If the vTarget exists in my cpcOfVariableX
        if (this->isInList(vTarget, cpcOfVariableX)) {
          cpcOfTargetVariable1.push_back(cpcOfTargetVariable.at(i));
        }
        cpcOfVariableX.clear();
      }
    }    

    // put individual cpc list in a map
    CPCs[vTarget]=cpcOfTargetVariable1;
    // put individual pc list in a map
    PCs[vTarget]=pcOfTargetVariable;
}

//=============================================================================
void AlgoMMPC_T::computeCPCbar_T(unsigned int& vTarget, std::vector<unsigned int>& cpcOfTargetVariable) {
    // check whether cpc of target variable already calculated or not.
    std::map <unsigned int, vector<unsigned int>>::const_iterator itrMap;
    itrMap = cpcMapList.find(vTarget);

    if (itrMap != cpcMapList.end()) {
      if (this->debugMode) {
        cout << "CPC for v(" << vTarget << ") already calculated" << endl;
      }
      cpcOfTargetVariable = itrMap->second;
    } else {
      // clock_t tStart = clock();
      MItime = 0; MIsearchTime = 0; subsetTime = 0;
      if (this->debugMode) {
        cout << "CPC for v(" << vTarget << ") not calculated: " << endl;
      }
      // Call for MMPCbar algo
      this->computeMMPCbar_T(vTarget, cpcOfTargetVariable);
      incrementCounters();

      // Insert CPC list in map for cache
      cpcMapList.insert( make_pair(vTarget, cpcOfTargetVariable));
      cpc_tMapList.insert( make_pair(t, cpcMapList));
    }
}

//=============================================================================
void AlgoMMPC_T::incrementCounters() {
  AlgoMMPC_T::totalFunctionCalls += nFunctionCalls;
}

//=============================================================================
void AlgoMMPC_T::resetCounters() {
  totalFunctionCalls = 0;
}

