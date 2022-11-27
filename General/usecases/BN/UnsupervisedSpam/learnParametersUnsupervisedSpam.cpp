
//============ Unsupervised Spam Learning : this usecase is the learning phase of the created datset of UnsupervisedSpam =========//
// The structure is a Naif Bayes structure where all the variable depend on the unobserved classe : Spam , learning is done with EM algorithm
//================================================================================================================================================//
#include "learnParametersUnsupervisedSpam.h"

using namespace std;
using namespace PILGRIM;

void learnParametersUnsupervisedSpam() {

  int ncSpam= 2; 
  const plVariable Spam("Spam", plIntegerType(0, ncSpam-1));
  size_t line_skipped = 1;
  size_t nb_param = 6; //the number present in the dataset
  char delimiter = ';';
  pmScoreValueType priorWeight = 1;


  /*****************************************************************************
   *  DONNEES COMPLETES
  *****************************************************************************/
  char* complete_data_file = "../../benchmarks/data/UnsupervisedSpam_csv.data";
  pmCSVDataSet* complete_data = new pmCSVDataSet(complete_data_file,
                                   line_skipped,
                                   nb_param,
                                   delimiter);
                                

  plVariablesConjunction vars_compl = complete_data->observed_variables();
  vars_compl = vars_compl ^ Spam; // Add the latent variable to the variable conjunction
  pmBayesianNetwork bnSpamML_compl(vars_compl); 
  bnSpamML_compl.naiveBayes(6); //Spam is the last variable 
  bnSpamML_compl.learnParameters(complete_data);

  // Creation of Bayesian network for which we will learns the parameters
  // from data using expectation a posteriori

  pmBayesianNetwork bnSpamEAP_compl(vars_compl);
  bnSpamEAP_compl.naiveBayes(6);
  bnSpamEAP_compl.setLearnObjectEAP(priorWeight);
  bnSpamEAP_compl.learnParameters(complete_data);
  //Print & export       
                    
  complete_data->summary();
         
  cout << "Bayesian network learned with ML and complete data" << endl;
  bnSpamML_compl.summary();
  bnSpamML_compl.save_as_xml("../../benchmarks/networks/lpBnUnsupervisedSpamML_compl.xml","lpBnUnsupervisedSpamML_compl");

  cout << "Bayesian network learned using EAP and complete data" << endl;
  bnSpamEAP_compl.summary();
  bnSpamEAP_compl.save_as_xml("../../benchmarks/networks/lpBnUnsupervisedSpamEAP_compl.xml","lpBnUnsupervisedSpamEAP_compl");
}
