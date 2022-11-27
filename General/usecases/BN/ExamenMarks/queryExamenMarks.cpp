#include "queryExamenMarks.h"
#include <iostream>

using namespace std;
using namespace PILGRIM;

void queryExamenMarks()
{
    try
    {
        unsigned int i = 0;
        // Load  and print the BNJuggler from xml file
        pmBayesianNetwork bnExamenMarks("../../benchmarks/networks/examenMarks.xml", "examenMarks");
        bnExamenMarks.summary();

        // Query the new loaded BNJuggler to compare results
        plVariablesConjunction Variables = bnExamenMarks.getVariables();
        plVariable Difficulties = Variables.get_variable_with_name("Difficulties");
        plVariable Level = Variables.get_variable_with_name("Level");
        plVariable Marks = Variables.get_variable_with_name("Marks");
        plVariable Health = Variables.get_variable_with_name("Health");
        plVariable Time = Variables.get_variable_with_name("Time");
        plVariable Qi = Variables.get_variable_with_name("Qi");
        plVariable Sick = Variables.get_variable_with_name("Sick");
        plVariable Party = Variables.get_variable_with_name("Party");
        plVariable Validation = Variables.get_variable_with_name("Validation");

        plDistribution pld;

        //Evidence and Query 1
       plValues evidenceMarks1_new(Difficulties^Time);
       evidenceMarks1_new[Difficulties]="easy";
       evidenceMarks1_new[Time]="+5h";

       cout << "1st query, P(Marks|" <<evidenceMarks1_new << ") = " << endl;
       plDistribution pld_1_new =bnExamenMarks.query(Marks,evidenceMarks1_new);
       cout << endl;
       pld_1_new.tabulate();
       cout << endl << endl ;

       //Evidence and Query 2
      plValues evidenceMarks2_new(Difficulties^Marks^Sick);
      evidenceMarks2_new[Difficulties]="easy";
      evidenceMarks2_new[Marks]="A";
      evidenceMarks2_new[Sick]="yes";

      cout << "2nd query, P(Party|" <<evidenceMarks2_new << ") = " << endl;
      plDistribution pld_2_new =bnExamenMarks.query(Party,evidenceMarks2_new);
      cout << endl;
      pld_2_new.tabulate();
      cout << endl << endl ;

      //Evidence and Query 3
     plValues evidenceMarks3_new(Difficulties^Marks^Sick);
     evidenceMarks3_new[Difficulties]="easy";
     evidenceMarks3_new[Marks]="C";
     evidenceMarks3_new[Sick]="no";

     cout << "3rd query, P(Party|" <<evidenceMarks3_new << ") = " << endl;
     plDistribution pld_3_new =bnExamenMarks.query(Party,evidenceMarks3_new);
     cout << endl;
     pld_3_new.tabulate();
     cout << endl << endl ;

     //Evidence and Query 4
    plValues evidenceMarks4_new(Difficulties^Party^Sick^Time^Qi);
    evidenceMarks4_new[Difficulties]="easy";
    evidenceMarks4_new[Party]="no";
    evidenceMarks4_new[Sick]="no";
    evidenceMarks4_new[Time]="+5h";
    evidenceMarks4_new[Qi]="+100";

    cout << "4th query, P(Validation|" <<evidenceMarks4_new << ") = " << endl;
    plDistribution pld_4_new =bnExamenMarks.query(Validation,evidenceMarks4_new);
    cout << endl;
    pld_4_new.tabulate();
    cout << endl << endl ;

    //Evidence and Query 5
   plValues evidenceMarks5_new(Difficulties^Party^Sick^Time^Qi);
   evidenceMarks5_new[Difficulties]="difficult";
   evidenceMarks5_new[Party]="yes";
   evidenceMarks5_new[Sick]="yes";
   evidenceMarks5_new[Time]="-5h";
   evidenceMarks5_new[Qi]="-100";

   cout << "5th query, P(Validation|" <<evidenceMarks5_new << ") = " << endl;
   plDistribution pld_5_new =bnExamenMarks.query(Validation,evidenceMarks5_new);
   cout << endl;
   pld_5_new.tabulate();
   cout << endl << endl ;

    }
    catch (const std::exception &e)
    {
        std::cerr << " Error while loading from file : " << e.what();
    }
}
