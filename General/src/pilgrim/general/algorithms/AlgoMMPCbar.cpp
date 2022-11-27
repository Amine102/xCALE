
#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <unordered_map>

#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/algorithms/AlgoMMPCbar.h"
#include "pilgrim/general/scores/computeMI.h"
#include "pilgrim/general/log.h"
#include <cstdlib>

unsigned int AlgoMMPCbar::varCounter = 0;
unsigned int AlgoMMPCbar::numVars; ///< Number of total vars
plVariablesConjunction AlgoMMPCbar::all_variable;
bool AlgoMMPCbar::incrementLearning = true;
std::map<unsigned int, CPC_t> AlgoMMPCbar::bestKvariables;

std::map<unsigned int, std::vector<unsigned int>> AlgoMMPCbar::cpcPath;
//int AlgoMMPCbar::nFunctionCalls = 0, AlgoMMPCbar::totalFunctionCalls=0, AlgoMMPCbar::nTOCOnonFired=0, AlgoMMPCbar::nTOCOFired=0, AlgoMMPCbar::nTemp=0, AlgoMMPCbar::TotalTOCOnonFired=0, AlgoMMPCbar::TotalTOCOFired=0;

//==========================================================================
AlgoMMPCbar::AlgoMMPCbar(pmCSVDataSet *_dataSet, const unsigned int &_condVarSize /*= 10*/, const unsigned int &_KBestSize /*= 8*/,
						 const unsigned int &_TOCOSize /*= 0*/, bool _debugMode /*= false */) : ds(_dataSet), conditionalVariableSize(_condVarSize),
																								KBestSize(_KBestSize), TOCOsize(_TOCOSize), debugMode(_debugMode)
{
	this->alpha = 0.05;
};
AlgoMMPCbar::~AlgoMMPCbar(){};
// void AlgoMMPCbar::resetIncrementalLearningCounters()
// 	{
// 	AlgoMMPCbar::TotalTOCOFired += AlgoMMPCbar::nTOCOFired;
// 	AlgoMMPCbar::TotalTOCOnonFired += AlgoMMPCbar::nTOCOnonFired;
// 	AlgoMMPCbar::nTemp = AlgoMMPCbar::totalFunctionCalls;
// 	AlgoMMPCbar::nTOCOFired = AlgoMMPCbar::nTOCOnonFired = 0;
// 	}

inline bool copyKBestVar(MaxMinVarMap &vMaxMinAll, unsigned int K, std::vector<unsigned int> &temp)
{
	// copy top k variables and store in a vector.
	// cout << "In copyKBestVar and K is " << K << endl;
	bool isMIgreaterThanZero = false;

	MaxMinVarMap::reverse_iterator iter1;
	unsigned int count;
	//cout<<"\n";
	for (iter1 = vMaxMinAll.rbegin(), count = 0; iter1 != vMaxMinAll.rend() && count <= K; ++iter1, ++count)
	{
		temp.push_back(iter1->second);
		//cout<<" , "<<iter1->second;
		if (iter1->first != 0)
			isMIgreaterThanZero = true;
	}
	if (count <= 2)
		temp.clear(); // if there are only two variables then no need to store as k best.

	return isMIgreaterThanZero;
}

void AlgoMMPCbar::computeMMPCbar(unsigned int &vTarget, std::vector<unsigned int> &CPCofTargetVar)
{
	// cout << "AlgoMMPCbar::computeMMPCbar begins" << endl;
	//    cout <<"\n("<<varCounter++<<")- Learning(v"<< vTarget <<").... ";

	// reset counters
	nTOCOFired = nTOCOnonFired = 0;

	//plVariablesConjunction all_variable = this->ds->observed_variables(); //*** TO DO
	CPCBar = &CPCofTargetVar; // copy the address of cpc variable

	ostrstream osBuffer;
	Log *printLog = new Log('l');
	if (debugMode)
	{
		osBuffer << "\n\n ----------------------  Target Var " << vTarget << "----------------------------\n";
		osBuffer << "\n"
				 << ends;
		printLog->Write(osBuffer.str());
		osBuffer.rdbuf()->freeze(false); // to unfreeze buffer
		osBuffer.seekp(0);				 //  to reset buffer
	}
	// cout << "\n\n----------------------------------------------------------------\n";
	this->Phase1(vTarget);
	//if (debugMode){
	//cout<<"\nCPC Phase1 v"<<vTarget<<" = { ";
	osBuffer << "\n\n----------------------------------------------------------------";
	osBuffer << "\nCPC Phase1 v" << vTarget << " ={";
	// cout << "\nCPC Phase1 v" << vTarget << " ={";
	for (cpcItr = CPCBar->begin(); cpcItr != CPCBar->end(); ++cpcItr)
	{
		//cout<<" "<<*cpcItr;
		osBuffer << " " << *cpcItr;
		// cout << " " << *cpcItr;
	}
	//cout<<" } ";
	osBuffer << " }";
	// cout << " }";
	// cout << "\n\n----------------------------------------------------------------\n";
	//}//end if
	// store cpc path
	if (incrementLearning)
		cpcPath[vTarget] = *CPCBar;

	this->Phase2(vTarget);
	//	cout<<"\nPhase2 v"<<vTarget<<" = { ";
	osBuffer << "\nCPC Phase2 v" << vTarget << " ={";
	// cout << "\nCPC Phase2 v" << vTarget << " ={";
	for (cpcItr = CPCBar->begin(); cpcItr != CPCBar->end(); ++cpcItr)
	{
		//	cout<<" "<<*cpcItr;
		osBuffer << " " << *cpcItr;
		// cout << " " << *cpcItr;
	}
	//cout<<" } ";
	osBuffer << " }";
	// cout << " }";
	// cout << "\n================================================================\n";
	// cout << "================================================================\n";
	// to print the results
	if (debugMode)
	{
		//cout<<"\nFn calls:"<<nmFunctionCalls<<"  Total Fn calls:"<<totalNmOfFunctionCalls;
		//cout<<"\nTOCO rev:"<<nmTOCOFired<<" TOCO not rev:"<<nmTOCOnonFired;
		osBuffer << "\nFunction calls=" << pmMI::functionCalls << "  TOCO Revised=" << nTOCOFired;
		osBuffer << "\n----------------------------------------------------------------";
		osBuffer << ends;
		printLog->Write(osBuffer.str());
	} //end if
	delete printLog;
}

void AlgoMMPCbar::Phase1(unsigned int &vTarget)
{
	// cout << "AlgoMMPCbar::Phase1 begins with target variable " << vTarget << endl;
	//cout<<"\n debug phase 1="<<varList.size();
	varList.reserve(numVars);
	fullVarList.reserve(numVars);
	varList.clear();
	fullVarList.clear();
	CPC_t KBestForT; // To store kbest variables for only target variable finally put in a global storage
	std::vector<unsigned int> tempKbest;
	tempKbest.reserve(numVars);

	unsigned int varF;
	float assocF = -1; //
	bool isChangeCPC = true;
	//bool isKbestExist =false;
	MaxMinVarMap mapMinAssoc; // to store further as a kbest

	ostrstream osBuffer; // to output in a logfile
	Log *printLog = new Log('l');
	// cout << "-------------" << endl;
	// cout << "At the beginnning of phase1, k best is " << endl;
	// printKbest();
	// cout << endl;
	// cout << "-------------" << endl;
	// cout << "put variables indexes in the varlist except target variable" << endl;
	for (size_t i = 0; i < numVars; i++)
	{
		if (i != vTarget)
			varList.push_back(i); // put variables indexes in the list except target variable
	}
	// cout << "\n----------------------" << endl;
	// cout << "var list is now :" << endl;
	// for (int i = 0; i <= varList.size(); i++)
	// 	std::cout << varList[i] << ' ';
	// cout << "\n----------------------" << endl;
	size_t i = 0;
	while (i < CPCBar->size())
	{
		// cout << "CPCBar->size() is greater than 0, remove the element " << i << " of the CPCBar from the varList" << endl;
		remove(varList.begin(), varList.end(), CPCBar->at(i));
		++i;
		// cout << "\n----------------------" << endl;
		// cout << "var list is now :" << endl;
		// for (int i = 0; i <= varList.size(); i++)
		// 	std::cout << varList[i] << ' ';
		// cout << "\n----------------------" << endl;
	}

	if (incrementLearning)
		fullVarList = varList;

	int step = 0;
	if (CPCBar->size() > 0)
	{
		//step= CPCBar->size();
		// cout << "CPCBar size is greater than 0" << endl;
		if (incrementLearning)
		{ // to store kbest variables for last step
			// cout << "step is " << step << endl;
			// cout << "incrementalLearning is set to TRUE, so adding previously learned cpc in the tempKbest which is concatenated to KBestForT" << endl;
			//if(!isKBestExist(vTarget, step)){
			tempKbest.clear();					// reset for each step
			tempKbest.push_back(CPCBar->at(0)); // add previously learned cpc
			KBestForT[step] = tempKbest;
			// cout << "----------------------" << endl;
			// cout << "tempKbest is now :" << endl;
			for (int i = 0; i < tempKbest.size(); i++)
				std::cout << tempKbest[i] << ' ';
			// cout << "\n----------------------" << endl;
			osBuffer << "\nKbest stored, step:" << step;
			// cout << "\nKbest stored, step:" << step;
			//}
			step++;
		}
	}

	//cout<<"\nv"<<vTarget<<" CPC Phase1={";
	bool ind = 0;
	bool isCPCEqual = false;
	bool LearningFromScratch = false;
	bool iskBestCalculated = false;
	bool useCPCcomparison = true;
	unsigned int oldCPC;

	if (incrementLearning)
	{
		iskBestCalculated = isInList(vTarget, bestKvariables); // check whether k best already calculated or not.
		// To limit the test for only k best variables, load k best variables of step zero in varlist.
		// cout << "iskBestCalculated IS " << iskBestCalculated << endl;
		if (iskBestCalculated)
			if (getKbest(vTarget, step))
			{ // retrieve the k best if exist in varList.
				// cout << "getKbest is called" << endl;
				// cout << "\nUsing Kbest, step:" << step << endl;
				osBuffer << "\nUsing Kbest, step:" << step << endl;
				nTOCOnonFired++;
				// cout << "++++++++nTOCOnonFired incremented and is equal " << nTOCOnonFired << endl;
			}
			else
			{
				nTOCOFired++;
				// cout << "++++++++nTOCOFired incremented and is equal " << nTOCOFired << endl;
			}
		else
		{
			nTOCOFired++;
			// cout << "++++++++nTOCOFired incremented and is equal " << nTOCOFired << endl;
		}
	}
	if (debugMode)
	{
		osBuffer << ends;
		printLog->Write(osBuffer.str());
		osBuffer.rdbuf()->freeze(false); // to unfreeze buffer
		osBuffer.seekp(0);
	}
	// cout << "isChangeCPC is " << isChangeCPC << endl;
	// cout << "varlist.empty is " << varList.empty() << " and contains " << endl;
	// for (int i = 0; i < varList.size(); i++)
	// 	std::cout << varList[i] << ' ';
	// cout << endl;
	while (isChangeCPC && !varList.empty())
	{
		// cout << "while loop" << endl;
		assocF = -1; // reset on each iteration to avoid duplication
		//cout<<", "<<varList.size()<<"-"<<varList[0];

		ind = this->computeMaxMinHeuristic(vTarget, varF, assocF, mapMinAssoc);

		// cout << "------------------" << endl;
		// cout << "mapMinAssoc is " << endl;
		// std::multimap<float, unsigned int>::iterator it;
		// for (it = mapMinAssoc.begin(); it != mapMinAssoc.end(); ++it)
		// {
		// 	cout << it->first << ":" << it->second << endl;
		// }
		// cout << "------------------" << endl;

		// cout << "varF is " << varF << endl;
		// cout << "assocF is " << assocF << endl;

		// cout << "ind is " << ind << endl;
		if (ind != true && assocF != -1)
		{
			// cout << "Entered in the condition of ind!=true and assocF !=-1 " << endl;
			//step++;
			CPCBar->push_back(varF);
			varList.erase(std::remove(varList.begin(), varList.end(), varF), varList.end());
			isChangeCPC = true;
		}
		else
		{ // To Do=====>>>>>> implement===>>> check if previous cpc={2 4} and next time cpc={} then.....
			// cout << "Didn't Enter in the condition of ind!=true and assocF !=-1 " << endl;
			// cout << "The else condition with the continue statement" << endl;
			if (incrementLearning)
			{
				// to store kbest variables for last step
				tempKbest.clear(); // reset for each step
				//if(copyKBestVar(mapMinAssoc, KBestSize, tempKbest)){ // Store k top variables as a k best for this CPC variable for each step
				// cout << "------------------" << endl;
				// cout << "mapMinAssoc is " << endl;
				// std::multimap<float, unsigned int>::iterator it;
				// for (it = mapMinAssoc.begin(); it != mapMinAssoc.end(); ++it)
				// {
				// 	cout << it->first << ":" << it->second << endl;
				// }
				// cout << "------------------" << endl;

				copyKBestVar(mapMinAssoc, KBestSize, tempKbest);

				// cout << "\n----------------------" << endl;
				// cout << "\n After copyKBestVar tempKbest list is now :" << endl;
				// for (int i = 0; i < tempKbest.size(); i++)
				// 	std::cout << tempKbest[i] << ' ';
				// cout << "\n----------------------" << endl;
				if (tempKbest.size() > 1)
				{
					// cout << "tempKbest size is greater than 1" << endl;
					KBestForT[step] = tempKbest;
					// cout << "------------------" << endl;
					// cout << "KBestForT is " << endl;
					// std::map<unsigned int, std::vector<unsigned int>>::const_iterator itr;

					// for (itr = KBestForT.begin(); itr != KBestForT.end(); itr++)
					// {
					// 	cout << itr->first << "{";
					// 	for (size_t j = 0; j < itr->second.size(); j++)
					// 	{
					// 		cout << itr->second.at(j);
					// 	}
					// 	cout << "}" << endl;
					// }

					// cout << "------------------" << endl;
					osBuffer << " Kbest stored for last step:" << step << endl;
					// cout << " Kbest stored for last step:" << step << endl;
				}
			}
			isChangeCPC = false;
			continue;
		}
		// cout << "\n----------------------" << endl;
		// cout << "after checking the independence condition" << endl;
		// cout << "var list is now :" << endl;
		// for (int i = 0; i < varList.size(); i++)
		// 	std::cout << varList[i] << ' ';
		// // cout << "\n tempKbest list is now :" << endl;
		// for (int i = 0; i < tempKbest.size(); i++)
		// 	std::cout << tempKbest[i] << ' ';
		// cout << "\n----------------------" << endl;
		////// For incremental learning load kbest for each step////////////////
		if (incrementLearning)
		{
			/* Store k top variables as a k best for this CPC variable for each step   */
			//if(!isKBestExist(vTarget, step)||LearningFromScratch){
			tempKbest.clear(); // reset for each step
			//if(copyKBestVar(mapMinAssoc, KBestSize, tempKbest)){
			// cout << "------------------" << endl;
			// cout << "mapMinAssoc is " << endl;
			std::multimap<float, unsigned int>::iterator it;
			// for (it = mapMinAssoc.begin(); it != mapMinAssoc.end(); ++it)
			// {
			// 	cout << it->first << ":" << it->second << endl;
			// }
			// cout << "------------------" << endl;

			copyKBestVar(mapMinAssoc, KBestSize, tempKbest);
			// cout << "\n----------------------" << endl;
			// cout << "\n After the copyKBestVar, tempKbest list is now :" << endl;
			// for (int i = 0; i < tempKbest.size(); i++)
			// 	std::cout << tempKbest[i] << ' ';
			// cout << "\n----------------------" << endl;
			if (tempKbest.size() > 1)
			{
				// cout << "tempKbest size is bigger than 1" << endl;
				KBestForT[step] = tempKbest;
				// cout << "------------------" << endl;
				// cout << "KBestForT is " << endl;
				// std::map<unsigned int, std::vector<unsigned int>>::const_iterator itr;

				// for (itr = KBestForT.begin(); itr != KBestForT.end(); itr++)
				// {
				// 	cout << itr->first << "{";
				// 	for (size_t j = 0; j < itr->second.size(); j++)
				// 	{
				// 		cout << itr->second.at(j);
				// 	}
				// 	cout << "}" << endl;
				// }

				// cout << "------------------" << endl;
				osBuffer << "\nKbest stored, step:" << step;
			}
			//	}

			/* check path */
			// cout << "isKestCalculated is " << iskBestCalculated << " and useCPCcomparison is " << useCPCcomparison << endl;
			if (!iskBestCalculated) // whether cpc path is already calculated or not
				fullVarList.erase(std::remove(fullVarList.begin(), fullVarList.end(), varF), fullVarList.end());
			else if (useCPCcomparison)
			{
				isCPCEqual = compareCPCPath(vTarget, varF, step, oldCPC);
				// cout << "isCPCEQUAL IS " << isCPCEqual << endl;
				osBuffer << "\nPath is:" << isCPCEqual;
				// cout << "\nPath is:" << isCPCEqual;

				if (isCPCEqual && TOCOsize > 1 && oldCPC != varF)
				{												  // if path is true and TOCO size is greater than 0 then recover the old CPC
					bool variableDep = isInList(oldCPC, varList); // check whether it is independent and deleted from the var list or not.
					if (variableDep)
					{
						CPCBar->erase(std::remove(CPCBar->begin(), CPCBar->end(), varF), CPCBar->end()); // delete added cpc variable and asign old cpc
						CPCBar->push_back(oldCPC);
						varList.push_back(varF); // put return back variable in the varList
						fullVarList.erase(std::remove(fullVarList.begin(), fullVarList.end(), oldCPC), fullVarList.end());
						varList.erase(std::remove(varList.begin(), varList.end(), oldCPC), varList.end()); // to consider previous cpc, it should be deleted from both lists
						osBuffer << "\nOldCPC assigned:" << oldCPC << " rather than:" << varF << " nTOCO:" << TOCOsize;
						// cout << "\nOldCPC assigned:" << oldCPC << " rather than:" << varF << " nTOCO:" << TOCOsize;
					}
					else
						fullVarList.erase(std::remove(fullVarList.begin(), fullVarList.end(), varF), fullVarList.end()); // cpc is correct so it should be remove from fullvarlist too.
				}
				else
					//if(isCPCEqual && TOCOsize < 2 )
					fullVarList.erase(std::remove(fullVarList.begin(), fullVarList.end(), varF), fullVarList.end()); // cpc is correct so it should be remove from fullvarlist too.

				if (!isCPCEqual)
					LearningFromScratch = true;
				else if (!this->getKbest(vTarget, step + 1, varList) || varList.size() == 0)
				{ //get k best in varList for next step
					osBuffer << "\nCan't retrieve Kbest for step:" << step + 1 << endl;
					//varList=fullVarList; // just for test
					useCPCcomparison = false;
				}
				else
				{
					osBuffer << "\nUsing Kbest, step:" << step + 1 << endl;
					// cout << "\nUsing Kbest, step:" << step + 1 << endl;
					//varList.erase(std::remove(varList.begin(), varList.end(), varF), varList.end());
					LearningFromScratch = false;
					nTOCOnonFired++;
					// cout << "++++++++nTOCOnonFired incremented and is equal to " << nTOCOnonFired << endl;
				}

				if (LearningFromScratch)
				{
					osBuffer << "\ncpc deleted:" << varF << endl;
					// cout << "\ncpc deleted:" << varF << endl;
					osBuffer << "\nReLearning: path invalid step:" << step << endl;
					// cout << "\nReLearning: path invalid step:" << step << endl;
					CPCBar->erase(std::remove(CPCBar->begin(), CPCBar->end(), varF), CPCBar->end()); // remove added cpc from the cpc list and retest for all variables ratherthan kbest only
					--step;
					fullVarList.push_back(varF); // As re learning process start so, re add deleted variable
					varList = fullVarList;
					useCPCcomparison = false;
					nTOCOFired++;
					// cout << "++++++++nTOCOFired incremented and is equal to " << nTOCOFired << endl;
				}
				//LearningFromScratch=true;
			} // end if iskbestCalculated
			step++;
		} //endif incremental
		if (debugMode)
		{
			osBuffer << ends;
			printLog->Write(osBuffer.str());
		}
		osBuffer.rdbuf()->freeze(false); // to unfreeze buffer
		osBuffer.seekp(0);
	} // endwhile
	//cout<<"}";

	delete printLog;
	//printKbest();

	if (incrementLearning)
		bestKvariables[vTarget] = KBestForT; // Store k best in a Map
											 //printKbest();
	// cout << "-------------" << endl;
	// cout << "printingKbest" << endl;
	// printKbest();
	// cout << endl;
	// cout << "-------------" << endl;
}
bool AlgoMMPCbar::computeMaxMinHeuristic(unsigned int &vTarget, unsigned int &varF, float &assocF, MaxMinVarMap &mapMinAssoc)
{
	//cout<<"\n debug maxmin";

	bool ind = 0;
	std::map<unsigned int, bool> mapInd; // to store the variable and its independence.
	std::map<unsigned int, bool>::iterator mapIndIter;
	mapMinAssoc.clear(); // reset for each iteration
	ostrstream osBuffer; // to output in a logfile
	Log *printLog = new Log('l');

	// To calculate independence of target variable with all variables
	pair_t minAssoc;
	std::vector<unsigned int>::iterator variableIndex = varList.begin();

	while (variableIndex != varList.end())
	{
		bool variableFound = isInList(*variableIndex, *CPCBar);
		if (*variableIndex == vTarget || variableFound)
		{ //*** to do recheck
			cout << "\n Warning: variables found twice, whether they are same or variable found in cpc:  " << vTarget << "=" << *variableIndex << " cpc.size" << CPCBar->size();
			//system("pause");
			if (variableFound)
				varList.erase(variableIndex); //
			else
				++variableIndex;
			continue; // skip if variable already in a cpc set
		}
		else
			ind = this->computeMinAssoc(vTarget, *variableIndex, minAssoc);

		mapMinAssoc.insert(make_pair(minAssoc.second, *variableIndex)); // Make a pair of variable index and relevant MI value then put in the map list.
		mapInd.insert(make_pair(*variableIndex, ind));
		osBuffer << "  -- min MI=" << minAssoc.second << " ---v" << *variableIndex;

		if (ind)
		{ //// to implement optimization: if once test true then no need to further test it, so variable is independent then remove it from the var list.
			if (incrementLearning)
				fullVarList.erase(std::remove(fullVarList.begin(), fullVarList.end(), *variableIndex), fullVarList.end()); //this->removeFromList(*variableIndex, fullVarList); remove same element from full var list also because it found ind
			variableIndex = varList.erase(variableIndex);																   //this->removeFromList(*variableIndex, varList, variableIndex);// remove and return next iterator
			osBuffer << " -->> Ind";
		}
		else
			++variableIndex;
		if (debugMode)
		{
			osBuffer << ends;
			printLog->Write(osBuffer.str());
		}
		osBuffer.rdbuf()->freeze(false); // to unfreeze buffer
		osBuffer.seekp(0);				 //  to reset buffer
	}									 //endwhile

	MaxMinVarMap::reverse_iterator iter1;

	if (debugMode)
	{
		osBuffer << "\n Sorted List: \n";
		size_t j = 0;
		for (iter1 = mapMinAssoc.rbegin(); iter1 != mapMinAssoc.rend(); iter1++)
			if (iter1->first != 0)
				osBuffer << " (" << j++ << ")_v" << iter1->second << ", MI=" << iter1->first;
		osBuffer << "\nTotal-Vars=" << j << ", Skipped vars having zero MI";
	} //endif
	  // To return the independence True/False,first pick the variable having the largest MI value from mapMinAssoc, then find that variable
	  // from "mapInd" and return the indp of that variable.
	iter1 = mapMinAssoc.rbegin();
	assocF = iter1->first;
	varF = iter1->second;
	mapIndIter = mapInd.find(iter1->second);
	if (debugMode)
	{
		osBuffer << "\n\nMaxMin selected v" << iter1->second << " --- " << iter1->first << "\n";
		osBuffer << ends;
		printLog->Write(osBuffer.str());
	}
	delete printLog;
	return mapIndIter->second; // return independence
}
bool AlgoMMPCbar::computeMinAssoc(const unsigned int &vTarget, const unsigned int &vMax, pair_t &minAssoc)
{
	//cout<<"\n debug minassoc";
	alpha = 0.05;
	p = 0;
	MI = 0;
	float minTestValue = 0; // to keep minimum value

	// Compute subsets of CPC
	SubSets allSubSetsOfCPC;
	SubSets::iterator subSetItr;

	pmMI *mi = new pmMI(this->ds, &all_variable, true); //**** TO DO

	ostrstream osBuffer; // to output in a logfile
	bool indExist = false;
	bool firstItration = true;
	unsigned int subSetSize = 0;

	while (subSetSize < conditionalVariableSize && subSetSize <= CPCBar->size())
	{

		this->computeSubSets(*CPCBar, subSetSize, allSubSetsOfCPC); // calculate all subsets

		subSetItr = allSubSetsOfCPC.begin();
		do
		{
			indExist = false;
			if (mi->isDataSufficientToProceedIndTest(vTarget, vMax, *subSetItr))
			{														 //allSubSetsOfCPC[x])){
				indExist = mi->computeMI(vTarget, vMax, *subSetItr); //allSubSetsOfCPC[x]);
				MI = mi->getMI();
				p = mi->getPValue();
				df = mi->getDF();
				// 			cout<<"\n cpc-size="<<CPCBar->size()<<" subset="<<allSubSetsOfCPC.size()<<"-"<<vTarget<<","<<vMax<<"|";
				// 			size_t j=0;
				// 			while( j < subSetItr->size()){
				// 				cout<<subSetItr->at(j)<<",";
				// 				j++;
				// 				}
			}
			else if (subSetItr->size() == 0)
			{ //if conditional var = 0 and data is not sufficient then test = true (indpendent)
				indExist = true;
				MI = 0;
				p = 0;
				df = mi->getDF();
			}
			else
			{
				df = mi->getDF();
				if (debugMode)
				{
					osBuffer << "\n"
							 << vTarget << "," << vMax << "|";
					size_t j = 0;
					while (j < subSetItr->size())
					{
						osBuffer << subSetItr->at(j) << ",";
						j++;
					}
					osBuffer << " df=" << df << " Break: Data is not sufficient";
					Log *printLog = new Log('l');
					osBuffer << ends;
					printLog->Write(osBuffer.str());
					delete printLog;
					osBuffer.rdbuf()->freeze(false); // to unfreeze buffer
					osBuffer.seekp(0);				 //  to reset buffer
				}
				delete mi;
				return indExist; // break the loop
			}					 //End if
			if (debugMode)
			{
				osBuffer << "\n"
						 << vTarget << "," << vMax << "|";
				size_t j = 0;
				while (j < subSetItr->size())
				{
					osBuffer << subSetItr->at(j) << ",";
					j++;
				}
				osBuffer << "      " << (indExist ? "True" : "False") << " Df=" << df << " MI=" << MI << " p=" << p;
				Log *printLog = new Log('l');
				osBuffer << ends;
				printLog->Write(osBuffer.str());
				delete printLog;
				osBuffer.rdbuf()->freeze(false); // to unfreeze buffer
				osBuffer.seekp(0);				 //  to reset buffer
			}									 //end debug
			if (firstItration)
			{
				minTestValue = MI;
				firstItration = false;
			}
			else if (MI < minTestValue)
				minTestValue = MI;
			if (indExist)
			{
				minAssoc = make_pair(indExist, minTestValue);
				delete mi;
				return indExist; // break the loop
			}
			minAssoc = make_pair(indExist, minTestValue);
			++subSetItr;
		} while (subSetItr != allSubSetsOfCPC.end() && subSetSize < conditionalVariableSize); //endwhile
		++subSetSize;
	} // end while

	delete mi;
	//cout<<" debug exit minassoc";
	return indExist;
}
void AlgoMMPCbar::Phase2(unsigned int &vTarget)
{
	//cout<<"\n debug phase 2";

	float test_value = 0;

	//std::vector <unsigned int> toBeDeleted;
	SubSets allSubSetsOfCPC;
	SubSets::iterator subSetItr;

	ostrstream osBuffer;
	osBuffer << "\n\n--------------- Phase2 v" << vTarget << "---------------\n";

	pmMI *mi = new pmMI(this->ds, &this->all_variable, true);

	size_t count = 0;
	unsigned int subSetSize = 0;

	bool isIndExist = false;
	while (subSetSize < conditionalVariableSize && subSetSize <= CPCBar->size())
	{ // to calculate subsets step by step
		//cout<<" D1 ";
		bool isTestContinue = false;
		try
		{
			computeSubSets(*CPCBar, subSetSize, allSubSetsOfCPC); // calculate all subsets
		}
		catch (std::exception &e)
		{
			cout << "\n Error in calling computesubset function subsetsize:" << subSetSize << "\n";
			std::cerr << e.what() << endl;
			// system("pause");
			cout << "stop!! " << endl;
		}

		//cout<<"-cpc_size:"<<CPCBar->size()<<" subsetsize:"<<subSetSize;
		cpcItr = CPCBar->begin();
		while (cpcItr != CPCBar->end())
		{															  // iterate all x belongs to CPC
			if (isIndExist)											  // If deletion has been occurred then recalculate subsets
				computeSubSets(*CPCBar, subSetSize, allSubSetsOfCPC); // otherwise use previous one.
			isIndExist = false;										  //reinitialize

			for (subSetItr = allSubSetsOfCPC.begin(); subSetItr != allSubSetsOfCPC.end(); ++subSetItr)
			{ // iterate all subsets of CPC for independence test
				if (!isInList(*cpcItr, *subSetItr))
				{ // if x is not in the subset then use it for independence test
					bool isDataSufficient;
					isDataSufficient = mi->isDataSufficientToProceedIndTest(vTarget, *cpcItr, *subSetItr);
					if (isDataSufficient)
					{
						isTestContinue = true;
						isIndExist = mi->computeMI(vTarget, *cpcItr, *subSetItr);
						test_value = mi->getMI();
						df = mi->getDF();
					}
					else if (subSetItr->size() == 0)
					{ //if conditional var = 0 and data is not sufficient then test = true (indpendent)
						isIndExist = true;
						test_value = 0;
						df = mi->getDF();
						isTestContinue = true;
					}
					else
					{
						df = mi->getDF();
						if (debugMode)
						{
							osBuffer << "\n"
									 << vTarget << "," << *cpcItr << "|";
							size_t j = 0;
							if (subSetItr->size() != 0)
								while (j != subSetItr->size())
								{
									osBuffer << " " << subSetItr->at(j);
									j++;
								}
							osBuffer << "Break: Data is not sufficient df=" << df;
						}
						break; // break from inner loop
					}		   // endif
					if (debugMode)
					{
						osBuffer << "\n"
								 << vTarget << "," << *cpcItr << "|";
						size_t j = 0;
						while (j < subSetItr->size())
						{
							osBuffer << " " << subSetItr->at(j);
							j++;
						}
						osBuffer << "             " << (isIndExist ? "True" : "False") << " Df=" << df << " MI=" << test_value;

						Log *printLog = new Log('l');
						osBuffer << ends;
						printLog->Write(osBuffer.str());
						delete printLog;
						osBuffer.rdbuf()->freeze(false); // to unfreeze buffer
						osBuffer.seekp(0);				 //  to reset buffer
					}
					if (isIndExist)
					{
						cpcItr = CPCBar->erase(cpcItr);
						break; // break from inner loop
					}
				} //endif
			}	  // end for
			if (!isIndExist)
			{ // if there is no deletion occurred
				++cpcItr;
			}

		} // end while
		++subSetSize;
		if (!isTestContinue)
			break;
	} //end outer while loop

	delete mi;
}
// void MMPCbar::computeSubSets(const std::vector<int>& org, size_t n, SubSets& c)
// {
//     //! To calculate all subsets
// 	//! First create plVariablesConjunction of all variables
// 	//! then by using plValues object, iterate all possible combinations
// 	//! of the form 01011. corresponding to 1 store variable index in the combination list
// 	//! at the end put all these combinations in another list.
//
// 	vector< int > combinations;
// 	plVariablesConjunction CPCvariables;
//
// 	// if Cpc list is empty then return empty set
// 	if (org.size()==0)
// 	{
// 		c.push_back(org);
//
// 	}else {
//
//     cout<<"s"<<org.size();
// 	// If list not empty then calculate subsets
// 	plSymbol *plS = NULL;
// 	char pNameVar[200];
//
// 	/** For all variable, create a plSymbol object and store it
// 	on a plVariablesConjunction object. **/
// 	for(int i = 0; i < org.size(); i++){
// 		sprintf(pNameVar, "%d",(i));
// 		plS = new plSymbol(string(pNameVar), plIntegerType(0,1));
// 		CPCvariables ^= (*plS);
// 		delete plS;
// 	}
// /////////////////
// // 	for (int i = 0; i < CpcList.size(); i++)
// // 	{
// // 		//cout<<" "<< CpcList[i]; 1,2,12,21
// // 		std::ostringstream symbol_name;
// // 		symbol_name << CpcList.at(i);
// // 		plSymbol symbol(symbol_name.str(), plIntegerType(0,1));
// // 		CPCvariables = CPCvariables ^ symbol;
// // 	}
// 	plValues vals(CPCvariables);
// 	vals.reset();
// 	cout<<vals;
//
// 	cout<<" valSize:"<<vals.size();
//
// try{
// 	cout<<"s";
// 	do {
// 		//cout<<"\n";
// 		for (unsigned int j = 0; j < org.size(); j++)
// 		{
// 		   //cout<<" "<< vals[j];
// 			if(vals[j] == 1) // combination will be in the form of 011011, corresponding to 1 add variable in the combination.
// 			{
// 				//cout<<"+"<< CpcList[j];
// 				combinations.push_back(org.at(j));
// 			}
// 		}
// 		c.push_back(combinations);
// 		combinations.clear(); // reset list
// 	}while (vals.next());
// }catch (std::exception & e){
// 	std::cerr << e.what() << endl;
// 	cout<<"Error: computeSubset   ";
// 	system("pause");
// 		cout << "stop!! " << endl;
// 	}
// }// endif
// //return subSetsCPC;
// }

// bool AlgoMMPCbar::removeFromList(unsigned int& vX, std::vector<unsigned int>& vList){
// 	std::vector<unsigned int>::iterator itr;
//   return this->removeFromList(vX, vList, itr);
//  }
// bool AlgoMMPCbar::removeFromList( std::vector<unsigned int>::iterator& position, std::vector<unsigned int>& vList ){
// 	if (position != vList.end()){ // == vlist.end() means the element was not found
// 		vList.erase(position);
// 		return true;
// 	}else
// 		return false;
// 	}
// bool AlgoMMPCbar::removeFromList( unsigned int& vX, std::vector<unsigned int>& vList, std::vector<unsigned int>::iterator& itr ){
// 	//cout<<"\n debug remove "<<vList.size();
// 	std::vector<unsigned int>::iterator position;
// 	position = std::find(vList.begin(), vList.end(), vX);
// 	if (position != vList.end()){ // == vector.end() means the element was not found
// 		itr = vList.erase(position);
// 		return true;
// 		}
// 	else
// 		return false;
// 	}
bool AlgoMMPCbar::isInList(const unsigned int &var, std::vector<unsigned int> &vList)
{
	std::vector<unsigned int>::iterator position;
	return this->isInList(var, vList, position);
}
bool AlgoMMPCbar::isInList(const unsigned int &var, std::vector<unsigned int> &vList, std::vector<unsigned int>::iterator &position)
{
	//cout<<"\n debug isInList";
	position = std::find(vList.begin(), vList.end(), var);
	if (position != vList.end()) // == vector.end() means the element was not found
		return true;
	else
		return false;
}
bool AlgoMMPCbar::isInList(const unsigned int &var, std::map<unsigned int, CPC_t> &mapList)
{
	std::map<unsigned int, CPC_t>::iterator mapItr;
	mapItr = mapList.find(var);
	if (mapItr != mapList.end()) // == vector.end() means the element was not found
		if (!mapItr->second.empty())
			return true;

	return false;
}
bool AlgoMMPCbar::getKbest(unsigned int &vTarget, int &step)
{
	return getKbest(vTarget, step, varList);
}
bool AlgoMMPCbar::getKbest(unsigned int &vTarget, int step, std::vector<unsigned int> &retKbest)
{
	// First find if target variable is exist or not.
	// then find the particular step
	std::map<unsigned int, CPC_t>::const_iterator outerItr;
	CPC_t::const_iterator innerItr;

	outerItr = bestKvariables.find(vTarget);
	if (outerItr != bestKvariables.end())
	{
		if (!outerItr->second.empty())
		{
			innerItr = outerItr->second.find(step);
			if (innerItr != outerItr->second.end())
				if (!innerItr->second.empty())
				{
					retKbest = innerItr->second;
					return true;
				}
				else
					return false;
		}
		else
			return false;
	}
	return false;
}
bool AlgoMMPCbar::compareCPCPath(unsigned int &vTarget, unsigned int &CurrntCPC, int &step, unsigned int &oldCPC)
{
	//cout<<"\n comparepath";
	// If current cpc is in the range of TOCOSize of kbest variables or it is in the set of previously learned CPCs then
	// we will consider the cpc is on a true path otherwise path has been changed so, relearn the process

	// 	std::vector<unsigned int> path1;
	// 	std::map <unsigned int, vector<unsigned int>>::const_iterator itrMap;

	//reload old path vector for target variable
	// 	itrMap = cpcPath.find(vTarget);
	// 	if ( itrMap != cpcPath.end( ) )
	// 		path1 = itrMap->second;
	//if(this->isInList(CurrntCPC, path1))
	//return true;

	// get nTOCO top variables from kBest list
	std::vector<unsigned int> nVars, tempVector;
	std::vector<unsigned int>::iterator itr;
	if (TOCOsize == 0)
		TOCOsize = 1; // use 1 as a default value

	if (this->getKbest(vTarget, step, tempVector))
	{																	 //
		nVars.assign(tempVector.begin(), tempVector.begin() + TOCOsize); // copy top nTOCO variables in a vector;

		if (this->isInList(CurrntCPC, nVars))
		{
			oldCPC = nVars[0];
			return true;
		}
	}

	return false;
}
void AlgoMMPCbar::printKbest()
{
	std::map<unsigned int, CPC_t>::const_iterator itr;
	CPC_t::iterator itr2;
	CPC_t temp;

	for (itr = bestKvariables.begin(); itr != bestKvariables.end(); itr++)
	{
		cout << "\n ==== first = " << itr->first << " -- size ij = " << itr->second.size();
		temp = itr->second;
		for (itr2 = temp.begin(); itr2 != temp.end(); itr2++)
		{
			cout << "\n    -- size ij = " << itr2->second.size();
			for (size_t j = 0; j < itr2->second.size(); j++)
			{
				cout << "\n    sencond == " << itr2->second.at(j);
			}
		}
	}
}
void AlgoMMPCbar::printFinalCPCPath()
{
	cout << "\n\n========>>Final CPC Path is " << endl;
	std::map<unsigned int, std::vector<unsigned int>>::const_iterator itr;
	std::vector<unsigned int>::iterator itr2;
	std::vector<unsigned int> temp;

	for (itr = cpcPath.begin(); itr != cpcPath.end(); itr++)
	{
		// cout<<"\n ==== first = "<<itr->first<< " -- size ij = "<<itr->second.size();
		cout << "\n ==== node " << itr->first << endl;
		temp = itr->second;
		for (size_t i = 0; i != temp.size(); i++)
		{
			cout << "\n"
				 << temp.at(i);
		}
	}
	cout << "\nEnd Of final CPC Path <<========\n\n"
		 << endl;
}
//******* compute Subsets  *****************
void AlgoMMPCbar::subs(const std::vector<unsigned int> &org, const std::vector<unsigned int> &pre, size_t k, size_t n, SubSets &c)
//void AlgoMMPCbar::subs(const std::vector<unsigned int>& pre, size_t k, size_t n, SubSets& c)
{
	if (n <= 1)
	{
		for (size_t i = k; i < org.size(); i++)
		{
			std::vector<unsigned int> v(pre); // instead of printing...
			v.push_back(org.at(i));
			c.push_back(v);
		}
	}
	else
	{
		size_t n1 = n - 1;
		for (size_t i = k; i != org.size() - n1; i++)
		{ // < , !=
			std::vector<unsigned int> s(pre);
			s.push_back(org.at(i));
			//subs(s,i+1,n1,c);
			subs(org, s, i + 1, n1, c);
		}
	}
}
void AlgoMMPCbar::computeSubSets(const std::vector<unsigned int> &org, unsigned int n, SubSets &c)
{
	//void AlgoMMPCbar::computeSubSets(SubSets& c, unsigned int& subSetSize){
	clock_t tStart = clock();
	c.clear(); // clear previous data

	// 	 if (CPCBar->size() == 0){
	// 			 c.push_back(*CPCBar);
	// 			}else{
	//for(size_t i = 0; i <= CPCBar->size() && i < subSetSize; i++){
	//if(subSetSize <= CPCBar->size()){
	std::vector<unsigned int> pre;
	pre.reserve(n + 1); // for performance
	if (n == 0)
		c.push_back(pre);
	else
		//subs(pre,0, subSetSize, c);
		subs(org, pre, 0, n, c);
	subsetTime += (clock() - tStart); // to check time complexity
}

bool AlgoMMPCbar::isKBestExist(unsigned int &vTarget, int &step)
{
	std::vector<unsigned int> temp;
	return getKbest(vTarget, step, temp);
}