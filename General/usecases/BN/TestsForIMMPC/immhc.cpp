#include "../structureLearning/Asia_GS_structureLearningWithMwstInit.h"
#include "../structureLearning/Asia_GS_structureLearningWithRandomInit.h"
#include "../structureLearning/Asia_MWST_structureLearning.h"
#include <boost/graph/adjacency_list.hpp>
#include "immhc.h"
#include "../DummyUseCase/HelperFunctions.h"

using namespace std;
using namespace PILGRIM;
using namespace boost;

/*
In this example we used a graphCPC to be a constraint for the learned Graph
*/

/*!!!!! Arguments Default values are TRUE  !!!!!*/
void immhc1(string path_to_csv)
{
     cout << "\n \n // ============  Start Asia Incremental Structural Learning ============= // \n\n";

     cout << "Loading First Data" << endl;
     // string path_to_csv = "../../benchmarks/data/Asia1_0.data";
     cout<<"string is "<<path_to_csv<<endl;
     char *cstr = new char[path_to_csv.length() + 1];
     strcpy(cstr, path_to_csv.c_str());
     PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);

     //Load Original BN
     pmBayesianNetwork OriginalBN("../../benchmarks/networks/Asia.xdsl");
     cout<<"original BN variables are"<<OriginalBN.getVariables()<<endl;
     //Tune parameters
     string score = "BIC"; // we can choose BIC or AIC or BDeu or QNML or MDL
     bool debugVerbose = false;
     //==========================================================================================================
     //Getting the variables from the loaded data
     plVariablesConjunction variables = data->observed_variables();

     // set up frequency counter
     cout << "FrequencyCounter without SlidingWindow" << endl;
     float ff=0.7;
     pmFrequencyCounter *fc = new pmFrequencyCounter(data,
                                                     variables, true, ff, true);
     cout << "\n===================================================================\n"
          << endl;
     // Use cache for faster computation
     pmCache *cache = new PILGRIM::pmCache(1000000000);

     //create score and algorithm
     pmBayesianNetwork bn_empty(variables);

     //Setting Scoring Metric
     //  cout << "Setting scoring metric to " << score << endl;

     pmDecomposableScore<rowDataType> *pScore;

     pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);

     pmAlgoMMHC<rowDataType> algoMMHC(data, pScore);

     cout << "Start Learning" << endl;

     pmBayesianNetwork learnedBN_GS(data->observed_variables());
     algoMMHC.run(&learnedBN_GS);

     learnedBN_GS.summary();

     //The line of code below is for random initialization of parameters
     // bn_learned->initComputableObjectList(true, true);

     //Parameters Learning
     learnedBN_GS.learnParameters(data);

     //Graphs comparisons
     pmGraph learnedBN_GS_Graph = learnedBN_GS.get_graph();
     pmGraph originalBN_Graph = OriginalBN.get_graph();

     // compare graphs
     unsigned int tp, fp, tn, fn, reverse, shd;
     OriginalBN.compareBNs(originalBN_Graph, learnedBN_GS_Graph,
                           tp, fp, tn, fn, shd);
     cout << "\n===================================================================\n"
          << endl;

     cout << "\nBN Original Graph  ------------------------------\n"
          << endl;
     OriginalBN.summary();
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
     cout << "\nThe SHD is calculated between CPDAGs" << endl;
     cout << "SHD:  \t\t" << shd << endl;

     cout << "\n \n // ============  Finished Asia Usecase Structural Learning with random Init ============= // \n\n";
     learnedBN_GS.save_as_xml("../../benchmarks/networks/asia_learnedStructureAlgoGS_RANDOMinitBN.xml", "asia_gs_randomInit");
     
     fc->updateSW(learnedBN_GS);
}

void immhc2(){
     cout << "\n \n // ============  Start Asia Incremental Structural Learning with second Dataset============= // \n\n";

     cout << "Loading First Data" << endl;
     string path_to_csv ="../../benchmarks/data/Asia1.data";
     char *cstr = new char[path_to_csv.length() + 1];
     strcpy(cstr, path_to_csv.c_str());
     PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);

     //Load Original BN
     pmBayesianNetwork OriginalBN("../../benchmarks/networks/Asia.xdsl");

     //Tune parameters
     string score = "BIC"; // we can choose BIC or AIC or BDeu or QNML or MDL
     bool debugVerbose = false;
     //==========================================================================================================
     //Getting the variables from the loaded data
     plVariablesConjunction variables = data->observed_variables();

     // set up frequency counter
     cout << "FrequencyCounter without SlidingWindow" << endl;
     pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
                                                     variables);
     cout << "\n===================================================================\n"
          << endl;
     // Use cache for faster computation
     pmCache *cache = new PILGRIM::pmCache(1000000000);

     //create score and algorithm
     pmBayesianNetwork bn_empty(variables);

     //Setting Scoring Metric
     //  cout << "Setting scoring metric to " << score << endl;

     pmDecomposableScore<rowDataType> *pScore;

     pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);

     pmAlgoMMHC<rowDataType> algoMMHC(data, pScore);

     cout << "Start Learning" << endl;

     // compute BN
     //  pmBayesianNetwork initialBN(data->observed_variables());
     //  initialBN.copy_structure(initBN);
     //  algoMMHC.setInitialNetwork(&initialBN);

     // pmBayesianNetwork bn_learned((algo.getInitialBN())->getVariables());
     // bn_learned.copyStructure(*algo.getInitialBN());

     pmBayesianNetwork learnedBN_GS(data->observed_variables());
     algoMMHC.run(&learnedBN_GS);

     learnedBN_GS.summary();

     //The line of code below is for random initialization of parameters
     // bn_learned->initComputableObjectList(true, true);

     //Parameters Learning
     learnedBN_GS.learnParameters(data);

     cout << "Finished Learning parameters" << endl;
     cout << "\n===================================================================\n"
          << endl;

     pmGraph learnedBN_GS_Graph = learnedBN_GS.get_graph();
     pmGraph originalBN_Graph = OriginalBN.get_graph();

     // compare graphs
     unsigned int tp, fp, tn, fn, reverse, shd;
     OriginalBN.compareBNs(originalBN_Graph, learnedBN_GS_Graph,
                           tp, fp, tn, fn, shd);
     cout << "\n===================================================================\n"
          << endl;

     cout << "\nBN Original Graph  ------------------------------\n"
          << endl;
     OriginalBN.summary();
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
     cout << "\nThe SHD is calculated between CPDAGs" << endl;
     cout << "SHD:  \t\t" << shd << endl;

     cout << "\n \n // ============  Finished Asia Usecase Structural Learning with random Init ============= // \n\n";
     learnedBN_GS.save_as_xml("../../benchmarks/networks/asia_learnedStructureAlgoGS_RANDOMinitBN.xml", "asia_gs_randomInit");
     
     fc->updateSW(learnedBN_GS);
}

void immhc(){
     // pmBayesianNetwork OriginalBN("../../benchmarks/networks/Asia.xdsl");
     // OriginalBN.generate_csvData("../../benchmarks/data/Asia1000.csv",1000);

     splitData("../../benchmarks/data/Asia1000.csv",10);

     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2U3.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2U3U4.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2U3U4U5.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2U3U4U5U6.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2U3U4U5U6U7.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2U3U4U5U6U7U8.data");
     // immhc1("../../benchmarks/data/Asia_datasetCumulated/Asia1000_0U1U2U3U4U5U6U7U8U9.data");

     immhc1("../../benchmarks/data/Asia1000.csv0.data");
     immhc1("../../benchmarks/data/Asia1000.csv1.data");
     immhc1("../../benchmarks/data/Asia1000.csv2.data");
     immhc1("../../benchmarks/data/Asia1000.csv3.data");
     immhc1("../../benchmarks/data/Asia1000.csv4.data");
     immhc1("../../benchmarks/data/Asia1000.csv5.data");
     immhc1("../../benchmarks/data/Asia1000.csv6.data");
     immhc1("../../benchmarks/data/Asia1000.csv7.data");
     immhc1("../../benchmarks/data/Asia1000.csv8.data");
     immhc1("../../benchmarks/data/Asia1000.csv9.data");
     // immhc2();
}