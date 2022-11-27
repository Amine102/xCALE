#include "createLabeledSpamBN.h"
using namespace std;
using namespace PILGRIM;

void createLabeledSpamBN() {
  vector<string> adr_label = vector<string>();
  adr_label.push_back("Seul");
  adr_label.push_back("UnParmiN");
  adr_label.push_back("Non");
  vector<string> bin_label = vector<string>();
  bin_label.push_back("O");
  bin_label.push_back("N");
  // Variable definitions
  plVariable Spam("Spam", plLabelType(bin_label));
  plVariable Viagra("Viagra",  plLabelType(bin_label));
  plVariable Polytech("Polytech",  plLabelType(bin_label));
  plVariable Felicitation("Felicitation",  plLabelType(bin_label));
  plVariable Adresse("Adresse",plLabelType(adr_label));

  //// Probability table of Spam
  plProbValue tableSpam[] = {0.2, 0.8};
  plProbTable P_Spam(Spam, tableSpam);

  //// Probability table of Adresse, which depends on Spam
  plProbValue tableAdresse_knowingSpam[] = {
    0.02, 0.20, 0.78, 0.78, 0.2, 0.02
  };
  plDistributionTable P_Adresse(Adresse, Spam, tableAdresse_knowingSpam);

  // probability table of adresse, which depends on spam
  plProbValue tableViagra_knowingSpam[] = {
    0.1, 0.9, 0.001, 0.999
  };
  plDistributionTable P_Viagra(Viagra, Spam, tableViagra_knowingSpam);

  // probability table of adresse, which depends on spam
  plProbValue tablePolytech_knowingSpam[] = {
    0.01, 0.99, 0.3, 0.7
  };
  plDistributionTable P_Polytech(Polytech, Spam, tablePolytech_knowingSpam);

  // probability table of adresse, which depends on spam
  plProbValue tableFelicitation_knowingSpam[] = {
    0.3, 0.7, 0.01, 0.99
  };
  plDistributionTable P_Felicitation(Felicitation,
                                     Spam,
                                     tableFelicitation_knowingSpam);

  // Joint distribution
  plJointDistribution jd(Spam^Adresse^Viagra^Polytech^Felicitation,
                         P_Spam*P_Adresse*P_Viagra*P_Polytech*P_Felicitation);

  //// Creation and print of the bayesian network
  pmBayesianNetwork bn(jd);
  bn.summary();
}
