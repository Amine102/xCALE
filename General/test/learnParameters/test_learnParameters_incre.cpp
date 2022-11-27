#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pl.h>

#include "testing.h"

using namespace std;

struct BNAsiaenvironment {
public:
    BNAsiaenvironment(){
        bnAsia = pmBayesianNetwork("../benchmarks/networks/Asia.xdsl");
        dataf = "../benchmarks/data/Asia_csv.data";
        wndw_size = 1;
        nbClms = 8;
        data = new pmCSVDataSet(dataf, wndw_size, nbClms, ';', vars);
    }
public:

    pmBayesianNetwork bnAsia;

    const plVariablesConjunction &vars = bnAsia.getVariables();

    char *dataf;
    size_t wndw_size;
    size_t nbClms;
    pmCSVDataSet *data;
};

/**
 * @brief checking if the parameters are probabilty
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    BNAsiaenvironment BNAsiaenvironment;


    BNAsiaenvironment.bnAsia.setForgettingFactor(0.7);
    BNAsiaenvironment.bnAsia.learnParameters(BNAsiaenvironment.data);
cout << 1 << endl;
    PILGRIM::pmGraph         graph = BNAsiaenvironment.bnAsia.get_graph();
    PILGRIM::pmNode          pNode;
    plComputableObject*      pCompObj;
    std::vector<plProbValue> probValues;
    plDistribution*          pDistribution;
    plCndDistribution*       pCndDistribution;
    //-----------------------------
    // Check probability table of Bronchitis
    pNode     = vertex(0, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // Bronchitis has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plDistribution kernel, compDistribution;
    plValues       parentValBr(BNAsiaenvironment.bnAsia.getParents(pNode));
    parentValBr.reset();
    //
    pCndDistribution->instantiate(kernel, parentValBr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValBr.next();
    //
    pCndDistribution->instantiate(kernel, parentValBr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValBr.next();
    //-----------------------------
    // Check probability table of Dyspnea
    pNode     = vertex(1, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // Dyspnea has 2 parents, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues       parentValDs(BNAsiaenvironment.bnAsia.getParents(pNode));
    parentValDs.reset();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValDs.next();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValDs.next();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValDs.next();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValDs.next();
    //-----------------------------
    // Check probability table of LungCancer
    pNode     = vertex(2, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // LungCancer has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValLC(BNAsiaenvironment.bnAsia.getParents(pNode));
    parentValLC.reset();
    //
    pCndDistribution->instantiate(kernel, parentValLC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValLC.next();
    //
    pCndDistribution->instantiate(kernel, parentValLC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValLC.next();
    //-----------------------------
    // Check probability table of Tuberculosis
    pNode     = vertex(5, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // Tuberculosis has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValTr(BNAsiaenvironment.bnAsia.getParents(pNode));
    parentValTr.reset();
    //
    pCndDistribution->instantiate(kernel, parentValTr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValTr.next();
    //
    pCndDistribution->instantiate(kernel, parentValTr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValTr.next();
    //-----------------------------
    // Check probability table of TbOrCa
    pNode     = vertex(4, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // TbOrCa has 2 parents, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValTC(BNAsiaenvironment.bnAsia.getParents(pNode));
    parentValTC.reset();
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValTC.next();
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValTC.next(); 
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValTC.next();
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValTC.next();
    //-----------------------------
    // Check probability table of VisitToAsia
    pNode     = vertex(6, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // VisitToAsia has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);
    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    //-----------------------------
    // Check probability table of XRay
    pNode     = vertex(7, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // XRay has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues       parentValXR(BNAsiaenvironment.bnAsia.getParents(pNode));
    parentValXR.reset();
    //
    pCndDistribution->instantiate(kernel, parentValXR);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValXR.next();
    //
    pCndDistribution->instantiate(kernel, parentValXR);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);
    parentValXR.next();
    //-----------------------------
    // Check probability table of Smoking
    pNode     = vertex(3, graph);
    pCompObj = BNAsiaenvironment.bnAsia.getComputableObject(pNode);
    // Smoking has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);
    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] <= 1.0 && probValues[0] >= 0.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] <= 1.0 && probValues[1] >= 0.0, the_errors);


    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();

    std::cout << "Error count = " << the_errors << std::endl;

    return the_errors;
}