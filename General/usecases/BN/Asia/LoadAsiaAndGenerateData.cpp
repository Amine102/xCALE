#include "LoadAsiaAndGenerateData.h"
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>

using namespace std;
using namespace PILGRIM;


void LoadAsiaAndGenerateData()
{
    pmBayesianNetwork bnTemp("../../benchmarks/networks/Asia.xdsl");

    bnTemp.summary();
    bnTemp.generate_csvData("../../benchmarks/data/Asia1.data", 1000);
    bnTemp.generate_csvData("../../benchmarks/data/Asia2.data", 800);
    bnTemp.generate_csvData("../../benchmarks/data/Asia3.data", 600);
    bnTemp.generate_csvData("../../benchmarks/data/Asia4.data", 500);
    bnTemp.generate_csvData("../../benchmarks/data/Asia4.data", 300);
    // bnDummyTemp.save_as_xml("../../benchmarks/networks/jdBnDummy1.xml", "lpBnDummy");
}