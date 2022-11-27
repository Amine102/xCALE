#include "queryUniAdmissions.h"
#include <exception>

using namespace std;
using namespace PILGRIM;
/**
 * \brief Load and print a summary of the BN created in 'createUniAdmissions.cpp', in xml.
 * Load the variables, et do queries.
 */
void queryUniAdmissions(){
    try
    {
        // Load  and print the BNUniAdmissions from xml file
        pmBayesianNetwork bnUniAdmissions_new("../../benchmarks/networks/jdBnUniAdmissions.xml","jdBnUniAdmissions");
        bnUniAdmissions_new.summary();

        // Query the new loaded BNUniAdmissions to compare results 
        plVariablesConjunction Variables = bnUniAdmissions_new.getVariables();
        plVariable ExamLevel_new = Variables.get_variable_with_name("ExamLevel");
        plVariable IQLevel_new = Variables.get_variable_with_name("IQLevel");
        plVariable Marks_new= Variables.get_variable_with_name("Marks");
        plVariable AptitudeScore_new = Variables.get_variable_with_name("AptitudeScore");
        plVariable Admission_new = Variables.get_variable_with_name("Admission");

        //Sample
        bnUniAdmissions_new.generate_csvData("../../benchmarks/data/UniAdmissions_csv_from_xml.data", 1000);

        //Evidence and Query 1
        plValues evidenceUniAdmissions1_new(Admission_new);
        evidenceUniAdmissions1_new[Admission_new]=1;

        cout << "1st query" << endl;
        plDistribution pld_1_new = bnUniAdmissions_new.query(Marks_new,evidenceUniAdmissions1_new);
        pld_1_new.tabulate();

        //Evidence and Query 2
        plValues evidenceUniAdmissions2_new(ExamLevel_new^IQLevel_new);
        evidenceUniAdmissions2_new[ExamLevel_new]=0;
        evidenceUniAdmissions2_new[IQLevel_new]=0;

        cout << "2nd query" << endl;
        plDistribution pld_2_new = bnUniAdmissions_new.query(Marks_new,evidenceUniAdmissions2_new);
        pld_2_new.tabulate();

    }
    catch(const std::exception & e)
    {
        std::cerr <<" Error while loading from file : "<< e.what();
    }
    

}