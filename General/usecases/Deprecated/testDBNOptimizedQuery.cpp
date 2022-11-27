#include "testDBNOptimizedQuery.h"
#include "pilgrim/general/algorithms/AlgoGS.h"
using namespace PILGRIM;
using namespace std;

void testDBNOptimizedQuery(){


	plJointDistribution jd0;
	jd0.load_from_genie("./benchmarks/networks/test_query_optimized_t0.xdsl");

	plJointDistribution jd0_dup = plJointDistribution(jd0);

	pmBayesianNetwork G0(jd0);
	pmBayesianNetwork G0_dup(jd0_dup);

	plJointDistribution jdt;
	jdt.load_from_genie("./benchmarks/networks/test_query_optimized_t.xdsl");
	plJointDistribution jdt_dup = plJointDistribution(jdt);
	cout<<jdt.get_variables().size() << endl;
	pmBayesianNetwork Gt(jdt);
	pmBayesianNetwork Gt_dup(jdt_dup);
	pmDynamicBayesianNetwork dBN = pmDynamicBayesianNetwork(&G0, &Gt);
	pmDynamicBayesianNetwork dBN_dup = pmDynamicBayesianNetwork(&G0_dup, &Gt_dup);


	/*
	std::cout << "Dynamic Bayesian Network" << std::endl;
	std::cout << "------------------------" << std::endl;
	dBN.summary();
	std::cout << std::endl;
	std::cout << std::endl;
	*/
	plValues voidEvidence;

	plVariablesConjunction vars = jdt.get_variables();

	plVariable target_variable = vars.get_variable_with_name("CCC_t");
	plVariable target_variable_dup = vars.get_variable_with_name("AAA_t");
	plVariable t1 = vars.get_variable_with_name("BBB_t");
	plVariable t2 = vars.get_variable_with_name("DDD_t");
	plVariable evs_A = vars.get_variable_with_name("AAA");
	plVariable evs_B = plVariable("BBB", evs_A.get_type());
	plVariable evs_C = vars.get_variable_with_name("CCC");
	plVariable evs_D = vars.get_variable_with_name("DDD");
	plVariable evs_D_t = vars.get_variable_with_name("DDD_t");
	plVariable evs_B_t = plVariable("BBB_t", evs_A.get_type());
	plValues evidence_0 = plValues(evs_D_t ^ evs_B_t);
	evidence_0[evs_D_t] = "True";
	evidence_0[evs_B_t] = "False";
	

	plValues evidence_1 = plValues(evs_B_t);
	evidence_1[evs_B_t] = "False";


	std::vector<plValues> * evidenceVector = new std::vector<plValues>();
	evidenceVector->push_back(evidence_0);
	evidenceVector->push_back(plValues());
	evidenceVector->push_back(evidence_1);

	std::vector<plValues>  evidenceVector_dup;
	evidenceVector_dup.push_back(evidence_0);
	evidenceVector_dup.push_back(evidence_1);

	
	vector<plVariablesConjunction> nodes;
	vector<int> instant;



	int inst = 10;
	
	instant.push_back(9);
	instant.push_back(inst + 1);
	instant.push_back(13);
	//instant.push_back(200);
	nodes.push_back(target_variable);
	nodes.push_back(t1^t2);
	nodes.push_back(t2);
	//nodes.push_back(target_variable);


	//test des m�thodes de g�n�ration de reseaux bay�sien gloutonnent 
	/*
	plVariablesConjunction variables_gs = jd0_dup.get_variables();
	plDistribution distri_gs(variables_gs);
	plJointDistribution jd_gs(variables_gs, distri_gs);
	pmBayesianNetwork *BN_GS = new pmBayesianNetwork(jd_gs);
	G0_dup.generate_csvData("test_simple", 10);
	char * filename = "./test_simple.csv";
	size_t entete(1);
	size_t row(10);

	plCSVDataDescriptor *data = new plCSVDataDescriptor(filename, jd0_dup.get_variables());
	pmFrequencyCounter<rowDataType>* test = new pmFrequencyCounter<rowDataType>(data, jd0_dup.get_variables());
	pmAlgoGS<rowDataType> algo(test,BN_GS,NULL);
	*/
	


	//g�n�ration d'un csv

	pmDynamicBayesianNetwork *dBN2 = new pmDynamicBayesianNetwork(&G0_dup, &Gt_dup);

	dBN2->serializeDBN("DBN_example");
	pm15dBN * dBN15 = new pm15dBN(dBN2);
	//dBN15->generate_csv_samples_Data("test_sample", 7, 20,false,2);




	/*
	int time = clock();
	
	dBN2->setAlgorithm(0);
	vector<vector<plDistribution>>test=dBN2->prediction(nodes, instant, evidenceVector_dup);
	int time2 = clock() - time;
	dBN2->setAlgorithm(1);
	vector<vector<plDistribution>>test2 = dBN2->prediction(nodes, instant, evidenceVector_dup);
	int time3 = clock() - (time + time2);
	cout << time2 << endl;
	cout << time3 << endl;

	for (int i = 0; i < test.size(); i++){
		for (int j = 0; j < test[i].size(); j++){
			cout << test[i][j] << endl;
		}
	}
	cout<<"---------------------------------------" << endl;
	for (int i = 0; i < test2.size(); i++){
		for (int j = 0; j < test2[i].size(); j++){
			cout << test2[i][j] << endl;
		}
	}

	*/


	/*
	affichage des pr�dictions
	cout<<test.size()<<endl;
	for (int i = 0; i < test.size(); i++){
		for (int j = 0; j < test[i].size(); j++){
			cout << test[i][j] << endl;
		}
	}*/
	//plValues evidence;
	//vector<plValues> evs;
	//pmDynamicBayesianNetwork *dBN2 = new pmDynamicBayesianNetwork(&G0_dup, &Gt_dup);
	//pmBayesianNetwork* unrolled =dBN2->unroll(10);
	//plJointDistribution distri = unrolled->getJointDistribution();
	//nodes = nodes^distri.get_variables().get_variable_with_name("BBB_t5") ^ distri.get_variables().get_variable_with_name("BBB_t7") ^ distri.get_variables().get_variable_with_name("BBB_t10");
	////cout << distri.ask(nodes).compile()<<endl;
	//plVariable t5 = distri.get_variables().get_variable_with_name("BBB_t5");
	//plVariable t7 = distri.get_variables().get_variable_with_name("BBB_t7");
	//plVariable t10 = distri.get_variables().get_variable_with_name("BBB_t10");
	//int test = clock();
	//plDistribution temp=distri.ask(nodes);
	//plComputableObjectList temp2 = temp.get_factors();
	//cout << temp2[0].concatenate(temp2[1]) << endl << temp2[1] << endl << temp2[2];

	//int test2 = clock()-test;
	//cout<<distri.ask(t5).compile()<<endl;
	//cout << distri.ask(t7).compile() << endl;
	//cout << distri.ask(t10).compile() << endl;

	//int test3 = clock() - (test + test2);
	//cout << test2 << endl << test3 <<endl;
	//cout << G0_dup.query(G0_dup.getJointDistribution().get_variables().get_variable_with_name("CCC_t0")).compile() << endl;
	/*dBN2->serializeDBN("test_DBN.xml");
	pmDynamicBayesianNetwork dBN3 = dBN2->load_from_xml("test_DBN.xml");*/
	
	
	/*time_t start;
	time(&start);
	time_t check1;
	time(&check1);
	vector<vector<plDistribution>> res = dBN15->predict(nodes, instant);

	cout << "-----------------1.5 DBN-----------------" << endl;

	//cout << res[0][0].compile() << endl;
	//cout << "-------------- - " << endl;
	cout << res[0][0].compile() << endl;
	time_t check2;
	time(&check2);*/
	cout << "-----------------DBN-----------------" << endl;
	//pmBayesianNetwork *unrolled = dBN_dup.unroll2(3);
	//plDistribution testDBN = unrolled->query(unrolled->getVariables().get_variable_with_name("CCC_t10"));

	//cout << testDBN.compile() << endl;
	//cout << testDBN.compile() << endl;
	//time_t check3;
	//time(&check3);
	
	//test fonction create_test_values() de BN 
	/*vector <string> result = unrolled->create_test_values();
	for (int i = 0; i < unrolled->getVariables().get_names().size(); i++){
		cout << unrolled->getVariables().get_names()[i] << endl;
		cout << result[i] << endl;
	}*/


	/*plDistribution result_qo = dBN.query_15dbn(target_variable_dup, 2, evidenceVector_dup);


	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "le resultat obtenu: " << std::endl;
	result_qo.write_head(std::cout);
	result_qo.write_body(std::cout);
	std::cout << std::endl;


	plDistribution result_attended = dBN_dup.query2(target_variable, 2, evidenceVector);

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "le resultat attendu: " << std::endl;
	result_attended.write_head(std::cout);
	result_attended.write_body(std::cout);
	std::cout << std::endl;



	result_attended = dBN_dup.query2(target_variable, 4, evidenceVector);


	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "le resultat obtenu: " << std::endl;
	result_qo.write_head(std::cout);
	result_qo.write_body(std::cout);
	std::cout << std::endl;


	result_qo = dBN_dup.query_15dbn(target_variable_dup, 4, evidenceVector_dup);

	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "le resultat attendu: " << std::endl;
	result_qo.write_head(std::cout);
	result_qo.write_body(std::cout);
	std::cout << std::endl;*/

}
