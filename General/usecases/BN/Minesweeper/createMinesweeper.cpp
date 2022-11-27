#include "createMinesweeper.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace PILGRIM;

void createMinesweeper()
{
    const double b = 0.5;
    // Variable definition

    // Edge of the lign
    vector<string> val_domain_edge;
    val_domain_edge.push_back("There_is_bomb");
    val_domain_edge.push_back("no_neighbors");
    val_domain_edge.push_back("one_neighbor");

    plLabelType labels_edge(val_domain_edge);

    // Center of the lign
    vector<string> val_domain_center;
    val_domain_center.push_back("There_is_bomb");
    val_domain_center.push_back("no_neighbors");
    val_domain_center.push_back("one_neighbor");
    val_domain_center.push_back("two_neighbors");

    plLabelType labels_center(val_domain_center);

    // All my nodes
    plVariable B1("B1", PL_BINARY_TYPE);
    plVariable B2("B2", PL_BINARY_TYPE);
    plVariable B3("B3", PL_BINARY_TYPE);
    plVariable B4("B4", PL_BINARY_TYPE);

    plVariable R1("R1", labels_edge);
    plVariable R2("R2", labels_center);
    plVariable R3("R3", labels_center);
    plVariable R4("R4", labels_edge);

    // Probability table of B1
    plProbValue tableB1[] = {1-b, b};
    plProbTable P_B1(   B1,
                        tableB1);
    
    // Probability table of B2
    plProbValue tableB2[] = {1-b, b};
    plProbTable P_B2(   B2,
                        tableB2);

    // Probability table of B3
    plProbValue tableB3[] = {1-b, b};
    plProbTable  P_B3(  B3, 
                        tableB3);

    // Probability table of B4
    plProbValue tableB4[] = {1-b, b};
    plProbTable P_B4(   B4,
                        tableB4);

    // Probability table of R1 knowing B1 and B2
    plProbValue tableR1_knowing_B1_B2[] = {
        0, 1, 0,
        0, 0, 1,
        1, 0, 0,
        1, 1, 0
    };
    plDistributionTable P_R1(   R1,
                                B1^B2,
                                tableR1_knowing_B1_B2);

    // Probability table of R2 knowing B1, B2 and B3
    plProbValue tableR2_knowing_B1_B2_B3[] = {
        0, 1, 0, 0,
        0, 0, 1, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
        1, 0, 0, 0,
        1, 0, 0, 0
    };
    plDistributionTable P_R2(   R2,
                                B1^B2^B3,
                                tableR2_knowing_B1_B2_B3);
                        

    // Probability table of R3 knowing B2, B3, B4
    plProbValue tableR3_knowing_B2_B3_B4[] = {
        0, 1, 0, 0,
        0, 0, 1, 0,
        1, 0, 0, 0,
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1,
        1, 0, 0, 0,
        1, 0, 0, 0
    };
    plDistributionTable P_R3(   R3,
                                B2^B3^B4,
                                tableR3_knowing_B2_B3_B4);

    // Probability table of R4 knowing B3 and B4
    plProbValue tableR4_knowing_B3_B4[] = {
        0, 1, 0,
        1, 0, 0,
        0, 0, 1,
        1, 0, 0
    };
    plDistributionTable P_R4(   R4,
                                B3^B4,
                                tableR4_knowing_B3_B4);

    // Joint distribution
    plJointDistribution jdMinesweeper(  B1^B2^B3^B4^R1^R2^R3^R4,
                                        P_B1*P_B2*P_B3*P_B4*P_R1*P_R2*P_R3*P_R4);

    // Creation of Bayesian network
    pmBayesianNetwork bnMinesweeper(jdMinesweeper);


    // Print Exam BN
    cout << "4*1 Minesweeper Bayesian Network : " << endl;
    bnMinesweeper.summary();
    cout << "\n\n";

    // Export as csv file
    bnMinesweeper.generate_csvData("benchmarks/data/Minesweeper_csv.data", 1000);
}