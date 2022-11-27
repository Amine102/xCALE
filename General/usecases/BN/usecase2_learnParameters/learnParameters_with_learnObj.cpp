#include "usecase2.h"

using namespace std;
using namespace PILGRIM;

void usecase2_learnObj(){

    pmCSVDataSet *data = new pmCSVDataSet("../../benchmarks/data/Asia_uc_csv.data");
    plVariablesConjunction vars = data->observed_variables();

    pmBayesianNetwork bnAsiaLearn(vars);

    plVariable Bronchitis = vars.get_variable_with_name("Bronchitis");
    plVariable Dyspnea = vars.get_variable_with_name("Dyspnea");
    plVariable LungCancer = vars.get_variable_with_name("LungCancer");
    plVariable Smoking = vars.get_variable_with_name("Smoking");
    plVariable TbOrCa = vars.get_variable_with_name("TbOrCa");
    plVariable Tuberculosis = vars.get_variable_with_name("Tuberculosis");
    plVariable VisitToAsia = vars.get_variable_with_name("VisitToAsia");
    plVariable XRay = vars.get_variable_with_name("XRay");

    // building the network's structure
    bnAsiaLearn.add_arrow(Bronchitis, Dyspnea);
    bnAsiaLearn.add_arrow(Smoking, Bronchitis);
    bnAsiaLearn.add_arrow(Smoking, LungCancer);
    bnAsiaLearn.add_arrow(LungCancer, TbOrCa);
    bnAsiaLearn.add_arrow(VisitToAsia, Tuberculosis);
    bnAsiaLearn.add_arrow(Tuberculosis, TbOrCa);
    bnAsiaLearn.add_arrow(TbOrCa, XRay);
    bnAsiaLearn.add_arrow(TbOrCa, Dyspnea);
    
    // creating the learn objects
    vector<plLearnObject *> learnObjectList;
    learnObjectList.push_back(new plLearnHistogram(VisitToAsia));
    learnObjectList.push_back(new plLearnHistogram(Smoking));

    plLearnDistributionTable PL_Bronchitis(Bronchitis, Smoking);
    PL_Bronchitis.push(plLearnHistogram(Bronchitis), get_Label_Index(Smoking, "Smoker"));
    PL_Bronchitis.push(plLearnHistogram(Bronchitis), get_Label_Index(Smoking, "NonSmoker"));
    learnObjectList.push_back(&PL_Bronchitis);

    plLearnDistributionTable PL_Dyspnea(Dyspnea, Bronchitis^TbOrCa);
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "Nothing")^get_Label_Index(Bronchitis, "Absent"));
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "Nothing")^get_Label_Index(Bronchitis, "Present"));
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "CancerORTuberculosis")^get_Label_Index(Bronchitis, "Absent"));
    PL_Dyspnea.push(plLearnHistogram(Dyspnea), get_Label_Index(TbOrCa, "CancerORTuberculosis")^get_Label_Index(Bronchitis, "Present"));
    learnObjectList.push_back(&PL_Dyspnea);
    
    plLearnDistributionTable PL_LungCancer(LungCancer, Smoking);
    PL_LungCancer.push(plLearnHistogram(LungCancer), get_Label_Index(Smoking, "NonSmoker"));
    PL_LungCancer.push(plLearnHistogram(LungCancer), get_Label_Index(Smoking, "Smoker"));
    learnObjectList.push_back(&PL_LungCancer);
    
    plLearnDistributionTable PL_Tuberculosis(Tuberculosis, VisitToAsia);
    PL_Tuberculosis.push(plLearnHistogram(Tuberculosis), get_Label_Index(VisitToAsia, "NoVisit"));
    PL_Tuberculosis.push(plLearnHistogram(Tuberculosis), get_Label_Index(VisitToAsia, "Visit"));
    learnObjectList.push_back(&PL_Tuberculosis);

    plLearnDistributionTable PL_XRay(XRay, TbOrCa);
    PL_XRay.push(plLearnHistogram(XRay), get_Label_Index(TbOrCa, "Nothing"));
    PL_XRay.push(plLearnHistogram(XRay), get_Label_Index(TbOrCa, "CancerORTuberculosis"));
    learnObjectList.push_back(&PL_XRay);

    cout << endl << "forcing TbOrCa to be an OR gate between Tuberculosis and LungCancer" << endl;
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
        " if (value_max == 0.0) value_max = 1.0; \n"
        " else value_max = 0.0;\n" 
        " vectMax[0]=value_max; \n"
        "}\n";

    cout << "starting creating dynamic code" << endl;
    const plExternalFunctionFromC max_fun(TbOrCa, LungCancer ^ Tuberculosis, my_code, "my_max");
    const plCndDeterministic P_TbOrCagivenTuberculosisLungCancer(TbOrCa, LungCancer ^ Tuberculosis, max_fun); 
    learnObjectList.push_back(new plLearnFrozenDistribution(P_TbOrCagivenTuberculosisLungCancer));
    
    bnAsiaLearn.setLearnObject(learnObjectList);
    cout << "learning parameters ..." << endl;
    bnAsiaLearn.learnParameters(data);
    cout << endl << "Learned BN summary :" << endl;
    bnAsiaLearn.summary();
    cout << endl;
    bnAsiaLearn.save_as_xml("../../benchmarks/networks/Asia_uc2_LO.xml","Asia_uc2_LO");
    cout << "Asia_uc2_LO.xml generated in benchmarks/networks/ " << endl;
}