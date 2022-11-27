#include "usecase1.h"

using namespace std;
using namespace PILGRIM;

/**
 * @brief Create a BN, save it as xml and build another with the xml
 * 
 */
void create_BN_and_xml() {

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
    plVariable Smoking("Smoking", plLabelType(smoking_labels));
    plVariable TbOrCa("TbOrCa", plLabelType(tbOrCa_labels));
    plVariable Tuberculosis("Tuberculosis", plLabelType(tuberculosis_labels));
    plVariable VisitToAsia("VisitToAsia", plLabelType(visitToAsia_labels));
    plVariable XRay("XRay", plLabelType(xRay_labels));

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
    plCndDistribution P_Bronchitis(Bronchitis,
                                            Smoking,
                                            tableBronchitis_knowingSmoking);

    // Probability table of LungCancer, which depends on Smoking
    plProbValue tableLungCancer_knowingSmoking[] = {
        0.99, 0.01,
        0.9, 0.1
    };
    plCndDistribution P_LungCancer(LungCancer,
                                        Smoking,
                                        tableLungCancer_knowingSmoking);

    // Probability table of Tuberculosis, which depends on VisitToAsia
    plProbValue tableTuberculosis_knowingVisitToAsia[] = {
        0.99, 0.01,
        0.95, 0.05
    };
    plCndDistribution P_Tuberculosis(Tuberculosis,
                                        VisitToAsia,
                                        tableTuberculosis_knowingVisitToAsia);

    // Probability table of TbOrCa, which depends on Tuberculosis
    plProbValue tableTbOrCa_knowingTuberculosisAndLungCancer[] = {
        1, 0,
        0, 1,
        0, 1,
        0, 1
    };
    plCndDistribution P_TbOrCa(TbOrCa,
                                        Tuberculosis^LungCancer,
                                        tableTbOrCa_knowingTuberculosisAndLungCancer);

    // Probability table of Dyspnea, which depends on TbOrCa and Bronchitis
    plProbValue tableDyspnea_knowingTbOrCaAndBronchitis[] = {
        0.9, 0.1,
        0.2, 0.8,
        0.3, 0.7,
        0.1, 0.9
    };
    plCndDistribution P_Dyspnea(Dyspnea,
                                        TbOrCa^Bronchitis,
                                        tableDyspnea_knowingTbOrCaAndBronchitis);

    // Probability table of XRAY, which depends on TbOrCa
    plProbValue tableXRay_knowingTbOrCa[] = {
        0.95, 0.05,
        0.02, 0.98,
    };
    plCndDistribution P_XRay(XRay,
                                        TbOrCa,
                                        tableXRay_knowingTbOrCa);

    // Joint distribution
    plJointDistribution jdAsia(Bronchitis^Dyspnea^LungCancer^Smoking^TbOrCa^Tuberculosis^VisitToAsia^XRay,
                            P_Bronchitis*P_Dyspnea*P_LungCancer*P_Smoking*P_TbOrCa*P_Tuberculosis*P_VisitToAsia*P_XRay);

    // creation of the bayesian network 
    pmBayesianNetwork bnAsia(jdAsia);
    
    // Summary
    // print of the bayesian network
    cout << endl;
    cout << "Network summary : " << endl;
    bnAsia.summary();
    cout << endl;
    
    // Save the BNAsia as xml
    bnAsia.save_as_xml("../../benchmarks/networks/jdBnAsia.xml","jdBnAsia");
    cout << "jdBnAsia.xml generated in benchmarks/networks/ " << endl;

    // creation of bayesian network from xml
    pmBayesianNetwork bnAsia_copy("../../benchmarks/networks/jdBnAsia.xml","jdBnAsia");
    cout << endl;
    cout << "BN from xml Network summary : " << endl;
    bnAsia_copy.summary();
    
} 