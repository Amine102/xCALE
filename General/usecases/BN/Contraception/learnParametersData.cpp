/* ##################################################################################
* Project :                                                                          *
* Function : Learn Param and struct with TAN to do query and compare with Multi-net  *
* (cf. Contraception/learnParametersData.cpp)                                        *
* cf. multinet usecase to use pmClassifier class                                     *
* Author : Soline PONSARD                                                            *
* (c) Copyright juin 2020                                                                *
#################################################################################### */

#include "learnParametersData.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/algorithms/AlgoGS.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/scores/cache.h>

using namespace std;
using namespace PILGRIM;

void learnParametersData() {

    size_t line_skipped = 1;
    size_t nb_param = 6;
    char delimiter = ';';
    pmScoreValueType priorWeight = 1;

    int classC = 0;

    plError error;
    error.ignore_this_message(35,true);
    error.ignore_this_message(50,true);

    char* data_file = "../../benchmarks/data/mammographic_masses.data";
    pmCSVDataSet* data = new pmCSVDataSet( data_file,
                                           line_skipped,
                                           nb_param,
                                           delimiter);

    plVariablesConjunction vars = data->observed_variables();

    // Creation of Bayesian network for which we will learns the parameters
    // from data using maximum likelyhood
    pmBayesianNetwork bnDataML(vars);
    bnDataML.naiveBayes(classC);
    bnDataML.learnParameters(data);

    cout << "\n \n // ============  Beggining Trading from Robankhood Structural Learning ============= // \n\n";

    string name_xml ="../../benchmarks/networks/lpBnDataEAP.xml";

    // set up frequency counter
    pmFrequencyCounter *fc = new pmFrequencyCounter(data->get_data_descriptor(),
    vars);
    fc->computeFrequencies();

    // Use cache for faster computation
    pmCache *cache = new PILGRIM::pmCache(1000000000);

    //create score and algorithm
    pmBayesianNetwork bn_empty(vars);

    pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
    pmAlgoTAN <rowDataType> algo_TAN(data, pScore,classC);
    // pmAlgoMWST <rowDataType> algo_MWST(data, pScore);


    // compute BN TAN
    pmBayesianNetwork *bn_learned_TAN = new pmBayesianNetwork(algo_TAN.getVariables());
    algo_TAN.run(bn_learned_TAN);
    bn_learned_TAN->initComputableObjectList(true, true);

    pmGraph bn_learned_Graph = bn_learned_TAN->get_graph();
    plBayesianNetwork pl_bn(bn_learned_TAN->getJointDistribution(),"BN Data TAN");
    pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn__Data_Trading_TAN.dot");
    // pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn__Iris_Trading_MWST.dot");

    // bn_learned_TAN->setLearnObjectEAP(priorWeight);
    bn_learned_TAN->learnParameters(data);

    bn_learned_TAN->summary();

    cout << "\n \n // ============  Finished Trading from Robankhood Structural Learning ============= // \n\n";

    plVariablesConjunction Variables = bn_learned_TAN->getVariables();

    plVariable A_new  = Variables.get_variable_with_name("Age");
    plVariable B_new  = Variables.get_variable_with_name("Shape");
    plVariable C_new  = Variables.get_variable_with_name("Margin");
    plVariable D_new  = Variables.get_variable_with_name("Density");
    plVariable E_new  = Variables.get_variable_with_name("Severity");

    plVariable Class_new  = Variables.get_variable_with_name("BI-RADS");

    plValues evidence1_new(A_new^B_new^C_new^D_new^E_new);

    evidence1_new[A_new]  = 71;
    evidence1_new[B_new]  = 4;
    evidence1_new[C_new]  = 4;
    evidence1_new[D_new]  = 3;
    evidence1_new[E_new]  = 1;


    cout << "1st query" << endl;
    plDistribution pld_new =  bn_learned_TAN->query(Class_new, evidence1_new);
    pld_new.tabulate();

    // map<map<plData, plData>,int> confMatrix_BN = bn_learned_TAN->confMatrix(4,data,true);
  }
