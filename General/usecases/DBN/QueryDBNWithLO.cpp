#include "QueryDBNWithLO.h"

void QueryDBNWithLO() {

    // Import with new constructor from XML file
    pmDynamicBayesianNetwork DBN_import("../../benchmarks/networks/dDBN_with_LO.xml", "jd0", "jdT");
    DBN_import.summary(); 

    plVariablesConjunction variables_g0 = DBN_import.getVariablesforG0();
    plVariablesConjunction variables_gt = DBN_import.getVariablesforGtransition();
    std::cout << "Variables are " << variables_g0 << std::endl;
    std::cout << "Variables are " << variables_gt << std::endl;

    /*plVariable A = variables.get_variable_with_name("NodeA");
    plVariable B = variables.get_variable_with_name("NodeB");
    plVariable C = variables.get_variable_with_name("NodeC");
    plVariable D = variables.get_variable_with_name("NodeD");
    plVariable At = variables.get_variable_with_name("NodeA_t");
    plVariable Bt = variables.get_variable_with_name("NodeB_t");
    plVariable Ct = variables.get_variable_with_name("NodeC_t");
    plVariable Dt = variables.get_variable_with_name("NodeD_t");*/

    pmBayesianNetwork* unrolled = DBN_import.unroll(5);

    //from the list of variables in the DBN, getting the value of NodeB_t at time slice 4
    plVariable asked = unrolled->getJointDistribution().get_variables().get_variable_with_name("NodeB_t4");

    //using query function to find the value of asked variable
    plDistribution distri = unrolled->query(asked);

    std::cout << distri.compile() << std::endl;
}