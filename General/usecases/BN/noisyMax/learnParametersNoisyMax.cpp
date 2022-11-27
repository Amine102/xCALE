/* NON-INCREMENTAL USING INCOMPLETE DATA */
#include "learnParametersNoisyMax.h"
#include <string>
#include <stdlib.h>
#include "Helper_Functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;
using namespace PILGRIM;

void learnParametersNoisyMax()
{
  pmBayesianNetwork bnNoisyMaxTemp("../../benchmarks/networks/jdNoisyMax.xml", "jdNoisyMax");
  pmBayesianNetwork bnToLearn(bnNoisyMaxTemp, false);

  plVariablesConjunction variables = bnToLearn.getVariables();
  cout << "Variables are " << variables << endl;

  plVariable A = variables.get_variable_with_name("A");
  plVariable B = variables.get_variable_with_name("B");
  plVariable C = variables.get_variable_with_name("C");
  plVariable L = variables.get_variable_with_name("L");
  plVariable U = variables.get_variable_with_name("U");
  plVariable V = variables.get_variable_with_name("V");
  plVariable W = variables.get_variable_with_name("W");
  plVariable Y = variables.get_variable_with_name("Y");

  std::vector<plLearnObject *> learnObjectList;
  learnObjectList.push_back(new plLearnHistogram(A));
  learnObjectList.push_back(new plLearnHistogram(B));
  learnObjectList.push_back(new plLearnHistogram(C));
  learnObjectList.push_back(new plLearnHistogram(L));
  // ---------------------------------------------------------------------------
  plDeterministic P_U0(U, get_Label_Index(U, "1-ok")); //takes a plValue as a second parameter
  plLearnDistributionTable PL_U(U, A);

  PL_U.push(plLearnFrozenDistribution(P_U0), get_Label_Index(A, "1-ok"));
  PL_U.push(plLearnHistogram(U), get_Label_Index(A, "3-ko"));
  PL_U.push(plLearnHistogram(U), get_Label_Index(A, "2-deg"));

  learnObjectList.push_back(&PL_U);
  // ---------------------------------------------------------------------------

  plDeterministic P_V0(V, get_Label_Index(V, "1-ok")); //takes a plValue as a second parameter
  plLearnDistributionTable PL_V(V, B);
  PL_V.push(plLearnFrozenDistribution(P_V0), get_Label_Index(B, "1-ok"));
  PL_V.push(plLearnHistogram(V), get_Label_Index(B, "3-ko"));
  PL_V.push(plLearnHistogram(V), get_Label_Index(B, "2-deg"));

  learnObjectList.push_back(&PL_V);

  plDeterministic P_W0(W, get_Label_Index(W, "1-ok")); //takes a plValue as a second parameter
  plLearnDistributionTable PL_W(W, C);
  PL_W.push(plLearnFrozenDistribution(P_W0), get_Label_Index(C, "1-ok"));
  PL_W.push(plLearnHistogram(W), get_Label_Index(C, "3-ko"));
  PL_W.push(plLearnHistogram(W), get_Label_Index(C, "2-deg"));

  learnObjectList.push_back(&PL_W);

  // C source code function :
  std::string my_code =
      "void my_max(double* output, unsigned int output_size,\n"
      " double* input, unsigned int input_size)\n"
      "{\n"
      " double* vectMax = output;\n"
      " double* vectProb = input;\n"
      "\n"
      " unsigned int n_probas; /* Number of values */\n"
      " double value_max=vectProb[0]; \n"
      " unsigned int i;\n"
      " \n"
      " n_probas = input_size; /* Get the number of values */\n"
      "\n"
      " for(i=0;i<n_probas;i++) /* Find the max value */\n"
      " if (vectProb[i]>value_max) value_max=vectProb[i];\n"
      " vectMax[0]=value_max; \n"
      "}\n";

  cout << "starting creating dynamic code" << endl;
  const plExternalFunctionFromC max_fun(Y, U ^ V ^ W ^ L, my_code, "my_max");
  const plCndDeterministic P_YgivenUVWL(Y, U ^ V ^ W ^ L, max_fun);

  learnObjectList.push_back(new plLearnFrozenDistribution(P_YgivenUVWL));

  //   //CUT from the 4th to the 7th column in the data file
  string str = "cut -f -3,8 -d';' ../../benchmarks/data/NoisyMax+latentVars_csv.data > ../../benchmarks/data/NoisyMax_csv.data";
  const char *command = str.c_str();
  ::system(command);

  char *data_file = "../../benchmarks/data/NoisyMax_csv.data";
  size_t window_size = 1;
  size_t numberOfColumns = 4;
  pmCSVDataSet *data = new pmCSVDataSet(data_file, window_size, numberOfColumns, ';', A ^ B ^ C ^ Y);
  data->summary();
  cout << "after data summary" << endl;

  bnToLearn.setLearnObject(learnObjectList);

  bnToLearn.learnParameters(data);
  cout << "printing summary after learning non-incrementally" << endl;
  bnToLearn.summary(true);
  
  bnToLearn.save_as_xml("../../benchmarks/networks/LearnedNoisyMax.xml", "jdNoisyMaxLearned");
}
