/*=============================================================================
* Product        : PILGRIM
* File           : pmClassifierSingleBN.h
* Author         : PONSARD Soline
* Creation       : Juillet 2020
*=============================================================================
*        (c) Copyright 2020, PILGRIM - all rights reserved
*=============================================================================
*/
#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/pmClassifierSingleBN.h"
#include "pilgrim/general/pmClassifier.h"
#include <pilgrim/general/pmUtility.h>
#include <pilgrim/general/pmFrequencyCounter.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/scores/ScoreBIC.h>
#include <pilgrim/general/scores/cache.h>

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <iterator>

using namespace boost;
using namespace std;
using namespace PILGRIM;

//=============================================================================

/*
=====CONSTRUCTORS=====
*/
pmClassifierSingleBN::pmClassifierSingleBN(){}
//=============================================================================
pmClassifierSingleBN::pmClassifierSingleBN(plVariable& classC):pmClassifier(classC) {}
//=============================================================================
pmClassifierSingleBN::pmClassifierSingleBN(plVariable& classC, plVariablesConjunction& VarsWithoutClass):pmClassifier(classC, VarsWithoutClass) {}
//=============================================================================
pmClassifierSingleBN::pmClassifierSingleBN(plVariable& classC, pmBayesianNetwork* bn)
{
  this->classC = classC;
  this->bn = bn;
}
//=============================================================================

/*
=====DESTRUCTEUR=====
*/
pmClassifierSingleBN::~pmClassifierSingleBN() {}
//=============================================================================

/*
=====METHODES=====
*/
pmBayesianNetwork* pmClassifierSingleBN::get_bn()
{
  return this->bn;
}
//=============================================================================
void pmClassifierSingleBN::set_bn(pmBayesianNetwork* bn)
{
  this->bn = bn;
}
//=============================================================================
plDistribution pmClassifierSingleBN::query(plValues evidence1_new)
{
  plDistribution pld_new;
  pmBayesianNetwork* bnT = this->bn;
  pld_new = bnT->query(this->classC ,evidence1_new);

  return pld_new;
}
//=============================================================================
void pmClassifierSingleBN::learnBN_TAN(pmCSVDataSet* data)
{
  cout << "\n \n // ============  Beggining Learning ============= // \n\n";

  plVariablesConjunction vars = data->observed_variables();

  int classT = vars.get_variable_position(this->classC);

  string name_xml ="../../benchmarks/networks/lpBnEAP.xml";

  // set up frequency counter
  pmFrequencyCounter *fc = new pmFrequencyCounter( data->get_data_descriptor(),
  vars);
  fc->computeFrequencies();

  // Use cache for faster computation
  pmCache *cache = new PILGRIM::pmCache(1000000000);

  //create score and algorithm
  pmBayesianNetwork bn_empty(vars);

  pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
  pmAlgoTAN <rowDataType> algoTAN(data, pScore, classT);

  // compute BN TAN
  pmBayesianNetwork *bn_learned_TAN = new pmBayesianNetwork(algoTAN.getVariables());
  algoTAN.run(bn_learned_TAN);
  bn_learned_TAN->initComputableObjectList(true, true);

  pmGraph bn_learned_Graph = bn_learned_TAN->get_graph();
  plBayesianNetwork pl_bn(bn_learned_TAN->getJointDistribution(),"BN TAN");
  pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn_TAN.dot");

  bn_learned_TAN->learnParameters(data);

  this->bn = bn_learned_TAN;

  cout << "\n \n // ============  Finished Learning ============= // \n\n";
}
//=============================================================================
void pmClassifierSingleBN::learnBN_NB(pmCSVDataSet* data)
{
  cout << "\n \n // ============  Beggining Learning ============= // \n\n";

  plVariablesConjunction vars = data->observed_variables();

  int classT = vars.get_variable_position(this->classC);

  pmBayesianNetwork *bnC = new pmBayesianNetwork(vars);
  bnC->naiveBayes(classT);
  bnC->learnParameters(data);

  this->bn = bnC;

  cout << "\n \n // ============  Finished Learning ============= // \n\n";
}
//=============================================================================
