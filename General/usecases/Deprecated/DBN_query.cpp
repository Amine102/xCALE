#include "DBN_query.h"

using namespace PILGRIM;
using namespace std;

void how_to_query()
{
  //using the query command
    pmDynamicBayesianNetwork *DBN2;
    pmDynamicBayesianNetwork *D2 = new pmDynamicBayesianNetwork();

    //loading DBN from an xml file
    DBN2 = D2->load_from_xml("DBN_Ayush");

    pmBayesianNetwork *unrolled;
    //using unroll function to compute all the DBNs till time slice 5
    unrolled=DBN2->unroll(5);

    //from the list of variables in the DBN, getting the value of NodeB_t at time slice 4
    plVariable asked =unrolled->getJointDistribution().get_variables().get_variable_with_name("NodeB_t4");

    //using query function to find the value of asked variable
    plDistribution distri=unrolled->query(asked);

    cout<<distri.compile()<<endl;

}
