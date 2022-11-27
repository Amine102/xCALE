#include "learnStructureFWB_MWST.h"
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/scores/cache.h>
#include <pilgrim/general/pmBayesianNetwork.h>

using namespace std;
using namespace PILGRIM;

void learnStructureFWB_MWST()
{
     //FWB with MWST structure Learning
     cout << "\n \n // ============  Start FWB MWST Structural Learning ============= // \n\n";
     int nClass = 6;
	size_t line_skipped = 1;
     size_t nb_param = 217;
     char delimiter = ',';
     string name_xml ="../../benchmarks/networks/jdBnFWB_MWST.xml";
     //Load Data
     const string &path_to_FWB_csv = "../../benchmarks/data/NFWBS_PUF_2016_data.csv";
     const string &path_to_FWB_csv_test = "../../benchmarks/data/NFWBS_PUF_2016_data_test.csv";
     char *cstr = new char[path_to_FWB_csv.length() + 1];
     strcpy(cstr, path_to_FWB_csv.c_str());
     PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);
     plVariablesConjunction variables = data->observed_variables();
	
     //Creating the pmCSVDataSet for the testing set
     cstr = new char[path_to_FWB_csv_test.length() + 1];
	strcpy(cstr, path_to_FWB_csv_test.c_str());
     PILGRIM::pmCSVDataSet *data_test = new PILGRIM::pmCSVDataSet(cstr,line_skipped,nb_param,delimiter,variables);

     //Tune parameters
     string score = "BIC"; // we can choose BIC or AIC or BDeu or QNML or MDL
     bool debugVerbose = false;



     // set up frequency counter
     pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
                                                     variables);
     fc->computeFrequencies();

     // Use cache for faster computation
     pmCache *cache = new PILGRIM::pmCache(1000000000);

     //create score and algorithm
     pmBayesianNetwork bn_empty(variables);

     pmDecomposableScore<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);

     pmAlgoMWST <rowDataType> algo(data, pScore);

     pmBayesianNetwork *bn_learned = new pmBayesianNetwork(algo.getVariables());
     //pmBayesianNetwork *bn_learned =
     algo.run(bn_learned);
     
     bn_learned->initComputableObjectList(true, true);

     bn_learned->learnParameters(data);
	//bn_learned->confMatrix(nClass,data_test,false);

     pmGraph bn_learned_Graph = bn_learned->get_graph();
     plBayesianNetwork pl_bn(bn_learned->getJointDistribution(),"bn");
     pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn_FWD_MWST.dot");


	if ( access( name_xml.c_str(), F_OK ) != -1 ) {
		cout << "XML Not saved "<<endl;
	 }
	else {
		cout << "XML saved "<<endl;
	//	bn_learned->save_as_xml(name_xml,"jdBnFWB_MWST");
			}

	cout << "\n \n // ============  Finished FWB Structural Learning with MWST algorithm ============= // \n\n";

}
