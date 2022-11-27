#include "noisyMaxProbt.h"
using namespace PILGRIM;

/**
 * \brief Creates a Noisy Max bayesian network (inspired from the ProBT doc) and prints it
 */
void noisyMaxProbt() {

  // Variable definitions
  //const plIntegerType my_type(0,2);
	
  // States declaration in reverse order compared to GeNIe
  std::vector<std::string> labels;
  labels.push_back("ko");
  labels.push_back("deg");
  labels.push_back("ok");
  const plLabelType my_type(labels);
  int i= 0;

  // Sympbols declaration
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
  plProbValue tableL[] = {0.8,0.15,0.05};
  plProbTable P_L(L, tableL);

  
/**
 *
 * Probability table of Y knowing his parents
 * 	GeNie Matrix		Pilgrim Matrix
 *	a	d	g			i,	h,	g,
 *  b	e	h	==>		f,	e,	d,
 *  c	f	i			c,	b,	a
 * 
**/

  // Probability table of Y knowing his parents
	//	GeNie Matrix		Pilgrim Matrix
	//  0.3	0.1	0			1	0	0
	//  0.2	0.7	0	==>		0.2	0.7	0.1
	//  0.5	0.2	1			0.5	0.2	0.3


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


  const plExternalFunction max_fun(Y, U^V^W^L, &plExternalFunction::max);
  const plCndDeterministic P_YgivenUVW(Y, U^V^W^L, max_fun);
  
  // Joint distribution
  plJointDistribution jd(A^B^C^U^V^W^L^Y, P_A*P_B*P_C*P_U*P_V*P_L*P_W*P_YgivenUVW);

  // Creation and print of the bayesian network
  pmBayesianNetwork bn(jd);
  bn.summary();

  plValues evidence(A^B^C);
  plDistribution pld;
  std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") =  ";
  pld =  bn.query(Y, evidence);
  std::cout << std::endl;
  pld.tabulate();std::cout    << std::endl<< std::endl;

  while(evidence.next())
  {
	  i++;
	  std::cout << "Query "<<i<<" :   P(Y /"<<evidence<<") = ";
	  pld =  bn.query(Y, evidence);
      std::cout << std::endl;
	  pld.tabulate();std::cout    << std::endl<< std::endl;
  }
}