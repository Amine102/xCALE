#include "noisyMaxSerialize.h"

using namespace PILGRIM;

/**
 * \brief Creates a noisyMAX bayesian network (inspired from the ProBT doc) , prints it , serializes it in a xsdl file and executes all queries on it
 */

void noisyMaxSerialize() {

  // Variable definitions
  std::vector<std::string> labels;
  labels.push_back("ko");
  labels.push_back("deg");
  labels.push_back("ok");
  const plLabelType my_type(labels);
  int i= 0;

  plVariable A("A", my_type);
  plVariable B("B", my_type);
  plVariable C("C", my_type);
  plVariable U("U", my_type);
  plVariable V("V", my_type);
  plVariable W("W", my_type);
  plVariable L("L", my_type);
  plVariable Y("Y", my_type);

  // Probability table of A
  plProbValue tableA[] = {0.5, 0.4,0.1};
  plProbTable P_A(A, tableA);


  // Probability table of B
  plProbValue tableB[] = {0.2,0.6,0.2 };
  plProbTable P_B(B, tableB);

  // Probability table of C
  plProbValue tableC[] = {0.7,0.2,0.1};
  plProbTable P_C(C, tableC);

  // Leaky
  plProbValue tableL[] = {1,0,0};
  plProbTable P_L(L, tableL);

  // Probability table of U
  plProbValue tableU_knowing_A[] = {
	  1,0,0,
	  0.2,0.7,0.1,
	  0.5,0.2,0.3
  };
  plDistributionTable P_U(U, A, tableU_knowing_A);


  // Probability table of V
  plProbValue tableV_knowing_B[] = {
	  1,0,0,
	  0.1,0.8,0.1,
	  0.1,0.1,0.8
  };
  plDistributionTable P_V(V, B, tableV_knowing_B);


  // Probability table of W
  plProbValue tableW_knowing_C[] = {
	  1,0,0,
	  0.15,0.75,0.1,
	  0.25,0.35,0.4
  };
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
  const plExternalFunctionFromC max_fun(Y, U^V^W^L,my_code,"my_max");
  const plCndDeterministic P_YgivenUVW(Y, U^V^W^L, max_fun);

  // Joint distribution
  plJointDistribution jd(A^B^C^U^V^W^L^Y, P_A*P_B*P_C*P_U*P_V*P_L*P_W*P_YgivenUVW);


  // Creation and print of the bayesian network
  pmBayesianNetwork bn(jd);
  bn.summary();
  
  
	//plComputableObjectList pCompL=bn.getComputableObjectList();
	//plComputableObject pCompO=P_YgivenUVW;



  // Create serializer
  plSerializer serial_save("serial_s");

  // Add objects to save
  std::string bn_jd="jd";
  serial_save.add_object(bn_jd,jd);
  std::string uri_file("benchmarks/networks/serialized_ABC_BN.xml");
  std::ofstream serialFile(uri_file.c_str());
  if(serialFile)
	{
		try{
			serial_save.save(serialFile);
		}
		catch ( const std::exception & e ) 
		{ 
			std::cerr <<" Error : cannot write in serialization file : "<< e.what();
		}
    }
  std::system("pause");

  // Query network
  plValues evidence(A^B^C);
  plDistribution pld;
  std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") =  ";
  pld =  bn.query(Y, evidence);
  std::cout << std::endl;
  pld.tabulate();
  std::cout    << std::endl<< std::endl;

  while(evidence.next())
  {
	  i++;
	  std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") = ";
	  pld =  bn.query(Y, evidence);
      std::cout << std::endl;
	  pld.tabulate();std::cout << std::endl<< std::endl;
  }
}