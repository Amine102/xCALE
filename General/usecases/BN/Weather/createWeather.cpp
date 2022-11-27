#include "createWeather.h"

using namespace std;
using namespace PILGRIM;
/**
 * \brief Creates a simple bayesian network of 3 variables
 * prints it, save it in xml format, generate sample data as a csv file.
 * This usecase was created in order to test the new method of incremental learning
 */
void createWeather(){

  // Variable definitions

  //0 for rain and 1 for sun
  plVariable Today("Today", PL_BINARY_TYPE);
  plVariable Tomorrow("Tomorrow", PL_BINARY_TYPE);
  //0:Take the bus, 1:Walk
  plVariable Bus("Bus", PL_BINARY_TYPE);

  // Probability table of Today
  plProbValue tableToday[] = {0.4, 0.6};
  plProbTable P_Today(Today, tableToday);

  // Probability table of Tomorrow, which depends on Today
  plProbValue tableTomorrow_knowingToday[] = {
    0.2, 0.8,
    0.5, 0.5
  };
  plDistributionTable P_Tomorrow(Tomorrow,
                            Today,
                            tableTomorrow_knowingToday);

  // Probability table of Bus, which depends on Today
  plProbValue tableBus_knowingToday[] = {
    0.9, 0.1,
    0.3, 0.7
  };
  plDistributionTable P_Bus(Bus,
                            Today,
                            tableBus_knowingToday);


  // Joint distribution
  plJointDistribution jdWeather(Today ^ Tomorrow ^ Bus,
                                P_Today * P_Tomorrow * P_Bus);

  // Creation and print of the bayesian network
  pmBayesianNetwork bnWeather(jdWeather);
  cout << endl;
  cout << "Network summary : " << endl;
  bnWeather.summary();
                                
  // Generate samples data as csv file
  bnWeather.generate_csvData("../../benchmarks/data/Weather_csv.data", 100);
  cout << "Weather_csv.data generated in benchmarks/data/ " << endl;

  
  // Save the BNWeather as xml
  bnWeather.save_as_xml("../../benchmarks/networks/jdBnWeather.xml","jdBnWeather");
  cout << "jdBnWeather.xml generated in benchmarks/networks/ " << endl;


}