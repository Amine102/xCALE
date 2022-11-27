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

    vector<string> bronchitis_labels;
    bronchitis_labels.push_back("Absent");
    bronchitis_labels.push_back("Present");
    
    vector<string> dyspnea_labels;
    dyspnea_labels.push_back("Absent");
    dyspnea_labels.push_back("Present");
    
    vector<string> lungCancer_labels;
    lungCancer_labels.push_back("Absent");
    lungCancer_labels.push_back("Present");
    
    vector<string> smoking_labels;
    smoking_labels.push_back("NonSmoker");
    smoking_labels.push_back("Smoker");
    
    vector<string> tbOrCa_labels;
    tbOrCa_labels.push_back("Nothing");
    tbOrCa_labels.push_back("CancerORTuberculosis");
    
    vector<string> tuberculosis_labels;
    tuberculosis_labels.push_back("Absent");
    tuberculosis_labels.push_back("Present");
    
    vector<string> visitToAsia_labels;
    visitToAsia_labels.push_back("NoVisit");
    visitToAsia_labels.push_back("Visit");
    
    vector<string> xRay_labels;
    xRay_labels.push_back("Normal");
    xRay_labels.push_back("Abnormal");
    

    plVariable Bronchitis("Bronchitis", plLabelType(bronchitis_labels));
    plVariable Dyspnea("Dyspnea", plLabelType(dyspnea_labels));
    plVariable LungCancer("LungCancer", plLabelType(lungCancer_labels));
    plVariable Tuberculosis("Tuberculosis", plLabelType(tuberculosis_labels));
    plVariable TbOrCa("TbOrCa", plLabelType(tbOrCa_labels));
    plVariable VisitToAsia("VisitToAsia", plLabelType(visitToAsia_labels));
    plVariable XRay("XRay", plLabelType(xRay_labels));
    plVariable Smoking("Smoking", plLabelType(smoking_labels));

    // Probability table of Smoking
    plProbValue tableSmoking[] = {0.5, 0.5};
    plProbTable P_Smoking(Smoking, tableSmoking);

    // Probability table of VistToAsia
    plProbValue tableVisitToAsia[] = {0.99, 0.01};
    plProbTable P_VisitToAsia(VisitToAsia, tableVisitToAsia);

    // Probability table of Bronchitis, which depends on Smoking
    plProbValue tableBronchitis_knowingSmoking[] = {
        0.7, 0.3,
        0.4, 0.6
    };
    plDistributionTable P_Bronchitis(Bronchitis,
                                            Smoking,
                                            tableBronchitis_knowingSmoking);

    // Probability table of LungCancer, which depends on Smoking
    plProbValue tableLungCancer_knowingSmoking[] = {
        0.99, 0.01,
        0.9, 0.1
    };
    plDistributionTable P_LungCancer(LungCancer,
                                        Smoking,
                                        tableLungCancer_knowingSmoking);

    // Probability table of Tuberculosis, which depends on VisitToAsia
    plProbValue tableTuberculosis_knowingVisitToAsia[] = {
        0.99, 0.01,
        0.95, 0.05
    };
    plDistributionTable P_Tuberculosis(Tuberculosis,
                                        VisitToAsia,
                                        tableTuberculosis_knowingVisitToAsia);

    // Probability table of TbOrCa, which depends on Tuberculosis
    plProbValue tableTbOrCa_knowingTuberculosisAndLungCancer[] = {
        1, 0,
        0, 1,
        0, 1,
        0, 1
    };
    plDistributionTable P_TbOrCa(TbOrCa,
                                        Tuberculosis^LungCancer,
                                        tableTbOrCa_knowingTuberculosisAndLungCancer);

    // Probability table of Dyspnea, which depends on TbOrCa and Bronchitis
    plProbValue tableDyspnea_knowingTbOrCaAndBronchitis[] = {
        0.9, 0.1,
        0.2, 0.8,
        0.3, 0.7,
        0.1, 0.9
    };
    plDistributionTable P_Dyspnea(Dyspnea,
                                        TbOrCa^Bronchitis,
                                        tableDyspnea_knowingTbOrCaAndBronchitis);

    // Probability table of XRAY, which depends on TbOrCa
    plProbValue tableXRay_knowingTbOrCa[] = {
        0.95, 0.05,
        0.02, 0.98,
    };
    plDistributionTable P_XRay(XRay,
                                        TbOrCa,
                                        tableXRay_knowingTbOrCa);

    // Joint distribution
    plJointDistribution jdAsia(Bronchitis^Dyspnea^LungCancer^Tuberculosis^TbOrCa^VisitToAsia^XRay^Smoking,
                            P_Bronchitis*P_Dyspnea*P_LungCancer*P_Tuberculosis*P_TbOrCa*P_VisitToAsia*P_XRay*P_Smoking);

    // creation of the bayesian network 
    bnAsia = PILGRIM::pmBayesianNetwork{jdAsia};
    }

public:
    PILGRIM::pmBayesianNetwork bnAsia;
};

/**
 * @brief checking the structure of the BN
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    const BayesianNetworkEnvironment the_test_environment;

    PILGRIM::pmGraph graph = the_test_environment.bnAsia.get_graph();
    PILG_TEST_FAIL_COUNT(num_vertices(graph) == 8, the_errors);
    PILGRIM::pmNode        pNode;
    plVariablesConjunction parents;
    // Bronchitis
    pNode    = vertex(0, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);
    // Dyspnea
    pNode    = vertex(1, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 2, the_errors);
    // LungCancer
    pNode    = vertex(2, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);
    // Tuberculosis
    pNode    = vertex(3, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);
    // TbOrCa
    pNode    = vertex(4, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 2, the_errors);
    // VisitToAsia
    pNode    = vertex(5, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);
    // XRay
    pNode    = vertex(6, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 1, the_errors);
    // Smoking
    pNode    = vertex(7, graph);
    parents = the_test_environment.bnAsia.getParents(pNode);
    PILG_TEST_FAIL_COUNT(parents.size() == 0, the_errors);

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

    const BayesianNetworkEnvironment the_test_environment;

    PILGRIM::pmGraph         graph = the_test_environment.bnAsia.get_graph();
    PILGRIM::pmNode          pNode;
    plComputableObject*      pCompObj;
    std::vector<plProbValue> probValues;
    plDistribution*          pDistribution;
    plCndDistribution*       pCndDistribution;
    //-----------------------------
    // Check probability table of Bronchitis
    pNode     = vertex(0, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // Bronchitis has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plDistribution kernel, compDistribution;
    plValues       parentValBr(the_test_environment.bnAsia.getParents(pNode));
    parentValBr.reset();
    //
    pCndDistribution->instantiate(kernel, parentValBr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .7, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .3, the_errors);
    parentValBr.next();
    //
    pCndDistribution->instantiate(kernel, parentValBr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .4, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .6, the_errors);
    parentValBr.next();
    //-----------------------------
    // Check probability table of Dyspnea
    pNode     = vertex(1, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // Dyspnea has 2 parents, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues       parentValDs(the_test_environment.bnAsia.getParents(pNode));
    parentValDs.reset();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .9, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .1, the_errors);
    parentValDs.next();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .2, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .8, the_errors);
    parentValDs.next();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .3, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .7, the_errors);
    parentValDs.next();
    //
    pCndDistribution->instantiate(kernel, parentValDs);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .1, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .9, the_errors);
    parentValDs.next();
    //-----------------------------
    // Check probability table of LungCancer
    pNode     = vertex(2, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // LungCancer has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValLC(the_test_environment.bnAsia.getParents(pNode));
    parentValLC.reset();
    //
    pCndDistribution->instantiate(kernel, parentValLC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .99, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .01, the_errors);
    parentValLC.next();
    //
    pCndDistribution->instantiate(kernel, parentValLC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .9, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .1, the_errors);
    parentValLC.next();
    //-----------------------------
    // Check probability table of Tuberculosis
    pNode     = vertex(3, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // Tuberculosis has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues       parentValTr(the_test_environment.bnAsia.getParents(pNode));
    parentValTr.reset();
    //
    pCndDistribution->instantiate(kernel, parentValTr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .99, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .01, the_errors);
    parentValTr.next();
    //
    pCndDistribution->instantiate(kernel, parentValTr);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .95, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .05, the_errors);
    parentValTr.next();
    //-----------------------------
    // Check probability table of TbOrCa
    pNode     = vertex(4, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // TbOrCa has 2 parents, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues parentValTC(the_test_environment.bnAsia.getParents(pNode));
    parentValTC.reset();
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == 1.0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .0, the_errors);
    parentValTC.next();
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == 1.0, the_errors);
    parentValTC.next();
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == 1.0, the_errors);
    parentValTC.next();
    //
    pCndDistribution->instantiate(kernel, parentValTC);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .0, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == 1.0, the_errors);
    parentValTC.next();
    //-----------------------------
    // Check probability table of VisitToAsia
    pNode     = vertex(5, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // VisitToAsia has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);
    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .99, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .01, the_errors);
    //-----------------------------
    // Check probability table of XRay
    pNode     = vertex(6, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // XRay has 1 parent, so we expect it to have a conditional probability table
    PILG_TEST_FAIL_COUNT(pCompObj->is_conditional(), the_errors);
    pCndDistribution = (plCndDistribution*)pCompObj;
    plValues       parentValXR(the_test_environment.bnAsia.getParents(pNode));
    parentValXR.reset();
    //
    pCndDistribution->instantiate(kernel, parentValXR);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .95, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .05, the_errors);
    parentValXR.next();
    //
    pCndDistribution->instantiate(kernel, parentValXR);
    kernel.compile(compDistribution);
    probValues.clear();
    compDistribution.tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .02, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .98, the_errors);
    parentValXR.next();
    //-----------------------------
    // Check probability table of Smoking
    pNode     = vertex(7, graph);
    pCompObj = the_test_environment.bnAsia.getComputableObject(pNode);
    // Smoking has no parent, so we expect it to have a non conditional probability table
    PILG_TEST_FAIL_COUNT(!pCompObj->is_conditional(), the_errors);
    pDistribution = (plDistribution*)pCompObj;
    probValues.clear();
    pDistribution->tabulate(probValues);
    PILG_TEST_FAIL_COUNT(probValues[0] == .5, the_errors);
    PILG_TEST_FAIL_COUNT(probValues[1] == .5, the_errors);

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
