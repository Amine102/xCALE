#include "learnParametersSpam.h"

using namespace std;
using namespace PILGRIM;

void learnParametersSpam() {

  size_t line_skipped = 1;
  size_t nb_param = 5;
  char delimiter = ';';
  pmScoreValueType priorWeight = 1;


  /*****************************************************************************
   *  DONNEES INCOMPLETES
  ******************************************************************************/
  char* incomplete_data_file = "../../benchmarks/data/Spam_incomplete_csv.data";

  pmCSVDataSet* incomplete_data = new pmCSVDataSet(incomplete_data_file);

  // Learning parameters from incomplete data set

  plVariablesConjunction vars_inc = incomplete_data->observed_variables();

  pmBayesianNetwork bnSpamML_inc(vars_inc);
  bnSpamML_inc.naiveBayes(0);
  bnSpamML_inc.learnParameters(incomplete_data);

  pmBayesianNetwork bnSpamEAP_inc(vars_inc);
  bnSpamEAP_inc.naiveBayes(0);
  bnSpamEAP_inc.setLearnObjectEAP(priorWeight);
  bnSpamEAP_inc.learnParameters(incomplete_data);
  
       
         

  
  cout << "Bayesian network learned with ML and incomplete data" << endl;
  bnSpamML_inc.summary();
  bnSpamML_inc.save_as_xml("../../benchmarks/networks/lpBnSpamML_inc.xml","lpBnSpamML_inc");

  cout << "Bayesian network learned using EAP  and incomplete data" << endl;
  bnSpamEAP_inc.summary();
  bnSpamEAP_inc.save_as_xml("../../benchmarks/networks/lpBnSpamEAP_inc.xml","lpBnSpamEAP_inc");


  /*****************************************************************************
   *  DONNEES COMPLETES
  ******************************************************************************/
  char* complete_data_file = "../../benchmarks/data/Spam_csv.data";
  pmCSVDataSet* complete_data = new pmCSVDataSet(complete_data_file,
                                   line_skipped,
                                   nb_param,
                                   delimiter);

  plVariablesConjunction vars_compl = complete_data->observed_variables();

  // Creation of Bayesian network for which we will learns the parameters
  // from data using maximum likelyhood
  pmBayesianNetwork bnSpamML_compl(vars_compl);
  bnSpamML_compl.naiveBayes(0);
  bnSpamML_compl.learnParameters(complete_data);

  // Creation of Bayesian network for which we will learns the parameters
  // from data using expectation a posteriori

  pmBayesianNetwork bnSpamEAP_compl(vars_compl);
  bnSpamEAP_compl.naiveBayes(0);
  bnSpamEAP_compl.setLearnObjectEAP(priorWeight);
  bnSpamEAP_compl.learnParameters(complete_data);
  //Print & export       
                    
  complete_data->summary();
         
  cout << "Bayesian network learned with ML and complete data" << endl;
  bnSpamML_compl.summary();
  bnSpamML_compl.save_as_xml("../../benchmarks/networks/lpBnSpamML_compl.xml","lpBnSpamML_compl");

  cout << "Bayesian network learned using EAP and complete data" << endl;
  bnSpamEAP_compl.summary();
  bnSpamEAP_compl.save_as_xml("../../benchmarks/networks/lpBnSpamEAP_compl.xml","lpBnSpamEAP_compl");
  
}
