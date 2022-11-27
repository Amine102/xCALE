#include <string>
#include <stdlib.h>
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <plCSVFileDataDescriptor.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/Score.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/Score.h>
#include <pilgrim/general/scores/ScoreAIC.h>
#include <pilgrim/general/scores/ScoreBDeu.h>
#include <pilgrim/general/scores/ScoreMDL.h>
#include <pilgrim/general/scores/ScoreQNML.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/scores/ScoreBIC.h>

#include "testing.h"

using namespace std;

testing::Uint32 fn1()
{
    testing::Uint32 the_errors = 0;
     //Asia with MWST structure Learning
     //Load Data
     string path_to_csv = "../benchmarks/data/Asia1.data";
     char *cstr = new char[path_to_csv.length() + 1];
     strcpy(cstr, path_to_csv.c_str());
     PILGRIM::pmCSVDataSet *data = new PILGRIM::pmCSVDataSet(cstr);

     //Load Original BN
     pmBayesianNetwork bnDummyOriginalTemp("../benchmarks/networks/Asia.xdsl");

     //Tune parameters
     string score = "BIC"; // we can choose BIC or AIC or BDeu or QNML or MDL
     bool debugVerbose = false;

     plVariablesConjunction variables = data->observed_variables();

     // set up frequency counter
     pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
                                                     variables);
     fc->computeFrequencies();

     // Use cache for faster computation
     pmCache *cache = new PILGRIM::pmCache(1000000000);

     //create score and algorithm
     pmBayesianNetwork bn_empty(variables);

     pmDecomposableScore<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);

     pmAlgoMWST<rowDataType> algoMWST(data, pScore);
     
    PILG_TEST_FAIL_COUNT(algoMWST.getScore()->cache != 0, the_errors);
    
     pmBayesianNetwork *learnedBN_MWST = new pmBayesianNetwork(algoMWST.getVariables());
     algoMWST.run(learnedBN_MWST);
     learnedBN_MWST->initComputableObjectList(true, true);

     std::cout << PILG_TEST_PRESENT_RESULTS(the_errors) << '\n';
     return the_errors;
}

int main() {
    testing::Uint32 the_errors = 0;

    the_errors += fn1();

    std::cout << "Error count = " << the_errors << std::endl;

    return the_errors;
}
