#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pl.h>

#include <iostream>

#include "testing.h"


////////////////////////////////////////////////////////////////////////////////
/// Test environment definition
////////////////////////////////////////////////////////////////////////////////

struct BNJugglerenvironment {
public:
    BNJugglerenvironment() noexcept {
        // Variable definitions
        plVariable BatterieFaible("BatterieFaible", PL_BINARY_TYPE);
        plVariable RobotLacheBalle("RobotLacheBalle", PL_BINARY_TYPE);
        plVariable SystemeCamera1("SystemeCamera1", PL_BINARY_TYPE);
        plVariable SystemeCamera2("SystemeCamera2", PL_BINARY_TYPE);

        // Probability table of BatterieFaible
        plProbValue tableBatterieFaible[] = {0.05, 0.95};
        plProbTable P_BatterieFaible(BatterieFaible, tableBatterieFaible);

        // Probability table of RobotLacheBalle, which depends on BatterieFaible
        plProbValue tableRobotLacheBalle_knowingBatterieFaible[] = {0.9, 0.1,
                                                                    0.01, 0.99};

        plDistributionTable P_RobotLacheBalle(RobotLacheBalle,
                                              BatterieFaible,
                                              tableRobotLacheBalle_knowingBatterieFaible);

        // Probability table of SystemeCamera1, which depends on RobotLacheBalle
        plProbValue tableSystemeCamera1_knowingRobotLacheBalle[] = {0.7, 0.3,
                                                                    0.3, 0.7};

        plDistributionTable P_SystemeCamera1(SystemeCamera1,
                                             RobotLacheBalle,
                                             tableSystemeCamera1_knowingRobotLacheBalle);

        // Probability table of SystemeCamera2, which depends on RobotLacheBalle
        plProbValue tableSystemeCamera2_knowingRobotLacheBalle[] = {0.9, 0.1,
                                                                    0.1, 0.9};

        plDistributionTable P_SystemeCamera2(SystemeCamera2,
                                             RobotLacheBalle,
                                             tableSystemeCamera2_knowingRobotLacheBalle);

        // Joint distribution
        plJointDistribution jdJuggler(BatterieFaible ^ RobotLacheBalle ^ SystemeCamera1 ^ SystemeCamera2,
                                      P_BatterieFaible * P_RobotLacheBalle * P_SystemeCamera1 * P_SystemeCamera2);

        // Creation of the bayesian network
        bnJugglerTest = PILGRIM::pmBayesianNetwork{jdJuggler};

        // Evidence for the queries
        plValues evidenceJuggler1(SystemeCamera1 ^ SystemeCamera2);
        evidenceJuggler1[SystemeCamera1] = 0;
        evidenceJuggler1[SystemeCamera2] = 1;

        plD_ev    = plDistribution{bnJugglerTest.query(RobotLacheBalle, evidenceJuggler1)};
        plD_no_ev = plDistribution{bnJugglerTest.query(RobotLacheBalle)};
    }

public:
    PILGRIM::pmBayesianNetwork bnJugglerTest;
    plDistribution             plD_ev;
    plDistribution             plD_no_ev;
};


////////////////////////////////////////////////////////////////////////////////
/// Test case definition
////////////////////////////////////////////////////////////////////////////////

testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    const BNJugglerenvironment the_test_environment;

    PILGRIM::pmGraph graph = the_test_environment.bnJugglerTest.get_graph();
    PILG_TEST_FAIL_COUNT(num_vertices(graph) == 4, the_errors);

    PILGRIM::pmNode        node;
    plVariablesConjunction parents;

    // BatterieFaible
    node    = vertex(0, graph);
    parents = the_test_environment.bnJugglerTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);

    // RobotLacheBalle
    node    = vertex(1, graph);
    parents = the_test_environment.bnJugglerTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);

    // SystemeCamera1
    node    = vertex(2, graph);
    parents = the_test_environment.bnJugglerTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);

    // SystemeCamera2
    node    = vertex(3, graph);
    parents = the_test_environment.bnJugglerTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

testing::Uint32 fn2() {
    testing::Uint32 the_errors = 0;

    const BNJugglerenvironment the_test_environment;

    PILGRIM::pmGraph         graph = the_test_environment.bnJugglerTest.get_graph();
    PILGRIM::pmNode          node;
    plComputableObject*      pCompObj;
    std::vector<plProbValue> probValues;
    plDistribution*          pDistribution;
    plCndDistribution*       pCndDistribution;

    // Check probability table of BatterieFaible
    node     = vertex(0, graph);
    pCompObj = the_test_environment.bnJugglerTest.getComputableObject(node);
    // BatterieFaible has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);

    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .05, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .95, the_errors);

    // Check probability table of RobotLacheBalle
    node     = vertex(1, graph);
    pCompObj = the_test_environment.bnJugglerTest.getComputableObject(node);
    // RobotLacheBalle has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plDistribution kernel, compDistribution;
    plValues       parentValRLB(the_test_environment.bnJugglerTest.getParents(node));
    parentValRLB.reset();

    pCndDistribution->instantiate(kernel, parentValRLB);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .9, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .1, the_errors);
    parentValRLB.next();

    pCndDistribution->instantiate(kernel, parentValRLB);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .01, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .99, the_errors);
    parentValRLB.next();

    // Check probability table of SystemeCamera1
    node     = vertex(2, graph);
    pCompObj = the_test_environment.bnJugglerTest.getComputableObject(node);
    // SystemeCamera1 has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValSC1(the_test_environment.bnJugglerTest.getParents(node));
    parentValSC1.reset();

    pCndDistribution->instantiate(kernel, parentValSC1);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .7, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .3, the_errors);
    parentValSC1.next();

    pCndDistribution->instantiate(kernel, parentValSC1);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .3, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .7, the_errors);
    parentValSC1.next();

    //-----------------------------
    // Check probability table of SystemeCamera2
    node     = vertex(3, graph);
    pCompObj = the_test_environment.bnJugglerTest.getComputableObject(node);
    // SystemeCamera2 has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValSC2(the_test_environment.bnJugglerTest.getParents(node));
    parentValSC2.reset();

    pCndDistribution->instantiate(kernel, parentValSC2);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .9, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .1, the_errors);
    parentValSC2.next();

    pCndDistribution->instantiate(kernel, parentValSC2);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .1, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .9, the_errors);

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

testing::Uint32 fn3() {
    testing::Uint32 the_errors = 0;

    const BNJugglerenvironment the_test_environment;

    std::vector<plValues>    values;
    std::vector<plProbValue> probValues;

    // plDistribution from the query with evidence
    the_test_environment.plD_ev.tabulate(values, probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] >= .014723, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[0] <= .014725, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] >= .985275, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= .985277, the_errors);

    // plDistribution from the query without evidence
    values.clear();
    probValues.clear();
    the_test_environment.plD_no_ev.tabulate(values, probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] >= .0544, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[0] <= .0546, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] >= .9454, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= .9456, the_errors);

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();
    the_errors += fn2();
    the_errors += fn3();

    std::cout << "Error count = " << the_errors << std::endl;

    return the_errors;
}
