#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pl.h>
#include <plCSVFileDataDescriptor.h>

#include <string>

#include "testing.h"


////////////////////////////////////////////////////////////////////////////////
/// Test case definition
////////////////////////////////////////////////////////////////////////////////

testing::Uint32 fn1() {
    testing::Uint32 the_errors = 0;

    try {
        // Naivly test for I/O errors
        pmBayesianNetwork bnTemp("../benchmarks/networks/Asia.xdsl");

        bnTemp.summary();
        bnTemp.generate_csvData("../benchmarks/data/Asia1.data", 1000);
        bnTemp.generate_csvData("../benchmarks/data/Asia2.data", 800);
        bnTemp.generate_csvData("../benchmarks/data/Asia3.data", 600);
        bnTemp.generate_csvData("../benchmarks/data/Asia4.data", 500);
        bnTemp.generate_csvData("../benchmarks/data/Asia4.data", 300);
    } catch(const std::exception& e) {
        PILG_TEST_FAIL_COUNT(false, the_errors);
    }

    std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
    return the_errors;
}

testing::Uint32 fn2() {
    testing::Uint32 the_errors = 0;

    constexpr testing::Uint32 kOptimizationCount = 100;

    for(testing::Uint32 i = 0; i < kOptimizationCount; ++i) {
        try {
            std::string path_to_csv = "../benchmarks/data/Asia1.data";
            // This might buffer overflow because pmCSVDataSet "offer" to the user to correct a path if it's not valid and by
            // doing so it can write on the c_string passed to it. This is not valid code, there is bug where the new filename
            // entered by the user can be longer than the filename passed originally, we then overflow a buffer !
            PILGRIM::pmCSVDataSet data{&path_to_csv[0]};

            //Load Original BN
            pmBayesianNetwork bnDummyOriginalTemp{"../benchmarks/networks/Asia.xdsl"};


            const plVariablesConjunction variables = data.observed_variables();

            // set up frequency counter
            pmFrequencyCounter fc{data.get_data_descriptor(),
                                  variables};
            fc.computeFrequencies();

            // Use cache for faster computation
            pmCache cache{1000000000};

            //create score and algorithm
            pmBayesianNetwork bn_empty{variables};

            pmScoreBIC<rowDataType> pScore{&bn_empty, &fc, &cache};

            pmAlgoMWST<rowDataType> algoMWST(&data, &pScore);

            pmBayesianNetwork learnedBN_MWST{algoMWST.getVariables()};

            algoMWST.run(&learnedBN_MWST);
            learnedBN_MWST.initComputableObjectList(true, true);

            pmGraph bn_learned_Graph  = learnedBN_MWST.get_graph();
            pmGraph bn_original_Graph = bnDummyOriginalTemp.get_graph();

            // compare graphs
            unsigned int tp, fp, tn, fn, reverse, shd;
            bnDummyOriginalTemp.compareBNs(bn_original_Graph, bn_learned_Graph,
                                           tp, fp, tn, fn, shd);

            learnedBN_MWST.save_as_xml("../benchmarks/networks/asia_learnedStructureAlgoMWST.xml", "mwst_asia");

            // This is not ideal be at least it can be used to check regression
            // or deviatino of the algo
            PILG_TEST_FAIL_COUNT(tp == 6, the_errors);
            PILG_TEST_FAIL_COUNT(fp == 1, the_errors);
            PILG_TEST_FAIL_COUNT(tn == 19, the_errors);
            PILG_TEST_FAIL_COUNT(fn == 2, the_errors);
            PILG_TEST_FAIL_COUNT(shd == 7, the_errors);

            auto the_bic_score = pScore.compute();

            PILG_TEST_FAIL_COUNT(the_bic_score < 0, the_errors);
            // Within +- 1% of the "true" value
            PILG_TEST_FAIL_COUNT((-2348.02 * 1.01) < the_bic_score && the_bic_score < (-2348.02 * 0.99), the_errors);
        } catch(const std::exception& e) {
            PILG_TEST_FAIL_COUNT(false, the_errors);
        }
    }

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
