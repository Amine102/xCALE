
#include "testDMMHC.h"



using namespace std;
using namespace PILGRIM;
using namespace boost;


void testDMMHC(){

	


	//test algo GS
	// Variable definitions
	pmDynamicBayesianNetwork *dBN;
	pmDynamicBayesianNetwork *dBN2=new pmDynamicBayesianNetwork();
	dBN= dBN2->load_from_xml("DBN_example");
	plJointDistribution jd0=dBN->getJointDistributionforG0();
	plJointDistribution jd_T = dBN->getJointDistributionforGtransition();


	vector<vector<vector<plVariable>>>test_clique1 = dBN->get_Linked_Vars();
	vector<vector<vector<plVariable>>>test_clique =dBN->get_max_clique(dBN->get_Linked_Vars());
	for (int j = 0; j < test_clique1.size(); j++) {
		cout << "-----clique " << j << "-----" << endl;
		for (int k = 0; k < test_clique1[j][0].size(); k++) {
			cout << test_clique1[j][0][k].name() << endl;
		}
		cout << "-----parents clique " << j << "-----" << endl;
		for (int k = 0; k < test_clique1[j][1].size(); k++) {
			cout << test_clique1[j][1][k].name() << endl;
		}
	}

		for (int j = 0; j < test_clique.size(); j++) {
			cout << "-----clique " << j <<"-----"<< endl;
			for (int k = 0; k < test_clique[j][0].size(); k++) {
				cout << test_clique[j][0][k].name() << endl;
			}
			cout << "-----parents clique " << j << "-----" << endl;
			for (int k = 0; k < test_clique[j][1].size(); k++) {
				cout << test_clique[j][1][k].name() << endl;
			}
		}
		dBN->summary();


	//dBN->generate_csv_data("test_simple_dbn",40,200,false,2);
	
	/*cout << "-------------------From DBN-------------------" << endl;
	pmDynamicBayesianNetwork learnedDBN("test_simple_dbn_T0.csv", "test_simple_dbn.csv");
	learnedDBN.summary();*/
	/*
	//Fichier csv G0
	size_t entete_T0(1);
	char* filenameDBN_T0 = "test_simple_dbn_T0.csv";
	plCSVDataDescriptor desc_T0(filenameDBN_T0, plVariablesConjunction(), false, false);
	plVariablesConjunction varCSV_T0 = desc_T0.guess_variables(filenameDBN_T0);
	size_t rowDBN_T0(varCSV_T0.size());
	pmCSVDataSet* dataDBN_T0 = new pmCSVDataSet(filenameDBN_T0, entete_T0, rowDBN_T0, ';', varCSV_T0);
	pmBayesianNetwork *empty_BNT0 = new pmBayesianNetwork(varCSV_T0);
	*/

	//fichier csv G_T
	/*size_t entete(1);
	char* filenameDBN = "repairExample_mieux.csv";
	plCSVDataDescriptor desc(filenameDBN, plVariablesConjunction(), false, false);
	plVariablesConjunction varCSV = desc.guess_variables(filenameDBN);
	cout<<varCSV.size()<<endl;
	size_t rowDBN(varCSV.size());
	pmCSVDataSet* dataDBN = new pmCSVDataSet(filenameDBN, entete, rowDBN, ';', varCSV);
	pmBayesianNetwork *empty_BNT = new pmBayesianNetwork(varCSV);*/
	
	
	/*cout << "-------------------Algo GS-------------------" << endl;
	//d�finition des variables utilis� dans la cr�ation de l'algo  GS
	char * filename = "test_simple_dbn_T0.csv";
	size_t entete(1);
	size_t row(jd0.get_variables().size());
	pmCSVDataSet* data = new pmCSVDataSet(filename, entete, row, ';', jd0.get_variables());
	const plVariablesConjunction vars = plVariablesConjunction(data->observed_variables());
	pmFrequencyCounter *FC=new pmFrequencyCounter(data->get_data_descriptor(), vars);
	
	//definition du score pour l'algo GS
	//pmScoreAIC<rowDataType> *score_but = new pmScoreAIC<rowDataType>(empty_BN0, FC);
	pmScoreBIC<rowDataType> *score_but = new pmScoreBIC<rowDataType>(empty_BN0, FC);
	//pmScoreMDL<rowDataType> *score_but = new pmScoreMDL<rowDataType>(empty_BN0, FC);
	//pmScoreBDeu<rowDataType> *score_but = new pmScoreBDeu<rowDataType>(empty_BN0, FC);
	//cr�ation du r�seau Bay�sien en utilisant l'algo GS
	pmAlgoGS<rowDataType> algo(FC, empty_BN0);
	algo.setScore(score_but);
	pmBayesianNetwork* G0=algo.computeBN();*/


	
	//d�finition des variables utilis� dans la cr�ation de l'algo  Dynamic GS
	

	cout << "-------------------Algo Dynamic GS-------------------" << endl;
	/*
	pmFrequencyCounter *FC_dbn = new pmFrequencyCounter(dataDBN->get_data_descriptor(), dataDBN->observed_variables());
	//definition du score pour l'algo Dynamic GS
	
	pmScoreAIC<rowDataType> *score_dbn = new pmScoreAIC<rowDataType>(empty_BNT, FC_dbn);
	//pmScoreBIC<rowDataType> *score_dbn = new pmScoreBIC<rowDataType>(empty_BNT, FC_dbn);
	//pmScoreMDL<rowDataType> *score_dbn = new pmScoreMDL<rowDataType>(empty_BNT, FC_dbn);
	//pmScoreBDeu<rowDataType> *score_dbn = new pmScoreBDeu<rowDataType>(empty_BNT, FC_dbn);

	//cr�ation du r�seau Bay�sien de transition en utilisant l'algo Dynamic GS
	
	pmAlgoDynamicGS<rowDataType> algoDynamic(FC_dbn, empty_BNT);
	algoDynamic.setScore(score_dbn);
	pmBayesianNetwork* testDGS = algoDynamic.computeBN();
	testDGS->summary();*/
	/*
	cout << "-------------------Algo MMHC-------------------" << endl;
	
	string score_name_T0 = "BDeu";
	AlgoMMHC construct_T0=AlgoMMHC(dataDBN_T0);
	construct_T0.setScoreName(score_name_T0);
	pmBayesianNetwork *G0 = construct_T0.createMMHCGraph(-1, empty_BNT0);
	pmCSVDataDescriptor* pmDD_T0 = new pmCSVDataDescriptor(filenameDBN_T0, G0->getVariables());
	G0->learnParametersML(pmDD_T0);
	G0->summary();
	*/
	cout << "-------------------Algo DMMHC-------------------" << endl;
	//test algo DMMHC
	
	/*string score_name = "BIC";
	DMMHC * construct= new DMMHC(dataDBN);
	construct->setScoreName(score_name);
	pmBayesianNetwork *G_t = construct->createDMMHCGraph(-1, empty_BNT,"test");

	pmCSVDataDescriptor* pmDD = new pmCSVDataDescriptor(filenameDBN, G_t->getVariables());
	G_t->learnParametersML(pmDD);
	G_t->summary();*/
	/*
	pmDynamicBayesianNetwork resultat(G0, G_t);
	resultat.summary();
	dBN->summary();
	*/

	/*//post-traitement
	plVariablesConjunction PosttempVar= G_t->getJointDistribution().get_variables();
	plVariablesConjunction PostfinalVar;
	plComputableObjectList PosttempCO = G_t->getJointDistribution().get_computable_object_list();
	plComputableObjectList PostfinalCO;
	string name;


	for (int i = 0; i < PosttempVar.size(); i++) {
		name= PosttempCO[i].get_left_variables().name();
		if (name.substr(name.size() - 1, 1) != "2") {
			PostfinalVar ^= PosttempVar.get_variable_with_name(name);
			PostfinalCO.push_back(PosttempCO[i]);
		}

	}
	
	plJointDistribution finalDistriTransition(PostfinalVar, PostfinalCO);
	pmBayesianNetwork* finalG_T=new pmBayesianNetwork(finalDistriTransition);
	//fin post-traitement*/

	//affichage de G_T
	

	//pmDynamicBayesianNetwork result(G0, finalG_T);
	//result.summary();




}