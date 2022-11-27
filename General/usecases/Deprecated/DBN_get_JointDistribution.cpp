#include "DBN_get_JointDistribution.h"

using namespace PILGRIM;
using namespace std;
void how_to_get_JointDistribution()
{
 pmBayesianNetwork BN;

      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

      //loading DBN from xml file
      DBN = D->load_from_xml("DBN_Ayush");

      cout<<"\n\n--------------Joint Distribution for Initial Graph---------------\n"<<endl;

      //getting Initial Graph
      BN=DBN->getInitial_G();

      plJointDistribution jd;

      jd=BN.getJointDistribution();
      cout<<jd<<endl;

      cout<<"\n\n--------------Joint Distribution for Transition Graph-------------\n"<<endl;

      plJointDistribution jd_T;

      //getting Transition Graph
      BN=DBN->getTransition_G();

      jd_T=BN.getJointDistribution();
      cout<<jd<<endl;

}
