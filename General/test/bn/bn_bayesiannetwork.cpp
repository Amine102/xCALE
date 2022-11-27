#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pl.h>

#include "testing.h"


struct BayesianNetworkEnvironment {
public:
    BayesianNetworkEnvironment() {
        // Variable definitions
        plVariable A("A", PL_BINARY_TYPE);
        plVariable B("B", PL_BINARY_TYPE);
        plVariable C("C", PL_BINARY_TYPE);
        plVariable D("D", PL_BINARY_TYPE);
        plVariable E("E", PL_BINARY_TYPE);

        // Probability table of A
        plProbValue tableA[] = {0.4, 0.6};
        plProbTable P_A(A, tableA);

        // Probability table of B
        plProbValue tableB[] = {0.18, 0.82};
        plProbTable P_B(B, tableB);

        // Probability table of C
        plProbValue tableC[] = {0.75, 0.25};
        plProbTable P_C(C, tableC);

        // Probability table of D, which depends on A and B
        plProbValue tableD_knowingA_B[] = {
            0.6, 0.4,
            0.3, 0.7,
            0.1, 0.9,
            0.5, 0.5};
        plDistributionTable P_D(D, A ^ B, tableD_knowingA_B);

        // Probability table of E, which depends on C and D
        plProbValue tableE_knowingC_D[] = {
            0.59, 0.41,
            0.25, 0.75,
            0.8, 0.2,
            0.35, 0.65};
        plDistributionTable P_E(E, C ^ D, tableE_knowingC_D);

        // Joint distribution
        plJointDistribution jd(A ^ B ^ C ^ D ^ E, P_A * P_B * P_C * P_D * P_E);

        // Creation and print of the bayesian network
        bnTest = PILGRIM::pmBayesianNetwork{jd};
    }

public:
    PILGRIM::pmBayesianNetwork bnTest;
};


testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    const BayesianNetworkEnvironment the_test_environment;

    PILGRIM::pmGraph graph = the_test_environment.bnTest.get_graph();
    PILG_TEST_FAIL_COUNT(num_vertices(graph) == 5, the_errors);

    PILGRIM::pmNode        node;
    plVariablesConjunction parents;
    // A
    node    = vertex(0, graph);
    parents = the_test_environment.bnTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);
    // B
    node    = vertex(1, graph);
    parents = the_test_environment.bnTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);
    // C
    node    = vertex(2, graph);
    parents = the_test_environment.bnTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);
    // D
    node    = vertex(3, graph);
    parents = the_test_environment.bnTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 2, the_errors);
    // E
    node    = vertex(4, graph);
    parents = the_test_environment.bnTest.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 2, the_errors);

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

testing::Uint32 fn2() {
    testing::Uint32 the_errors = 0;

    const BayesianNetworkEnvironment the_test_environment;

    PILGRIM::pmGraph         graph = the_test_environment.bnTest.get_graph();
    PILGRIM::pmNode          node;
    plComputableObject*      pCompObj;
    std::vector<plProbValue> probValues;
    plDistribution*          pDistribution;
    plCndDistribution*       pCndDistribution;
    //-----------------------------
    // Check probability table of A
    node     = vertex(0, graph);
    pCompObj = the_test_environment.bnTest.getComputableObject(node);
    // A has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);
    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .4, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .6, the_errors);
    //-----------------------------
    // Check probability table of B
    node     = vertex(1, graph);
    pCompObj = the_test_environment.bnTest.getComputableObject(node);
    // B has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);
    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .18, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .82, the_errors);
    //-----------------------------
    // Check probability table of C
    node     = vertex(2, graph);
    pCompObj = the_test_environment.bnTest.getComputableObject(node);
    // C has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);
    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .75, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .25, the_errors);
    //-----------------------------
    // Check probability table of D
    node     = vertex(3, graph);
    pCompObj = the_test_environment.bnTest.getComputableObject(node);
    // D has 2 parents, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plDistribution kernel, compDistribution;
    plValues       parentValD(the_test_environment.bnTest.getParents(node));
    parentValD.reset();
    //
    pCndDistribution->instantiate(kernel, parentValD);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .6, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .4, the_errors);
    parentValD.next();
    //
    pCndDistribution->instantiate(kernel, parentValD);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .3, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .7, the_errors);
    parentValD.next();
    //
    pCndDistribution->instantiate(kernel, parentValD);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .1, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .9, the_errors);
    parentValD.next();
    //
    pCndDistribution->instantiate(kernel, parentValD);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .5, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .5, the_errors);
    parentValD.next();
    //-----------------------------
    // Check probability table of E
    node     = vertex(4, graph);
    pCompObj = the_test_environment.bnTest.getComputableObject(node);
    // E has 2 parents, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValE(the_test_environment.bnTest.getParents(node));
    parentValE.reset();
    //
    pCndDistribution->instantiate(kernel, parentValE);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .59, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .41, the_errors);
    parentValE.next();
    //
    pCndDistribution->instantiate(kernel, parentValE);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .25, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .75, the_errors);
    parentValE.next();
    //
    pCndDistribution->instantiate(kernel, parentValE);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .8, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .2, the_errors);
    parentValE.next();
    //
    pCndDistribution->instantiate(kernel, parentValE);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .35, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .65, the_errors);
    parentValE.next();

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

testing::Uint32 fn3() {
    testing::Uint32 the_errors = 0;

    const BayesianNetworkEnvironment the_test_environment;

    plJointDistribution jd;
    PILGRIM::pmNode     node;

    std::vector<plProbValue> probValues;
    plVariablesConjunction   parents, variables;
    plDistribution           pld;
    plVariable               target, var1, var2;
    plValues                 evidenceJuggler;
    jd.load_from_genie("../benchmarks/networks/Juggler.xdsl");
    PILGRIM::pmBayesianNetwork                                         bnJuggler(jd);
    PILGRIM::pmGraph                                                   graph = bnJuggler.get_graph();
    boost::property_map<PILGRIM::pmGraph, boost::vertex_index_t>::type nodes_id =
        get(boost::vertex_index, graph);
    PILG_TEST_FAIL_COUNT(num_vertices(graph) == 4, the_errors);

    // Check structure and probability table of Batterie
    node    = vertex(0, graph);
    parents = bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);
    variables = bnJuggler.getVariables();
    target    = variables.get_symbol_with_name("Batterie");
    pld       = bnJuggler.query(target);
    PILG_TEST_FAIL_COUNT(!pld.is_conditional(), the_errors);
    pld.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .05, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .95, the_errors);

    // Check structure and probability table of Robot
    node    = vertex(1, graph);
    parents = bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);
    variables = bnJuggler.getVariables();
    target    = variables.get_symbol_with_name("Robot");
    var1      = variables.get_symbol_with_name("Batterie");

    evidenceJuggler       = plValues(var1);
    evidenceJuggler[var1] = "Faible";
    pld                   = bnJuggler.query(target, evidenceJuggler);
    pld.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(0.9 == probValues[0], the_errors);
    PILG_TEST_FAIL_COUNT((0.1 * 0.99999) < probValues[1] && probValues[1] < (0.1 * 1.00001), the_errors);

    evidenceJuggler[var1] = "PasFaible";
    pld                   = bnJuggler.query(target, evidenceJuggler);
    pld.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == 0.01, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == 0.99, the_errors);

    // Check structure and probability table of SV1
    node    = vertex(2, graph);
    parents = bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);
    variables = bnJuggler.getVariables();
    target    = variables.get_symbol_with_name("SV1");
    var1      = variables.get_symbol_with_name("Robot");

    evidenceJuggler       = plValues(var1);
    evidenceJuggler[var1] = "Lache";
    pld                   = bnJuggler.query(target, evidenceJuggler);
    pld.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(0.7 == probValues[0], the_errors);
    PILG_TEST_FAIL_COUNT(0.3 == probValues[1], the_errors);

    evidenceJuggler[var1] = "PasLache";
    pld                   = bnJuggler.query(target, evidenceJuggler);
    pld.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(0.3 == probValues[0], the_errors);
    PILG_TEST_FAIL_COUNT(0.7 == probValues[1], the_errors);

    // Check structure and probability table of SV2
    node    = vertex(2, graph);
    parents = bnJuggler.getParents(node);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);
    variables = bnJuggler.getVariables();
    target    = variables.get_symbol_with_name("SV2");
    var1      = variables.get_symbol_with_name("Robot");

    evidenceJuggler       = plValues(var1);
    evidenceJuggler[var1] = "Lache";
    pld                   = bnJuggler.query(target, evidenceJuggler);
    pld.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(0.9 == probValues[0], the_errors);
    PILG_TEST_FAIL_COUNT(0.1 == probValues[1], the_errors);

    evidenceJuggler[var1] = "PasLache";
    pld                   = bnJuggler.query(target, evidenceJuggler);
    pld.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(0.1 == probValues[0], the_errors);
    PILG_TEST_FAIL_COUNT(0.9 == probValues[1], the_errors);

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
