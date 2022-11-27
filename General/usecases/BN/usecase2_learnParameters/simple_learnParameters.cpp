#include "usecase2.h"

using namespace std;
using namespace PILGRIM;

void usecase2(){

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

    cout << endl << "learning parameters ..." << endl; 
    bnAsiaLearn.learnParameters(data);
    
    cout << endl;
    cout << "BN with learned parameters summary  : " << endl;
    bnAsiaLearn.summary();
    cout << endl;
    bnAsiaLearn.getLearnObjectList();
    bnAsiaLearn.save_as_xml("../../benchmarks/networks/Asia_uc2.xml","Asia_uc2");
    cout << "Asia_uc2.xml generated in benchmarks/networks/ " << endl;
}