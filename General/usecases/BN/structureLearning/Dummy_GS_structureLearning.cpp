#include "Dummy_GS_structureLearning.h"
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace PILGRIM;
using namespace boost;

pmGraph *CreateGraphCPC()
{
    pmGraph graphCPC;
    pmNode X0 = add_vertex(graphCPC);
    pmNode X1 = add_vertex(graphCPC);
    pmNode X2 = add_vertex(graphCPC);
    pmNode X3 = add_vertex(graphCPC);
    pmNode X4 = add_vertex(graphCPC);

    add_edge(X0, X3, graphCPC);
    add_edge(X3, X0, graphCPC);
    add_edge(X1, X3, graphCPC);
    add_edge(X3, X1, graphCPC);
    add_edge(X2, X4, graphCPC);
    add_edge(X4, X2, graphCPC);
    add_edge(X3, X4, graphCPC);
    add_edge(X4, X3, graphCPC);

    add_edge(X0, X2, graphCPC);
    return &graphCPC;
}

/* 
The path to the xml and the the xml label are used to 
load the the original structure to compare it with the learned one
*/

void Dummy_GS_structureLearning()
{
    cout << "\n \n // ============  Start Dummy Usecase Structural Learning ============= // \n\n";

    //Load Data
    string path_to_csv = "../../benchmarks/data/Dummy_csv1.data";
    char *cstr = new char[path_to_csv.length() + 1];
    strcpy(cstr, path_to_csv.c_str());
    PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);

    //Load Original BN
    pmBayesianNetwork OriginalBN("../../benchmarks/networks/jdBnDummy1.xml", "jdDummy");

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
    pmBayesianNetwork *learnedBN_GS=new pmBayesianNetwork(data->observed_variables());
    algoGS.run(learnedBN_GS);
    cout << "Finished Learning " << endl;
    
    pmGraph learnedBN_GS_Graph = learnedBN_GS->get_graph();
    pmGraph originalBN_Graph = OriginalBN.get_graph();

    // compare graphs
    unsigned int tp, fp, tn, fn, reverse, shd;
    OriginalBN.compareBNs(originalBN_Graph, learnedBN_GS_Graph,
                                   tp, fp, tn, fn, shd);

    cout << "Structural differences ------------------------------" << endl;

    cout << "\nBN Original Graph  ------------------------------\n"
         << endl;
    OriginalBN.summary();

    cout << "\nBN learned Graph ------------------------------\n"
         << endl;
    learnedBN_GS->summary();

    cout << "\nTP, FP, TN, FN are calculated between the skeletons not the DAGs" << endl;
    cout << "TP:  \t\t" << tp << endl;
    cout << "FP:  \t\t" << fp << endl;
    cout << "TN:  \t\t" << tn << endl;
    cout << "FN:  \t\t" << fn << endl;
    cout << "\nThe SHD is calculated between DAGs" << endl;
    cout << "SHD:  \t\t" << shd << endl;

    cout << "\n \n // ============  Finished Asia Usecase Structural Learning ============= // \n\n";

    cout << "\n \n // ============  Finished Dummy Usecase Structural Learning ============= // \n\n";
}
