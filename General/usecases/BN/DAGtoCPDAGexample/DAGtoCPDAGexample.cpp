#include "DAGtoCPDAGexample.h"

void DAGtoCPDAGexample() {
    std::string org_bn = "../../benchmarks/networks/Asia_bn1_learned_for_DAG_to_CPDAG.xdsl";
    //org_bn = "../../benchmarks/networks/ABC.xdsl";
    org_bn = "../../benchmarks/networks/Z_network.xdsl";
    pmBayesianNetwork originalBN(org_bn);

    plVariablesConjunction variables = originalBN.getVariables();
    std::cout << "variables: " << variables << std::endl;
    for (auto it = variables.begin(); it != variables.end(); it++)
        std::cout << *it << " index: " <<  originalBN.getVariableIndex(*it) << std::endl;

    pmGraph test = originalBN.getCPDAG();
    originalBN.print_CPDAG(test);
}