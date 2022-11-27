#include "testNoisyConditionals.h"

using namespace std;
using namespace PILGRIM;

/**
 * \brief Load the BN created in 'createNoisyConditionals.cpp', in xml.
 * Load the data set created in 'createNoisyConditionals.cpp' in csv.data
 * and learn BN's parameters using the old method (incrementation on probabilities)
 * Apply noisyConditionals on the BN with computed parameters
 * Save it as xml file
 */
void testNoisyConditionals(){
  //Load data
  string path_to_data= "../../benchmarks/data/NoisyConditionals_csv.data";
  char *data_file = new char[path_to_data.length() +1];
  strcpy(data_file, path_to_data.c_str());

  pmBayesianNetwork bnTemp("../../benchmarks/networks/jdBnNoisyConditionals.xml","jdBnNoisyConditionals");//
  const plVariablesConjunction &vars = bnTemp.getVariables();// 

  size_t window_size = 1;
  size_t numberOfColumns = 4;
  pmCSVDataSet *data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', vars);
  cout<<"finished loading the data"<<endl;

  //Print & export
  cout << "Network summary : " << endl;
  data->summary();

  pmBayesianNetwork bnTest(vars);
  bnTest.copy_structure(bnTemp);

  bnTest.learnParameters(data);

  //bnTest.noisyConditionals(data,eps);

  cout << "Bayesian network learned using default ML and complete data" << endl;
  bnTest.summary();

  bnTest.noisyConditionals(data, 0.44);
  cout << "Bayesian network learned using default ML and complete data" << endl;
  bnTest.summary();
  cout << "lpBnNoisyConditionals.xml generated in benchmarks/networks/ " << endl;
  bnTest.save_as_xml("../../benchmarks/networks/lpBnNoisyConditionals.xml","lpBnNoisyConditionals");


}