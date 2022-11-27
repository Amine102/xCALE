#include "DBN15_use_predict.h"
#include "createBN.h"
using namespace PILGRIM;
using namespace std;
void how_to_use_predict()
{

    pmDynamicBayesianNetwork *DBN;
    pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();
	createBN2();
    //loading DBN from xml file
    DBN = D->load_from_xml("DBN");

    //convertting loaded DBN into DBN 1.5
    pm15dBN *DB15 = new pm15dBN(DBN);

    //creating plVariables type nodes to be sent as vectors in Predict function
    plVariable asked1 =DB15->getJointDistribution().get_variables().get_variable_with_name("NodeB_t");
    plVariable asked2 =DB15->getJointDistribution().get_variables().get_variable_with_name("NodeA_t");
	plVariable known1 = DB15->getJointDistribution().get_variables().get_variable_with_name("NodeB");
	vector<plValues> evidences;
	plValues evidence0 = plValues();
	evidence0.push_back(plValues(known1));
	evidences.push_back(evidence0);
    //creating integer vector t for time slices
        vector<plVariablesConjunction> var;
        vector<int> t;

        var.push_back(asked2);
        var.push_back(asked1);
        
        t.push_back(1);
        t.push_back(4);
		DB15->summary();
		cout << evidences.size() << endl;;
		cout << evidences[0].size() << endl;
        //using predict function to find the values of entered nodes entered time slices
		vector<plComputableObjectList > result=DB15->predict(var,t,evidences);
        
        cout<<result[0][0]<<endl;
}
