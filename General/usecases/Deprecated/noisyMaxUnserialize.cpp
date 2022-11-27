#include "noisyMaxUnserialize.h"

using namespace PILGRIM;

/**
 * \brief Loads a noisyMAX bayesian network saved in a xsdl/xml file, prints it and executes queries on by using diectly the names of variables as we do not know the order of variables in the network
 */


void noisyMaxUnserialize() {

  int i= 0;

  // Serializer creation
  plSerializer serial_load("serial_l");

  // Load bayesian network
  plJointDistribution loaded_jd;
  
  std::string uri_file("benchmarks/networks/serialized_ABC_BN.xdsl");
  std::ifstream serialFile(uri_file.c_str());
    if(serialFile)
    {
		try{
			serial_load.load(uri_file);
		serial_load.get_object("jd",loaded_jd);
		}
		catch ( const std::exception & e ) 
		{ 
			std::cerr << "Error : cannot upload jd from file (Make sure the jd label is correct )\n\n";
			std::cerr << e.what(); 
		}
    }
    else
    {
        std::cout << "ERROR: Cannot open serialization file.\n" << std::endl;
        exit(0);
    }


  // Creation and print of the bayesian network
  pmBayesianNetwork bn_serialized(loaded_jd);
  bn_serialized.summary();

  // Query BN
  plVariablesConjunction Variables;
  Variables = bn_serialized.getVariables();
  
  plVariable Y = Variables.get_variable_with_name("Y");
  plVariable A = Variables.get_variable_with_name("A");
  plVariable B = Variables.get_variable_with_name("B");
  plVariable C = Variables.get_variable_with_name("C");

  plValues evidence(A^B^C);
  plDistribution pld;
  std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") =  ";
  pld =  bn_serialized.query(Y, evidence);
  std::cout << std::endl;
  pld.tabulate();
  std::cout    << std::endl<< std::endl;

  while(evidence.next())
  {
	  i++;
	  std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") = ";
	  pld =  bn_serialized.query(Y, evidence);
      std::cout << std::endl;
	  pld.tabulate();std::cout    << std::endl<< std::endl;
  }
}
