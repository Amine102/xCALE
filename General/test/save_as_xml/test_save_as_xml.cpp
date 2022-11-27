#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pl.h>

#include "testing.h"

using namespace std;

struct BayesianNetworkEnvironment {
public:
    BayesianNetworkEnvironment() {
        // Creation of the BN

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
    bnJuggler = PILGRIM::pmBayesianNetwork{jdJuggler};
    }

public:
    PILGRIM::pmBayesianNetwork bnJuggler;
};

/**
 * @brief testing the save_as_xml method by comparing to an xml reference
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn1(){
    testing::Uint32 the_errors = 0;
    
    BayesianNetworkEnvironment the_test_environment;

    the_test_environment.bnJuggler.save_as_xml("../benchmarks/networks/jdBnJuggler_test.xml", "jdBnJuggler_test");

    ifstream testXml ("../benchmarks/networks/jdBnJuggler_test.xml");
    string testStr;
    ifstream refXml ("../benchmarks/networks/jdBnJuggler.xml");
    string refStr;
    int i = 1;
    if(testXml.is_open() && refXml.is_open()){
        while(testXml || refXml){
            if(!testXml) {
                PILG_TEST_FAIL_COUNT(false, the_errors);
                break;
            }
            if(!refXml){
                PILG_TEST_FAIL_COUNT(false, the_errors);
                break;
            }
            getline(testXml, testStr);
            getline(refXml, refStr);
            if(i != 12){
                PILG_TEST_FAIL_COUNT(testStr == refStr, the_errors);
            }
            ++i;
        }
    }

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();

    std::cout << "Error count = " << the_errors << std::endl;

    return the_errors;
}