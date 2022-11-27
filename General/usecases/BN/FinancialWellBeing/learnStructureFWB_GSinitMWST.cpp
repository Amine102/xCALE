#include "learnStructureFWB_GSinitMWST.h"
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/ScoreAIC.h>
#include <pilgrim/general/scores/ScoreBDeu.h>
#include <pilgrim/general/scores/ScoreMDL.h>
#include <pilgrim/general/scores/ScoreQNML.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/scores/cache.h>
#include <pilgrim/general/pmBayesianNetwork.h>

using namespace std;
using namespace PILGRIM;

void learnStructureFWB_GSinitMWST()
{

     cout << "\n \n // ============  Start FWB GS intialised with MWST Structural Learning ============= // \n\n";
     string name_xml ="../../benchmarks/networks/jdBnFWB_GSinitMWST.xml";
     //Load Data
     const string &path_to_FWB_csv = "../../benchmarks/data/NFWBS_PUF_2016_data.csv";
     char *cstr = new char[path_to_FWB_csv.length() + 1];
     strcpy(cstr, path_to_FWB_csv.c_str());
     PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);


     //Tune parameters
     string score = "BIC"; // we can choose BIC or AIC or BDeu or QNML or MDL
     bool debugVerbose = false;

    
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
     bnLearned_MWST_Init.save_as_xml("../../benchmarks/networks/FWB_MWST_initBN.xdsl", "FWB_mwst_initBN");

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
     cout << "Setting scoring metric"<< endl;

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


     cout << "Start Learning Structure and Parameters with the GS" << endl;
     // compute BN

 
     pmBayesianNetwork initBN(data->observed_variables());
     initBN.copy_structure(bnLearned_MWST_Init);
     algoGS.setInitialNetwork(&initBN);

     pmBayesianNetwork learnedBN_GS(data->observed_variables());
     algoGS.run(&learnedBN_GS);

     // bn_learned->initComputableObjectList(true, true);
     pmGraph bn_learned_Graph = learnedBN_GS.get_graph();
     plBayesianNetwork pl_bn(learnedBN_GS.getJointDistribution(),"bn");
     pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn_FWD_GSinitMWST.dot");


	if ( access( name_xml.c_str(), F_OK ) != -1 ) {
		cout << "XML Not saved "<<endl;
	 }
	else {
		cout << "XML saved "<<endl;
		learnedBN_GS.save_as_xml(name_xml,"jdBnFWB_GSinitMWST");
			}

	cout << "\n \n // ============  Finished FWB Structural Learning with GS initialised with MWST algorithm ============= // \n\n";


}
