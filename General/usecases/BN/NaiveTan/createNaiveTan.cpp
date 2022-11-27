/* ########################################################################
* Project :                                                               *
* Function : Create a naive bayes augmented to check algo TAN             *
* cf. SingleBN usecase to use pmClassifier class                          *
* Author : Soline PONSARD                                                 *
* (c) Copyright juin 2020                                                     *
######################################################################### */

#include "createNaiveTan.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace PILGRIM;

void createNaiveTan()
{
    // Variable definition
    plVariable A("A", PL_BINARY_TYPE);
    plVariable B("B", PL_BINARY_TYPE);
    plVariable C("C", PL_BINARY_TYPE);
    plVariable D("D", PL_BINARY_TYPE);
    plVariable E("E", PL_BINARY_TYPE);
    plVariable F("F", PL_BINARY_TYPE);
    plVariable G("G", PL_BINARY_TYPE);
    plVariable H("H", PL_BINARY_TYPE);
    plVariable Class("Class", PL_BINARY_TYPE);

    // Probability table of Class
    plProbValue tableClass[] = {0.5, 0.5};
    plProbTable P_Class(  Class,
                          tableClass);

    // Probability table of A knowing class
    plProbValue tableA_knowing_Class[] = {
        0.2, 0.8,
        1, 0
    };
    plDistributionTable P_A(   A,
                               Class,
                               tableA_knowing_Class);

   // Probability table of B knowing class and A
   plProbValue tableB_knowing_Class_A[] = {
       0.1, 0.9,
       0.4, 0.6,
       1, 0,
       0.8, 0.2
   };
   plDistributionTable P_B(   B,
                              Class^A,
                              tableB_knowing_Class_A);

   // Probability table of C knowing class and B
   plProbValue tableC_knowing_Class_B[] = {
       0.5, 0.5,
       0.2, 0.8,
       0.7, 0.3,
       1, 0
   };
   plDistributionTable P_C(   C,
                              Class^B,
                              tableC_knowing_Class_B);

    // Probability table of D knowing Class
    plProbValue tableD_knowing_Class[] = {
        0.2, 0.8,
        0.5, 0.5
    };
    plDistributionTable P_D(   D,
                               Class,
                               tableD_knowing_Class);

    // Probability table of E knowing Class and D
    plProbValue tableE_knowing_Class_D[] = {
        0.2, 0.8,
        0.5, 0.5,
        1, 0,
        0.4, 0.6
    };
    plDistributionTable P_E(   E,
                               Class^D,
                               tableE_knowing_Class_D);

    // Probability table of F knowing Class and A
    plProbValue tableF_knowing_Class_A[] = {
        0.2, 0.8,
        0.5, 0.5,
        1, 0,
        0.4, 0.6
    };
    plDistributionTable P_F(   F,
                               Class^A,
                               tableF_knowing_Class_A);

   // Probability table of G knowing Class, C and D
   plProbValue tableG_knowing_Class_C_D[] = {
       0.2, 0.8,
       0.5, 0.5,
       1, 0,
       0.4, 0.6,
       0.2, 0.8,
       0.5, 0.5,
       1, 0,
       0.4, 0.6
   };
   plDistributionTable P_G(   G,
                              Class^C^D,
                              tableG_knowing_Class_C_D);

   // Probability table of H knowing Class and B
   plProbValue tableH_knowing_Class_B[] = {
       0.2, 0.8,
       0.5, 0.5,
       1, 0,
       0.4, 0.6
   };
   plDistributionTable P_H(   H,
                              Class^B,
                              tableH_knowing_Class_B);


    // Joint distribution
    plJointDistribution jdNaiveTan(  A^B^C^D^E^F^G^H^Class,
                                     P_A*P_B*P_C*P_D*P_E*P_F*P_G*P_H*P_Class);

    // Creation of Bayesian network
    pmBayesianNetwork bnNaiveTan(jdNaiveTan);

    // Print Exam BN
    bnNaiveTan.summary();

    // Export as csv file
    bnNaiveTan.generate_csvData("../../benchmarks/data/NaiveTan_csv1.data", 10000);
    bnNaiveTan.generate_csvData("../../benchmarks/data/NaiveTan_csv2.data", 50000);
    bnNaiveTan.generate_csvData("../../benchmarks/data/NaiveTan_csv3.data", 1000);

}
