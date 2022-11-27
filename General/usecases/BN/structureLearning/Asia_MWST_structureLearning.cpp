#include "Asia_MWST_structureLearning.h"
#include <boost/graph/adjacency_list.hpp>

using namespace std;

void Asia_MWST_structureLearning()
{
     //Asia with MWST structure Learning
     cout << "\n \n // ============  Start Asia MWST Structural Learning ============= // \n\n";

     //Load Data
     string path_to_csv = "../../benchmarks/data/Asia1.data";
     char *cstr = new char[path_to_csv.length() + 1];
     strcpy(cstr, path_to_csv.c_str());
     PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);

     //Load Original BN
     pmBayesianNetwork bnDummyOriginalTemp("../../benchmarks/networks/Asia.xdsl");

     //Tune parameters
     string score = "BIC"; // we can choose BIC or AIC or BDeu or QNML or MDL
     bool debugVerbose = false;

     plVariablesConjunction variables = data->observed_variables();

     // set up frequency counter
     pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
                                                     variables);
     fc->computeFrequencies();

     // Use cache for faster computation
     pmCache *cache = new PILGRIM::pmCache(1000000000);

     //create score and algorithm
     pmBayesianNetwork bn_empty(variables);

     pmDecomposableScore<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);

     pmAlgoMWST<rowDataType> algoMWST(data, pScore);

     pmBayesianNetwork *learnedBN_MWST = new pmBayesianNetwork(algoMWST.getVariables());
     algoMWST.run(learnedBN_MWST);
     learnedBN_MWST->initComputableObjectList(true, true);

     pmGraph bn_learned_Graph = learnedBN_MWST->get_graph();
     pmGraph bn_original_Graph = bnDummyOriginalTemp.get_graph();

     // compare graphs
     unsigned int tp, fp, tn, fn, reverse, shd;
     bnDummyOriginalTemp.compareBNs(bn_original_Graph, bn_learned_Graph,
                                    tp, fp, tn, fn, shd);

     cout << "\nBN learned Graph with random parameters ------------------------------\n"
          << endl;
     learnedBN_MWST->summary();
     cout << "\n===================================================================\n"
          << endl;
     cout << "\nTP, FP, TN, FN are calculated between the skeletons not the DAGs" << endl;
     cout << "TP:  \t\t" << tp << endl;
     cout << "FP:  \t\t" << fp << endl;
     cout << "TN:  \t\t" << tn << endl;
     cout << "FN:  \t\t" << fn << endl;
     cout << "\nThe SHD is calculated between CPDAGs" << endl;
     cout << "SHD:  \t\t" << shd << endl;

     cout << "\n \n // ============  Finished Asia Usecase Structural Learning ============= // \n\n";
     learnedBN_MWST->save_as_xml("../../benchmarks/networks/asia_learnedStructureAlgoMWST.xml","mwst_asia");
}