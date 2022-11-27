#include "learnParametersUniAdmissions.h"

using namespace std;
using namespace PILGRIM;
/**
 * \brief Load the BN created in 'createUniAdmissions.cpp', in xml.
 * Load the data set created in 'createUniAdmissions.cpp' in csv.data
 * and learn BN's parameters using the old method (incrementation on probabilities)
 * Save it as xml file
 */
void learnParametersUniAdmissions(){

  //Load data
  string path_to_data = "../../benchmarks/data/UniAdmissions_csv.data";
  char *data_file = new char[path_to_data.length() + 1];
  strcpy(data_file, path_to_data.c_str());
  
  pmCSVDataSet* data = new pmCSVDataSet(data_file);

  plVariablesConjunction vars = data->observed_variables();

  //Print & export
  cout << "Network summary : " << endl;
  data->summary();

  pmBayesianNetwork bnUniAdmissionsML(vars);
  pmBayesianNetwork bnUniAdmissionsMLTemp("../../benchmarks/networks/jdBnUniAdmissions.xml","jdBnUniAdmissions");
  bnUniAdmissionsML.copy_structure(bnUniAdmissionsMLTemp);

  bnUniAdmissionsML.learnParameters(data);
  cout << "Bayesian network learned using default ML and complete data" << endl;
  bnUniAdmissionsML.summary();
  cout << "lpBnUniAdmissions.xml generated in benchmarks/networks/ " << endl;
  bnUniAdmissionsML.save_as_xml("../../benchmarks/networks/lpBnUniAdmissions.xml","lpBnUniAdmissions");

}