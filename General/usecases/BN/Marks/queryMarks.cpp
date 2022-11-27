#include "queryMarks.h"
#include <exception>

using namespace std;
using namespace PILGRIM;
/**
 * \brief Load and print a summary of the BN created in 'createMarks.cpp', in xml.
 * Load the variables, et do queries.
 */
void queryMarks(){
    try{
        // Load  and print the BNMarks from xml file
        pmBayesianNetwork bnMarks_new("../../benchmarks/networks/jdBnMarks.xml","jdBnMarks");
        bnMarks_new.summary();

        // Query the new loaded BNMarks to compare results 
        plVariablesConjunction Variables = bnMarks_new.getVariables();
        plVariable ExamType_new = Variables.get_variable_with_name("ExamType");
        plVariable LivingEnvironment_new = Variables.get_variable_with_name("LivingEnvironment");
        plVariable StudentJob_new = Variables.get_variable_with_name("StudentJob");
        plVariable ExamLevel_new = Variables.get_variable_with_name("ExamLevel");
        plVariable Sleep_new = Variables.get_variable_with_name("Sleep");
        plVariable AttentiveInClass_new = Variables.get_variable_with_name("AttentiveInClass");
        plVariable StudentLevel_new = Variables.get_variable_with_name("StudentLevel");
        plVariable StudyTime_new = Variables.get_variable_with_name("StudyTime");
        plVariable Mark_new = Variables.get_variable_with_name("Mark");

        //Sample
        bnMarks_new.generate_csvData("../../benchmarks/data/Marks_csv_from_xml.data", 1000);

        //Examples
        cout<< "Examples:" << endl;
        //Evidence and Query 1
        plValues evidenceMarks1_new(Sleep_new^ExamLevel_new);
        evidenceMarks1_new[Sleep_new]="-7H";
        evidenceMarks1_new[ExamLevel_new]="Difficult";

        cout << "1st query, P(Mark_new|" <<evidenceMarks1_new << ") = " << endl;
        plDistribution pld_1_new =bnMarks_new.query(Mark_new,evidenceMarks1_new);
        cout << endl;
        pld_1_new.tabulate();
        cout << endl << endl ;

        //Evidence and Query 2
        plValues evidenceMarks2_new(StudentLevel_new);
        evidenceMarks2_new[StudentLevel_new]=1;

        cout << "2nd query, P(Mark_new|" <<evidenceMarks2_new << ") = " << endl;
        plDistribution pld_2_new =bnMarks_new.query(Sleep_new,evidenceMarks2_new);
        cout << endl ;
        pld_2_new.tabulate();
        cout << endl << endl ;
        

        //Iterations
        cout<< "Iterations:" << endl;
        plValues evidence_Mark(LivingEnvironment_new^StudentJob_new^ExamType_new);
        plDistribution pld_new;

        unsigned int i =3;

        cout << "Query n°" << i <<" : P(Mark_new|"<< evidence_Mark <<") = ";
        pld_new = bnMarks_new.query(Mark_new, evidence_Mark);
        cout << endl;
        pld_new.tabulate();
        cout << endl << endl ;

        while (evidence_Mark.next()){
            i++;
                    cout << "Query n°" << i <<" : P(Mark_new|"<< evidence_Mark <<") = ";
        pld_new = bnMarks_new.query(Mark_new, evidence_Mark);
        cout << endl;
        pld_new.tabulate();
        cout << endl << endl ;
        }



    }
    catch(const std::exception & e)
    {
        std::cerr <<" Error while loading from file : "<< e.what();
    }

}