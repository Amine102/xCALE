#include "queryNoisyMax.h"

using namespace PILGRIM;
using namespace std;

void queryNoisyMax(){

  unsigned int i = 0;

  pmBayesianNetwork bn("../../benchmarks/networks/jdNoisyMax.xml","jdNoisyMax");
  bn.summary();

  cout<<"xml loaded"<<endl;
  // Query BN
  plVariablesConjunction Variables;
  Variables = bn.getVariables();
  
  plVariable Y = Variables.get_variable_with_name("Y");
  plVariable A = Variables.get_variable_with_name("A");
  plVariable B = Variables.get_variable_with_name("B");
  plVariable C = Variables.get_variable_with_name("C");

  plValues evidence(A^B^C);
  plDistribution pld;
  std::cout << "Query "<< i <<" :   P(Y /"<<evidence<<") =  ";
  pld =  bn.query(Y, evidence);
  std::cout << std::endl;
  pld.tabulate();
  std::cout << std::endl<< std::endl;

  while(evidence.next())
  {
	  i++;
	  std::cout << "Query "<< i <<" :   P(Y /"<<evidence<<") = ";
	  pld =  bn.query(Y, evidence);
    std::cout << std::endl;
	  pld.tabulate();
    std::cout << std::endl<< std::endl;
  }
}
