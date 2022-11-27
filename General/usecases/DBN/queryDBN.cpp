#include "queryDBN.h"

using namespace PILGRIM;
using namespace std;


  //using the query command
    pmDynamicBayesianNetwork *DBN;
    pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

    //loading DBN from an xml file
    DBN = D->load_from_xml("benchmarks/networks/jdDBN.xml","jdDBN");

    pmBayesianNetwork *unrolled;
    //using unroll function to compute all the DBNs till time slice 5
    unrolled=DBN->unroll(5);

    //from the list of variables in the DBN, getting the value of NodeB_t at time slice 4
    plVariable asked =unrolled->getJointDistribution().get_variables().get_variable_with_name("NodeB_t4");

    //using query function to find the value of asked variable
    plDistribution distri=unrolled->query(asked);

    cout<<distri.compile()<<endl;

