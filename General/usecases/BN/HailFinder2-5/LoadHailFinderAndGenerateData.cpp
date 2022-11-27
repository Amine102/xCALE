#include "LoadHailFinderAndGenerateData.h"
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>

using namespace std;
using namespace PILGRIM;


void LoadHailFinderAndGenerateData()
{
    pmBayesianNetwork bnTemp("../../benchmarks/networks/Hailfinder2-5.xdsl");

    bnTemp.summary();
    bnTemp.generate_csvData("../../benchmarks/data/Hailfinder2-5_1.data", 1000);
    // bnTemp.generate_csvData("../../benchmarks/data/Hailfinder2-5_2.data", 1000);
    // bnTemp.generate_csvData("../../benchmarks/data/Hailfinder2-5_3.data", 1000);
    // bnTemp.generate_csvData("../../benchmarks/data/Hailfinder2-5_4.data", 500);
}