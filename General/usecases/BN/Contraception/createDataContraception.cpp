/* ####################################################################
* Project :                                                           *
* Function : Creation of dataset to check if the Multinet algo works  *
* cf. multinet usecase to use pmClassifier class                      *
* Author : Soline PONSARD                                             *
* (c) Copyright juin 2020                                                 *
##################################################################### */

#include "createDataContraception.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iterator>

using namespace std;
using namespace PILGRIM;

void createDataContraception() {

  vector<string> label = vector<string>();
  label.push_back("1");
  label.push_back("2");
  label.push_back("3");

  vector<string> label_f = vector<string>();
  label_f.push_back("1");
  label_f.push_back("2");
  label_f.push_back("3");
  label_f.push_back("4");


  plVariable Adresse("Adresse",plLabelType(label));

  // ########################## Value class = 1 ##########################
  // Variable definitions
  plVariable A_1("A_1", plLabelType(label_f));
  plVariable B_1("B_1", plLabelType(label_f));
  plVariable C_1("C_1", plLabelType(label_f));
  plVariable D_1("D_1", plLabelType(label_f));
  plVariable E_1("E_1", plLabelType(label_f));


  // Probability table of A
  plProbValue tableA_1[] = {0.15, 0.35, 0.1, 0.4};
  plProbTable P_A_1(A_1, tableA_1);

  // Probability table of B, which depends on A
  plProbValue tableB_knowingA_1[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_B_1( B_1,
                             A_1,
                             tableB_knowingA_1);

  // Probability table of C, which depends on B
  plProbValue tableC_knowingB_1[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_C_1( C_1,
                             B_1,
                             tableC_knowingB_1);

  // Probability table of D, which depends on C
  plProbValue tableD_knowingC_1[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_D_1( D_1,
                             C_1,
                             tableD_knowingC_1);

  // Probability table of E, which depends on B
  plProbValue tableE_knowingB_1[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_E_1( E_1,
                             B_1,
                             tableE_knowingB_1);

  // Joint distribution
  plJointDistribution jd_1( A_1^B_1^C_1^D_1^E_1,
                            P_A_1*P_B_1*P_C_1*P_D_1*P_E_1);

  //// Creation and print of the bayesian network
  pmBayesianNetwork bnData_1(jd_1);
  bnData_1.summary();

  bnData_1.generate_csvData("../../benchmarks/data/cmc_1.data", 10000);

  // ########################## Value class = 2 ##########################
  // Variable definitions
  plVariable A_2("A_2", plLabelType(label_f));
  plVariable B_2("B_2", plLabelType(label_f));
  plVariable C_2("C_2", plLabelType(label_f));
  plVariable D_2("D_2", plLabelType(label_f));
  plVariable E_2("E_2", plLabelType(label_f));


  // Probability table of A
  plProbValue tableA_2[] = {0.15, 0.35, 0.1, 0.4};
  plProbTable P_A_2(A_2, tableA_2);

  // Probability table of B, which depends on A
  plProbValue tableB_knowingA_2[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_B_2( B_2,
                             A_2,
                             tableB_knowingA_2);

  // Probability table of C, which depends on A
  plProbValue tableC_knowingA_2[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_C_2( C_2,
                             A_2,
                             tableC_knowingA_2);

  // Probability table of D, which depends on A
  plProbValue tableD_knowingA_2[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_D_2( D_2,
                             A_2,
                             tableD_knowingA_2);

  // Probability table of E, which depends on C
  plProbValue tableE_knowingC_2[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_E_2( E_2,
                             C_2,
                             tableE_knowingC_2);

  // Joint distribution
  plJointDistribution jd_2( A_2^B_2^C_2^D_2^E_2,
                            P_A_2*P_B_2*P_C_2*P_D_2*P_E_2);

  //// Creation and print of the bayesian network
  pmBayesianNetwork bnData_2(jd_2);
  bnData_2.summary();

  bnData_2.generate_csvData("../../benchmarks/data/cmc_2.data", 10000);

  // ########################## Value class = 3 ##########################
  // Variable definitions
  plVariable A_3("A_3", plLabelType(label_f));
  plVariable B_3("B_3", plLabelType(label_f));
  plVariable C_3("C_3", plLabelType(label_f));
  plVariable D_3("D_3", plLabelType(label_f));
  plVariable E_3("E_3", plLabelType(label_f));


  // Probability table of A
  plProbValue tableA_3[] = {0.15, 0.35, 0.1, 0.4};
  plProbTable P_A_3(A_3, tableA_3);

  // Probability table of B, which depends on C
  plProbValue tableB_knowingC_3[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_B_3( B_3,
                             C_3,
                             tableB_knowingC_3);

  // Probability table of C, which depends on A
  plProbValue tableC_knowingA_3[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_C_3( C_3,
                             A_3,
                             tableC_knowingA_3);

  // Probability table of D, which depends on B
  plProbValue tableD_knowingB_3[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_D_3( D_3,
                             B_3,
                             tableD_knowingB_3);

  // Probability table of E, which depends on B
  plProbValue tableE_knowingD_3[] = {
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15,
    0.2, 0.1, 0.3, 0.4,
    0.33, 0.27, 0.2, 0.2,
    0.2, 0.3, 0.15, 0.35,
    0.4, 0.1, 0.1, 0.4,
    0.5, 0.2, 0.15, 0.15
  };
  plDistributionTable P_E_3( E_3,
                             D_3,
                             tableE_knowingD_3);

  // Joint distribution
  plJointDistribution jd_3( A_3^B_3^C_3^D_3^E_3,
                            P_A_3*P_B_3*P_C_3*P_D_3*P_E_3);

  //// Creation and print of the bayesian network
  pmBayesianNetwork bnData_3(jd_3);
  bnData_3.summary();

  bnData_3.generate_csvData("../../benchmarks/data/cmc_3.data", 10000);


  // Creation 1 data

  for (unsigned int i = 0; i < 3; i++) {

    const char* new_str1 = label[i].c_str();
    char* new_str2 = "../../benchmarks/data/cmc_";
    char* new_str3 = ".data";

    int size1 = strlen(new_str1);
    int size2 = strlen(new_str2);
    int size3 = strlen(new_str3);

    char file[size1+size2+size3];
    file[0]='\0';

    strcat(file,new_str2);
    strcat(file,new_str1);
    strcat(file,new_str3);

    char* data_file = file;

    string myFile("../../benchmarks/data/data_cmc_"+label[i]+".data");

    ofstream new_file(myFile.c_str());
    ifstream myFlux(data_file);
    int n = 0;
    while (myFlux) {
      if (n == 0) {
        string str;
        getline(myFlux,str);
        new_file << str << ";\"Class\"" << endl;
      } else {
        string str;
        getline(myFlux,str);
        new_file << str << ";" << label[i] << endl;
      }
      n++;
    }
  }
}
