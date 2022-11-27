
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/algorithms/AlgoMMPCbar_T.h"
#include "pilgrim/general/scores/computeMI.h"
#include "pilgrim/general/log.h"

unsigned int PILGRIM::AlgoMMPCbar_T::varCounter = 0;
unsigned int PILGRIM::AlgoMMPCbar_T::numVars;                  // Number of total vars
plVariablesConjunction PILGRIM::AlgoMMPCbar_T::all_variable;


int PILGRIM::AlgoMMPCbar_T::t = 0;
std::vector<unsigned int> PILGRIM::AlgoMMPCbar_T::CPC;
std::vector<unsigned int> PILGRIM::AlgoMMPCbar_T::CC;
std::vector<unsigned int> PILGRIM::AlgoMMPCbar_T::PC;

std::map <unsigned int, std::vector<unsigned int> > PILGRIM::AlgoMMPCbar_T::CPC_tBar;
std::map <unsigned int, std::vector<unsigned int> > PILGRIM::AlgoMMPCbar_T::cpcPath;

using namespace PILGRIM;
using namespace boost;
using namespace std;

//==========================================================================
AlgoMMPCbar_T::AlgoMMPCbar_T( pmCSVDataSet* _dataSet, 
                              const unsigned int &_condVarSize /*= 10*/, 
                              bool _debugMode /*= false */
                              ) : ds(_dataSet), 
                                  conditionalVariableSize(_condVarSize), 
                                  debugMode(_debugMode) {
  this->alpha = 0.05;
}

//==========================================================================
AlgoMMPCbar_T::~AlgoMMPCbar_T(){}

//==========================================================================
void AlgoMMPCbar_T::setAlpha(float alpha) {
	if (alpha >= 0 && alpha <= 1){
		this->alpha = alpha;
	}
	else{
		std::cout << "\n Alpha must be a float [0,1] - set to 0.05 \n";
	}
}

//==========================================================================
void AlgoMMPCbar_T::computeMMPCbar_T( unsigned int& vTarget, std::vector<unsigned int>& CPCofTargetVar) {

  cout << endl << "\t(" << varCounter++ << ") - Learning(v" << vTarget << ").... " << endl << endl;

  CPCBar = &CPCofTargetVar;  // copy the address of cpc variable

  cout << " - Starting phase1 " << endl;
  this->phase1_T(vTarget, 1);
  cout << " - Starting phase2" << endl;
  this->phase2_T(vTarget, 1);

  
}

//=============================================================================
void AlgoMMPCbar_T::phase1_T(unsigned int& vTarget, int step) {
  varList.reserve(numVars);
  fullVarList.reserve(numVars);
  varList.clear();
  fullVarList.clear();

  std::vector<unsigned int> tempKbest;
  tempKbest.reserve(numVars);

  unsigned int varF;
  float assocF = -1;
  bool isChangeCPC = true;

  MaxMinVarMap mapMinAssoc;  // to store further as a kbest

  for (size_t i = 0; i < numVars; i++) {
    if (i != vTarget) {
      varList.push_back(i);
    }
  } 
  

  size_t i = 0;

  while (i < CPCBar->size()) {
    remove(varList.begin(), varList.end(), CPCBar->at(i));
    ++i;
  }

 

  bool ind = 0;


  while (isChangeCPC && !varList.empty()) {
    assocF = -1;  // reset on each iteration to avoid duplication
    ind = this->computeMaxMinHeuristic(vTarget, varF, assocF, mapMinAssoc);
    if (ind != true && assocF != -1) {
      CPCBar->push_back(varF);
      varList.erase(std::remove(varList.begin(), varList.end(), varF), varList.end());
      isChangeCPC = true;
    } else {
      isChangeCPC = false;
      continue;
    }
  }  // endwhile
}

//=============================================================================
bool AlgoMMPCbar_T::computeMaxMinHeuristic(unsigned int& vTarget, unsigned int& varF, float& assocF, MaxMinVarMap& mapMinAssoc) {
  bool ind = 0;
  std::map<unsigned int, bool> mapInd;  // to store the variable and its independence.
  std::map<unsigned int, bool>::iterator mapIndIter;
  mapMinAssoc.clear();  // reset for each iteration
  ostrstream osBuffer;  // to output in a logfile
  Log *printLog = new Log('l');

  // To calculate independence of target variable with all variables
  pair_t minAssoc;
  std::vector<unsigned int>::iterator variableIndex = varList.begin() ;

  for (int x=0; x < varList.size();) {
    bool variableFound = isInList(*variableIndex, *CPCBar);

	

    if (*variableIndex == vTarget || variableFound) {  //*** to do recheck
		
      cout << "\n Warning: variables found twice, whether they are same or variable found in cpc:  " << vTarget << "=" << *variableIndex << " cpc.size" << CPCBar->size();
	  if (variableFound){
		  varList.erase(variableIndex);
	  }
	  else{
		  ++x;
		  if (variableIndex != varList.end()){
			  ++variableIndex;
		  }
	  }
	  
      continue;  // skip if variable already in a cpc set
    } else {
      ind = this->computeMinAssoc(vTarget, *variableIndex, minAssoc);
    }

    mapMinAssoc.insert(make_pair(minAssoc.second, *variableIndex));  // Make a pair of variable index and relevant MI value then put in the map list.
    mapInd.insert(make_pair(*variableIndex, ind));
    osBuffer << "  -- min MI=" << minAssoc.second << " ---v" << *variableIndex;

    if (ind) {  // to implement optimization: if once test true then no need to further test it, so variable is independent then remove it from the var list.
      variableIndex = varList.erase(variableIndex);  // this->removeFromList(*variableIndex, varList, variableIndex);// remove and return next iterator
      osBuffer << " -->> Ind";
    } else {
      ++variableIndex;
	    ++x;
    }
    if (debugMode) {
      osBuffer << ends;
      printLog->Write(osBuffer.str());
    }
    osBuffer.rdbuf()->freeze(false);  // to unfreeze buffer
    osBuffer.seekp(0);                //  to reset buffer
  }  // endwhile

  MaxMinVarMap::reverse_iterator iter1;

  if (debugMode) {
    osBuffer << "\n Sorted List: \n";
    size_t j = 0;
    for (iter1 = mapMinAssoc.rbegin(); iter1 != mapMinAssoc.rend(); ++iter1) {
      if (iter1->first != 0)
        osBuffer << " (" << j++ << ")_v" << iter1->second << ", MI=" << iter1->first;
    }
    osBuffer << "\nTotal-Vars=" << j << ", Skipped vars having zero MI";
  }  // endif
  // To return the independence True/False,first pick the variable having the largest MI value from mapMinAssoc, then find that variable
  // from "mapInd" and return the indp of that variable.
  iter1 = mapMinAssoc.rbegin();
  assocF = iter1->first; varF = iter1->second;
  mapIndIter = mapInd.find(iter1->second);
  if (debugMode) {
    osBuffer << "\n\nMaxMin selected v" << iter1->second << " --- " << iter1->first << "\n";
    osBuffer << ends;
    printLog->Write(osBuffer.str());
  }
  delete printLog;
  return mapIndIter->second;  // return independence
}

//=============================================================================
bool AlgoMMPCbar_T::computeMinAssoc(const unsigned int& vTarget, const unsigned int& vMax, pair_t& minAssoc) {
  alpha = 0.05;
  p = 0;
  MI = 0;
  float minTestValue = 0;  // to keep minimum value

  // Compute subsets of CPC
  SubSets allSubSetsOfCPC;
  SubSets::iterator subSetItr;

  pmMI* mi = new pmMI(this->ds, &all_variable, true);  //**** TO DO

  ostrstream osBuffer;  // to output in a logfile
  bool indExist = false;
  bool firstItration = true;
  size_t subSetSize = 0;

  while (subSetSize < conditionalVariableSize && subSetSize <= CPCBar->size()) {
    this->computeSubSets(*CPCBar, subSetSize, allSubSetsOfCPC);  // calculate all subsets

    subSetItr = allSubSetsOfCPC.begin();
    do {
      indExist = false;
      if (mi->isDataSufficientToProceedIndTest(vTarget, vMax, *subSetItr)) {  // allSubSetsOfCPC[x])){
        indExist = mi->computeMI(vTarget, vMax, *subSetItr);  // allSubSetsOfCPC[x]);
        MI = mi->getMI(); p = mi->getPValue(); df = mi->getDF();

      } else if (subSetItr->size() == 0) {  // if conditional var = 0 and data is not sufficient then test = true (indpendent)
        indExist = true; MI = 0; p = 0; df = mi->getDF();
      } else {
        df = mi->getDF();
        if (debugMode) {
          osBuffer << "\n" << vTarget << "," << vMax << "|";
          size_t j = 0;
          while (j < subSetItr->size()) {
            osBuffer << subSetItr->at(j) << ",";
            j++;
          }
          osBuffer << " df=" << df << " Break: Data is not sufficient";
          Log *printLog = new Log('l');
          osBuffer << ends;
          printLog->Write(osBuffer.str());
          delete printLog;
          osBuffer.rdbuf()->freeze(false);  // to unfreeze buffer
          osBuffer.seekp(0);                //  to reset buffer
        }
       delete mi;
       return indExist;  // break the loop
     }  // End if
     if (debugMode) {
       osBuffer << "\n" << vTarget << "," << vMax << "|";
       size_t j = 0;
       while (j < subSetItr->size()) {
         osBuffer << subSetItr->at(j) << ",";
         j++;
       }
       osBuffer << "      " << (indExist? "True":"False") << " Df=" << df << " MI=" << MI << " p=" << p;
       Log *printLog = new Log('l');
       osBuffer << ends;
       printLog->Write(osBuffer.str());
       delete printLog;
       osBuffer.rdbuf()->freeze(false);  // to unfreeze buffer
       osBuffer.seekp(0);                //  to reset buffer
     }  // end debug
     if (firstItration) {
       minTestValue = MI;
       firstItration = false;
     } else if (MI < minTestValue)
       minTestValue = MI;
     if (indExist) {
       minAssoc = make_pair(indExist, minTestValue);
       delete mi;
       return indExist;  // break the loop
     }
     minAssoc = make_pair(indExist, minTestValue);
     ++subSetItr;
   } while (subSetItr != allSubSetsOfCPC.end() && subSetSize < conditionalVariableSize);  // endwhile
   ++subSetSize;
  }  // end while

  delete mi;
  return indExist;
}

//=============================================================================
void AlgoMMPCbar_T::phase2_T(unsigned int& vTarget, int step) {
  float test_value = 0;
  bool isIndExist = false;

  SubSets allSubSetsOfCPC;
  SubSets::iterator subSetItr;

  ostrstream osBuffer;
  osBuffer << "\n\n--------------- Phase2 v" << vTarget << "---------------\n";

  /**************************optimised method for phase 2**********************************/
  // under constraction
  /*****************************************************************************************/

  /**************************only naive method for phase 2**********************************/

  pmMI* mi = new pmMI(this->ds, &this->all_variable, true);

  size_t subSetSize = 0;

  while (subSetSize < conditionalVariableSize && subSetSize <= CPCBar->size()) {  // to calculate subsets step by step
    bool isTestContinue = false;
    try {
      computeSubSets(*CPCBar, subSetSize, allSubSetsOfCPC);  // calculate all subsets
    } catch (std::exception & e) {
      cout << "\n Error in calling computesubset function subsetsize:" << subSetSize << "\n";
      std::cerr << e.what() << endl;
      std::system("pause");
      cout << "stop!! " << endl;
    }
    cpcItr = CPCBar->begin();
    while (cpcItr != CPCBar->end()) {  // iterate all x belongs to CPC
      if (isIndExist)                                      // If deletion has been occurred then recalculate subsets
        computeSubSets(*CPCBar, subSetSize, allSubSetsOfCPC);  // otherwise use previous one.
      isIndExist = false;  //reinitialize

      for (subSetItr = allSubSetsOfCPC.begin(); subSetItr != allSubSetsOfCPC.end(); ++subSetItr){  // iterate all subsets of CPC for independence test
        if (!isInList(*cpcItr, *subSetItr)) {  // if x is not in the subset then use it for independence test
          bool isDataSufficient;
          isDataSufficient = mi->isDataSufficientToProceedIndTest(vTarget, *cpcItr, *subSetItr);
          if (isDataSufficient) {
            isTestContinue = true;
            isIndExist = mi->computeMI(vTarget, *cpcItr, *subSetItr);
            test_value = mi->getMI(); df = mi->getDF();
          } else if (subSetItr->size() == 0) {   // if conditional var = 0 and data is not sufficient then test = true (indpendent)
            isIndExist = true; test_value = 0; df = mi->getDF();
            isTestContinue = true;
          } else {
            df = mi->getDF();
            if (debugMode) {
              osBuffer << "\n" << vTarget << "," << *cpcItr << "|";
              size_t j = 0;
              if (subSetItr->size() != 0)
                while (j != subSetItr->size()) {
                  osBuffer << " " << subSetItr->at(j);
                  j++;
                }
              osBuffer << "Break: Data is not sufficient df=" << df;
            }
            break;  // break from inner loop
          }  // endif
          if (debugMode) {
            osBuffer << "\n" << vTarget << "," << *cpcItr << "|";
            size_t j = 0;
            while (j < subSetItr->size()) {
              osBuffer << " " << subSetItr->at(j);
              j++;
            }
            osBuffer << "             " << (isIndExist? "True":"False") << " Df=" << df << " MI=" << test_value;

            Log *printLog = new Log('l');
            osBuffer << ends;
            printLog->Write(osBuffer.str());
            delete printLog;
            osBuffer.rdbuf()->freeze(false);  // to unfreeze buffer
            osBuffer.seekp(0);                //  to reset buffer
          }
          if (isIndExist) {
            cpcItr = CPCBar->erase(cpcItr);
            break;  // break from inner loop
          }
        }  //endif
      }  // end for
      if (!isIndExist) {  // if there is no deletion occurred
        ++cpcItr;
      }

    }  // end while
    ++subSetSize;
    if (!isTestContinue)
      break;
  }  // end outer while loop

  delete mi;
}

//=============================================================================
bool AlgoMMPCbar_T::isInList(const unsigned int& var, std::vector<unsigned int>& vList) {
  std::vector<unsigned int>::iterator position;
  return this->isInList(var, vList, position);
}

//=============================================================================
bool AlgoMMPCbar_T::isInList(const unsigned int& var, std::vector<unsigned int>& vList, std::vector<unsigned int>::iterator& position) {
  position = std::find(vList.begin(), vList.end(), var);
  if (position != vList.end()){  // == vector.end() means the element was not found
	  return true;
  }
  else{
	  return false;
  }
}

//=============================================================================
bool AlgoMMPCbar_T::isInList(const unsigned int& var, std::map <unsigned int, CPC_t>& mapList) {
  std::map <unsigned int, CPC_t>::iterator mapItr;
  mapItr = mapList.find(var);
  if (mapItr != mapList.end())  // == vector.end() means the element was not found
    if (!mapItr->second.empty())
      return true;
  return false;
}

//******* compute Subsets  *****************
//=============================================================================
void AlgoMMPCbar_T::subs(const std::vector<unsigned int>& org, const std::vector<unsigned int>& pre, size_t k, size_t n, SubSets& c) {
  if (n <= 1) {
    for (size_t i = k; i < org.size(); i++) {
      std::vector<unsigned int> v(pre);  // instead of printing...
      v.push_back(org.at(i));
      c.push_back(v);
    }
  } else {
    size_t n1 = n - 1;
    for (size_t i = k; i != org.size() - n1; i++) {   // < , !=
      std::vector<unsigned int> step(pre);
      step.push_back(org.at(i));
      subs(org, step, i+1, n1, c);
    }
  }
}

//=============================================================================
void AlgoMMPCbar_T::computeSubSets(const std::vector<unsigned int>& org, size_t& n, SubSets& c) {
  clock_t tStart = clock();
  c.clear();  // clear previous data

  std::vector<unsigned int> pre;
  pre.reserve(n+1);  // for performance
  if (n == 0)
    c.push_back(pre);
  else
    //subs(pre,0, subSetSize, c);
    subs(org, pre, 0, n, c);
  subsetTime += (clock() - tStart);  // to check time complexity
}
