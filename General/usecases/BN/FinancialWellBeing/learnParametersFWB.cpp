#include "learnParametersFWB.h"
using namespace std;
using namespace PILGRIM;


void learnParametersFWB() {


 /*****************************************************************************
   *  DONNEES COMPLETES
  ******************************************************************************/
  size_t line_skipped = 1;
  size_t nb_param = 217;
  char delimiter = ';';
  pmScoreValueType priorWeight = 1;


  char* complete_data_file = "../../benchmarks/data/NFWBS_PUF_2016_data.csv";
  pmCSVDataSet* complete_data = new pmCSVDataSet(complete_data_file,
                                   line_skipped,
                                   nb_param,
                                   delimiter);

  plVariablesConjunction vars_compl = complete_data->observed_variables();

  //Learns the parameters from data using maximum likelyhood
  cout << "\n \n // ============  Start Financial Well Being Parameter Learning with ML ============= // \n\n";
  pmBayesianNetwork bnFWBML_compl(vars_compl);
  bnFWBML_compl.learnParameters(complete_data);
  cout << "Finished the learning with ML and complete data" << endl;

  //Learns the parameters from data using expectation a posteriori
   cout << "\n \n // ============  Start Financial Well Being Parameter Learning with EAP ============= // \n\n";
  pmBayesianNetwork bnFWBEAP_compl(vars_compl);
  //bnFWBEAP_compl.naiveBayes(0);
  bnFWBEAP_compl.setLearnObjectEAP(priorWeight);
  bnFWBEAP_compl.learnParameters(complete_data);
  //Print & export       
  cout << "Finished the learning with EAP and complete data" << endl;
                    
  complete_data->summary();
         
  cout << "Bayesian network learned with ML and complete data" << endl;
  bnFWBML_compl.summary();
  bnFWBML_compl.save_as_xml("../../benchmarks/networks/lpBnFWBML_compl.xml","lpBnFWBML_compl");

  cout << "Bayesian network learned using EAP and complete data" << endl;
  bnFWBEAP_compl.summary();
  bnFWBEAP_compl.save_as_xml("../../benchmarks/networks/lpBnFWBEAP_compl.xml","lpBnFWBEAP_compl");
  

}
