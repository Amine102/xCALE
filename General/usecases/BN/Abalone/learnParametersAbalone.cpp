/* #####################################################################################
* Project :                                                                             *
* Function : Create the confusion matrix with TAN and naif bayes (and MWST) to compare  *
/!\ WARNING : non-functional -> discretization on class and we want non discretization  *
* Author : Soline PONSARD                                                               *
* Copyright juin 2020                                                                   *
####################################################################################### */

#include "learnParametersAbalone.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/cache.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <string>

using namespace std;
using namespace PILGRIM;

void learnParametersAbalone() {

    size_t line_skipped = 1;
    size_t nb_param = 9;
    char delimiter = ';';
    pmScoreValueType priorWeight = 1;

    int classC = 8;

    char* data_file = "../../benchmarks/data/abalone.data";

    pmCSVDataSet* data = new pmCSVDataSet( data_file,
                                           line_skipped,
                                           nb_param,
                                           delimiter);

    plVariablesConjunction vars = data->observed_variables();
    cout << "vars : " << vars << endl;
    plVariable var = vars[classC];
    plType type = var.get_type();
    plVariableType typeLab = var.get_var_type();
    cout << "type : " << type << endl;
    cout << "typeLab : " << typeLab << endl;
    long double card = var.cardinality();
    cout << "card : " << card << endl;

    // Creation of Bayesian network for which we will learns the parameters
    // from data using maximum likelyhood
    pmBayesianNetwork bnAbaloneML(vars);
    bnAbaloneML.naiveBayes(classC);
    bnAbaloneML.learnParameters(data);

    // Creation of Bayesian network for which we will learns the parameters
    // from data using expectation a posteriori
    pmBayesianNetwork bnAbaloneEAP(vars);
    bnAbaloneEAP.naiveBayes(classC);
    bnAbaloneEAP.setLearnObjectEAP(priorWeight);
    bnAbaloneEAP.learnParameters(data);

    cout << "\n \n // ============  Beggining Trading from Robankhood Structural Learning ============= // \n\n";

    string name_xml ="../../benchmarks/networks/lpBnAbaloneEAP.xml";

    // set up frequency counter
    pmFrequencyCounter *fc = new pmFrequencyCounter( data->get_data_descriptor(),
                                                     vars);
    fc->computeFrequencies();

    // Use cache for faster computation
    pmCache *cache = new PILGRIM::pmCache(1000000000);

    //create score and algorithm
    pmBayesianNetwork bn_empty(vars);

    pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
    pmAlgoTAN <rowDataType> algo_TAN(data, pScore,classC);
    pmAlgoMWST <rowDataType> algo_MWST(data, pScore);

    // compute BN TAN
    pmBayesianNetwork *bn_learned_TAN = new pmBayesianNetwork(algo_TAN.getVariables());
    algo_TAN.run(bn_learned_TAN);
    bn_learned_TAN->initComputableObjectList(true, true);

    // compute BN MWST
    pmBayesianNetwork *bn_learned_MWST = new pmBayesianNetwork(algo_MWST.getVariables());
    algo_MWST.run(bn_learned_MWST);
    bn_learned_MWST->initComputableObjectList(true, true);

    pmGraph bn_learned_Graph = bn_learned_TAN->get_graph();
    plBayesianNetwork pl_bn(bn_learned_TAN->getJointDistribution(),"BN Abalone TAN");
    pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn_Abalone_Trading_TAN.dot");
    // pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn_Abalone_Trading_MWST.dot");

    cout << "\n \n // ============  Finished Trading from Robankhood Structural Learning ============= // \n\n";
    //Print & export

    data->summary();

    cout << "Bayesian network learned with ML" << endl;
    bnAbaloneML.summary();
    bnAbaloneML.save_as_xml("../../benchmarks/networks/lpBnAbaloneML.xml","lpBnAbaloneML");

    cout << "Bayesian network learned using EAP" << endl;
    bnAbaloneEAP.summary();
    bnAbaloneEAP.save_as_xml("../../benchmarks/networks/lpBnAbaloneEAP.xml","lpBnAbaloneEAP");

    // // Calculate confusion matrix
    // cout << "\n \n // ============  Calculate Confusion Matrix Bayes Naif ============= // \n\n";
    //
    // map<map<plData, plData>,int> confMatrix_BN = bnAbaloneEAP.confMatrix(classC,data,true);
    //
    // cout << "\n \n // ============  Calculate Confusion Matrix Algorithm TAN ============= // \n\n";
    //
    // map<map<plData, plData>,int> confMatrix_TAN = bn_learned_TAN->confMatrix(classC,data,true);

  }
