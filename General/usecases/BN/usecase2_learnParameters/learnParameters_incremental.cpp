#include "usecase2.h"

using namespace std;
using namespace PILGRIM;

void usecase2_incre(){

    pmBayesianNetwork bnAsia("../../benchmarks/networks/Asia_uc2.xml", "Asia_uc2");
    cout << "Network summary : " << endl;
    bnAsia.summary();

    const plVariablesConjunction &vars = bnAsia.getVariables();
    char *data_file = "../../benchmarks/data/Asia_uc2_csv.data";
    size_t window_size = 1;
    size_t numberOfColumns = 8;
    pmCSVDataSet *data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', vars);

    cout << endl << "learning parameters ..." << endl; 
    bnAsia.setForgettingFactor(0.7);
    bnAsia.learnParameters(data);

    cout << endl << "new network summary :" << endl << endl;
    bnAsia.summary();
    cout << endl;

}