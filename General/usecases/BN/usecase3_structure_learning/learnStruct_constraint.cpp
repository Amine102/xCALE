#include "learnStruct.h"

using namespace std;
using namespace PILGRIM;

void usecase3_learnStruct_constraint(){

    pmGraph graphCPC;
    pmNode X0 = add_vertex(graphCPC); // Bronchitis
    pmNode X1 = add_vertex(graphCPC); // Dyspnea
    pmNode X2 = add_vertex(graphCPC); // LungCancer
    pmNode X3 = add_vertex(graphCPC); // Smocking
    pmNode X4 = add_vertex(graphCPC); // TbOrCa
    pmNode X5 = add_vertex(graphCPC); // Tuberculosis
    pmNode X6 = add_vertex(graphCPC); // VisitToAsia
    pmNode X7 = add_vertex(graphCPC); // XRay

    add_edge(X0, X1, graphCPC);
    add_edge(X0, X2, graphCPC);
    add_edge(X0, X3, graphCPC);
    add_edge(X0, X5, graphCPC);
    add_edge(X0, X6, graphCPC);
    add_edge(X0, X7, graphCPC);

    add_edge(X1, X2, graphCPC);
    add_edge(X1, X3, graphCPC);
    add_edge(X1, X5, graphCPC);
    add_edge(X1, X6, graphCPC);
    add_edge(X1, X7, graphCPC);

    add_edge(X2, X3, graphCPC);
    add_edge(X2, X4, graphCPC);
    add_edge(X2, X5, graphCPC);
    add_edge(X2, X6, graphCPC);
    add_edge(X2, X7, graphCPC);

    add_edge(X3, X5, graphCPC);
    add_edge(X3, X6, graphCPC);
    add_edge(X3, X7, graphCPC);

    add_edge(X4, X5, graphCPC);
    add_edge(X4, X7, graphCPC);

    add_edge(X5, X6, graphCPC);
    add_edge(X5, X7, graphCPC);

    add_edge(X6, X7, graphCPC);
    

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
    algoGS.setCPCGraph(&graphCPC);
    algoGS.setDebugVerbose(debugVerbose);

    cout << "Start Learning  ------------------------------" << endl;
    // compute BN
    pmBayesianNetwork learnedBN_GS(data->observed_variables());
    algoGS.run(&learnedBN_GS);
    cout << "Finished Learning " << endl;
    pmGraph learnedBN_GS_Graph = learnedBN_GS.get_graph();

    learnedBN_GS.summary();
}