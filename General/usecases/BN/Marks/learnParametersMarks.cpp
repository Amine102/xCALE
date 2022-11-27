#include "learnParametersMarks.h"

using namespace std;
using namespace PILGRIM;
/**
 * \brief Load the BN created in 'createMarks.cpp', in xml.
 * Load the data set created in 'createMarks.cpp' in csv.data
 * and learn BN's parameters using the old method (incrementation on probabilities)
 * Save it as xml file
 */
void learnParametersMarks(){

  //Load data
  string path_to_data = "../../benchmarks/data/Marks_csv.data";
  char *data_file = new char[path_to_data.length() + 1];
  strcpy(data_file, path_to_data.c_str());

  pmCSVDataSet* data = new pmCSVDataSet(data_file);

  plVariablesConjunction vars = data->observed_variables();

  //Print & export
  cout << "Network summary : " << endl;
  data->summary();

  /** 
   * Usecases of the new learning methods
   */

  /**
   * Default method : Learning object ML-generated
   */ 
  pmBayesianNetwork bnMarksML(vars);
  pmBayesianNetwork bnUniMarksMLTemp("../../benchmarks/networks/jdBnMarks.xml","jdBnMarks");
  bnMarksML.copy_structure(bnUniMarksMLTemp);

  bnMarksML.learnParameters(data);
  cout << "Bayesian network learned using default ML and complete data" << endl;
  bnMarksML.summary();
  cout << "lpBnMarks.xml generated in benchmarks/networks/ " << endl;
  bnMarksML.save_as_xml("../../benchmarks/networks/lpBnMarks.xml","lpBnMarks");

}