#include "queryJuggler.h"
#include <exception>

using namespace std;
using namespace PILGRIM;
/**
 * \brief Load and print a summary of the BN created in 'createJuggler.cpp', in xml.
 * Load the variables, et do queries.
 */
void queryJuggler(){
	//Avoid crashes caused by errors while manipulating xml file
	try
			{
			// Load  and print the BNJuggler from xml file
			pmBayesianNetwork bnJuggler_new("../../benchmarks/networks/jdBnJuggler.xml","jdBnJuggler");
			bnJuggler_new.summary();

			// Query the new loaded BNJuggler to compare results
			plVariablesConjunction Variables = bnJuggler_new.getVariables();
			plVariable RobotLacheBalle_new = Variables.get_variable_with_name("RobotLacheBalle");
			plVariable SystemeCamera1_new = Variables.get_variable_with_name("SystemeCamera1");
			plVariable SystemeCamera2_new = Variables.get_variable_with_name("SystemeCamera2");
			plVariable BatterieFaible_new = Variables.get_variable_with_name("BatterieFaible");

			//Sample
			cout << "Juggler_csv_from_xml.data generated in benchmarks/data/ " << endl;
			bnJuggler_new.generate_csvData("../../benchmarks/data/Juggler_csv_from_xml.data", 1000);

			plValues evidenceJuggler1_new(SystemeCamera1_new^SystemeCamera2_new);
			evidenceJuggler1_new[SystemeCamera1_new] = 0;
			evidenceJuggler1_new[SystemeCamera2_new] = 1;

            cout << "1st query" << endl;
			plDistribution pld_new =  bnJuggler_new.query(RobotLacheBalle_new, evidenceJuggler1_new);
			pld_new.tabulate();

            cout << "2nd query" << endl;
			plDistribution pld_2_new =  bnJuggler_new.query(RobotLacheBalle_new);
			pld_2_new.tabulate();
		}
	catch (const std::exception & e)
		{ 
			std::cerr <<" Error while loading from file : "<< e.what();
		}
}
