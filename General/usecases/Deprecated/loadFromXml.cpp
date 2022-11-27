#include "loadFromXml.h"
#include <exception>

using namespace PILGRIM;


void loadFromXml() {

	int i=0;
	const std::string uri_file="benchmarks/networks/noisyMax_ABC_ok-ko-deg.xml";
	try
		{
			// Creation and print of the bayesian network using the serialized jd
			pmBayesianNetwork bn(uri_file,"jd");
			bn.summary();

			// Query BN
			plVariablesConjunction Variables;
			Variables = bn.getVariables();
			plVariable Y = Variables.get_variable_with_name("Y");
			plVariable A = Variables.get_variable_with_name("A");
			plVariable B = Variables.get_variable_with_name("B");
			plVariable C = Variables.get_variable_with_name("C");

			plValues evidence(A^B^C);
			plDistribution pld;
			std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") =  ";
			pld =  bn.query(Y, evidence);
			std::cout << std::endl;
			pld.tabulate();
			std::cout<< std::endl<< std::endl;

			while(evidence.next())
			{
				i++;
				std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") = ";
				pld =  bn.query(Y, evidence);
				std::cout << std::endl;
				pld.tabulate();std::cout << std::endl<< std::endl;
			}
  	}
	catch ( const std::exception & e )
	{ 
		std::cerr <<" Error while loading from file : "<< e.what();
	}
}