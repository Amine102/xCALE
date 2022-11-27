#include "learnParametersExamenMarks.h"

using namespace std;
using namespace PILGRIM;

void learnParametersExamenMarks() {

  char* data_file = "../../benchmarks/data/ExamenMarks_csv.data";
  pmCSVDataSet* data = new pmCSVDataSet(data_file);

  plVariablesConjunction vars = data->observed_variables();

  //Print & export
  cout << "Network summary : " << endl;
  data->summary();

  pmBayesianNetwork bnExamenMarks(vars);
  pmBayesianNetwork bnExamenMarksTemp("../../benchmarks/networks/examenMarks.xml", "examenMarks");
  bnExamenMarks.copy_structure(bnExamenMarksTemp);

  bnExamenMarks.learnParameters(data);
  cout << "Bayesian network learned using default ML and complete data" << endl;
  bnExamenMarks.summary();
  cout << "lpBnExamenMarks.xml generated in benchmarks/networks/ " << endl;
  bnExamenMarks.save_as_xml("../../benchmarks/networks/lpBnExamenMarks.xml","lpBnExamenMarks");
}
