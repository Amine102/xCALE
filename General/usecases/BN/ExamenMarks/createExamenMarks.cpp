#include "createExamenMarks.h"
#include <iostream>

using namespace std;
using namespace PILGRIM;

void createExamenMarks()
{
    // Create labels
    vector<string> difficulties_labels;
    difficulties_labels.push_back("difficult");
    difficulties_labels.push_back("easy");

    vector<string> qi_labels;
    qi_labels.push_back("+100qi");
    qi_labels.push_back("-100qi");

    vector<string> time_labels;
    time_labels.push_back("+5h");
    time_labels.push_back("-5h");

    vector<string> level_labels;
    level_labels.push_back("high");
    level_labels.push_back("low");

    vector<string> marks_labels;
    marks_labels.push_back("A");
    marks_labels.push_back("B");
    marks_labels.push_back("C");

    vector<string> health_labels;
    health_labels.push_back("bad");
    health_labels.push_back("good");

    vector<string> party_labels;
    party_labels.push_back("no");
    party_labels.push_back("yes");

    vector<string> sick_labels;
    sick_labels.push_back("no");
    sick_labels.push_back("yes");

    vector<string> validation_labels;
    validation_labels.push_back("no");
    validation_labels.push_back("yes");

    // Create Variables
    plVariable Qi("Qi", plLabelType(qi_labels));
    plVariable Time("Time", plLabelType(time_labels));
    plVariable Difficulties("Difficulties", plLabelType(difficulties_labels));
    plVariable Level("Level", plLabelType(level_labels));
    plVariable Marks("Marks", plLabelType(marks_labels));
    plVariable Health("Health", plLabelType(health_labels));
    plVariable Party("Party", plLabelType(party_labels));
    plVariable Sick("Sick", plLabelType(sick_labels));
    plVariable Validation("Validation", plLabelType(validation_labels));


    // Probability table of Difficulties
    plProbValue tableDifficulties[] = { 0.4, 0.6 };
    plProbTable P_Difficulties( Difficulties,
                                tableDifficulties);

    // Probability table of QI
    plProbValue tableQI[] = { 0.6, 0.4 };
    plProbTable P_Qi( Qi,
                      tableQI);

    // Probability table of time
    plProbValue tableTime[] = { 0.6, 0.4 };
    plProbTable P_Time( Time,
                        tableTime);

    // Probability table of sick
    plProbValue tableSick[] = { 0.3, 0.7 };
    plProbTable P_Sick( Sick,
                        tableSick);

    // Probability table of party
    plProbValue tableParty[] = { 0.5, 0.5 };
    plProbTable P_Party( Party,
                         tableParty);

     // Probability table Level (from Time / Qi)
     plProbValue tableLevel_knowing_Time_Qi[] = {
         0.8, 0.2,
         0.6, 0.4,
         0.6, 0.4,
         0.3, 0.7,
     };
     plDistributionTable P_Level( Level,
                                  Time ^ Qi,
                                  tableLevel_knowing_Time_Qi);

     // Probability table Health (from Party / Sick)
     plProbValue tableHealth_knowing_Party_Sick[] = {
         0.2, 0.8,
         0.3, 0.7,
         0.3, 0.7,
         0.8, 0.2,
     };
     plDistributionTable P_Health( Health,
                                   Party ^ Sick,
                                   tableHealth_knowing_Party_Sick);

    // Probability table Marks (from Difficulties / Level / Health)
    plProbValue tableMarks_knowing_Difficulties_Level_Health[] = {
        0.3, 0.5, 0.2,
        0.4, 0.5, 0.1,
        0.1, 0.5, 0.4,
        0.1, 0.4, 0.5,
        0.55, 0.35, 0.1,
        0.6, 0.3, 0.1,
        0.2, 0.4, 0.2,
        0.3, 0.5, 0.2,
    };
    plDistributionTable P_Marks( Marks,
                                 Difficulties ^ Level ^ Health,
                                 tableMarks_knowing_Difficulties_Level_Health);

     // Probability table Validation (from Marks)
     plProbValue tableValidation_knowing_Marks[] = {
         0.8, 0.2,
         0.6, 0.4,
         0.2, 0.8,
     };
     plDistributionTable P_Validation( Validation,
                                       Marks,
                                       tableValidation_knowing_Marks);

    // Joint distribution
    plJointDistribution jdExamenMarks( Difficulties ^ Level ^ Marks ^ Health ^ Time ^ Qi ^ Sick ^ Party ^ Validation,
                                       P_Difficulties * P_Level * P_Marks * P_Health * P_Time * P_Qi * P_Sick * P_Party * P_Validation);

    // Creation of Bayesian network
    pmBayesianNetwork bnExamenMarks(jdExamenMarks);

    // Print Exam BN
    cout << "ExamenMarks Bayesian Network : " << endl;
    bnExamenMarks.summary();
    cout << "\n\n";

    //Exporting as csv file
    bnExamenMarks.generate_csvData("../../benchmarks/data/ExamenMarks_csv.data", 10000);
    cout << "ExamenMarks_csv.data generated in benchmarks/data/ " << endl;

    // Save the bnExamenMarks as xml
    bnExamenMarks.save_as_xml("../../benchmarks/networks/examenMarks.xml", "examenMarks");
    cout << "examenMarks.xml generated in benchmarks/networks/ " << endl;
}
