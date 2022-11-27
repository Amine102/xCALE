
#include <pilgrim/general/pmBayesianNetwork.h>
#include "pilgrim/general/algorithms/AlgoMMPC.h"
#include "pilgrim/general/log.h"

using namespace PILGRIM;

//to temporarily store learned cpc from MMPCbar
std::map<unsigned int, std::vector<unsigned int>> AlgoMMPC::cpcMapList;
float AlgoMMPC::MItime = 0, AlgoMMPC::MIsearchTime = 0;
int AlgoMMPC::cachesize;

int AlgoMMPC::totalFunctionCalls = 0, AlgoMMPC::TotalTOCOnonFired = 0, AlgoMMPC::TotalTOCOFired = 0;

//==========================================================================
AlgoMMPC::AlgoMMPC(pmCSVDataSet *_dataSet, const unsigned int &_condVarSize /*= 10*/, const unsigned int &_kBestSize /*= 20*/,
				   const unsigned int &_TOCOsize /*= 3*/, bool _debugMod /*=false */) : AlgoMMPCbar(_dataSet, _condVarSize, _kBestSize, _TOCOsize, _debugMod),
																						ds(_dataSet), debugMode(_debugMod){}; // initialize class members

AlgoMMPC::~AlgoMMPC(){
	//cpcMapList.clear(); //reset static variable
};

void AlgoMMPC::computeMMPC(unsigned int &vTarget, CPC_t &CPCs)
{
	// cout<<"AlgoMMPC::ComputeMMPC begins"<<endl;
	std::vector<unsigned int> cpcOfVariableX, cpcOfTargetVariable;
	cpcOfTargetVariable.reserve(numVars);
	cpcOfVariableX.reserve(numVars);
	std::vector<unsigned int>::iterator itr;

	//- find in the cache if already calculated then retrieve
	this->computeCPCBar(vTarget, cpcOfTargetVariable); // call for MMPC bar algorithm

	// 	size_t count=0;
	// 		itr = cpcOfTargetVariable.begin();
	//
	// 		while(itr != cpcOfTargetVariable.end()){
	//
	// 			cpcOfVariableX.push_back(vTarget); // add vTarget variable as a cpc.
	// 			this->computeCPCBar(*itr, cpcOfVariableX); // use vTarget as already learned cpc.
	//
	// 			if(!this->isInList(vTarget, cpcOfVariableX)){
	// 				itr=cpcOfTargetVariable.erase(cpcOfTargetVariable.begin()+count);
	// 			}else{
	// 				++itr;
	// 				++count;
	// 				}
	// 		  cpcOfVariableX.clear();
	// 		}

	CPCs[vTarget] = cpcOfTargetVariable; // put individual cpc list in a map
}

void AlgoMMPC::computeCPCBar(unsigned int &vTarget, std::vector<unsigned int> &cpcOfTargetVariable)
{
	// cout<<"AlgoMMPC::computeCPCBar begins using a target variable vTarget"<<endl;
	//check whether cpc of target variable already calculated or not.
	std::map<unsigned int, vector<unsigned int>>::const_iterator itrMap;
	itrMap = cpcMapList.find(vTarget);

	if (itrMap != cpcMapList.end())
	{
		//cout<<" \n cpc already calculated......"<<vTarget;
		cpcOfTargetVariable = itrMap->second;
	}
	else
	{
		clock_t tStart = clock();

		MItime = 0;
		MIsearchTime = 0;
		subsetTime = 0;

		this->computeMMPCbar(vTarget, cpcOfTargetVariable); // call for MMPC bar algorithm
		incrementCounters();

		// 		if (debugMode)
		// 		{
		// 			Log *printSummary = new Log('T');
		// 			ostrstream osBuffer;
		// 			float ProcessTime;
		// 			ProcessTime= clock()- tStart;
		// 			osBuffer<< "\nvTarget:"<<vTarget<<" Total:"<<ProcessTime<<" subset:"<<subsetTime;
		// 			osBuffer<<" MI_Test: "<<MItime<<" (Processing-MI_Time):"<<ProcessTime-MItime<<" cache search Time:"<<MIsearchTime<<" cache_size:"<<cachesize;
		// 			MItime=0; MIsearchTime=0;
		//
		// 			osBuffer<<ends; //dont forget to ends the buffer
		// 			printSummary->Write(osBuffer.str());
		// 			delete printSummary;
		// 		}

		// Insert CPC list in map for cache
		cpcMapList.insert(make_pair(vTarget, cpcOfTargetVariable));
	}
}

void AlgoMMPC::incrementCounters()
{
	AlgoMMPC::TotalTOCOFired += nTOCOFired;
	AlgoMMPC::TotalTOCOnonFired += nTOCOnonFired;
	AlgoMMPC::totalFunctionCalls += nFunctionCalls;
}

void AlgoMMPC::resetCounters()
{
	totalFunctionCalls = TotalTOCOnonFired = TotalTOCOFired = 0;
}
