#include "LoadAlarmAndGenerateData.h"
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>

using namespace std;
using namespace PILGRIM;


void LoadAlarmAndGenerateData()
{
    pmBayesianNetwork bnTemp("../../benchmarks/networks/Alarm.xdsl");

    bnTemp.summary();
    bnTemp.generate_csvData("../../benchmarks/data/Alarm1.data", 1000);
    // bnTemp.generate_csvData("../../benchmarks/data/Alarm2.data", 1000);
    // bnTemp.generate_csvData("../../benchmarks/data/Alarm3.data", 1000);
    // bnTemp.generate_csvData("../../benchmarks/data/Alarm4.data", 500);
    // bnDummyTemp.save_as_xml("../../benchmarks/networks/jdBnDummy1.xml", "lpBnDummy");
}