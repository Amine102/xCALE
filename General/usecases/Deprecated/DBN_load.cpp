#include "DBN_load.h"

using namespace PILGRIM;

void how_to_load()
{

//Loading Saved Bayesian Network

  
    pmDynamicBayesianNetwork *DBN2;
    pmDynamicBayesianNetwork *D2 = new pmDynamicBayesianNetwork();

    DBN2 = D2->load_from_xml("DBN_Ayush");
    DBN2->summary();
}
