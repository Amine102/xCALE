#include "DBN15_getOutgoingInterface.h"

using namespace PILGRIM;
using namespace std;
void how_to_getOutgoingInterface()
{
      
      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

      DBN = D->load_from_xml("DBN_Ayush");

   
      pm15dBN *DB15 = new pm15dBN(DBN);

      vector<plVariable> result=DB15->getOutgoingInterface();
      
      for(int i=0;i<result.size();i++)
      cout<<result[i]<<endl;


}
