/** =============================================================================
* Product        : ProBT
* File           : testMMHC.cpp
* Author         : Amanullah YASIN
* Creation       : June 08 2010
*
*=============================================================================
*        (c) Copyright 2007, ProBayes SAS - all rights reserved
*=============================================================================
*
*------------------------- Description ---------------------------------------
*    Containing the main function, data load function and then
*     it calls MMHC functions
*-----------------------------------------------------------------------------
*/

#include <pl.h>
#include <iostream>
#include <time.h>
#include <cstring>

#include <pilgrim/general/pmBayesianNetwork.h>

#include "pilgrim/general/algorithms/AlgoMMHC.h"
#include "pilgrim/general/LoadDataSet.h"
#include "pilgrim/general/pmCommonTypeDefinations.h"
#include "pilgrim/general/log.h"


using namespace std;
using namespace PILGRIM;

bool ComputeAlgorithm_MMPCbar(char *, char *);
void ComputeAlgorithm_iMMPCbar(char *, char *);

string dataSetFile = "";
string outputPrefix = "";
string orignalNetFileName = "";
string scoreName="BIC";
string algorithmName = "MMHC";
char separator = ',';

char cDataSetFile[256] = "";
char cOutFileName[256] = "";

// size_t numKbestVars = 20;
size_t numOfVariables = 0;
// size_t windowSize = 0;
size_t conditioningVarSize = 5;
// size_t TOCOsize = 0;
int targetVariable = -1;

// bool testTypeIncrement = false;
bool debugMode = false;
bool initializeAlgoGSwithEmptyNet = true;

int main(int argc, char* argv[]) {
// Read parameters:
  for (int i = 0; i < argc; i++) {
    if ((strcmp(argv[i], "-F")) == 0) {
      i++;
      dataSetFile = argv[i];
    } else if ((strcmp(argv[i], "-V")) == 0) {
      i++;
      numOfVariables = atoi(argv[i]);
    } else if ((strcmp(argv[i], "-T")) == 0) {
      i++;
      targetVariable = atoi(argv[i]);
    } else if ((strcmp(argv[i], "-D")) == 0) {
      i++;
      debugMode = atoi(argv[i]);
    } else if ((strcmp(argv[i], "-S")) == 0) {
      i++;
      scoreName = argv[i];
    } else if ((strcmp(argv[i], "-A")) == 0) {
      i++;
      algorithmName = argv[i];
    } else if ((strcmp(argv[i], "-O")) == 0) {
      i++;
      orignalNetFileName = argv[i];
    } else if ((strcmp(argv[i], "-E")) == 0) {
      i++;
      initializeAlgoGSwithEmptyNet = atoi(argv[i]);
    } else if ((strcmp(argv[i], "-H")) == 0) {
      i++;

      cout << "\n -F <DataSet file name>";
      cout << "\n -V <number of variables in the data set>";
      cout << "\n -T <Target variable, Default values is -1> ";
      cout << "\n -D <DebugMode> "
          << "\n -S <Score Function Name AIC/BIC/MDL/DaShiMDL/BDeu, defualt value is BDeu>"
          << "\n -A <Learning Algorithm MMHC/PROBT/GS, Default values is MMHC> ";
      cout << "\n -O <Orignal network file name>"
          << "\n -E <Initialize GS algortihm with empty net, defualt value is one> ";
      system("pause");
      exit(1);
    }
  }


  // if parameters are not defined then get from user
  if ((strcmp(dataSetFile.c_str(), "")) == 0) {
    cout << "\n Please enter dataset file name along with full path: ";
    cin >> dataSetFile;
    // dataSetFile = "E:\\DataSet\\Alarm\\alarm5.txt"; //" E:\\DataSet\\link\\link2.txt"; //"E:\\PhD_Softwares\\Causal_Explorer\\Data\\mySave.txt"; //"E:\\DataSet\\MMHC-datasets\\ins_data\\Insurance_s5000_v1.txt";    //

    cout << "\n Please enter OrignalNet file name along with full path: ";
    cin >> orignalNetFileName;
    // orignalNetFileName = "E:\\DataSet\\Alarm\\alarm_AdjMatrix.txt"; //"E:\\DataSet\\link\\link724_AdjMatrix.txt";  // "E:\\DataSet\\MMHC-datasets\\ins_data\\Insurance_graph.txt"; //

    cout << "\n Please enter score name to be used for learning (AIC/BIC/MDL/BDeu): ";
    cin >> scoreName;
    // scoreName = "B";

    cout << "\n Please enter number of variables in a datafile: ";
    cin >> numOfVariables;
    // numOfVariables = 37;
    cout << "\n Please enter index of Target variable or enter -1 to ignore this option: ";
    cin >> targetVariable;
    // targetVariable = -1;
  }

  AlgoMMHC::orignalNetFileName = orignalNetFileName;

  strcpy(cDataSetFile, dataSetFile.c_str());

  // Input and output File definition:

  size_t pos1 = dataSetFile.find_last_of("\\");
  size_t pos2 = dataSetFile.find_last_of(".");
  outputPrefix = dataSetFile.substr(pos1+1, pos2-pos1-1);

  char the_path[256];

  getcwd(the_path, 255);  // get current working directory

  // Create a output folder'
  ostringstream osOutFolderName;

  osOutFolderName << the_path << "\\" <<outputPrefix << "_" << algorithmName;

  string outFolderName = osOutFolderName.str();
  _mkdir(outFolderName.c_str());

  osOutFolderName << "\\" << outputPrefix;
  outputPrefix = osOutFolderName.str();

  strcpy(cOutFileName, outputPrefix.c_str());

  ComputeAlgorithm_MMPCbar(cDataSetFile, cOutFileName);

  return 0;
}


bool ComputeAlgorithm_MMPCbar(char *file_name, char *outFilePrefix) {
  pmCSVDataSet* ds;
  clock_t tStart, tE;
  unsigned int dataSetSize = 0;

  size_t tempV = 0;
  Log::setOutFilePrefix(outFilePrefix, tempV, false);

  tStart = clock();
  LoadDataSet* loadDataObj = new LoadDataSet(file_name);
  ds = loadDataObj->LoadData(dataSetSize, numOfVariables, separator);
  dataSetSize = ds->get_n_records();
  cout << "\n Time taken: " << (float)(clock()- tStart)/CLOCKS_PER_SEC << "sec  Dataset Size: " << dataSetSize;

  unsigned int temp = 0;

  pmBayesianNetwork *pBN = new pmBayesianNetwork(ds->observed_variables());
  pmBayesianNetwork *pBN_empty = new pmBayesianNetwork(ds->observed_variables());

  tStart = clock();
  ds = loadDataObj->LoadData(temp, numOfVariables, separator);
  cout << "\n Time taken: " << (float)(clock()- tStart)/CLOCKS_PER_SEC << " Seconds";
  tStart = clock();

  AlgoMMHC* mmhcV = new AlgoMMHC(ds, conditioningVarSize, debugMode);
  mmhcV->setScoreName(scoreName);
  mmhcV->initializeGSwithEmptyNet = initializeAlgoGSwithEmptyNet;

  if (algorithmName == "MMHC")
    pBN = mmhcV->createMMHCGraph(targetVariable, pBN_empty);
  else if (algorithmName == "PROBT")
    pBN = mmhcV->createGraphUsingProBTLearner();
  else if(algorithmName == "GS")
    pBN = mmhcV->createGSGraph();

  cout << "\n Time taken: " << (float)(clock()- tStart)/CLOCKS_PER_SEC << " Seconds";

  delete mmhcV;
  delete pBN;
  delete loadDataObj;
  return true;
}
