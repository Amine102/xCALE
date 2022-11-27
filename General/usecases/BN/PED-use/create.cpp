#include "create.h"
#include <iostream>

using namespace std;
using namespace PILGRIM;

void create()
{
    // Create labels
    vector<string> PED_labels;
    PED_labels.push_back("oui");
    PED_labels.push_back("non");

    vector<string> imc_labels;
    imc_labels.push_back("p30");
    imc_labels.push_back("m30");

    vector<string> bf_labels; //body fat
    bf_labels.push_back("m15%");
    bf_labels.push_back("p15%");

    vector<string> top3_labels; //place sur le podium
    top3_labels.push_back("oui");
    top3_labels.push_back("non");

    vector<string> cheated_labels; //avait t-il ete teste positif auparavant ?
    cheated_labels.push_back("oui");
    cheated_labels.push_back("non");

    // Create Variables
    plVariable PED("PED", plLabelType(PED_labels));
    plVariable imc("imc", plLabelType(imc_labels));
    plVariable bf("bf", plLabelType(bf_labels));
    plVariable top3("top3", plLabelType(top3_labels));
    plVariable cheated("cheated", plLabelType(cheated_labels));
    

    // Probability table of imc
    plProbValue tableImc[] = { 0.1, 0.9 };
    plProbTable P_Imc( imc, tableImc);

    // Probability table of bf
    plProbValue tableBf[] = { 0.9, 0.1 };
    plProbTable P_Bf( bf, tableBf);


    // Probability table of cheated
    plProbValue tableCheated[] = { 0.1, 0.9 };
    plProbTable P_Cheated( cheated,
                        tableCheated);

     // Probability table PED (from imc / bf / cheated)
     plProbValue tablePED_knowing_imc_bf_cheated[] = {
         0.99, 0.01,
         0.7, 0.3,
         0.7, 0.3,
         0.7, 0.3,
         0.99, 0.01,
         0.5, 0.5,
         0.5, 0.5,
         0.5, 0.5
     };
     plDistributionTable P_PED( PED,
                                  imc ^ bf ^ cheated,
                                  tablePED_knowing_imc_bf_cheated);


     // Probability table top3 (from PED)
     plProbValue tableTop3_knowing_PED[] = {
         0.9, 0.1,
         0.5, 0.5
     };
     plDistributionTable P_Top3( top3, PED,
                            tableTop3_knowing_PED);

     
    // Joint distribution
    plJointDistribution jd( imc ^ bf ^ cheated ^ PED ^ top3,
                                       P_Imc * P_Bf * P_Cheated * P_PED * P_Top3);

    // Creation of Bayesian network
    pmBayesianNetwork bn(jd);

    // Print BN
    cout << "PED-use Bayesian Network : " << endl;
    bn.summary();
    cout << "\n\n";

    //Exporting as csv file
    bn.generate_csvData("PED-use_csv.data", 10000);
    cout << "PED-use_csv.data generated in current repository" << endl;

    // Save the bn as xml
    bn.save_as_xml("PED-use.xml", "PED-use");
    cout << "PED-use.xml generated in current repository" << endl;
}
