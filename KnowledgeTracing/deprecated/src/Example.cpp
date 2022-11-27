#include "../header/Example.h"

#include <pilgrim/general/pmBayesianNetwork.h>

using namespace PILGRIM;

void createBN() {
  // Variable definitions
  plSymbol A("A", PL_BINARY_TYPE);
  plSymbol B("B", PL_BINARY_TYPE);
  plSymbol C("C", PL_BINARY_TYPE);
  plSymbol D("D", PL_BINARY_TYPE);
  plSymbol E("E", PL_BINARY_TYPE);

  // Probability table of A
  plProbValue tableA[] = {0.4, 0.6};
  plProbTable P_A(A, tableA);

  // Probability table of B
  plProbValue tableB[] = {0.18, 0.82};
  plProbTable P_B(B, tableB);

  // Probability table of C
  plProbValue tableC[] = {0.75, 0.25};
  plProbTable P_C(C, tableC);

  // Probability table of D, which depends on A and B
  plProbValue tableD_knowingA_B[] = {
    0.6, 0.4,
    0.3, 0.7,
    0.1, 0.9,
    0.5, 0.5
  };
  plDistributionTable P_D(D, A^B, tableD_knowingA_B);

  // Probability table of E, which depends on C and D
  plProbValue tableE_knowingC_D[] = {
    0.59, 0.41,
    0.25, 0.75,
    0.8, 0.2,
    0.35, 0.65
  };
  plDistributionTable P_E(E, C^D, tableE_knowingC_D);

  // Joint distribution
  plJointDistribution jd(A^B^C^D^E, P_A*P_B*P_C*P_D*P_E);

  // Creation and print of the bayesian network
  pmBayesianNetwork bn(jd);
  bn.summary();
}