#include "Alarm_GS_structureLearning.h"

using namespace std;
using namespace PILGRIM;

/*
To do your own structural learning, you will have to modify:

1- the path to csv
2- the path to the xml file 
3- xml_label if necessary
4- score --> BIC or AIC or BDeu or QNML or MDL
5- debugVerbose boolean flag
6- set initBN if desired
7- set a graphCPC if desired

*/
void Alarm_GS_structureLearning()
{
    cout << "\n \n // ============  Start Alarm Structural Learning ============= // \n\n";

    //Load Data 
    string path_to_csv = "../../benchmarks/data/Alarm1.data";
    char *cstr = new char[path_to_csv.length() + 1];
    strcpy(cstr, path_to_csv.c_str());
    PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);

    //Load Original BN
    pmBayesianNetwork originalBN("../../benchmarks/networks/Alarm.xdsl");

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
    pmGraph learnedBN_GS_Graph = learnedBN_GS.get_graph();
    pmGraph bn_original = originalBN.get_graph();

    // compare graphs
    unsigned int tp, fp, tn, fn, reverse, shd;
    originalBN.compareBNs(bn_original, learnedBN_GS_Graph,
                                   tp, fp, tn, fn, shd);

    cout << "Structural differences ------------------------------" << endl;

    cout << "\nBN Original Graph  ------------------------------\n"
         << endl;
    originalBN.summary();

    cout << "\nBN learned Graph ------------------------------\n"
         << endl;
    learnedBN_GS.summary();

    cout << "\nTP, FP, TN, FN are calculated between the skeletons not the DAGs" << endl;
    cout << "TP:  \t\t" << tp << endl;
    cout << "FP:  \t\t" << fp << endl;
    cout << "TN:  \t\t" << tn << endl;
    cout << "FN:  \t\t" << fn << endl;
    cout << "\nThe SHD is calculated between DAGs" << endl;
    cout << "SHD:  \t\t" << shd << endl;

    cout << "\n \n // ============  Finished Alarm Usecase Structural Learning ============= // \n\n";
}