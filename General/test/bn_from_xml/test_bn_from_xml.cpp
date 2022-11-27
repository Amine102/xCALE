#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pl.h>

#include "testing.h"

using namespace std;

struct BNJugglerenvironment {
public:
    BNJugglerenvironment(){
        bnJuggler = pmBayesianNetwork("../benchmarks/networks/jdBnJuggler.xml", "jdBnJuggler");
    }
public:
    pmBayesianNetwork bnJuggler;

};

/**
 * @brief checking the parameters of the BN
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    const BNJugglerenvironment the_test_environment;

    PILGRIM::pmGraph graph = the_test_environment.bnJuggler.get_graph();
    PILG_TEST_FAIL_COUNT(num_vertices(graph) == 4, the_errors);

    PILGRIM::pmNode        node;
    plVariablesConjunction parents;

    // BatterieFaible
    node    = vertex(0, graph);
    parents = the_test_environment.bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);

    // RobotLacheBalle
    node    = vertex(1, graph);
    parents = the_test_environment.bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);

    // SystemeCamera1
    node    = vertex(2, graph);
    parents = the_test_environment.bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);

    // SystemeCamera2
    node    = vertex(3, graph);
    parents = the_test_environment.bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

/**
 * @brief checking the parameters of the BN
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn2() {
    testing::Uint32 the_errors = 0;

    const BNJugglerenvironment the_test_environment;

    PILGRIM::pmGraph         graph = the_test_environment.bnJuggler.get_graph();
    PILGRIM::pmNode          node;
    plComputableObject*      pCompObj;
    std::vector<plProbValue> probValues;
    plDistribution*          pDistribution;
    plCndDistribution*       pCndDistribution;

    // Check probability table of BatterieFaible
    node     = vertex(0, graph);
    pCompObj = the_test_environment.bnJuggler.getComputableObject(node);
    // BatterieFaible has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);

    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .05, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .95, the_errors);

    // Check probability table of RobotLacheBalle
    node     = vertex(1, graph);
    pCompObj = the_test_environment.bnJuggler.getComputableObject(node);
    // RobotLacheBalle has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plDistribution kernel, compDistribution;
    plValues       parentValRLB(the_test_environment.bnJuggler.getParents(node));
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
    pCompObj = the_test_environment.bnJuggler.getComputableObject(node);
    // SystemeCamera1 has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValSC1(the_test_environment.bnJuggler.getParents(node));
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
    pCompObj = the_test_environment.bnJuggler.getComputableObject(node);
    // SystemeCamera2 has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValSC2(the_test_environment.bnJuggler.getParents(node));
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

int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();
    the_errors += fn2();

    std::cout << "Error count = " << the_errors << std::endl;

    return the_errors;
}
