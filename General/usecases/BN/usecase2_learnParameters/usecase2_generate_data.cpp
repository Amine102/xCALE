#include "usecase2.h"

using namespace std;
using namespace PILGRIM;

void usecase2_dataGen(){

    pmBayesianNetwork bnAsia("../../benchmarks/networks/jdBnAsia.xml","jdBnAsia");

    bnAsia.generate_csvData("../../benchmarks/data/Asia_uc_csv.data", 1000);
    cout << "data generated in Asia_uc_csv.data in  benchmarks/data" << endl;
    bnAsia.generate_csvData("../../benchmarks/data/Asia_uc2_csv.data", 1000);
    cout << "data generated in Asia_uc2_csv.data in  benchmarks/data" << endl;
}