    
#include "DBN_get_initial_and_Transition_graphs.h"

using namespace PILGRIM;
using namespace std;
void how_to_get_initial_and_Transitian_graphs()
{
      pmBayesianNetwork BN;
      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();
      //loading DBN from xml file
      DBN = D->load_from_xml("DBN_Ayush");

      cout<<"--------------------------Initial Graph--------------------------------"<<endl;

      BN=DBN->getInitial_G();
      BN.summary();
    

      cout<<"--------------------------Transition Graph------------------------------"<<endl;
      BN=DBN->getTransition_G();
      BN.summary();
}
