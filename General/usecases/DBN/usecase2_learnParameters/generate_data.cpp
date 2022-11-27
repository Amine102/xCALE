#include "usecase2.h"

using namespace std;
using namespace PILGRIM;

void dataGen(){

    pmDynamicBayesianNetwork *DBN = new pmDynamicBayesianNetwork();
    DBN->load_from_xml("../../benchmarks/networks/jdDBN.xml","jdDBN");
    DBN->generate_csv_data("../../benchmarks/data/DBN_data.data", 1000, 1, false);
    cout << "data generated in Asia_uc_csv.data in  benchmarks/data" << endl;
}