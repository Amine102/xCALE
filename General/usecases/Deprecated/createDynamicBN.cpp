#include "createDynamicBN.h"
#include <ctime>


using namespace PILGRIM;

/**
 * \brief Creates a simple bayesian network (inspired from the ProBT doc) and prints it
 */
void createDynamicBN() {

  plJointDistribution jd0;
  jd0.load_from_genie("benchmarks/networks/OrateurG0.xdsl");
  pmBayesianNetwork G0(jd0);
  plJointDistribution jdt;
  jdt.load_from_genie("benchmarks/networks/OrateurGt.xdsl");
  pmBayesianNetwork Gt(jdt);
  pmDynamicBayesianNetwork dBN = pmDynamicBayesianNetwork(&G0, &Gt);
  std::cout << "Dynamic Bayesian Network" << std::endl;
  std::cout << "------------------------" << std::endl;
  //dBN.summary();
  std::cout << std::endl;  
   std::cout << "=======================" << std::endl;

   float temps_initial = clock ();
   pmBayesianNetwork* unrolled = dBN.unroll(30);
  std::cout << "Unrolled Bayesian Network" << std::endl;
  std::cout << "-------------------------" << std::endl;
  //unrolled->summary();
  float temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;

/*temps_initial = clock ();
    pmBayesianNetwork* unrolled2 = dBN.unroll2(30);
  std::cout << "Unrolled Bayesian Network(PhL)" << std::endl;
  std::cout << "-------------------------" << std::endl;
 // unrolled2->summary();
  temps_final = clock ();
std::cout << temps_final-temps_initial << std::endl;*/

}
