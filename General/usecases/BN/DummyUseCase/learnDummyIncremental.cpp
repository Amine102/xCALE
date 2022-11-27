#include "learnDummyIncremental.h"

using namespace std;
using namespace PILGRIM;

void learnDummyIncremental()
{
  cout << "Loading file " << endl;
  pmBayesianNetwork bnDummyTemp("../../benchmarks/networks/jdBnDummy1.xml", "jdDummy");
  const plVariablesConjunction &vars = bnDummyTemp.getVariables();
  // --------------------------------------------------------------------------------------------------------------
  char *data_file = "../../benchmarks/data/Dummy1_csv_file2.data";
  size_t window_size = 1;
  size_t numberOfColumns = 5;
  pmCSVDataSet *data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', vars);
  //Print & export
  cout << "Network summary : " << endl;
  data->summary();
  //----------------------------------------------------------------------------------------------------------------
  cout << "Set Forgetting Factor" << endl;
  bnDummyTemp.setForgettingFactor(0.7);
  cout << "Will enter learnParameters" << endl;
  bnDummyTemp.learnParameters(data);
  cout << "SUmmary is " << endl;
  bnDummyTemp.summary();
  bnDummyTemp.save_as_xml("../../benchmarks/networks/jdBnDummy1.xml", "jdDummy");
}