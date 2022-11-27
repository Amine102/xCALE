#include <pl.h>
#include <stdio.h>
#include <ctime>
#include "queryDynamicBN.h"

using namespace boost;
using namespace std;
using namespace PILGRIM;

/**
 * \brief Queries a simple dynamic bayesian network
 * \author  GHERASIM Toader
 */
void queryDynamicBN() {
  plJointDistribution jd0;
  jd0.load_from_genie("benchmarks/networks/OrateurG0.xdsl");
  pmBayesianNetwork G0(jd0);
  plJointDistribution jdt;
  jdt.load_from_genie("benchmarks/networks/OrateurGt.xdsl");
  pmBayesianNetwork Gt(jdt);
  pmDynamicBayesianNetwork dBN = pmDynamicBayesianNetwork(&G0, &Gt);
  std::cout << "Dynamic Bayesian Network" << std::endl;
  std::cout << "------------------------" << std::endl;
 // dBN.summary();
  std::cout << std::endl;

  // construct the evidence
  plVariablesConjunction varconj = dBN.getVariablesforGtransition();

  plVariable orateur_t = varconj.get_variable_with_name("Orateur_t");
  plVariable mot_t = varconj.get_variable_with_name("Mot_t");

  plValues evidence0(orateur_t);
  plValues evidence1 = * new plValues();
  plValues evidence2(mot_t);

  evidence0[orateur_t] = "A";
  evidence2[mot_t] = "dodo";

  vector<plValues> evidence_globale = vector<plValues>();

  evidence_globale.push_back(evidence0);
  evidence_globale.push_back(evidence1);
  evidence_globale.push_back(evidence2);

  plVariable target_variable = varconj.get_variable_with_name("Orateur_t");

  std::cout << "Query 1 :   P(Orateur_t, 2, {{Orateur_t = 'A'}, {}, "
               "{Mot_t = 'dodo'}})"
            << std::endl;
float temps_initial = clock ();
plDistribution result_query = dBN.query(target_variable,
                                          2,
                                          &evidence_globale);
  result_query.tabulate();
  float temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;

temps_initial = clock ();
  plDistribution result_query2 = dBN.query2(target_variable,
                                          2,
                                          &evidence_globale);
  result_query2.tabulate();
   temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;

temps_initial = clock ();

  std::cout << "Query 2 :   P(Orateur_t, 4, {{Orateur_t = 'A'}, {}, "
               "{Mot_t = 'dodo'}})"
            << std::endl;
  result_query = dBN.query(target_variable, 4, &evidence_globale);
  result_query.tabulate();
   temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;

temps_initial = clock ();

   result_query2 = dBN.query2(target_variable, 4, &evidence_globale);
  result_query2.tabulate();
   temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;

temps_initial = clock ();

  std::cout << "Query 3 :   P(Mot_t, 4, {{Orateur_t = 'A'}, {}, "
               "{Mot_t = 'dodo'}})"
            << std::endl;
  target_variable = varconj.get_variable_with_name("Mot_t");
  result_query = dBN.query(target_variable, 4, &evidence_globale);
  result_query.tabulate();
   temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;

temps_initial = clock ();
  result_query2 = dBN.query2(target_variable, 4, &evidence_globale);
  result_query2.tabulate();
   temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;


}

