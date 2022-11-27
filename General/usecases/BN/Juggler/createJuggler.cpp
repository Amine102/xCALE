#include "createJuggler.h"

using namespace std;
using namespace PILGRIM;

/**
 * \brief Creates a simple bayesian network (inspired from the ProBT doc), 
 *prints it, save it in xml format, generate sample data as a csv file.
 */
void createJuggler() {
  // Variable definitions
  plVariable BatterieFaible("BatterieFaible", PL_BINARY_TYPE);
  plVariable RobotLacheBalle("RobotLacheBalle", PL_BINARY_TYPE);
  plVariable SystemeCamera1("SystemeCamera1", PL_BINARY_TYPE);
  plVariable SystemeCamera2("SystemeCamera2", PL_BINARY_TYPE);

  // Probability table of BatterieFaible
  plProbValue tableBatterieFaible[] = {0.05, 0.95};
  plProbTable P_BatterieFaible(BatterieFaible, tableBatterieFaible);

  // Probability table of RobotLacheBalle, which depends on BatterieFaible
  plProbValue tableRobotLacheBalle_knowingBatterieFaible[] = {
    0.9, 0.1,
    0.01, 0.99
  };
  plDistributionTable P_RobotLacheBalle(RobotLacheBalle,
                                        BatterieFaible,
                                        tableRobotLacheBalle_knowingBatterieFaible);

  // Probability table of SystemeCamera1, which depends on RobotLacheBalle
  plProbValue tableSystemeCamera1_knowingRobotLacheBalle[] = {
    0.7, 0.3,
    0.3, 0.7
  };
  plDistributionTable P_SystemeCamera1(SystemeCamera1,
                                       RobotLacheBalle,
                                       tableSystemeCamera1_knowingRobotLacheBalle);

  // Probability table of SystemeCamera2, which depends on RobotLacheBalle
  plProbValue tableSystemeCamera2_knowingRobotLacheBalle[] = {
    0.9, 0.1,
    0.1, 0.9
  };
  plDistributionTable P_SystemeCamera2(SystemeCamera2,
                                       RobotLacheBalle,
                                       tableSystemeCamera2_knowingRobotLacheBalle);

  // Joint distribution
  plJointDistribution jdJuggler(BatterieFaible^RobotLacheBalle^SystemeCamera1^SystemeCamera2,
                        P_BatterieFaible*P_RobotLacheBalle*P_SystemeCamera1*P_SystemeCamera2);

  // Creation and print of the bayesian network
  pmBayesianNetwork bnJuggler(jdJuggler);
  cout << endl;
  cout << "Network summary : " << endl;
  bnJuggler.summary();

  // Generate sample data as csv file
  bnJuggler.generate_csvData("../../benchmarks/data/Juggler_csv.data", 1000);
  cout << "Juggler_csv.data generated in benchmarks/data/ " << endl;
  
  // Save the BNJuggler as xml
  bnJuggler.save_as_xml("../../benchmarks/networks/jdBnJuggler.xml","jdBnJuggler");
  cout << "jdBnJuggler.xml generated in benchmarks/networks/ " << endl;
}
