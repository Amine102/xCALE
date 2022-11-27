#include "learnParametersJuggler.h"

using namespace std;
using namespace PILGRIM;

void learnParametersJuggler() {

  char* data_file = "../../benchmarks/data/Juggler_csv.data";
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
  pmBayesianNetwork bnJugglerML(vars);
  pmBayesianNetwork bnJugglerMLTemp("../../benchmarks/networks/jdBnJuggler.xml","jdBnJuggler");
  bnJugglerML.copy_structure(bnJugglerMLTemp);

  bnJugglerML.learnParameters(data);
  cout << "Bayesian network learned using default ML and complete data" << endl;
  bnJugglerML.summary();
  cout << "lpBnJuggler.xml generated in benchmarks/networks/ " << endl;
  bnJugglerML.save_as_xml("../../benchmarks/networks/lpBnJuggler.xml","lpBnJuggler");
}

