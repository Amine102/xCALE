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
 * @brief testing the summary methode by comparing the output to an expected summary string.
 * 
 * @return testing::Uint32 
 */
testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    BayesianNetworkEnvironment the_test_environment;
    
    stringstream buffer;
    streambuf * old = cout.rdbuf(buffer.rdbuf());
    string expectedSummary = 
        "Bronchitis\t|\tSmoking\n"
        "'Absent' :\t0.7\t0.4\n"
        "'Present' :\t0.3\t0.6\n"
        "Dyspnea\t|\tTbOrCa\tBronchitis\n"
        "'Absent' :\t0.9\t0.2\t0.3\t0.1\n"
        "'Present' :\t0.1\t0.8\t0.7\t0.9\n"
        "LungCancer\t|\tSmoking\n"
        "'Absent' :\t0.99\t0.9\n"
        "'Present' :\t0.01\t0.1\n"
        "Tuberculosis\t|\tVisitToAsia\n"
        "'Absent' :\t0.99\t0.95\n"
        "'Present' :\t0.01\t0.05\n"
        "TbOrCa\t|\tTuberculosis\tLungCancer\n"
        "'Nothing' :\t1\t0\t0\t0\n"
        "'CancerORTuberculosis' :\t0\t1\t1\t1\n"
        "VisitToAsia\n"
        "'NoVisit' :\t0.99\n"
        "'Visit' :\t0.01\n"
        "XRay\t|\tTbOrCa\n"
        "'Normal' :\t0.95\t0.02\n"
        "'Abnormal' :\t0.05\t0.98\n"
        "Smoking\n"
        "'NonSmoker' :\t0.5\n"
        "'Smoker' :\t0.5\n"
        "Bronchitis -->  Dyspnea \n"
        "Dyspnea -->  \n"
        "LungCancer -->  TbOrCa \n"
        "Tuberculosis -->  TbOrCa \n"
        "TbOrCa -->  Dyspnea XRay \n"
        "VisitToAsia -->  Tuberculosis \n"
        "XRay -->  \n"
        "Smoking -->  Bronchitis LungCancer \n";

    the_test_environment.bnAsia.summary();
    PILG_TEST_FAIL_COUNT(buffer.str() == expectedSummary, the_errors);
    cout.rdbuf(old);
    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();
    
    std::cout << "Error count = " << the_errors << std::endl;
    return the_errors;
}
