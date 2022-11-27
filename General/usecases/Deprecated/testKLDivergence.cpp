#include "testKLDivergence.h"

using namespace PILGRIM;
using namespace std;

/**
 * \brief Creates a simple bayesian network (inspired from the ProBT doc) and prints it
 */
void testKLDivergence() {
  /*
  // Variable definitions
  vector<string> adr_labels;
  adr_labels.push_back("seul");
  adr_labels.push_back("un_parmi_n");
  adr_labels.push_back("non");

  plLabelType labels(adr_labels);

  plVariable Spam("v0", PL_BINARY_TYPE);  // Spam
  plVariable Adress("v1", labels);  // Adress
  plVariable Viagra("v2", PL_BINARY_TYPE);  // Viagra
  plVariable Polytech("v3", PL_BINARY_TYPE);  // Polytech
  plVariable Felicitation("v4", PL_BINARY_TYPE);  // Felicitation

  // Probability table of Spam
  plProbValue tableSpam[] = {0.8, 0.2};
  plProbTable P_Spam(Spam, tableSpam);

  // Probability table of Adress, which depends on Spam
  plProbValue tableAdress_knowing_Spam[] = {
    0.02, 0.2, 0.78,
    0.78, 0.2, 0.02
  };
  plDistributionTable P_Adress(Adress, Spam, tableAdress_knowing_Spam);

  // Probability table of Viagra, which depends on Spam
  plProbValue tableViagra_knowing_Spam[] = {
    0.999, 0.001,
    0.9, 0.1
  };
  plDistributionTable P_Viagra(Viagra, Spam, tableViagra_knowing_Spam);

  // Probability table of Polytech, which depends on Spam
  plProbValue tablePolytech_knowing_Spam[] = {
    0.7, 0.3,
    0.99, 0.01
  };
  plDistributionTable P_Polytech(Polytech, Spam, tablePolytech_knowing_Spam);

  // Probability table of Felicitation, which depends on Spam
  plProbValue tableFelicitation_knowing_spam[] = {
    0.99, 0.01,
    0.7, 0.3
  };
  plDistributionTable P_Felicitation(Felicitation,
                                     Spam,
                                     tableFelicitation_knowing_spam);

  // Joint distribution
  plJointDistribution jdSpam(Spam^Adress^Viagra^Polytech^Felicitation,
                             P_Spam*P_Adress*P_Viagra*P_Polytech*P_Felicitation);

  // Creation of Bayesian network
  pmBayesianNetwork bnSpam(jdSpam);

  // Generate csv, 1000 samples
  bnSpam.generate_csvData("benchmarks/data/Spam_csv.data", 1000);

  // Generate csv, 1000 samples with incomplete data
  vector<unsigned int> indexes = vector<unsigned int>();
  vector<double> probs_unk = vector<double>();
  indexes.push_back(0);
  indexes.push_back(1);
  indexes.push_back(2);
  indexes.push_back(3);
  indexes.push_back(4);
  probs_unk.push_back(0.5);
  probs_unk.push_back(0.5);
  probs_unk.push_back(0.5);
  probs_unk.push_back(0.5);
  probs_unk.push_back(0.5);
  bnSpam.generate_csvIncompleteData("benchmarks/data/Spam_incomplete_csv.data",
                                    1000,
                                    indexes,
                                    probs_unk);

  size_t line_skipped = 1;
  size_t nb_param = 5;

  pmCSVDataSet data("benchmarks/data/Spam_csv.data",
                    line_skipped,
                    nb_param,
                    ';',
                    bnSpam.getVariables());
  pmCSVDataSet incomplete_data("benchmarks/data/Spam_incomplete_csv.data",
                               line_skipped,
                               nb_param,
                               ';',
                               bnSpam.getVariables());

  // Creation of Bayesian network
  plVariablesConjunction vars = data.observed_variables();
  pmBayesianNetwork bnSpamML(vars);
  bnSpamML.naiveBayes(0);

  bnSpamML.learnParametersML(data.get_data_descriptor());

  // Creation of Bayesian network with missing data
  vars = incomplete_data.observed_variables();
  pmBayesianNetwork bnSpamML_inc(vars);
  bnSpamML_inc.naiveBayes(0);

  bnSpamML_inc.learnParametersML(incomplete_data.get_data_descriptor());

  cout << "Theoretical BN" << endl;
  bnSpam.summary();

  cout << endl << "BN learned from data" << endl;
  bnSpamML.summary();

  cout << endl << "BN learned from incomplete data" << endl;
  bnSpamML_inc.summary();

  // Without estimation
  cout << endl << "KLdiv theoretical <--> learned" << endl;
  bnSpam.computeKLDivergence(bnSpamML, false);

  // With estimation
  cout<<endl << "KLdiv estimation theoretical <--> learned" << endl;
  bnSpam.computeKLDivergence(bnSpamML, true);

  cout << endl << "KLdiv theoretical <--> learned from incomplete data" << endl;
  //Compare Theoretical BN with BN learned from incomplete data
  bnSpam.computeKLDivergence(bnSpamML_inc, false);
*/
}

