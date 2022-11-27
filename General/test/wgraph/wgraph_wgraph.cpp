#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pilgrim/general/pmWGraph.h>
#include <pl.h>

#include <iostream>
#include <memory>

#include "testing.h"

////////////////////////////////////////////////////////////////////////////////
/// Test environment definition
////////////////////////////////////////////////////////////////////////////////

class pmWGraphEnvironment {
public:
    pmWGraphEnvironment() noexcept {
        plJointDistribution jd;
        jd.load_from_genie("../benchmarks/networks/Asia.xdsl");
        PILGRIM::pmBayesianNetwork bnAsia(jd);
        PILGRIM::pmGraph           asiaGraph = bnAsia.get_graph();
        wgraphTest                           = std::make_unique<PILGRIM::pmWGraph>(asiaGraph);
    }

public:
    std::unique_ptr<PILGRIM::pmWGraph> wgraphTest;
};


////////////////////////////////////////////////////////////////////////////////
/// Test case definition
////////////////////////////////////////////////////////////////////////////////

testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    const pmWGraphEnvironment the_test_environment;

    PILGRIM::pmGraph graph = the_test_environment.wgraphTest->getGraph();
    PILG_TEST_FAIL_COUNT(num_vertices(graph) == 8, the_errors);

    PILGRIM::pmNode        node;
    plVariablesConjunction parents;
    // Bronchitis
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(0, graph), graph) == 1, the_errors);
    // Dyspnea
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(1, graph), graph) == 2, the_errors);
    // LungCancer
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(2, graph), graph) == 1, the_errors);
    // Smoking
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(3, graph), graph) == 0, the_errors);
    // TbOrCa
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(4, graph), graph) == 2, the_errors);
    // Tuberculosis
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(5, graph), graph) == 1, the_errors);
    // VisitToAsia
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(6, graph), graph) == 0, the_errors);
    // XRay
    PILG_TEST_FAIL_COUNT(boost::in_degree(vertex(7, graph), graph) == 1, the_errors);

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}


int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();

    std::cout << "Error count = " << the_errors << std::endl;

    return the_errors;
}
