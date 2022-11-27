#include "createBN.h"

using namespace PILGRIM;

/**
 * \brief Creates a simple bayesian network (inspired from the ProBT doc) and prints it
 */
void createBN() {
  // Variable definitions
  plVariable A("A", PL_BINARY_TYPE);
  plVariable B("B", PL_BINARY_TYPE);
  plVariable C("C", PL_BINARY_TYPE);
  plVariable D("D", PL_BINARY_TYPE);
  plVariable E("E", PL_BINARY_TYPE);

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



void createBN2() {



  plVariable A("NodeA_t0", PL_BINARY_TYPE);
  plVariable B("NodeB_t0", PL_BINARY_TYPE);
  plVariable C("NodeC_t0", PL_BINARY_TYPE);
  plVariable D("NodeD_t0", PL_BINARY_TYPE);
  //plVariable E("E", PL_BINARY_TYPE);

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

  plVariable NodeA("NodeA", PL_BINARY_TYPE);
  plVariable NodeB("NodeB", PL_BINARY_TYPE);
  plVariable NodeC("NodeC", PL_BINARY_TYPE);
  plVariable NodeD("NodeD", PL_BINARY_TYPE);
  plVariable NodeA_T("NodeA_t", PL_BINARY_TYPE);
  plVariable NodeB_T("NodeB_t", PL_BINARY_TYPE);
  plVariable NodeC_T("NodeC_t", PL_BINARY_TYPE);
  plVariable NodeD_T("NodeD_t", PL_BINARY_TYPE);
  
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


  // Probability table of table A_T
  plProbValue tableA_T_knowingB[] = {
    0.2, 0.8,
    0.4, 0.6
    };
  plDistributionTable P_A_T(NodeA_T, NodeB, tableA_T_knowingB);


  // Probability table of B_T Which depends on C_T
  plProbValue tableB_T_knowing_C_T[] = {
  0.2, 0.8,
  0.7, 0.3
    };
  plDistributionTable P_B_T(NodeB_T, NodeC_T, tableB_T_knowing_C_T);

  // Probability table of C_T which depends on A, A_T
  plProbValue tableC_T_knowing_A_and_A_T[] = {
  1,   0,   1,   0,
  0.3, 0.6, 0.4, 0.2,
  0.7, 0.4, 0.6, 0.8
  };
  plDistributionTable P_C_T(NodeC_T, NodeA^NodeA_T, tableC_T_knowing_A_and_A_T);

  // Probability table of D_T, which depends on C and B_T
  plProbValue tableD_T_knowingC_and_B_T[] = {
  1,   0,   1,   0,
  0.8, 0.7, 0.2, 0.6,
  0.2, 0.3, 0.8, 0.4
  };
  plDistributionTable P_D_T(NodeD_T,NodeC^NodeB_T, tableD_T_knowingC_and_B_T);

  // Joint distribution
  plJointDistribution jd_T(NodeA^NodeB^NodeC^NodeD^NodeA_T^NodeB_T^NodeC_T^NodeD_T, P_NodeA*P_NodeB*P_NodeC*P_NodeD*P_A_T*P_B_T*P_C_T*P_D_T);

  // Creation and print of the bayesian network
  pmBayesianNetwork* BNT_0=new pmBayesianNetwork(jd);

  pmBayesianNetwork* BNTransition=new pmBayesianNetwork(jd_T);

  pmDynamicBayesianNetwork* DBNtest=new pmDynamicBayesianNetwork(BNT_0, BNTransition);
  DBNtest->serializeDBN("DBN");
  


  /*
 
  pmDynamicBayesianNetwork d;
   d.load_from_xml("DBN_Ayush");


  // pmBayesianNetwork* BNT_0=new pmBayesianNetwork(jd0);
  // pmBayesianNetwork* BNTransition= new pmBayesianNetwork(jd_T); 

  // pmDynamicBayesianNetwork* DBNtest=new pmDynamicBayesianNetwork(BNT_0, BNTransition);

 
  d.summary();

  */




}
