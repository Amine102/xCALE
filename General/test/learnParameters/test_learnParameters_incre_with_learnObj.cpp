#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pl.h>

#include "testing.h"

using namespace std;

plValues get_Label_Index(plVariable x, string s)
{
    return plValues().add(x, plLabelType(x.get_type()).label_to_index(s));
}

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
 * @brief checking if the parameters are probabilty and frozen learnObject
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    BNAsiaenvironment BNAsiaenvironment;


    plVariablesConjunction variables = BNAsiaenvironment.bnAsia.getVariables();

    plVariable Bronchitis = variables.get_variable_with_name("Bronchitis");
    plVariable Dyspnea = variables.get_variable_with_name("Dyspnea");
    plVariable LungCancer = variables.get_variable_with_name("LungCancer");
    plVariable Smoking = variables.get_variable_with_name("Smoking");
    plVariable TbOrCa = variables.get_variable_with_name("TbOrCa");
    plVariable Tuberculosis = variables.get_variable_with_name("Tuberculosis");
    plVariable VisitToAsia = variables.get_variable_with_name("VisitToAsia");
    plVariable XRay = variables.get_variable_with_name("XRay");

    vector<plLearnObject *> learnObjectList;
    learnObjectList.push_back(new plLearnHistogram(VisitToAsia));
    learnObjectList.push_back(new plLearnHistogram(Smoking));

    plLearnDistributionTable PL_Bronchitis(Bronchitis, Smoking);
    PL_Bronchitis.push(plLearnHistogram(Bronchitis), get_Label_Index(Smoking, "Smoker"));
    PL_Bronchitis.push(plLearnHistogram(Bronchitis), get_Label_Index(Smoking, "NonSmoker"));
    learnObjectList.push_back(&PL_Bronchitis);

    plLearnDistributionTable PL_Dyspnea(Dyspnea, TbOrCa^Bronchitis);
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "Nothing")^get_Label_Index(Bronchitis, "Absent"));
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "Nothing")^get_Label_Index(Bronchitis, "Present"));
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "CancerORTuberculosis")^get_Label_Index(Bronchitis, "Absent"));
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "CancerORTuberculosis")^get_Label_Index(Bronchitis, "Present"));
    learnObjectList.push_back(&PL_Dyspnea);
    
    plLearnDistributionTable PL_LungCancer(LungCancer, Smoking);
    PL_LungCancer.push(plLearnHistogram(LungCancer), get_Label_Index(Smoking, "Smoker"));
    PL_LungCancer.push(plLearnHistogram(LungCancer), get_Label_Index(Smoking, "NonSmoker"));
    learnObjectList.push_back(&PL_LungCancer);
    
    plLearnDistributionTable PL_Tuberculosis(Tuberculosis, VisitToAsia);
    PL_Tuberculosis.push(plLearnHistogram(Tuberculosis), get_Label_Index(VisitToAsia, "NoVisit"));
    PL_Tuberculosis.push(plLearnHistogram(Tuberculosis), get_Label_Index(VisitToAsia, "Visit"));
    learnObjectList.push_back(&PL_Tuberculosis);

    plLearnDistributionTable PL_XRay(XRay, TbOrCa);
    PL_XRay.push(plLearnHistogram(XRay), get_Label_Index(TbOrCa, "Nothing"));
    PL_XRay.push(plLearnHistogram(XRay), get_Label_Index(TbOrCa, "CancerORTuberculosis"));
    learnObjectList.push_back(&PL_XRay);

    // C source code function :
    const std::string my_code = 
        "void my_max(double* output, unsigned int output_size,\n"
        " double* input, unsigned int input_size)\n"
        "{\n"
        " double* vectMax = output;\n"
        " double* vectProb = input;\n"
        "\n"
        " unsigned int n_probas; /* Number of values */\n"
        " double value_max=vectProb[0]; \n"
        " unsigned int i;\n"
        " \n"
        " n_probas = input_size; /* Get the number of values */\n"
        "\n"
        " for(i=0;i<n_probas;i++) /* Find the max value */\n"
        " if (vectProb[i]>value_max) value_max=vectProb[i];\n"
        " vectMax[0]=value_max; \n"
        "}\n";

    cout << "starting creating dynamic code" << endl;
    const plExternalFunctionFromC max_fun(TbOrCa, Tuberculosis ^ LungCancer, my_code, "my_max");
    const plCndDeterministic P_TbOrCagivenTuberculosisLungCancer(TbOrCa, Tuberculosis ^ LungCancer , max_fun); 
    learnObjectList.push_back(new plLearnFrozenDistribution(P_TbOrCagivenTuberculosisLungCancer));
     
    BNAsiaenvironment.bnAsia.setLearnObject(learnObjectList);
    BNAsiaenvironment.bnAsia.setForgettingFactor(0.7);   
    BNAsiaenvironment.bnAsia.learnParameters(BNAsiaenvironment.data);
    BNAsiaenvironment.bnAsia.summary();
    
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
    PILG_TEST_FAIL_COUNT(probValues[0] == 1.0 , the_errors);
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