#include "createBNJuggler.h"

using namespace PILGRIM;

/**
 * \brief Creates a simple bayesian network (inspired from the ProBT doc), prints and queries it
 */
void createBNJuggler() {
  // Variable definitions
  plVariable BatterieFaible("BatterieFaible", PL_BINARY_TYPE);
  plVariable RobotLacheBalle("RobotLacheBalle", PL_BINARY_TYPE);
  plVariable SystemeCamera1("SystemeCamera1", PL_BINARY_TYPE);
  plVariable SystemeCamera2("SystemeCamera2", PL_BINARY_TYPE);

  // Probability table of BatterieFaible
  plProbValue tableBatterieFaible[] = {0.05, 0.95};
  plProbTable P_BatterieFaible(BatterieFaible, tableBatterieFaible);

  // Probability table of RobotLacheBalle, which depends on BatterieFaible
  plProbValue tableRobotLacheBalle_knowingBatterieFaible[] = {
    0.9, 0.1,
    0.01, 0.99
  };
  plDistributionTable P_RobotLacheBalle(RobotLacheBalle,
                                        BatterieFaible,
                                        tableRobotLacheBalle_knowingBatterieFaible);

  // Probability table of SystemeCamera1, which depends on RobotLacheBalle
  plProbValue tableSystemeCamera1_knowingRobotLacheBalle[] = {
    0.7, 0.3,
    0.3, 0.7
  };
  plDistributionTable P_SystemeCamera1(SystemeCamera1,
                                       RobotLacheBalle,
                                       tableSystemeCamera1_knowingRobotLacheBalle);

  // Probability table of SystemeCamera2, which depends on RobotLacheBalle
  plProbValue tableSystemeCamera2_knowingRobotLacheBalle[] = {
    0.9, 0.1,
    0.1, 0.9
  };
  plDistributionTable P_SystemeCamera2(SystemeCamera2,
                                       RobotLacheBalle,
                                       tableSystemeCamera2_knowingRobotLacheBalle);

  // Joint distribution
  plJointDistribution jdJuggler(BatterieFaible^RobotLacheBalle^SystemeCamera1^SystemeCamera2,
                        P_BatterieFaible*P_RobotLacheBalle*P_SystemeCamera1*P_SystemeCamera2);

  // Creation and print of the bayesian network
  pmBayesianNetwork bnJuggler(jdJuggler);
  bnJuggler.summary();

  bnJuggler.generate_csvData("benchmarks/data/Juggler_csv.data", 1000);

  plValues evidenceJuggler1(SystemeCamera1^SystemeCamera2);
  evidenceJuggler1[SystemeCamera1] = 0;
  evidenceJuggler1[SystemeCamera2] = 1;

  plDistribution pld =  bnJuggler.query(RobotLacheBalle, evidenceJuggler1);
  pld.tabulate();

  plDistribution pld_2 =  bnJuggler.query(RobotLacheBalle);
  pld_2.tabulate();	
  
  // Save the BNJuggler as xml
  bnJuggler.save_as_xml("benchmarks/networks/jdBnJuggler.xml","jdBnJuggler");



	// ================================================================
	std::cout << "\n // ============================ Results using the XML file ==================================== // \n";
	// ================================================================



	// Load  and print the BNJuggler from xml file
  	pmBayesianNetwork bnJuggler_new("benchmarks/networks/jdBnJuggler.xml","jdBnJuggler");
	bnJuggler_new.summary();

	// Query the new loaded BNJuggler to compare results
	plVariablesConjunction Variables = bnJuggler_new.getVariables();
	plVariable RobotLacheBalle_new = Variables.get_variable_with_name("RobotLacheBalle");
	plVariable SystemeCamera1_new = Variables.get_variable_with_name("SystemeCamera1");
	plVariable SystemeCamera2_new = Variables.get_variable_with_name("SystemeCamera2");
	plVariable BatterieFaible_new = Variables.get_variable_with_name("BatterieFaible");


	bnJuggler_new.generate_csvData("benchmarks/data/Juggler_csv_from_xml.data", 1000);

	plValues evidenceJuggler1_new(SystemeCamera1_new^SystemeCamera2_new);
	evidenceJuggler1_new[SystemeCamera1_new] = 0;
	evidenceJuggler1_new[SystemeCamera2_new] = 1;

	plDistribution pld_new =  bnJuggler_new.query(RobotLacheBalle_new, evidenceJuggler1_new);
	pld_new.tabulate();

	plDistribution pld_2_new =  bnJuggler_new.query(RobotLacheBalle_new);
	pld_2_new.tabulate();
}