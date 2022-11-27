#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pilgrim/general/pmPDAG.h>
#include <pl.h>

#include <iostream>
#include <memory>

#include "testing.h"


////////////////////////////////////////////////////////////////////////////////
/// Test environment definition
////////////////////////////////////////////////////////////////////////////////

struct pmPDAGEnvironment {
public:
    pmPDAGEnvironment() noexcept {
        plJointDistribution jd;
        jd.load_from_genie("../benchmarks/networks/Asia.xdsl");
        PILGRIM::pmBayesianNetwork bnAsia{jd};
        PILGRIM::pmGraph           asiaGraph = bnAsia.get_graph();
        pdagTest                             = std::make_unique<PILGRIM::pmPDAG>(asiaGraph);
    }

public:
    std::unique_ptr<PILGRIM::pmPDAG> pdagTest;
};


////////////////////////////////////////////////////////////////////////////////
/// Test case definition
////////////////////////////////////////////////////////////////////////////////

testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    const pmPDAGEnvironment the_test_environment;

    PILGRIM::pmGraph graph = the_test_environment.pdagTest->getGraph();
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

testing::Uint32 fn2() {
    testing::Uint32 the_errors = 0;

    const pmPDAGEnvironment the_test_environment;

    PILGRIM::pmGraph                                                  graph     = the_test_environment.pdagTest->getGraph();
    boost::property_map<PILGRIM::pmGraph, boost::edge_weight_t>::type weightmap = get(boost::edge_weight, graph);

    PILGRIM::pmEdgeIterator ei, ei_end;
    boost::tie(ei, ei_end) = edges(graph);

    // 3 --1--> 0
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 1, the_errors);
    ++ei;
    // 3 --1--> 2
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 1, the_errors);
    ++ei;
    // 6 --1--> 5
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 1, the_errors);
    ++ei;
    // 5 --2--> 4
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 2, the_errors);
    ++ei;
    // 2 --2--> 4
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 2, the_errors);
    ++ei;
    // 4 --2--> 7
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 2, the_errors);
    ++ei;
    // 4 --2--> 1
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 2, the_errors);
    ++ei;
    // 0 --2--> 1
    PILG_TEST_FAIL_COUNT(weightmap[*ei] == 2, the_errors);

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
