#include "learn.h"

using namespace std;
using namespace PILGRIM;

void learn() {

  char* data_file = "PED-use_csv.data";
  pmCSVDataSet* data = new pmCSVDataSet(data_file);

  plVariablesConjunction vars = data->observed_variables();

  //Print & export
  cout << "Network summary : " << endl;
  data->summary();

  pmBayesianNetwork bn(vars);
  pmBayesianNetwork bnTemp("PED-use.xml", "PED-use");
  bn.copy_structure(bnTemp);

  bn.learnParameters(data);
  cout << "Bayesian network learned using default ML and complete data" << endl;
  bn.summary();
  cout << "lpBn.xml generated in current repository " << endl;
  bn.save_as_xml("lpBn.xml","lpBn");
}
