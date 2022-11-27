#include "DBN_unroll.h"

using namespace PILGRIM;


void how_to_unroll()
{
    pmDynamicBayesianNetwork *DBN2;
    pmDynamicBayesianNetwork *D2 = new pmDynamicBayesianNetwork();

    //loading DBN from an xml file
    DBN2 = D2->load_from_xml("DBN_Ayush");

    //using unroll function to display all the DBNs till time slice 5
    DBN2->unroll(5)->summary();
    DBN2->summary();

}
