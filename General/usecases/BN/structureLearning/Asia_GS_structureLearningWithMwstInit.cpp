#include "Asia_GS_structureLearningWithMwstInit.h"
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace PILGRIM;
using namespace boost;

/*
In this example we used a graphCPC to be a constraint for the learned Graph
*/
void Asia_GS_structureLearningWithMwstInit()
{
     cout << "\n \n // ============  Start Asia Structural Learning ============= // \n\n";

     //Load Data
     string path_to_csv = "../../benchmarks/data/Asia1.data";
     char *cstr = new char[path_to_csv.length() + 1];
     strcpy(cstr, path_to_csv.c_str());
     PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);

     //Load Original BN
     pmBayesianNetwork originalBN("../../benchmarks/networks/Asia.xdsl");

     //Tune parameters
     string score = "BIC"; // we can choose BIC or AIC or BDeu or QNML or MDL
     bool debugVerbose = false;

     //=======================================================================================
     //CREATING A GRAPH CPC
     pmGraph graphCPC;
     pmNode X0 = add_vertex(graphCPC);
     pmNode X1 = add_vertex(graphCPC);
     pmNode X2 = add_vertex(graphCPC);
     pmNode X3 = add_vertex(graphCPC);
     pmNode X4 = add_vertex(graphCPC);
     pmNode X5 = add_vertex(graphCPC);
     pmNode X6 = add_vertex(graphCPC);
     pmNode X7 = add_vertex(graphCPC);

     add_edge(X0, X1, graphCPC);
     add_edge(X1, X0, graphCPC);

     add_edge(X2, X4, graphCPC);
     add_edge(X4, X2, graphCPC);

     add_edge(X0, X3, graphCPC);
     add_edge(X3, X0, graphCPC);

     add_edge(X3, X2, graphCPC);
     add_edge(X2, X3, graphCPC);

     add_edge(X4, X1, graphCPC);
     add_edge(X1, X4, graphCPC);

     add_edge(X4, X7, graphCPC);
     add_edge(X7, X4, graphCPC);

     add_edge(X4, X5, graphCPC);
     add_edge(X5, X4, graphCPC);

     add_edge(X6, X5, graphCPC);
     add_edge(X5, X6, graphCPC);

     add_edge(X3, X6, graphCPC);
     add_edge(X6, X3, graphCPC);

     add_edge(X2, X1, graphCPC);
     add_edge(X1, X2, graphCPC);

     //Printing the graph
     cout << "Graph CPC is \n"
          << endl;
     unsigned int i, j, n;
     bool exists;
     pmEdge e;
     n = (unsigned int)boost::num_vertices(graphCPC);
     cout << "n is " << n << endl;
     for (i = 0; i < n; i++)
     {
          cout << i << " -->  ";
          for (j = 0; j < n; j++)
          {
               if (j == i)
                    continue;
               boost::tie(e, exists) = edge(vertex(i, graphCPC), vertex(j, graphCPC), graphCPC);
               if (exists)
                    cout << j << " ";
          }
          cout << endl;
     }
     cout << "\n===================================================================\n"
          << endl;

     //=======================================================================================
     //INIT BN from algo MWST
     plVariablesConjunction init_variables = data->observed_variables();

     // set up frequency counter
     pmFrequencyCounter *init_fc = new pmFrequencyCounter(data->get_data_descriptor(),
                                                          init_variables);
     init_fc->computeFrequencies();

     // Use cache for faster computation
     pmCache *init_cache = new PILGRIM::pmCache(1000000000);

     //create score and algorithm
     pmBayesianNetwork init_bn_empty(init_variables);

     pmDecomposableScore<rowDataType> *init_pScore = new pmScoreBIC<rowDataType>(&init_bn_empty, init_fc, init_cache);

     pmAlgoMWST<rowDataType> algoMWST(data, init_pScore);

     pmBayesianNetwork bnLearned_MWST_Init(algoMWST.getVariables());
     algoMWST.run(&bnLearned_MWST_Init);
     
     bnLearned_MWST_Init.initComputableObjectList(true, true);

     cout << "Init BN Summary (with random parameters) obtained from MWST structure Learning is:\n"
          << endl;
     bnLearned_MWST_Init.summary();
     cout << "\n===================================================================\n"
          << endl;
     bnLearned_MWST_Init.save_as_xml("../../benchmarks/networks/MWST_initBN.xml", "mwst_initBN");

     //==========================================================================================================

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

     //Setting Scoring Metric
     cout << "Setting scoring metric to " << score << endl;

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
     cout << "\n===================================================================\n"
          << endl;

     pmAlgoGS<rowDataType> algoGS(data, pScore);

     cout << "Debug Verbose is " << debugVerbose << "\n"
          << endl;
     cout << "\n===================================================================\n"
          << endl;

     algoGS.setDebugVerbose(debugVerbose);

     // algo.setCPCGraph(&graphCPC);

     cout << "Start Learning Structure and Parameters" << endl;
     // compute BN

     // pmBayesianNetwork initComputedBN(data->observed_variables());
     // initComputedBN.copy_structure(*bnLearned_MWST_Init);
     pmBayesianNetwork initBN(data->observed_variables());
     initBN.copy_structure(bnLearned_MWST_Init);
     algoGS.setInitialNetwork(&initBN);

     pmBayesianNetwork learnedBN_GS(data->observed_variables());
     algoGS.run(&learnedBN_GS);

     // bn_learned->initComputableObjectList(true, true);

     cout << "Finished Learning Structure and Parameters" << endl;
     cout << "\n===================================================================\n"
          << endl;
     pmGraph bn_learned_Graph = learnedBN_GS.get_graph();
     pmGraph originalBN_Graph = originalBN.get_graph();

     // compare graphs
     unsigned int tp, fp, tn, fn, reverse, shd;
     originalBN.compareBNs(originalBN_Graph, bn_learned_Graph,
                                    tp, fp, tn, fn, shd);
     cout << "\n===================================================================\n"
          << endl;

     cout << "\nBN Original Graph  ------------------------------\n"
          << endl;
     originalBN.summary();
     cout << "\n-------------------------------------------------------------------\n"
          << endl;

     cout << "\nBN learned Graph with random parameters ------------------------------\n"
          << endl;
     learnedBN_GS.summary();

     cout << "\n===================================================================\n"
          << endl;
     cout << "\nTP, FP, TN, FN are calculated between the skeletons not the DAGs" << endl;
     cout << "TP:  \t\t" << tp << endl;
     cout << "FP:  \t\t" << fp << endl;
     cout << "TN:  \t\t" << tn << endl;
     cout << "FN:  \t\t" << fn << endl;
     cout << "\nThe SHD is calculated between DAGs" << endl;
     cout << "SHD:  \t\t" << shd << endl;

     cout << "\n \n // ============  Finished Asia Usecase Structural Learning ============= // \n\n";
     learnedBN_GS.save_as_xml("../../benchmarks/networks/asia_learnedStructureAlgoGS_MWSTinitBN.xml", "asia_gs_mwstInitBN");
}