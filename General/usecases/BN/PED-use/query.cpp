#include "query.h"
#include <iostream>

using namespace std;
using namespace PILGRIM;

void query()
{
    try
    {
        unsigned int i = 0;
        // Load  and print the BN from xml file
        pmBayesianNetwork bn("PED-use.xml", "PED-use");
        bn.summary();

        // Query the new loaded BN to compare results
        plVariablesConjunction Variables = bn.getVariables();
        plVariable PED = Variables.get_variable_with_name("PED");
        plVariable imc = Variables.get_variable_with_name("imc");
        plVariable bf = Variables.get_variable_with_name("bf");
        plVariable cheated = Variables.get_variable_with_name("cheated");
        plVariable top3 = Variables.get_variable_with_name("top3");
        
        plDistribution pld;

        //Evidence and Query 1
       plValues evidence1(imc^bf);
       evidence1[imc]="p30";
       evidence1[bf]="m15%";

       cout << "1st query, P(PED|" <<evidence1 << ") = " << endl;
       plDistribution pld_1 =bn.query(PED,evidence1);
       cout << endl;
       pld_1.tabulate();
       cout << endl << endl ;

       //Evidence and Query 2
      plValues evidence2(imc^bf^cheated);
      evidence2[imc]="p30";
      evidence2[bf]="m15%";
      evidence2[cheated]="non";

      cout << "2nd query, P(PED|" <<evidence2 << ") = " << endl;
      plDistribution pld_2 =bn.query(PED,evidence2);
      cout << endl;
      pld_2.tabulate();
      cout << endl << endl ;

      //Evidence and Query 3
     plValues evidence3(top3);
     evidence3[top3]="oui";

     cout << "3rd query, P(PED|" <<evidence3 << ") = " << endl;
     plDistribution pld_3 =bn.query(PED,evidence3);
     cout << endl;
     pld_3.tabulate();
     cout << endl << endl ;

     //Evidence and Query 4
     plValues evidence4(cheated);
     evidence4[cheated]="oui";

     cout << "4th query, P(PED|" <<evidence4 << ") = " << endl;
     plDistribution pld_4 =bn.query(PED,evidence4);
     cout << endl;
     pld_4.tabulate();
     cout << endl << endl ;
     

    }
    catch (const std::exception &e)
    {
        std::cerr << " Error while loading from file : " << e.what();
    }
}
