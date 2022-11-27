#include "createNoisyMax.h"

using namespace std;
using namespace PILGRIM;

/**
 * \brief Creates a noisyMAX bayesian network (inspired from the ProBT doc) , prints it , serializes it in a xsdl file and executes all queries on it
 */

void createNoisyMax()
{

  // Variable definitions
  std::vector<std::string> labels;
  labels.push_back("1-ok");
  labels.push_back("2-deg");
  labels.push_back("3-ko");
  sort(labels.begin(), labels.end());
  
  const plLabelType my_type(labels);
  int i = 0;

  plVariable A("A", my_type);
  plVariable B("B", my_type);
  plVariable C("C", my_type);
  plVariable U("U", my_type);
  plVariable V("V", my_type);
  plVariable W("W", my_type);
  plVariable L("L", my_type);
  plVariable Y("Y", my_type);

  // Probability table of A
  plProbValue tableA[] = {0.5, 0.4, 0.1};
  plProbTable P_A(A, tableA);

  // Probability table of B
  plProbValue tableB[] = {0.2, 0.6, 0.2};
  plProbTable P_B(B, tableB);

  // Probability table of C
  plProbValue tableC[] = {0.7, 0.2, 0.1};
  plProbTable P_C(C, tableC);

  // Leaky
//   plProbValue tableL[] = {1, 0, 0};
//   plProbTable P_L(L, tableL);
    plUniform P_L(L);

  // Probability table of U
  plProbValue tableU_knowing_A[] = {
      1, 0, 0,
      0.2, 0.7, 0.1,
      0.5, 0.2, 0.3};
  plDistributionTable P_U(U, A, tableU_knowing_A);

  // Probability table of V
  plProbValue tableV_knowing_B[] = {
      1, 0, 0,
      0.1, 0.8, 0.1,
      0.1, 0.1, 0.8};
  plDistributionTable P_V(V, B, tableV_knowing_B);

  // Probability table of W
  plProbValue tableW_knowing_C[] = {
      1, 0, 0,
      0.15, 0.75, 0.1,
      0.25, 0.35, 0.4};
  plDistributionTable P_W(W, C, tableW_knowing_C);

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

  // Create the external function
  const plExternalFunctionFromC max_fun(Y, U ^ V ^ W ^ L, my_code, "my_max");
  cout << "plexternalfnfromC " << max_fun << endl;
  const plCndDeterministic P_YgivenUVW(Y, U ^ V ^ W ^ L, max_fun);
  cout << "plCndDeterministic " << P_YgivenUVW << endl;

  // Joint distribution
  plJointDistribution jd(A ^ B ^ C ^ U ^ V ^ W ^ L ^ Y, P_A * P_B * P_C * P_U * P_V * P_L * P_W * P_YgivenUVW);

  // Creation and print of the bayesian network
  cout << endl;
  cout << "Network summary : " << endl;
  pmBayesianNetwork bn(jd);
  bn.summary();

  // Generate sample data as csv file
  cout << "NoisyMax_csv.data generated in benchmarks/data/ " << endl;
  bn.generate_csvData("../../benchmarks/data/NoisyMax+latentVars_csv.data", 1000);

  // Save the BN as xml
  bn.save_as_xml("../../benchmarks/networks/jdNoisyMax.xml", "jdNoisyMax");
  cout << "jdNoisyMax.xml generated in benchmarks/networks/ " << endl;
}