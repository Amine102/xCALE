#include "learnStructureFWB_GS.h"
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/scores/cache.h>
#include <pilgrim/general/pmBayesianNetwork.h>

using namespace std;
using namespace PILGRIM;


void learnStructureFWB_GS()
{
	cout << "\n \n // ============  Start Financial Well Being Structural Learning ============= // \n\n";
	int nClass = 57;
	size_t line_skipped = 1;
    size_t nb_param = 217;
    char delimiter = ',';
	string name_xml ="../../benchmarks/networks/jdBnFWB.xml";
	const string &path_to_FWB_csv = "../../benchmarks/data/NFWBS_PUF_2016_data.csv";
	const string &path_to_FWB_csv_test = "../../benchmarks/data/NFWBS_PUF_2016_data_test.csv";
	string path_to_csv = path_to_FWB_csv;
	char *cstr = new char[path_to_csv.length() + 1];
	strcpy(cstr, path_to_csv.c_str());
	PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);
	//Getting the variables from the loaded data for the test 
	plVariablesConjunction variables = data->observed_variables();


	//Creating the pmCSVDataSet for the testing set
    path_to_csv = path_to_FWB_csv_test;
    cstr = new char[path_to_csv.length() + 1];
	strcpy(cstr, path_to_csv.c_str());


    PILGRIM::pmCSVDataSet *data_test = new PILGRIM::pmCSVDataSet(cstr,line_skipped,nb_param,delimiter,variables);

	// set up frequency counter
	pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
													variables);
	fc->computeFrequencies();

	// Use cache for faster computation
	pmCache *cache = new PILGRIM::pmCache(1000000000);

	//create score and algorithm
	pmBayesianNetwork bn_empty(variables);

	pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
	pmAlgoGS<rowDataType> algo(data, pScore);

	cout << "Start Learning" << endl;
	// compute BN
    pmBayesianNetwork* initBN=algo.getInitialBN();
    //pmBayesianNetwork *bn_learned = algo.run(initBN);
    pmBayesianNetwork bn_learned(data->observed_variables());
    algo.run(&bn_learned);
    bn_learned.learnParameters(data);
	//bn_learned->confMatrix(nClass,data_test,true);
	pmGraph bn_learned_Graph = bn_learned.get_graph();
    plBayesianNetwork pl_bn(bn_learned.getJointDistribution(),"bn");
    pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn_FWD.dot");
     

	if ( access( name_xml.c_str(), F_OK ) != -1 ) {
		cout << "XML Not saved "<<endl;
	 }
	else {
		cout << "XML saved "<<endl;
		bn_learned.save_as_xml(name_xml,"jdBnFWB");
			}

	cout << "\n \n // ============  Finished FWB Structural Learning ============= // \n\n";

	
}
