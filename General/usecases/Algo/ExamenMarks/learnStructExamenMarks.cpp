#include "learnStructExamenMarks.h"

using namespace PILGRIM;
using namespace std;
using namespace boost;

void runBNAlgoExamenMarks() {

    // My static BN data examenMarks model
    char* file_name = "../../benchmarks/data/ExamenMarks_csv.data";

    // Using pmCSVDataSet guessing constructor
    pmCSVDataSet* ds = new pmCSVDataSet(file_name);

    // Using BIC score
    pmScoreBIC<rowDataType>* pScore = new pmScoreBIC<rowDataType>();

    // Print variable name
    ds->print_variables();

    // MY GS ALGO

    //Creating a greedy search algorithm and computing the BN
    pmAlgoGS<rowDataType> algoGS(ds, pScore);

    pmBayesianNetwork* initBN=algoGS.getInitialBN();
    algoGS.run(initBN);

    cout << endl << "========== GS algorithm ==========" << endl;
    algoGS.getInitialBN()->summary();

    // MY MMHC ALGO

    pmAlgoMMHC<rowDataType> algoMMHC(ds, pScore);

    algoMMHC.run(NULL);

    cout << endl << endl << "========== MMHC algorithm ==========" << endl;
    algoMMHC.getInitialBN()->summary();

}
