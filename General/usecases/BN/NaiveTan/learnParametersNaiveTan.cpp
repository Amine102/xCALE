/* ############################################################################
* Project :                                                                   *
* Function : Create matrix with TAN algo. Learn parameters and struct         *
* cf. SingleBN usecase to use pmClassifier class                              *                                         *
* Author : Soline PONSARD                                                     *
* (c) Copyright juin 2020                                                         *
############################################################################# */

#include "learnParametersNaiveTan.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/scores/cache.h>

using namespace std;
using namespace PILGRIM;

void learnParametersNaiveTan() {

    size_t line_skipped = 1;
    size_t nb_param = 9;
    char delimiter = ';';
    pmScoreValueType priorWeight = 1;

    plError error;
    error.ignore_this_message(35,true);
    error.ignore_this_message(50,true);

    char* data_file = "../../benchmarks/data/NaiveTan_csv3.data";
    pmCSVDataSet* data = new pmCSVDataSet( data_file,
                                           line_skipped,
                                           nb_param,
                                           delimiter);

    plVariablesConjunction vars = data->observed_variables();

    cout << "\n \n // ============  Beggining Trading from Robankhood Structural Learning ============= // \n\n";

    string name_xml ="../../benchmarks/networks/lpBnNaiveTanEAP.xml";

    // set up frequency counter
    pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
    vars);
    fc->computeFrequencies();

    // Use cache for faster computation
    pmCache *cache = new PILGRIM::pmCache(1000000000);

    //create score and algorithm
    pmBayesianNetwork bn_empty(vars);

    pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
    int classC = 8;
    pmAlgoTAN <rowDataType> algo_TAN(data, pScore,classC);
    // pmAlgoMWST <rowDataType> algo_TAN(data, pScore);

    // compute BN TAN
    pmBayesianNetwork *bn_learned_TAN = new pmBayesianNetwork(algo_TAN.getVariables());
    algo_TAN.run(bn_learned_TAN);
    bn_learned_TAN->initComputableObjectList(true, true);

    bn_learned_TAN->learnParameters(data);

    pmGraph bn_learned_Graph = bn_learned_TAN->get_graph();
    plBayesianNetwork pl_bn(bn_learned_TAN->getJointDistribution(),"BN NaiveTan TAN");
    pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn__NaiveTan3_Trading_TAN.dot");


    cout << "\n \n // ============  Finished Trading from Robankhood Structural Learning ============= // \n\n";
    //Print & export

    data->summary();

    cout << "\n \n // ============  Calculate Confusion Matrix Algorithm TAN ============= // \n\n";

    // map<map<plData, plData>,int> confMatrix_TAN = bn_learned_TAN->confMatrix(8,data,true);
  }
