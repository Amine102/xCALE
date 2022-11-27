#include "NewDBN.h"

using namespace PILGRIM;
using namespace std;
void NewDBN()
{



/*
 how_to_create_BN
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


  // Second Bayesian Network

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
  plProbValue tableD_t_knowingC_and_B_t[] = {
  1,   0,   1,   0,
  0.8, 0.7, 0.2, 0.6,
  0.2, 0.3, 0.8, 0.4
  };
  plDistributionTable P_D_t(NodeD_t,NodeC^NodeB_t, tableD_t_knowingC_and_B_t);

  // Joint distribution
  plJointDistribution jd_t(NodeA^NodeB^NodeC^NodeD^NodeA_t^NodeB_t^NodeC_t^NodeD_t, P_NodeA*P_NodeB*P_NodeC*P_NodeD*P_A_t*P_B_t*P_C_t*P_D_t);

  */
  
  /* Creation and printing of the bayesian network
  pmBayesianNetwork* BNT_0=new pmBayesianNetwork(jd);

  pmBayesianNetwork* BNTransition=new pmBayesianNetwork(jd_t);

  pmDynamicBayesianNetwork* DBNtest=new pmDynamicBayesianNetwork(BNT_0, BNTransition);
  //DBNtest->summary();


  //Saving Dynaminc Bayesian Network

  DBNtest->serializeDBN("DBN_Ayush");
  

  //Loading Saved Bayesian Network

  
    pmDynamicBayesianNetwork *DBN2;
    pmDynamicBayesianNetwork *D2 = new pmDynamicBayesianNetwork();

    DBN2 = D2->load_from_xml("DBN_Ayush");
      cout<<"\n\n--------------Joint Distribution for Initial Graph---------------\n"<<endl;
    DBN2->unroll(5)->summary();
    DBN2->summary();
    //test->summary();

    cout<< "Phase 2"<< endl;




    //using the query command

    pmBayesianNetwork *unrolled;
    unrolled=DBN2->unroll(5);
    plVariable asked =unrolled->getJointDistribution().get_variables().get_variable_with_name("NodeB_t4");
    plDistribution distri=unrolled->query(asked);

    cout<<distri.compile()<<endl;

    */

    /*DBN 1.5

    pmDynamicBayesianNetwork *DBN;
    pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

    DBN = D->load_from_xml("DBN_Ayush");
    //DBN->summary();

    //cout<<"-----------------DBN 1.5---------------"<<endl;


    
    pm15dBN *DB15 = new pm15dBN(DBN);
    //DB15->summary();

    */

    /*How_to_use_predict

    pmDynamicBayesianNetwork *DBN;
    pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

    DBN = D->load_from_xml("DBN_Ayush");
    pm15dBN *DB15 = new pm15dBN(DBN);
    plVariable asked1 =DB15->getJointDistribution().get_variables().get_variable_with_name("NodeB_t");
    plVariable asked2 =DB15->getJointDistribution().get_variables().get_variable_with_name("NodeA_t");

        vector<plVariable> var;
        vector<int> t;

        var.push_back(asked1);
        var.push_back(asked2);
        cout<<"\nEnter Time Slice: "<<endl;
        t.push_back(2);
        t.push_back(4);
        vector<vector<plDistribution>> result=DB15->predict(var,t);
        
        cout<<result[0][1].compile()<<endl;

    
    */
  
      /*how_to_use_generate_csv_data

      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

      DBN = D->load_from_xml("DBN_Ayush");
      DBN->generate_csv_data("Random_csv_Ayush",5,3);
      //DBN->generate_csv_data("Random_csv_Ayush",5,3,,);
      //DBN->generate_csv_data("Random_csv_Ayush",5,3,0,2);


    */

    /*

      pmBayesianNetwork BN;
      

      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

      DBN = D->load_from_xml("DBN_Ayush");

      cout<<"--------------------------Initial Graph--------------------------------"<<endl;

      BN=DBN->getInitial_G();
      BN.summary();
    

      cout<<"--------------------------Transition Graph------------------------------"<<endl;
      BN=DBN->getTransition_G();
      BN.summary();


      */



    //how_to_get_JointDistribution

    /*  pmBayesianNetwork BN;

      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

      DBN = D->load_from_xml("DBN_Ayush");

      cout<<"\n\n--------------Joint Distribution for Initial Graph---------------\n"<<endl;

      BN=DBN->getInitial_G();

      plJointDistribution jd;

      jd=BN.getJointDistribution();
      cout<<jd<<endl;

      cout<<"\n\n--------------Joint Distribution for Transition Graph-------------\n"<<endl;

      plJointDistribution jd_T;

      BN=DBN->getTransition_G();

      jd_T=BN.getJointDistribution();
      cout<<jd<<endl;

*/

    //how_to_getOutgoingInterface

      pmDynamicBayesianNetwork *DBN;
      pmDynamicBayesianNetwork *D = new pmDynamicBayesianNetwork();

      DBN = D->load_from_xml("DBN_Ayush");

   
      pm15dBN *DB15 = new pm15dBN(DBN);

      vector<plVariable> result=DB15->getOutgoingInterface();
      
      for(int i=0;i<result.size();i++)
      cout<<result[i]<<endl;
}
