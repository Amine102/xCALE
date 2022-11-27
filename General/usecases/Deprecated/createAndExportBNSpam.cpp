#include "createAndExportBNSpam.h"
#include <vector>
#include <string>

using namespace std;
using namespace PILGRIM;

void createAndExportSpam() {
  // Variable definitions
  vector<string> adr_labels;
  adr_labels.push_back("seul");
  adr_labels.push_back("un_parmi_n");
  adr_labels.push_back("non");

  plLabelType labels(adr_labels);

  plVariable Spam("Spam", PL_BINARY_TYPE);
  plVariable Adress("Adress", labels);
  plVariable Viagra("Viagra", PL_BINARY_TYPE);
  plVariable Polytech("Polytech", PL_BINARY_TYPE);
  plVariable Felicitation("Felicitation", PL_BINARY_TYPE);

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
  plDistributionTable P_Felicitation(Felicitation, Spam,
                                     tableFelicitation_knowing_spam);

  // Joint distribution
  plJointDistribution jdSpam(Spam^Adress^Viagra^Polytech^Felicitation,
                             P_Spam*P_Adress*P_Viagra*P_Polytech*P_Felicitation);

  // Creation of Bayesian network
  pmBayesianNetwork bnSpam(jdSpam);

  // Print Spam BN
  cout << "Spam bayesian network : " << endl;
  bnSpam.summary();
  cout << "\n\n";

  // Generate csv, 1000 samples
  bnSpam.generate_csvData("Spam_csv.data", 1000);

  // Evidence with label
  plValues evidenceSpam1(Adress);
  evidenceSpam1[Adress] = "seul";

  plDistribution pld = bnSpam.query(Spam, evidenceSpam1);
  pld.tabulate();

  bnSpam.save_as_genie("benchmarks/networks/SpamBN.xdsl");
}
