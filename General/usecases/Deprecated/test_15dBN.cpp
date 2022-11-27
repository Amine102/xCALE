#include "test_15dBN.h"

using namespace PILGRIM;
using namespace boost;
using namespace std;


void test_15dBN(){


	/* plJointDistribution jd0;
	jd0.load_from_genie("benchmarks/networks/test_query_optimized_t0.xdsl");

	plJointDistribution jd0_dup = plJointDistribution(jd0);

	pmBayesianNetwork G0(jd0);
	pmBayesianNetwork G0_dup(jd0_dup);

	plJointDistribution jdt;
	jdt.load_from_genie("benchmarks/networks/test_query_optimized_t.xdsl");
	plJointDistribution jdt_dup = plJointDistribution(jdt);
	pmBayesianNetwork Gt(jdt);
	pmBayesianNetwork Gt_dup(jdt_dup);
	pmDynamicBayesianNetwork dBN = pmDynamicBayesianNetwork(&G0, &Gt);
	pmDynamicBayesianNetwork dBN_dup = pmDynamicBayesianNetwork(&G0_dup, &Gt_dup);
	//*
	std::cout << "Dynamic Bayesian Network" << std::endl;
	std::cout << "------------------------" << std::endl;
	dBN.summary();
	std::cout << std::endl;
	std::cout << std::endl;
	*/


	/* std::cout << "1.5 Dynamic Bayesian Network" << std::endl;
	std::cout << "----------------------------" << std::endl;
	pm15dBN dbn_15(dBN);

	dbn_15.summary_1_5();

	std::cout << "Query 1.5 DBN" << std::endl;
	std::cout << "----------------------------" << std::endl;

	plVariablesConjunction vars = jdt.get_variables();

	plVariable target_variable = vars.get_variable_with_name("C");

	plVariable evs_A = vars.get_variable_with_name("A");
	plVariable evs_B = plVariable("B", evs_A.get_type());
	plVariable evs_C = vars.get_variable_with_name("C");
	plVariable evs_D = vars.get_variable_with_name("D");

	plValues evidence_vide;

	plValues evidence_0 = plValues(evs_D ^ evs_B);
	evidence_0[evs_D] = "True";
	evidence_0[evs_B] = "False";


	plValues evidence_2 = plValues(evs_B);
	evidence_2[evs_B] = "False";


	std::vector<plValues> * evidenceVector = new std::vector<plValues>();
	evidenceVector->push_back(evidence_0);
	evidenceVector->push_back(evidence_2);

	std::vector<plValues> * evidenceVector_dup = new std::vector<plValues>();
	evidenceVector_dup->push_back(evidence_0);
	evidenceVector_dup->push_back(evidence_2);


	plDistribution result_qo = dbn_15.query(target_variable,  evidenceVector,3);
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << "le resultat obtenu: " << std::endl;
	result_qo.write_head(std::cout);
	result_qo.write_body(std::cout);
	std::cout << std::endl;


	std::cout << "Serialization of 1.5 Dynamic Bayesian Network" << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
	dbn_15.serialize("benchmarks/networks/serialized_1_5_dbn.xml","1_5_jd","OI_id");

	std::cout << "Unserialization of 1.5 Dynamic Bayesian Network" << std::endl;
	std::cout << "---------------------------------------------" << std::endl;
	pm15dBN unserialized_15dBN=pm15dBN::unserialize("benchmarks/networks/serialized_1_5_dbn.xml","1_5_jd","OI_id");
	//pm15dBN dbn_15_unserialized(unserialized_15dBN);
	unserialized_15dBN.summary_1_5();

	}

	void test_update_15dBN(){


	plJointDistribution jd0;
	jd0.load_from_genie("benchmarks/networks/test_query_optimized_t0.xdsl");

	plJointDistribution jd0_dup = plJointDistribution(jd0);

	pmBayesianNetwork G0(jd0);
	pmBayesianNetwork G0_dup(jd0_dup);

	plJointDistribution jdt;
	jdt.load_from_genie("benchmarks/networks/test_query_optimized_t.xdsl");
	plJointDistribution jdt_dup = plJointDistribution(jdt);
	pmBayesianNetwork Gt(jdt);
	pmDynamicBayesianNetwork dBN = pmDynamicBayesianNetwork(&G0, &Gt);
	//*
	std::cout << "Dynamic Bayesian Network" << std::endl;
	std::cout << "------------------------" << std::endl;
	dBN.summary();
	std::cout << std::endl;
	std::cout << std::endl;
	//*/


	/*std::cout << "1.5 Dynamic Bayesian Network" << std::endl;
	std::cout << "----------------------------" << std::endl;
	pm15dBN dbn_15(dBN);

	dbn_15.summary_1_5();

	std::cout << "Update 1.5 DBN" << std::endl;
	std::cout << "----------------------------" << std::endl;

	plVariablesConjunction vars = jdt.get_variables();

	plVariable target_variable = vars.get_variable_with_name("C");

	plVariable evs_A = vars.get_variable_with_name("A");
	plVariable evs_B = plVariable("B", evs_A.get_type());
	plVariable evs_C = vars.get_variable_with_name("C");
	plVariable evs_D = vars.get_variable_with_name("D");

	plValues evidence_vide;

	plValues evidence_0 = plValues(evs_D ^ evs_B);
	evidence_0[evs_D] = "True";
	evidence_0[evs_B] = "False";


	plValues evidence_2 = plValues(evs_B);
	evidence_2[evs_B] = "False";


	std::vector<plValues> * evidenceVector = new std::vector<plValues>();
	evidenceVector->push_back(evidence_0);
	//evidenceVector->push_back(evidence_2);


	dbn_15.update(evidenceVector);
	dbn_15.summary_1_5();

	}

	void test_predict_15dBN(){
	/*
	plJointDistribution jd0;
	jd0.load_from_genie("benchmarks/networks/test_query_optimized_t0.xdsl");

	plJointDistribution jd0_dup = plJointDistribution(jd0);

	pmBayesianNetwork G0(jd0);

	plJointDistribution jdt;
	jdt.load_from_genie("benchmarks/networks/test_query_optimized_t.xdsl");
	plJointDistribution jdt_dup = plJointDistribution(jdt);
	pmBayesianNetwork Gt(jdt);
	pmDynamicBayesianNetwork dBN = pmDynamicBayesianNetwork(&G0, &Gt);
	//*
	std::cout << "Dynamic Bayesian Network" << std::endl;
	std::cout << "------------------------" << std::endl;
	dBN.summary();
	std::cout << std::endl;
	std::cout << std::endl;
	//*/
	/*std::cout << "1.5 Dynamic Bayesian Network" << std::endl;
	std::cout << "----------------------------" << std::endl;
	pm15dBN dbn_15(dBN);
	dbn_15.summary_1_5();

	plVariablesConjunction vars = jdt.get_variables();
	plVariable target_variable = vars.get_variable_with_name("C");

	plVariable evs_A = vars.get_variable_with_name("A");
	plVariable evs_B = plVariable("B", evs_A.get_type());
	plVariable evs_C = vars.get_variable_with_name("C");
	plVariable evs_D = vars.get_variable_with_name("D");


	std::cout << "Predict 1.5 Dynamic Bayesian Network" << std::endl;
	std::cout << "---------------------------------------------" << std::endl;

	plVariable s_A = vars.get_variable_with_name("A");
	plVariable s_C = vars.get_variable_with_name("C");
	plVariable s_D = vars.get_variable_with_name("D");
	plVariable s_B = plVariable("B", evs_A.get_type());
	vector<plVariable> predicted_nodes;
	predicted_nodes.push_back(s_A);
	predicted_nodes.push_back(s_B);
	predicted_nodes.push_back(s_C);
	predicted_nodes.push_back(s_D);
	vector<int> instants;
	instants.push_back(1);
	instants.push_back(2);
	instants.push_back(3);
	//instants.push_back(4);
	instants.push_back(5);
	//instants.push_back(6);

	vector<vector<plDistribution> > result_prediction;
	result_prediction=dbn_15.predict(predicted_nodes,instants);

	for(int i=0;i<result_prediction.size();i++)
	{
	cout<<"\n == instant "<< i<<" ==\n";
	for(int j=0;j<result_prediction[i].size();j++)
	{
	result_prediction[i][j].write_head(cout);
	result_prediction[i][j].write_body(cout);
	}

	}*/


	//Type definitions
	std::vector<std::string> etats;
	etats.push_back("OK");
	etats.push_back("KO");
	plLabelType Etat_TYPE(etats);

	std::vector<std::string> users;
	users.push_back("Loc");
	users.push_back("Prop");
	plLabelType User_TYPE(users);

	std::vector<std::string> reparations;
	reparations.push_back("Non");
	reparations.push_back("Leger");
	reparations.push_back("Complet");
	plLabelType Reparation_TYPE(reparations);

	plDiscreteIntervalType Cout_TYPE(0, 11000, 10);

	//First BAYSIEN NETWORK 

	// Variable definitions
	plVariable Etat0("Etat_t0", Etat_TYPE);
	plVariable User0("User_t0", User_TYPE);
	plVariable Reparation0("Reparation_t0", Reparation_TYPE);
	plVariable Cout0("Cout_t0", Cout_TYPE);

	// Probability table of Cout
	plDistributionTable P_coutGivenRep0(Cout0, Reparation0);
	P_coutGivenRep0.push(plDeterministic(Cout0, plValues().add(Cout0, 0)), 0);
	P_coutGivenRep0.push(plDeterministic(Cout0, plValues().add(Cout0, 2000)), 1);
	P_coutGivenRep0.push(plDeterministic(Cout0, plValues().add(Cout0, 8000)), 2);

	// Probability table of Etat
	plProbValue tableEtat[] = { 0.99, 0.01 };
	plProbTable P_Etat0(Etat0, tableEtat);

	// Probability table of User
	plProbValue tableUser[] = { 0.5, 0.5 };
	plProbTable P_User0(User0, tableUser);

	// Probability table of Reparation | Etat, User
	plProbValue tableReparation[] = {
		1, 0, 0,
		0.1, 0.8, 0.1,
		1, 0, 0,
		0, 0.1, 0.9,
	};
	plDistributionTable P_Reparation0(Reparation0, User0^Etat0, tableReparation);

	// Joint distribution
	plJointDistribution jd0(User0^Etat0^Reparation0, P_User0*P_Etat0*P_Reparation0);

	// Creation and print of the bayesian network
	pmBayesianNetwork G0(jd0);
	//G0.summary();

	//Second BAYSIEN NETWORK

	// Variable definitions
	plVariable Etat_t0("Etat", Etat_TYPE);
	plVariable Etat_t("Etat_t", Etat_TYPE);
	plVariable User_t0("User", User_TYPE);
	plVariable User_t("User_t", User_TYPE);
	plVariable Reparation_t0("Reparation", Reparation_TYPE);
	plVariable Reparation_t("Reparation_t", Reparation_TYPE);
	//plVariable Cout_t0("Cout", Cout_TYPE);
	// plVariable Cout_t("Cout_t", Cout_TYPE);

	// Probability table of Cout
	/* plDistributionTable P_coutGivenRep_t0(Cout_t0, Reparation_t0);
	P_coutGivenRep_t0.push( plDeterministic(Cout_t0, plValues().add(Cout_t0,0)), 0);
	P_coutGivenRep_t0.push( plDeterministic(Cout_t0, plValues().add(Cout_t0,2000)), 1);
	P_coutGivenRep_t0.push( plDeterministic(Cout_t0, plValues().add(Cout_t0,8000)), 2);

	plDistributionTable P_coutGivenRep_t(Cout_t, Reparation_t);
	P_coutGivenRep_t.push( plDeterministic(Cout_t, plValues().add(Cout_t,0)), 0);
	P_coutGivenRep_t.push( plDeterministic(Cout_t, plValues().add(Cout_t,2000)), 1);
	P_coutGivenRep_t.push( plDeterministic(Cout_t, plValues().add(Cout_t,8000)), 2);*/

	// Probability table of Etat
	plProbValue tableEtat_t0[] = { 0.99, 0.01 };
	plProbTable P_Etat_t0(Etat_t0, tableEtat_t0);

	// Probability table of User
	plProbValue tableUser_t0[] = { 0.5, 0.5 };
	plProbTable P_User_t0(User_t0, tableUser_t0);

	// Probability table of Reparation_t0 which depends on Etat_t0, User_t0
	plProbValue tableReparation_t0[] = {
		0.1, 0.8, 0.1,
	};
	//plDistributionTable P_Reparation_t0(Reparation_t0, User_t0^Etat_t0, tableReparation_t0);
	plProbTable P_Reparation_t0(Reparation_t0, tableReparation_t0);
	// Probability table of Etat_t, which depends on Etat_t0, Reparation_t0
	plProbValue tableEtat_t[] = {
		0.9, 0.1,
		0.9, 0.1,
		0.9, 0.1,
		0, 1,
		0.8, 0.2,
		1, 0,
	};
	plDistributionTable P_Etat_t(Etat_t, Etat_t0^Reparation_t0, tableEtat_t);
	//plProbTable P_Etat_t(Etat_t, tableEtat_t);
	// Probability table of User_t
	plProbValue tableUser_t[] = { 0.5, 0.5 };
	plProbTable P_User_t(User_t, tableUser_t);

	// Probability table of Reparation_t, which depends on Etat_t, User_t
	plProbValue tableReparation_t[] = {
		1, 0, 0,
		0.1, 0.8, 0.1,
		1, 0, 0,
		0, 0.1, 0.9,
	};
	plDistributionTable P_Reparation_t(Reparation_t, User_t^Etat_t, tableReparation_t);
	//plProbTable P_Reparation_t(Reparation_t, tableReparation_t);
	// Joint distribution
	plJointDistribution
		jdt(User_t0^Etat_t0^Reparation_t0/*^Cout_t0*/^User_t^Etat_t^Reparation_t/*^Cout_t*/,
		P_User_t0*P_Etat_t0*P_Reparation_t0*/*P_coutGivenRep_t0**/P_User_t*P_Etat_t*P_Reparation_t/**P_coutGivenRep_t*/);

	// Creation and print of the bayesian network
	pmBayesianNetwork Gt(jdt);

	// Creation Dynamic BN
	pmDynamicBayesianNetwork dBN = pmDynamicBayesianNetwork(&G0, &Gt);
	pmDynamicBayesianNetwork *dBN2 = new pmDynamicBayesianNetwork(&G0, &Gt);
	std::cout << "Dynamic Bayesian Network" << std::endl;
	std::cout << "------------------------" << std::endl;
	//dBN.summary();
	pm15dBN * dBN15 = new pm15dBN(dBN2);
	vector<plVariable> nodes;
	vector<int> instant;
	plVariablesConjunction Variables;
	Variables = dBN15->getJointDistribution().get_variables();
	plVariable evidenceUser = Variables.get_variable_with_name("User_t");
	plVariable evidenceEtat = Variables.get_variable_with_name("Etat_t");
	plVariable evidenceRep = Variables.get_variable_with_name("Reparation_t");
	plVariable target = Variables.get_variable_with_name("Etat_t");
	nodes.push_back(target);
	/*plValues evidenceU(evidenceUser);
	plValues evidenceE(evidenceEtat);
	evidenceU[evidenceUser] = "Prop";
	evidenceU[evidenceEtat] = "OK";
	//
	//evidences->push_back(evidenceE);
	evidences->push_back(evidenceU);*/
	plValues evidence;
	plValues evidences;
	//evidence=evidence.add(Variables.get_variable_with_name("User"), 0);
	evidence = evidence.add(evidenceUser, 1);
	evidence = evidence.add(evidenceEtat, 0);
	//evidence=evidence.add(evidenceRep, 0);
	nodes.push_back(evidenceEtat);
	//instant.push_back(2);

	instant.push_back(6);
	instant.push_back(10);
	//instant.push_back(2000);


	//dBN15->print_graph();

	//dBN15->querySimple(target,evidences);
	//cout << "query: " << difftime(check2, check1) << endl;
	//dBN15->update(evidence);
	//dBN15->update3(evidences);
	std::cout << "-------------------------Sample test -----------------" << std::endl;

	//dBN15->update(evidence);
	/*time_t start;
	time(&start);
	time_t check1;
	time(&check1);
	vector<vector<plDistribution>> res = dBN15->predict(nodes, instant);
	cout << difftime(check1, start) << endl;
	time_t check2;
	time(&check2);
	*/
	//cout << difftime(check2, check1) << endl;
	//testDBNOptimizedQuery();
	/*pmBayesianNetwork *unrolled = dBN.unroll2(4);
	vector <string> result = unrolled->create_test_values();
	for (int i = 0; i < unrolled->getVariables().get_names().size(); i++){
		cout << unrolled->getVariables().get_names()[i] << endl;
		cout << result[i] << endl;
	}*/
}
