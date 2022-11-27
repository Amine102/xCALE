#include "createNoisyConditionals.h"

using namespace std;
using namespace PILGRIM;

/**
 * \brief Creates a bayesian network based on an example in 
 * "PrivBayes: Private Data Release via Bayesian Networks"
 * by Jun Zhang, Graham Cormode, Cecilia M. Procopiuc, Divesh Srivastava and Xiaokui Xiao 
 * prints it, save it in xml format, generate sample data as a csv file.
 */
void createNoisyConditionals(){
  // Variable definitions

  plVariable Age("Age", PL_BINARY_TYPE);
  plVariable Workplace("Workplace", PL_BINARY_TYPE);
  plVariable Title("Title", PL_BINARY_TYPE);
  plVariable Income("Income", PL_BINARY_TYPE);

  // Probability table of Age
  plProbValue tableAge[] = {0.4, 0.6};
  plProbTable P_Age(Age, tableAge);

  // Probability table of Workplace, which depends on Age
  plProbValue tableWorkplace_knowing_Age[] = {
    0.7, 0.3,
    0.3, 0.7
  };
  plDistributionTable P_Workplace(Workplace,
                            Age,
                            tableWorkplace_knowing_Age);

  // Probability table of Title, which depends on Age and Workplace
  plProbValue tableTitle_knowing_Age_Workplace[] = {
    0.2, 0.8,
    0.3, 0.7,
    0.25, 0.75,
    0.4, 0.6
  };
  plDistributionTable P_Title(Title,
                            Age^Workplace,
                            tableTitle_knowing_Age_Workplace);

  // Probability table of Income, which depends on Title and Workplace
  plProbValue tableIncome_knowing_Title_Workplace[] = {
    0.2, 0.8,
    0.25, 0.75,
    0.4, 0.6,
    0.3, 0.7
  };
  plDistributionTable P_Income(Income,
                            Title^Workplace,
                            tableIncome_knowing_Title_Workplace);

  // Joint distribution
  plJointDistribution jdTest(Age ^ Workplace ^ Title ^ Income,
                                P_Age * P_Workplace * P_Title * P_Income);

  // Creation of the bayesian network
  pmBayesianNetwork bnTest(jdTest);

  //Print of the bayesian network 
  cout << endl;
  cout << "Test Bayesian Network summary : " << endl;
  bnTest.summary();

  // Generate samples data as csv file
  bnTest.generate_csvData("../../benchmarks/data/NoisyConditionals_csv.data", 1000);
  cout << "NoisyConditionals_csv.data generated in benchmarks/data/ " << endl;

  
  // Save the BNWeather as xml
  bnTest.save_as_xml("../../benchmarks/networks/jdBnNoisyConditionals.xml","jdBnNoisyConditionals");
  cout << "jdBnNoisyConditionals.xml generated in benchmarks/networks/ " << endl;

}