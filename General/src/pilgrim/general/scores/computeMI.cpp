
#include "pilgrim/general/scores/computeMI.h"
#include <iostream>
#include <math.h>

using namespace std;
using namespace PILGRIM;

unsigned int pmMI::functionCalls = 0;
std::map<std::string, MIResultBackup> pmMI::testValuesCache;



pmMI::pmMI(pmCSVDataSet * const _pCompDataSet, plVariablesConjunction *_variables, bool _cache, bool _usingSlidingWindow,
  float *_alpha, float *_p):mutualInformation( _pCompDataSet, _variables, _usingSlidingWindow, _alpha, _p) {
  this->cache= _cache;
}

bool pmMI::computeMI(const unsigned int& a, const unsigned int& b) {
  std::vector<unsigned int> cond_vars;

  return computeMI(a, b, cond_vars);
}

bool pmMI::computeMI(const unsigned int& a, const unsigned int& b, std::vector<unsigned int> cond_vars) {
  std::ofstream log_file(
		"log_file.txt", std::ios_base::out | std::ios_base::app);
	log_file << "====computeMI() method called from the computeMI class" << endl;
  ++functionCalls;  // function call counter
  log_file<<"Checking Cache if it's set to TRUE in the computeMI class constructor"<<endl;
  if (cache) {
    log_file<<"Cache variable is TRUE"<<endl;
    // To put MI values in cache generate key by joining variable indexes
    // Put variables in a vector then sort them
    std::vector<unsigned int> varAB;
    varAB.reserve(2);
    varAB.push_back(a); varAB.push_back(b);
    sort(varAB.begin(), varAB.end());
    sort(cond_vars.begin(), cond_vars.end());

    // Generate a string by variable names and conditional variables like v12_1_23_4
    stringstream varNames;
    varNames << "v";
    varNames << varAB[0];
    varNames << "_";
    varNames << varAB[1];
    for (size_t j = 0; j < cond_vars.size(); j++) {
      varNames << cond_vars[j];
      varNames << "_";  // to avoid v1 v2 and v12
    }

    // Find whether test already calculated.
    std::map<std::string, MIResultBackup>::iterator map_Iter;
    clock_t tStart;
    tStart = clock();
    map_Iter = testValuesCache.find(varNames.str());

    if (debugMode) {   // only for test purpose it can be removed if it generates error.
       AlgoMMPC::MIsearchTime += (clock()- tStart);
       AlgoMMPC::cachesize= testValuesCache.size();
    }

    bool test = false;
    MIResultBackup testResult;  // to store the chi2 values
    if (map_Iter == testValuesCache.end()) {
      tStart = clock();
      log_file<<"======calling computeMI of the pmMI class"<<endl;
      test = mutualInformation::computeMI(a, b, cond_vars);
      if (debugMode) {
         AlgoMMPC::MItime += (clock()- tStart);   // only for test purpose it can be removed if it generates error.
      }

      testResult.test = test;
      testResult.MIValue= mutualInformation::getMI();
      testResult.pValue = mutualInformation::getPValue();
      testResult.quantileValue = mutualInformation::getQuantileValue();
      testValuesCache.insert(make_pair(varNames.str(), testResult));
    } else {
      test = map_Iter->second.test;
      mutualInformation::MI = map_Iter->second.MIValue;
      mutualInformation::p = map_Iter->second.pValue ;
      mutualInformation::Quantile_value = map_Iter->second.quantileValue;
    }
    return test;
  } else {
    return mutualInformation::computeMI(a, b, cond_vars);
  }
}

void pmMI::clearCache() {
  testValuesCache.clear();
}
