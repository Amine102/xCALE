#include "how_to_create_DBN15.h"

using namespace PILGRIM;
using namespace std;
void how_to_create_DBN15()
{
//DBN 1.5

    pmDynamicBayesianNetwork *DBN;
    pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

     //loading DBN from xml file
    DBN = D->load_from_xml("DBN_Ayush");
    DBN->summary();

    cout<<"-----------------DBN 1.5---------------"<<endl;

    //convertting loaded DBN into DBN 1.5
    pm15dBN *DB15 = new pm15dBN(DBN);
    DB15->summary();

}
