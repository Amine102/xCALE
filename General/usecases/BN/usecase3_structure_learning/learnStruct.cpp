#include "learnStruct.h"

using namespace std;
using namespace PILGRIM;

void usecase3_learnStruct(){


    //Load Data 
    string path_to_csv = "../../benchmarks/data/Asia_uc_csv.data";
    char *cstr = new char[path_to_csv.length() + 1];
    strcpy(cstr, path_to_csv.c_str());
    PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);


    //Tune parameters
    string score = "BIC";
    bool debugVerbose = false;

    //Getting the variables from the loaded data
    plVariablesConjunction variables = data->observed_variables();

    // set up frequency counter
    pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
                                                    variables);
    fc->computeFrequencies();

    // Use cache for faster computation
    pmCache *cache = new PILGRIM::pmCache(1000000000);

    //create score and algorithm
    pmBayesianNetwork bn_empty(variables);
    pmDecomposableScore<rowDataType> *pScore;

    if (score == "BIC")
    {
        pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
    }
    else if (score == "AIC")
    {
        pScore = new pmScoreAIC<rowDataType>(&bn_empty, fc, cache);
    }
    else if (score == "BDeu")
    {
        pScore = new pmScoreBDeu<rowDataType>(&bn_empty, fc, 1, cache);
    }
    else if (score == "QNML")
    {
        pScore = new pmScoreQNML<rowDataType>(&bn_empty, fc, cache);
    }
    else if (score == "MDL")
    {
        pScore = new pmScoreMDL<rowDataType>(&bn_empty, fc, cache);
    }

    pmAlgoGS<rowDataType> algoGS(data, pScore);
    algoGS.setDebugVerbose(debugVerbose);

    cout << "Start Learning  ------------------------------" << endl;
    // compute BN
    pmBayesianNetwork learnedBN_GS(data->observed_variables());
    algoGS.run(&learnedBN_GS);
    cout << "Finished Learning " << endl;

    learnedBN_GS.summary();
}