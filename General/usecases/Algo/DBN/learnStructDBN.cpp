#include "learnStructDBN.h"

using namespace PILGRIM;
using namespace std;
using namespace boost;

void runDBNAlgoGS()
{
  // My file
  char *file_name = "dbn.csv";
  string str = "dbn.csv";

  pmUtility::split(str);

  // My BIC Score
  pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>();
  // pmScoreAIC<rowDataType>* pScore = new pmScoreAIC<rowDataType>();

  /*
    ===== WITH NEW CONSTRUCTOR =====
    */

  // My data set
  pmCSVDataSet *ds_t0 = new pmCSVDataSet("dbn_t0.csv");
  pmCSVDataSet *ds_T = new pmCSVDataSet("dbn_t.csv");

  // Creating initial algos
  pmAlgoGS<rowDataType> *algoGS = new pmAlgoGS<rowDataType>(ds_t0, pScore);
  pmAlgoMMHC<rowDataType> *algoMMHC = new pmAlgoMMHC<rowDataType>(ds_t0, pScore);
  algoMMHC->setDebugVerbose(true);

  // Creating transition algos
  pmAlgoGS_T<rowDataType> *algoGS_T = new pmAlgoGS_T<rowDataType>(ds_T, pScore);
  pmAlgoMMHC_T<rowDataType> *algoMMHC_T = new pmAlgoMMHC_T<rowDataType>(ds_T, pScore);

  // Creating algoGS and algoMMHC as dynamic
  pmAlgoDynamic<rowDataType> algoDBNGS(algoGS, algoGS_T);
  pmAlgoDynamic<rowDataType> algoDBNMMHC(algoMMHC, algoMMHC_T);

  cout << endl
       << endl
       << "========== MY GS DYNAMIC ALGO ==========" << endl;

  pmDynamicBayesianNetwork *dynamicBNGS = algoDBNGS.getDynamicInitBN();
  algoDBNGS.run(dynamicBNGS);
  algoDBNGS.summary();

  cout << endl
       << endl
       << "========== MY MMHC DYNAMIC ALGO ==========" << endl;
  pmDynamicBayesianNetwork *dynamicDBNMMHC = algoDBNMMHC.getDynamicInitBN();
  algoDBNMMHC.run(dynamicDBNMMHC);
  algoDBNMMHC.summary();
}