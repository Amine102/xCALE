// WARINING /!\	  : STATUS ON PROGRESS

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "pilgrim/general/scores/pmMTpScore.h"


using namespace std;
using namespace PILGRIM;
using namespace boost;

/*=====CONSTRUCTORS=====*/

pmMTpScore::pmMTpScore(pmMTCSVDataSet mt_data, std::string score_type){

    int nb_tasks = mt_data.get_nb_datasets();

    for (int d=0; d < nb_tasks; d++){

        this->push_mtScores(d, mt_data, score_type);

    }


}

/*=====PUSHERS=====*/
void pmMTpScore::push_mtScores(int task_id, pmMTCSVDataSet & mt_data, std::string score_type){

    //Getting the variables from the loaded data
    plVariablesConjunction variables = mt_data.all_data_vec[task_id]->observed_variables();
    this->all_plvars.push_back(variables);

    //Set up frequency counter
    pmFrequencyCounter* fc = new pmFrequencyCounter(mt_data.all_data_vec[task_id]->get_data_descriptor(), this->all_plvars[task_id]);
    fc->computeFrequencies();
    this->all_fc_vec.push_back(fc);

    // Use cache for faster computation
    pmCache *cache = new PILGRIM::pmCache(100000000);
    this->all_cache.push_back(cache);

    // Create score
    pmBayesianNetwork* bn_empty = new pmBayesianNetwork(this->all_plvars[task_id]);
    this->all_bn_vec.push_back(bn_empty);

    pmDecomposableScore<rowDataType> *pScore;

    if (score_type == "BIC")
    {
        pScore = new pmScoreBIC<rowDataType>(this->all_bn_vec[task_id], this->all_fc_vec[task_id], this->all_cache[task_id]);
    }
    else if (score_type == "AIC")
    {
        pScore = new pmScoreAIC<rowDataType>(this->all_bn_vec[task_id], this->all_fc_vec[task_id], this->all_cache[task_id]);
    }
    else if (score_type == "BDeu")
    {
        pScore = new pmScoreBDeu<rowDataType>(this->all_bn_vec[task_id], this->all_fc_vec[task_id], 1, this->all_cache[task_id]);
    }
    else if (score_type == "QNML")
    {
        pScore = new pmScoreQNML<rowDataType>(this->all_bn_vec[task_id], this->all_fc_vec[task_id], this->all_cache[task_id]);
    }
    else if (score_type == "MDL")
    {
        pScore = new pmScoreMDL<rowDataType>(this->all_bn_vec[task_id], this->all_fc_vec[task_id], this->all_cache[task_id]);
    }

    this->all_pScores.push_back(pScore);    

}