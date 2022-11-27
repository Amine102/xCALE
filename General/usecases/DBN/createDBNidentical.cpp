#include "createDBNidentical.h"

using namespace PILGRIM;

// Creates a DBN in which G0 = GT (minus the t+1 connections)
void createDBNidentical() {

  pmDynamicBayesianNetwork* DBN = createActualDBNidentical();
  
  DBN->summary(true);
  
  DBN->generate_csv_data("../../benchmarks/data/dbn", 10, 1000, true, 2);
  DBN->save_as_xml("../../benchmarks/networks/new_jdDBN_identical.xml", "jd0", "jdT");


 //using unroll function to display all the DBNs till time slice 5
  DBN->unroll(5)->summary(true);
  DBN->summary(true);
}

pmDynamicBayesianNetwork* createActualDBNidentical() {
  //Bayesian Network of the initial state
  plVariable A("NodeA_t0", PL_BINARY_TYPE);
  plVariable B("NodeB_t0", PL_BINARY_TYPE);
  plVariable C("NodeC_t0", PL_BINARY_TYPE);
  plVariable D("NodeD_t0", PL_BINARY_TYPE);

  // Probability table of A
  plProbValue tableA[] = {0.4, 0.6};
  plProbTable P_A(A, tableA);

  // Probability table of B, which depends on C
  plProbValue tableB_knowingC[] = {
    0.4, 0.6,
    0.1, 0.9
    };
  plDistributionTable P_B(B,C, tableB_knowingC);

  // Probability table of C which depends on A
  plProbValue tableC_knowingA[] = {
  0.2, 0.8,
  0.3, 0.7
  };
  plDistributionTable P_C(C,A, tableC_knowingA);

  // Probability table of D, which depends on C
  plProbValue tableD_knowingC[] = {
  0.5, 0.5,
  0.2, 0.8
  };
  plDistributionTable P_D(D,C, tableD_knowingC);

   // Joint distribution
  plJointDistribution jd(A^B^C^D, P_A*P_B*P_C*P_D);


  //Transition Bayesian Network 
  plVariable NodeA("NodeA", PL_BINARY_TYPE);
  plVariable NodeB("NodeB", PL_BINARY_TYPE);
  plVariable NodeC("NodeC", PL_BINARY_TYPE);
  plVariable NodeD("NodeD", PL_BINARY_TYPE);
  plVariable NodeA_t("NodeA_t", PL_BINARY_TYPE);
  plVariable NodeB_t("NodeB_t", PL_BINARY_TYPE);
  plVariable NodeC_t("NodeC_t", PL_BINARY_TYPE);
  plVariable NodeD_t("NodeD_t", PL_BINARY_TYPE);
  
  // Probability table of table NodeA
  plProbValue tableNodeA[] = {
    0.4, 0.6
    };
  plProbTable P_NodeA(NodeA, tableNodeA);

  // Probability table of table B
  plProbValue tableNodeB[] = {
    0.25, 0.75
    };
  plProbTable P_NodeB(NodeB, tableNodeB);

    // Probability table of table NodeC
  plProbValue tableNodeC[] = {
    0.25, 0.5
    };
  plProbTable P_NodeC(NodeC, tableNodeC);

    // Probability table of table NodeD
  plProbValue tableNodeD[] = {
    0.35, 0.65
    };
  plProbTable P_NodeD(NodeD, tableNodeD);


  // Probability table of table A_t
  plProbValue tableA_t_knowingB[] = {
    0.2, 0.8,
    0.4, 0.6
    };
  plDistributionTable P_A_t(NodeA_t, NodeB, tableA_t_knowingB);


  // Probability table of B_t Which depends on C_t
  plProbValue tableB_t_knowing_C_t[] = {
  0.2, 0.8,
  0.7, 0.3
    };
  plDistributionTable P_B_t(NodeB_t, NodeC_t, tableB_t_knowing_C_t);

  // Probability table of C_t which depends on A, A_t
  plProbValue tableC_t_knowing_A_and_A_t[] = {
  1,   0,   1,   0,
  0.3, 0.6, 0.4, 0.2,
  0.7, 0.4, 0.6, 0.8
  };
  plDistributionTable P_C_t(NodeC_t, NodeA^NodeA_t, tableC_t_knowing_A_and_A_t);

  // Probability table of D_t, which depends on C and B_t
  plProbValue tableD_t_knowingC_and_C_t[] = {
  1,   0,   1,   0,
  0.8, 0.7, 0.2, 0.6,
  0.2, 0.3, 0.8, 0.4
  };
  plDistributionTable P_D_t(NodeD_t,NodeC^NodeC_t, tableD_t_knowingC_and_C_t);

  // Joint distribution
  plJointDistribution jd_t(NodeA^NodeB^NodeC^NodeD^NodeA_t^NodeB_t^NodeC_t^NodeD_t, P_NodeA*P_NodeB*P_NodeC*P_NodeD*P_A_t*P_B_t*P_C_t*P_D_t);
  
  //Creation and printing of the bayesian network
  pmBayesianNetwork* BN_0 = new pmBayesianNetwork(jd);
  pmBayesianNetwork* BN_Transition = new pmBayesianNetwork(jd_t);
  pmDynamicBayesianNetwork* DBN = new pmDynamicBayesianNetwork(BN_0, BN_Transition);

  return DBN;
}