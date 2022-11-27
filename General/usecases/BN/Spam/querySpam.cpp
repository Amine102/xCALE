#include "querySpam.h"
#include <exception>

using namespace PILGRIM;
using namespace std;
void querySpam(){
	//Avoid crashes caused by errors while manipulating xml file
	try
		{
			// Load  and print the BNSpam from xml file
			pmBayesianNetwork bnSpam_new("../../benchmarks/networks/jdBnSpam.xml","jdBnSpam");
			bnSpam_new.summary();

			//Load the variables
			plVariablesConjunction Variables = bnSpam_new.getVariables();
			plVariable Spam_new = Variables.get_variable_with_name("Spam");
			plVariable Viagra_new = Variables.get_variable_with_name("Viagra");
			plVariable Polytech_new = Variables.get_variable_with_name("Polytech");
			plVariable Felicitation_new = Variables.get_variable_with_name("Felicitation");
			plVariable Adresse_new = Variables.get_variable_with_name("Adresse");


			//Sample
			bnSpam_new.generate_csvData("../../benchmarks/data/Spam_csv_from_xml.data", 1000);

			// Query P(SPAM | Polytech = True)
			 
			cout << "Query P(SPAM | Polytech = True)" << endl;
			plValues evidence_polytech(Polytech_new);
			evidence_polytech[Polytech_new] = "O";
			plDistribution pld_SPAM_Polytech = bnSpam_new.query(Spam_new, evidence_polytech);
			pld_SPAM_Polytech.tabulate();

			// Query P(SPAM | Viagra = True)
			cout << "Query P(SPAM | Viagra = True)" << endl;
			plValues evidence_viagra(Viagra_new);
			evidence_viagra[Viagra_new] = "O";
			plDistribution pld_SPAM_Viagra = bnSpam_new.query(Spam_new, evidence_viagra);
			pld_SPAM_Viagra.tabulate();

		}
	catch (const std::exception & e)
		{ 
			std::cerr <<" Error while loading from file : "<< e.what();
		}
}
