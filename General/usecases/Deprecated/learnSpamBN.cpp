#include "learnSpamBN.h"

using namespace std;
using namespace PILGRIM;


void learnSpamBN() {
  /*
  size_t line_skipped = 1;
  size_t nb_param = 5;
  char delimiter = ';';
  char* complete_data_file = "benchmarks/data/Spam_csv.data";
  char* incomplete_data_file = "benchmarks/data/Spam_incomplete_csv.data";
  pmCSVDataSet data = pmCSVDataSet(complete_data_file,
                                   line_skipped,
                                   nb_param,
                                   delimiter);
  pmCSVDataSet incomplete_data = pmCSVDataSet(incomplete_data_file,
                                              line_skipped,nb_param,
                                              delimiter);

  plVariablesConjunction vars = data.observed_variables();

  // Creation of Bayesian network for which we will learns the parameters
  // from data using maximum likelyhood
  pmBayesianNetwork bnSpamML(vars);
  bnSpamML.naiveBayes(0);
  bnSpamML.learnParametersML(data.get_data_descriptor());

  // Creation of Bayesian network for which we will learns the parameters
  // from data using expectation a posteriori
  pmScoreValueType priorWeight = 1;
  pmBayesianNetwork bnSpamEAP(vars);
  bnSpamEAP.naiveBayes(0);
  bnSpamEAP.learnParametersEAP(data.get_data_descriptor(), priorWeight);

  cout << "Bayesian network learned with ML" << endl;
  bnSpamML.summary();

  cout << "Bayesian network learned using EAP" << endl;
  bnSpamEAP.summary();

  // Learning parameters from incomplete data set

  pmBayesianNetwork bnSpamML_inc(incomplete_data.observed_variables());
  bnSpamML_inc.naiveBayes(0);
  bnSpamML_inc.learnParametersML(incomplete_data.get_data_descriptor());

  pmBayesianNetwork bnSpamEAP_inc(incomplete_data.observed_variables());
  bnSpamEAP_inc.naiveBayes(0);
  bnSpamEAP_inc.learnParametersEAP(incomplete_data.get_data_descriptor(),
                                   priorWeight);
  incomplete_data.summary();
  cout << "Bayesian network learned with ML and incomplete data" << endl;
  bnSpamML_inc.summary();

  cout << "Bayesian network learned using EAP  and incomplete data" << endl;
  bnSpamEAP_inc.summary();
*/
}

