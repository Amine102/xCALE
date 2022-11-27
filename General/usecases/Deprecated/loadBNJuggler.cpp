#include "loadBNJuggler.h"


using namespace PILGRIM;

/**
 * \brief Loads a simple bayesian network (Juggler) saved in a xsdl file and executes queries on it by using directly the names of variables as we do not know the order of variables in the network
 * Loads a simple bayesian network (Juggler) saved in a xsdl file and executes queries on by using diectly the names of variables as we do not know the order of variables in the network
 */
void loadBNJuggler() {
  //load bayesian network  
  std::cout << "\n // ============================ Results using the XDSL file ==================================== // \n";

  pmBayesianNetwork loadedBnJuggler("benchmarks/networks/Juggler.xdsl");
  loadedBnJuggler.summary();

  plVariablesConjunction Variables;
  Variables = loadedBnJuggler.getVariables();
  
  plVariable variableInterrogee1 = Variables.get_variable_with_name("SV1");
  plVariable variableInterrogee2 = Variables.get_variable_with_name("SV2");

  plValues evidenceJuggler1(variableInterrogee1^variableInterrogee2);
  evidenceJuggler1[variableInterrogee1] = "Lache";
  evidenceJuggler1[variableInterrogee2] = "PasLache";

  plVariable target = Variables.get_variable_with_name("Robot");
  plDistribution pld =  loadedBnJuggler.query(target, evidenceJuggler1);
  pld.tabulate();

  plVariable target2 = Variables[1];
  plDistribution pld_2 =  loadedBnJuggler.query(target2);
  pld_2.tabulate();

  	// ================================================================
	std::cout << "\n // ============================ Results using the XML file ==================================== // \n";
	// ================================================================

	// Save the bayesian network as xml
	loadedBnJuggler.save_as_xml("benchmarks/networks/jdBnJuggler.xml","jdBnJuggler");

	// Load  and print the BNJuggler from xml file
  	pmBayesianNetwork bnJuggler_fromXml("benchmarks/networks/jdBnJuggler.xml","jdBnJuggler");
	bnJuggler_fromXml.summary();

	// Query the new loaded BNJuggler to compare results
	plVariablesConjunction Variables_fromXml;
	Variables_fromXml = bnJuggler_fromXml.getVariables();
  
	plVariable variableInterrogee1_fromXml = Variables_fromXml.get_variable_with_name("SV1");
	plVariable variableInterrogee2_fromXml = Variables_fromXml.get_variable_with_name("SV2");

	plValues evidenceJuggler1_fromXml(variableInterrogee1_fromXml^variableInterrogee2_fromXml);
	evidenceJuggler1_fromXml[variableInterrogee1_fromXml] = "Lache";
	evidenceJuggler1_fromXml[variableInterrogee2_fromXml] = "PasLache";

	plVariable target_fromXml = Variables_fromXml.get_variable_with_name("Robot");
	plDistribution pld_fromXml =  bnJuggler_fromXml.query(target_fromXml, evidenceJuggler1_fromXml);
	pld_fromXml.tabulate();

	plVariable target2_fromXml = Variables_fromXml[1];
	plDistribution pld_2_fromXml =  bnJuggler_fromXml.query(target2_fromXml);
	pld_2_fromXml.tabulate();
  
}
