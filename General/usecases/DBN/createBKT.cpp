#include "createBKT.h"

void createAndQueryBKT() {
    /*pmDynamicBayesianNetwork* DBN = createBKT();
  
    DBN->summary(true);*/
    
    /*DBN->generate_csv_data("../../benchmarks/data/bkt_data", 10, 1000, false, 2);
    DBN->save_as_xml("../../benchmarks/networks/BKT.xml", "jd0", "jdT");*/

    //using unroll function to display all the DBNs till time slice 5
    //DBN->unroll(8)->summary(true);
    //DBN->summary(true);

    //queryBKT(DBN);
    /*queryBKTS1(DBN);
    queryBKTS2(DBN);
    queryBKTS3(DBN);*/
}

pmDynamicBayesianNetwork* createBKT() {
    // A = nombres, B = calcul littéral, C = démarche scientifique
	//User-defined parameters
	double PA_INITIAL = 0.05;
    double PB_INITIAL = 0.02;
    double PC_INITIAL = 0.01;

    // All P_GUESS are the same
    double P_GUESS = 0.1;
    
    // All P_SLIP are the same
    double P_SLIP = 0.05;

    // All P_LEARN are the same
    double P_LEARN = 0.1;

    // All P_forget are 0 for now
	double PA_FORGET = 0;
    double PB_FORGET = 0;
    double PC_FORGET = 0;

    std::vector<std::string> niveau_maitrise;
    niveau_maitrise.emplace_back("1 - non acquis");
    niveau_maitrise.emplace_back("2 - acquis");

    plVariable A_t0("Skill_nombres_t0", plLabelType(niveau_maitrise));
    plVariable B_t0("Skill_calcul_litteral_t0", plLabelType(niveau_maitrise));
    plVariable C_t0("Skill_demarche_scientifique_t0", plLabelType(niveau_maitrise));
    plVariable ResA_t0("Res_nombres_t0", plLabelType(niveau_maitrise));
    plVariable ResB_t0("Res_calcul_litteral_t0", plLabelType(niveau_maitrise));
    plVariable ResC_t0("Res_demarche_scientifique_t0", plLabelType(niveau_maitrise));

    plProbValue tableA_t0[] = {1 - PA_INITIAL, PA_INITIAL};
    plProbTable P_A_t0(A_t0, tableA_t0);

    plProbValue tableB_t0[] = {1 - PB_INITIAL, PB_INITIAL};
    plProbTable P_B_t0(B_t0, tableB_t0);

    plProbValue tableC_t0[] = {1 - PC_INITIAL, PC_INITIAL};
    plProbTable P_C_t0(C_t0, tableC_t0);

    plProbValue tableResA_t0_knowingA_t0[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };

    plDistributionTable P_ResA_t0(ResA_t0, A_t0, tableResA_t0_knowingA_t0);

    plProbValue tableResB_t0_knowingB_t0[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResB_t0(ResB_t0, B_t0, tableResB_t0_knowingB_t0);

    plProbValue tableResC_t0_knowingC_t0[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResC_t0(ResC_t0, C_t0, tableResC_t0_knowingC_t0);

    // Joint distribution
    plJointDistribution jd0(A_t0   ^ B_t0   ^ C_t0   ^ ResA_t0   ^ ResB_t0   ^ ResC_t0, 
                            P_A_t0 * P_B_t0 * P_C_t0 * P_ResA_t0 * P_ResB_t0 * P_ResC_t0);


    //Transition Bayesian Network 

    plVariable A("Skill_nombres", plLabelType(niveau_maitrise));
    plVariable B("Skill_calcul_litteral", plLabelType(niveau_maitrise));
    plVariable C("Skill_demarche_scientifique", plLabelType(niveau_maitrise));
    plVariable ResA("Res_nombres", plLabelType(niveau_maitrise));
    plVariable ResB("Res_calcul_litteral", plLabelType(niveau_maitrise));
    plVariable ResC("Res_demarche_scientifique", plLabelType(niveau_maitrise));

    plVariable A_t("Skill_nombres_t", plLabelType(niveau_maitrise));
    plVariable B_t("Skill_calcul_litteral_t", plLabelType(niveau_maitrise));
    plVariable C_t("Skill_demarche_scientifique_t", plLabelType(niveau_maitrise));
    plVariable ResA_t("Res_nombres_t", plLabelType(niveau_maitrise));
    plVariable ResB_t("Res_calcul_litteral_t", plLabelType(niveau_maitrise));
    plVariable ResC_t("Res_demarche_scientifique_t", plLabelType(niveau_maitrise));
    
    plProbValue tableA[] = {1 - PA_INITIAL, PA_INITIAL};
    plProbTable P_A(A, tableA);

    plProbValue tableB[] = {1 - PB_INITIAL, PB_INITIAL};
    plProbTable P_B(B, tableB);

    plProbValue tableC[] = {1 - PC_INITIAL, PC_INITIAL};
    plProbTable P_C(C, tableC);

    plProbValue tableResA_knowingA[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResA(ResA, A, tableResA_knowingA);

    plProbValue tableResB_knowingB[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResB(ResB, B, tableResB_knowingB);

    plProbValue tableResC_knowingC[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResC(ResC, C, tableResC_knowingC);

    // Probability table of table A_t
    plProbValue tableA_t_knowingA[] = {
        1 - P_LEARN,       P_LEARN,
          PA_FORGET, 1 - PA_FORGET
    };
    plDistributionTable P_A_t(A_t, A, tableA_t_knowingA);

        // Probability table of table B_t
    plProbValue tableB_t_knowingB[] = {
        1 - P_LEARN,       P_LEARN,
          PB_FORGET, 1 - PB_FORGET
    };
    plDistributionTable P_B_t(B_t, B, tableB_t_knowingB);

        // Probability table of table C_t
    plProbValue tableC_t_knowingC[] = {
        1 - P_LEARN,       P_LEARN,
          PC_FORGET, 1 - PC_FORGET
    };
    plDistributionTable P_C_t(C_t, C, tableC_t_knowingC);

    plProbValue tableResA_t_knowingA_t[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResA_t(ResA_t, A_t, tableResA_t_knowingA_t);

    plProbValue tableResB_t_knowingB_t[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResB_t(ResB_t, B_t, tableResB_t_knowingB_t);

    plProbValue tableResC_t_knowingC_t[] = {
        1 - P_GUESS,     P_GUESS,
             P_SLIP, 1 - P_SLIP
    };
    plDistributionTable P_ResC_t(ResC_t, C_t, tableResC_t_knowingC_t);

    // Joint distribution
    plJointDistribution jdt(A     ^ B     ^ C     ^ ResA     ^ ResB     ^ ResC   ^ 
                            A_t   ^ B_t   ^ C_t   ^ ResA_t   ^ ResB_t   ^ ResC_t , 
                            P_A   * P_B   * P_C   * P_ResA   * P_ResB   * P_ResC *
                            P_A_t * P_B_t * P_C_t * P_ResA_t * P_ResB_t * P_ResC_t);

    //Creation and printing of the bayesian network
    pmBayesianNetwork* BN_0 = new pmBayesianNetwork(jd0);
    pmBayesianNetwork* BN_Transition = new pmBayesianNetwork(jdt);
    pmDynamicBayesianNetwork* DBN = new pmDynamicBayesianNetwork(BN_0, BN_Transition);

    return DBN;
}


void queryBKT(pmDynamicBayesianNetwork* DBN) {
    /*
        Exercice                    | C1 | C2 | C3 
        ----------------------------+----+----+-----
        Calcul avec des fractions   | X  |    |    
        Évolution d'une proportion  | X  |    |    
        Proportionnalité            | X  |    |    
        Vente de livres             | X  |    | X   
        Conjecture                  | X  | X  |     
        Programmes de calcul        | X  | X  |     
        Devinette Mathématique      |    | X  | X   
        Récupérateur d'eau de pluie |    |    | X   
    */

    const unsigned int nb_slices = 8;
    std::vector<std::string> niveau_maitrise;

    niveau_maitrise.emplace_back("1 - non acquis");
    niveau_maitrise.emplace_back("2 - acquis");

    plVariablesConjunction variables_t0 = DBN->getInitial_G().getVariables();
    plVariable ResA_t0 = variables_t0.get_variable_with_name("Res_nombres_t0");
    plVariable ResB_t0 = variables_t0.get_variable_with_name("Res_calcul_litteral_t0");
    plVariable ResC_t0 = variables_t0.get_variable_with_name("Res_demarche_scientifique_t0");


    plVariablesConjunction variables_t = DBN->getTransition_G().getVariables();
    plVariable ResA_t = variables_t.get_variable_with_name("Res_nombres_t");
    plVariable ResB_t = variables_t.get_variable_with_name("Res_calcul_litteral_t");
    plVariable ResC_t = variables_t.get_variable_with_name("Res_demarche_scientifique_t");
    
    plVariable A_t = variables_t.get_variable_with_name("Skill_nombres_t");
    plVariable B_t = variables_t.get_variable_with_name("Skill_calcul_litteral_t");
    plVariable C_t = variables_t.get_variable_with_name("Skill_demarche_scientifique_t");

    plValues evidences_t0(ResA_t0 ^ ResB_t0 ^ ResC_t0);
    /*evidences_t0[ResA_t0] = niveau_maitrise[0];
    evidences_t0[ResB_t0] = niveau_maitrise[0];
    evidences_t0[ResC_t0] = niveau_maitrise[0];*/
    getEvidences(evidences_t0, niveau_maitrise, std::vector<unsigned int>{0, 0, 0});

    plValues evidences_t1(ResA_t ^ ResB_t ^ ResC_t);
    getEvidences(evidences_t1, niveau_maitrise, std::vector<unsigned int>{0, 0, 0});

    plValues evidences_t2(ResA_t ^ ResB_t ^ ResC_t);
    getEvidences(evidences_t2, niveau_maitrise, std::vector<unsigned int>{2, 0, 0});
    
    plValues evidences_t3(ResA_t ^ ResB_t ^ ResC_t);
    getEvidences(evidences_t3, niveau_maitrise, std::vector<unsigned int>{2, 0, 0});

    plValues evidences_t4(ResA_t ^ ResB_t ^ ResC_t);
    getEvidences(evidences_t4, niveau_maitrise, std::vector<unsigned int>{3, 0, 0});

    plValues evidences_t5(ResA_t ^ ResB_t ^ ResC_t);
    getEvidences(evidences_t5, niveau_maitrise, std::vector<unsigned int>{3, 1, 0});

    plValues evidences_t6(ResA_t ^ ResB_t ^ ResC_t);
    getEvidences(evidences_t6, niveau_maitrise, std::vector<unsigned int>{3, 2, 1});

    plValues evidences_t7(ResA_t ^ ResB_t ^ ResC_t);
    getEvidences(evidences_t7, niveau_maitrise, std::vector<unsigned int>{3, 2, 1});


    std::vector<plValues> all_evidences;
    //all_evidences.push_back(evidences_t0);
    all_evidences.push_back(evidences_t1);
    all_evidences.push_back(evidences_t2);
    all_evidences.push_back(evidences_t3);
    all_evidences.push_back(evidences_t4);
    all_evidences.push_back(evidences_t5);
    all_evidences.push_back(evidences_t6);
    all_evidences.push_back(evidences_t7);

    // 8 = nb slices
    std::vector<int> instants(nb_slices - 1);
    // Fill the vector with incrementally augmenting values
    std::iota(std::begin(instants), std::end(instants), 1);

    std::vector<plVariablesConjunction> all_targets;

    for (size_t i = 0; i < nb_slices - 1; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(A_t);
        new_target.push_back(B_t);
        new_target.push_back(C_t);
        all_targets.push_back(new_target);
    }

    pm15dBN* interfaceDBN = new pm15dBN(DBN, evidences_t0);

    std::vector<plComputableObjectList> results = interfaceDBN->predict(all_targets, instants, all_evidences);

    printBKTResult(results, instants, all_targets);
}

void queryBKTS1(pmDynamicBayesianNetwork* DBN) {
     /*
        Exercice                    | C1 | C2 | C3 
        ----------------------------+----+----+-----
        Calcul avec des fractions   | X  |    |    
        Évolution d'une proportion  | X  |    |    
        Proportionnalité            | X  |    |    
        Vente de livres             | X  |    | X   
        Conjecture                  | X  | X  |     
        Programmes de calcul        | X  | X  |     
        Devinette Mathématique      |    | X  | X   
        Récupérateur d'eau de pluie |    |    | X   
    */

    unsigned int nb_slices = 6;
    std::vector<std::string> niveau_maitrise;

    niveau_maitrise.emplace_back("1 - non acquis");
    niveau_maitrise.emplace_back("2 - acquis");

    plVariablesConjunction variables_t0 = DBN->getInitial_G().getVariables();
    plVariable ResA_t0 = variables_t0.get_variable_with_name("Res_nombres_t0");
    plVariable ResB_t0 = variables_t0.get_variable_with_name("Res_calcul_litteral_t0");
    plVariable ResC_t0 = variables_t0.get_variable_with_name("Res_demarche_scientifique_t0");


    plVariablesConjunction variables_t = DBN->getTransition_G().getVariables();
    plVariable ResA_t = variables_t.get_variable_with_name("Res_nombres_t");
    plVariable ResB_t = variables_t.get_variable_with_name("Res_calcul_litteral_t");
    plVariable ResC_t = variables_t.get_variable_with_name("Res_demarche_scientifique_t");
    
    plVariable A_t = variables_t.get_variable_with_name("Skill_nombres_t");
    plVariable B_t = variables_t.get_variable_with_name("Skill_calcul_litteral_t");
    plVariable C_t = variables_t.get_variable_with_name("Skill_demarche_scientifique_t");

    plValues evidences_t0A(ResA_t0);
    getEvidences(evidences_t0A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t1A(ResA_t);
    getEvidences(evidences_t1A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t2A(ResA_t);
    getEvidences(evidences_t2A, niveau_maitrise, std::vector<unsigned int>{0});
    
    plValues evidences_t3A(ResA_t);
    getEvidences(evidences_t3A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t4A(ResA_t);
    getEvidences(evidences_t4A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t5A(ResA_t);
    getEvidences(evidences_t5A, niveau_maitrise, std::vector<unsigned int>{0});


    std::vector<plValues> all_evidences;
    all_evidences.push_back(evidences_t0A);
    all_evidences.push_back(evidences_t1A);
    all_evidences.push_back(evidences_t2A);
    all_evidences.push_back(evidences_t3A);
    all_evidences.push_back(evidences_t4A);
    all_evidences.push_back(evidences_t5A);

    // 8 = nb slices
    std::vector<int> instants(nb_slices);
    // Fill the vector with incrementally augmenting values
    std::iota(std::begin(instants), std::end(instants), 0);

    std::vector<plVariablesConjunction> all_targets;

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(A_t);
        all_targets.push_back(new_target);
    }

    std::vector<plComputableObjectList> results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL A: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);

    // Skill B
    nb_slices = 3;
    all_evidences.clear();
    instants.clear();
    all_targets.clear();
    results.clear();

    plValues evidences_t0B(ResB_t0);
    getEvidences(evidences_t0B, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t1B(ResB_t);
    getEvidences(evidences_t1B, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t2B(ResB_t);
    getEvidences(evidences_t2B, niveau_maitrise, std::vector<unsigned int>{1});


    all_evidences.push_back(evidences_t0B);
    all_evidences.push_back(evidences_t1B);
    all_evidences.push_back(evidences_t2B);

    instants.resize(nb_slices);
    std::iota(std::begin(instants), std::end(instants), 0);

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(B_t);
        all_targets.push_back(new_target);
    }

    results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL B: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);

    // Skill C
    nb_slices = 3;
    all_evidences.clear();
    instants.clear();
    all_targets.clear();
    results.clear();

    plValues evidences_t0C(ResC_t0);
    getEvidences(evidences_t0C, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t1C(ResC_t);
    getEvidences(evidences_t1C, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t2C(ResC_t);
    getEvidences(evidences_t2C, niveau_maitrise, std::vector<unsigned int>{0});

    all_evidences.push_back(evidences_t0C);
    all_evidences.push_back(evidences_t1C);
    all_evidences.push_back(evidences_t2C);

    instants.resize(nb_slices);
    std::iota(std::begin(instants), std::end(instants), 0);

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(C_t);
        all_targets.push_back(new_target);
    }

    results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL C: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);
}

void queryBKTS2(pmDynamicBayesianNetwork* DBN) {
     /*
        Exercice                    | C1 | C2 | C3 
        ----------------------------+----+----+-----
        Calcul avec des fractions   | X  |    |    
        Évolution d'une proportion  | X  |    |    
        Proportionnalité            | X  |    |    
        Vente de livres             | X  |    | X   
        Conjecture                  | X  | X  |     
        Programmes de calcul        | X  | X  |     
        Devinette Mathématique      |    | X  | X   
        Récupérateur d'eau de pluie |    |    | X   
    */

    unsigned int nb_slices = 6;
    std::vector<std::string> niveau_maitrise;

    niveau_maitrise.emplace_back("1 - non acquis");
    niveau_maitrise.emplace_back("2 - acquis");

    plVariablesConjunction variables_t0 = DBN->getInitial_G().getVariables();
    plVariable ResA_t0 = variables_t0.get_variable_with_name("Res_nombres_t0");
    plVariable ResB_t0 = variables_t0.get_variable_with_name("Res_calcul_litteral_t0");
    plVariable ResC_t0 = variables_t0.get_variable_with_name("Res_demarche_scientifique_t0");


    plVariablesConjunction variables_t = DBN->getTransition_G().getVariables();
    plVariable ResA_t = variables_t.get_variable_with_name("Res_nombres_t");
    plVariable ResB_t = variables_t.get_variable_with_name("Res_calcul_litteral_t");
    plVariable ResC_t = variables_t.get_variable_with_name("Res_demarche_scientifique_t");
    
    plVariable A_t = variables_t.get_variable_with_name("Skill_nombres_t");
    plVariable B_t = variables_t.get_variable_with_name("Skill_calcul_litteral_t");
    plVariable C_t = variables_t.get_variable_with_name("Skill_demarche_scientifique_t");

    plValues evidences_t0A(ResA_t0);
    getEvidences(evidences_t0A, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t1A(ResA_t);
    getEvidences(evidences_t1A, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t2A(ResA_t);
    getEvidences(evidences_t2A, niveau_maitrise, std::vector<unsigned int>{1});
    
    plValues evidences_t3A(ResA_t);
    getEvidences(evidences_t3A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t4A(ResA_t);
    getEvidences(evidences_t4A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t5A(ResA_t);
    getEvidences(evidences_t5A, niveau_maitrise, std::vector<unsigned int>{1});


    std::vector<plValues> all_evidences;
    all_evidences.push_back(evidences_t0A);
    all_evidences.push_back(evidences_t1A);
    all_evidences.push_back(evidences_t2A);
    all_evidences.push_back(evidences_t3A);
    all_evidences.push_back(evidences_t4A);
    all_evidences.push_back(evidences_t5A);

    // 8 = nb slices
    std::vector<int> instants(nb_slices);
    // Fill the vector with incrementally augmenting values
    std::iota(std::begin(instants), std::end(instants), 0);

    std::vector<plVariablesConjunction> all_targets;

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(A_t);
        all_targets.push_back(new_target);
    }

    std::vector<plComputableObjectList> results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL A: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);

    // Skill B
    nb_slices = 3;
    all_evidences.clear();
    instants.clear();
    all_targets.clear();
    results.clear();

    plValues evidences_t0B(ResB_t0);
    getEvidences(evidences_t0B, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t1B(ResB_t);
    getEvidences(evidences_t1B, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t2B(ResB_t);
    getEvidences(evidences_t2B, niveau_maitrise, std::vector<unsigned int>{1});


    all_evidences.push_back(evidences_t0B);
    all_evidences.push_back(evidences_t1B);
    all_evidences.push_back(evidences_t2B);

    instants.resize(nb_slices);
    std::iota(std::begin(instants), std::end(instants), 0);

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(B_t);
        all_targets.push_back(new_target);
    }

    results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL B: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);

    // Skill C
    nb_slices = 3;
    all_evidences.clear();
    instants.clear();
    all_targets.clear();
    results.clear();

    plValues evidences_t0C(ResC_t0);
    getEvidences(evidences_t0C, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t1C(ResC_t);
    getEvidences(evidences_t1C, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t2C(ResC_t);
    getEvidences(evidences_t2C, niveau_maitrise, std::vector<unsigned int>{0});

    all_evidences.push_back(evidences_t0C);
    all_evidences.push_back(evidences_t1C);
    all_evidences.push_back(evidences_t2C);

    instants.resize(nb_slices);
    std::iota(std::begin(instants), std::end(instants), 0);

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(C_t);
        all_targets.push_back(new_target);
    }

    results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL C: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);
}

void queryBKTS3(pmDynamicBayesianNetwork* DBN) {
     /*
        Exercice                    | C1 | C2 | C3 
        ----------------------------+----+----+-----
        Calcul avec des fractions   | X  |    |    
        Évolution d'une proportion  | X  |    |    
        Proportionnalité            | X  |    |    
        Vente de livres             | X  |    | X   
        Conjecture                  | X  | X  |     
        Programmes de calcul        | X  | X  |     
        Devinette Mathématique      |    | X  | X   
        Récupérateur d'eau de pluie |    |    | X   
    */

    unsigned int nb_slices = 6;
    std::vector<std::string> niveau_maitrise;

    niveau_maitrise.emplace_back("1 - non acquis");
    niveau_maitrise.emplace_back("2 - acquis");

    plVariablesConjunction variables_t0 = DBN->getInitial_G().getVariables();
    plVariable ResA_t0 = variables_t0.get_variable_with_name("Res_nombres_t0");
    plVariable ResB_t0 = variables_t0.get_variable_with_name("Res_calcul_litteral_t0");
    plVariable ResC_t0 = variables_t0.get_variable_with_name("Res_demarche_scientifique_t0");


    plVariablesConjunction variables_t = DBN->getTransition_G().getVariables();
    plVariable ResA_t = variables_t.get_variable_with_name("Res_nombres_t");
    plVariable ResB_t = variables_t.get_variable_with_name("Res_calcul_litteral_t");
    plVariable ResC_t = variables_t.get_variable_with_name("Res_demarche_scientifique_t");
    
    plVariable A_t = variables_t.get_variable_with_name("Skill_nombres_t");
    plVariable B_t = variables_t.get_variable_with_name("Skill_calcul_litteral_t");
    plVariable C_t = variables_t.get_variable_with_name("Skill_demarche_scientifique_t");

    plValues evidences_t0A(ResA_t0);
    getEvidences(evidences_t0A, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t1A(ResA_t);
    getEvidences(evidences_t1A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t2A(ResA_t);
    getEvidences(evidences_t2A, niveau_maitrise, std::vector<unsigned int>{0});
    
    plValues evidences_t3A(ResA_t);
    getEvidences(evidences_t3A, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t4A(ResA_t);
    getEvidences(evidences_t4A, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t5A(ResA_t);
    getEvidences(evidences_t5A, niveau_maitrise, std::vector<unsigned int>{0});


    std::vector<plValues> all_evidences;
    all_evidences.push_back(evidences_t0A);
    all_evidences.push_back(evidences_t1A);
    all_evidences.push_back(evidences_t2A);
    all_evidences.push_back(evidences_t3A);
    all_evidences.push_back(evidences_t4A);
    all_evidences.push_back(evidences_t5A);

    // 8 = nb slices
    std::vector<int> instants(nb_slices);
    // Fill the vector with incrementally augmenting values
    std::iota(std::begin(instants), std::end(instants), 0);

    std::vector<plVariablesConjunction> all_targets;

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(A_t);
        all_targets.push_back(new_target);
    }

    std::vector<plComputableObjectList> results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL A: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);

    // Skill B
    nb_slices = 3;
    all_evidences.clear();
    instants.clear();
    all_targets.clear();
    results.clear();

    plValues evidences_t0B(ResB_t0);
    getEvidences(evidences_t0B, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t1B(ResB_t);
    getEvidences(evidences_t1B, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t2B(ResB_t);
    getEvidences(evidences_t2B, niveau_maitrise, std::vector<unsigned int>{1});


    all_evidences.push_back(evidences_t0B);
    all_evidences.push_back(evidences_t1B);
    all_evidences.push_back(evidences_t2B);

    instants.resize(nb_slices);
    std::iota(std::begin(instants), std::end(instants), 0);

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(B_t);
        all_targets.push_back(new_target);
    }

    results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL B: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);

    // Skill C
    nb_slices = 3;
    all_evidences.clear();
    instants.clear();
    all_targets.clear();
    results.clear();

    plValues evidences_t0C(ResC_t0);
    getEvidences(evidences_t0C, niveau_maitrise, std::vector<unsigned int>{0});

    plValues evidences_t1C(ResC_t);
    getEvidences(evidences_t1C, niveau_maitrise, std::vector<unsigned int>{1});

    plValues evidences_t2C(ResC_t);
    getEvidences(evidences_t2C, niveau_maitrise, std::vector<unsigned int>{1});

    all_evidences.push_back(evidences_t0C);
    all_evidences.push_back(evidences_t1C);
    all_evidences.push_back(evidences_t2C);

    instants.resize(nb_slices);
    std::iota(std::begin(instants), std::end(instants), 0);

    for (size_t i = 0; i < nb_slices; i++) {
        plVariablesConjunction new_target;
        new_target.push_back(C_t);
        all_targets.push_back(new_target);
    }

    results = DBN->prediction(all_targets, instants, all_evidences);

    std::cout << "--------" << std::endl;
    std::cout << "SKILL C: " << std::endl;
    std::cout << "--------" << std::endl;
    printBKTResult(results, instants, all_targets);
}

void getEvidences(plValues& evidences_vector, std::vector<std::string> masteries_names, std::vector<unsigned int> mastery_level) {
    if (mastery_level.size() != evidences_vector.size()) {
        std::cout << "ERROR: mastery level length is different from evidences vector length. Not assigning masteries levels." << std::endl;
        return;
    }

    if (*max_element(std::begin(mastery_level), std::end(mastery_level)) > masteries_names.size() - 1) {
        std::cout << "ERROR: max mastery level is higher than number of masteries levels. Not assigning masteries levels." << std::endl;
        return;
    }

    for (size_t i = 0; i < mastery_level.size(); i++) {
        if (mastery_level[i] != -1)
            evidences_vector[i] = masteries_names[mastery_level[i]];
        // TODO: what to put instead of mastery_level[i]
    }
}

// Made by Théo DAUDIN and adapted by Quentin COULAND
void printBKTResult(std::vector<plComputableObjectList> results, std::vector<int> instants, 
                    std::vector<plVariablesConjunction> all_targets, bool formatted_for_spreadsheet) {
    
    std::cout << "Displaying the results:" << std::endl << std::endl;

    for(size_t i = 0; i < results.size(); i++) {
        plHMM::Observation_t outputData;

        if(formatted_for_spreadsheet)
            std::cout << "instant: " << instants[i] << std::endl;
        else
            std::cout << "[main.cpp] - instant: " << instants[i] << ",\t";

        for(size_t y = 0; y < results[i].size(); y++) {
            results[i][y].tabulate(outputData);

            if(!formatted_for_spreadsheet)
                std::cout << all_targets[i][y].name() << ": [";

            for(size_t k = 0; k < outputData.size(); k++) {
                if(formatted_for_spreadsheet)
                    std::cout << outputData.data()[k] << (k < outputData.size() - 1 ? "," : "");

                else
                    std::cout << k << ": " << outputData.data()[k] << (k < outputData.size() - 1 ? "," : "");
            }
            
            if (!formatted_for_spreadsheet)
                std::cout << "]" << (y < results[i].size() - 1 ? "," : "");
        }

        std::cout << std::endl;
    }

    std::cout << std::endl << std::endl;
}
