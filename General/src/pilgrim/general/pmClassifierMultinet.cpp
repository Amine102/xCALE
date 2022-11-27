/*=============================================================================
* Product        : PILGRIM
* File           : pmClassifierMultinet.h
* Author         : PONSARD Soline
* Creation       : Juillet 2020
*=============================================================================
*        (c) Copyright 2020, PILGRIM - all rights reserved
*=============================================================================
*/
#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/pmCSVDataSet.h"
#include "pilgrim/general/pmClassifierMultinet.h"
#include "pilgrim/general/pmClassifier.h"
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
pmClassifierMultinet::pmClassifierMultinet() {}
//=============================================================================
pmClassifierMultinet::pmClassifierMultinet(plVariable& classC, plVariablesConjunction& VarsWithoutClass):pmClassifier(classC, VarsWithoutClass) {}
//=============================================================================
pmClassifierMultinet::pmClassifierMultinet(plVariable& classC):pmClassifier(classC) {}
//=============================================================================

/*
=====DESTRUCTEUR=====
*/
pmClassifierMultinet::~pmClassifierMultinet() {}
//=============================================================================

/*
=====METHODES=====
*/
vector<pmBayesianNetwork*> pmClassifierMultinet::get_vectorbn()
{
  return this->vectorBN;
}
//=============================================================================
void split2(const string &str, char delimiter, vector<string> &elem)
{
  stringstream ss(str);
  string str2;
  while (getline(ss, str2, delimiter))
  {
    elem.push_back(str2);
  }
}

vector<string> split1(const string &str, char delimiter)
{
  vector<string> elem;
  split2(str, delimiter, elem);
  return elem;
}

void pmClassifierMultinet::createFiles(pmCSVDataSet* data)
{
  plVariablesConjunction vars = data->observed_variables();

  int classT = vars.get_variable_position(this->classC);

  plType type = vars[classT].get_type();
  vector<string> vecVar =  type.get_values_as_strings();

  // All the values of the class
  for (string str : vecVar) {

    string myFile("../../benchmarks/data/dataclass_"+str+".data");

    std::string filename = data->get_filename();
    char* copy = strdup(filename.c_str());
    ifstream myFlux(copy);
    ofstream newFile(myFile.c_str());

    if(myFlux) {
      string row;
      int n = 0;

      while(getline(myFlux, row)) {
        // rewriting the header row to a file
        if (n == 0) {
          char delimiter = ';';
          vector<string> x = split1(row, delimiter);
          x.erase(x.begin() + classT);
          auto y = x.back();
          x.pop_back();
          for (auto elem : x) {
            newFile << elem;
            newFile << ";";
          }
          newFile << y << endl;
        }
        // rewriting the row to a file if the value of the class == the class value of the data
        n=1;
        char delimiter = ';';
        vector<string> x = split1(row, delimiter);

        if (x[classT]==str) {
          if(newFile) {
            x.erase(x.begin() + classT);
            auto y = x.back();
            x.pop_back();
            for (auto elem : x) {
              newFile << elem;
              newFile << ";";
            }
            newFile << y << endl;
          }
          else {
            cout << "ERROR : Unable to open the file dataclass_"+str+".data" << endl;
          }
        }
      }
    }
    else {
      cout << "ERROR : Unable to open the file." << endl;
    }
  }
}
//=============================================================================
void pmClassifierMultinet::learnBN(pmCSVDataSet* data)
{
  this->createFiles(data);

  plType type = this->classC.get_type();
  vector<string> vecVar =  type.get_values_as_strings();

  for (string str : vecVar)
  {
    const char* new_str1 = str.c_str();
    char* new_str2 = "../../benchmarks/data/dataclass_";
    char* new_str3 = ".data";

    int size1 = strlen(new_str1);
    int size2 = strlen(new_str2);
    int size3 = strlen(new_str3);

    char file[size1+size2+size3];
    file[0]='\0';

    strcat(file,new_str2);
    strcat(file,new_str1);
    strcat(file,new_str3);

    char* new_data_file = file;

    size_t line_skipped = 1;
    char delimiter = ';';
    size_t nb_param = data->guess_nb_variables(delimiter);
    size_t nb_param_inc = nb_param-1;
    pmScoreValueType priorWeight = 1;

    pmCSVDataSet* new_data = new pmCSVDataSet( new_data_file,
                                               line_skipped,
                                               nb_param_inc,
                                               delimiter,
                                               VarsWithoutClass);

    plCSVFileDataDescriptor* new_plDataDescriptor = new_data->get_data_descriptor();
    int new_nbRow = new_plDataDescriptor->get_n_records();

    plCSVFileDataDescriptor* plDataDescriptor = data->get_data_descriptor();
    int nbRow = plDataDescriptor->get_n_records();

    double proba = double(new_nbRow)/nbRow;
    // cout << "P( C = "+str+" ) : " << proba << endl;

    plVariablesConjunction new_vars = new_data->observed_variables();

    cout << "\n \n // ============  Beggining Learning ============= // \n\n";

    string name_xml ="../../benchmarks/networks/lpBnClassificationEAP_"+str+".xml";

    // set up frequency counter
    pmFrequencyCounter *fc = new pmFrequencyCounter( new_data->get_data_descriptor(),
                                                     new_vars);
    fc->computeFrequencies();

    // Use cache for faster computation
    pmCache *cache = new PILGRIM::pmCache(1000000000);

    //create score and algorithm
    pmBayesianNetwork bn_empty(new_vars);

    plVariablesConjunction vars = data->observed_variables();
    int classT = vars.get_variable_position(this->classC);

    pmScoreBIC<rowDataType> *pScore = new pmScoreBIC<rowDataType>(&bn_empty, fc, cache);
    pmAlgoTAN <rowDataType> algo_TAN(new_data, pScore, classT);

    // compute BN MWST
    pmBayesianNetwork *bn_learned_TAN = new pmBayesianNetwork(algo_TAN.getVariables());
    algo_TAN.run(bn_learned_TAN);
    bn_learned_TAN->initComputableObjectList(true, true);

    pmGraph bn_learned_Graph = bn_learned_TAN->get_graph();
    plBayesianNetwork pl_bn(bn_learned_TAN->getJointDistribution(),"BN classification "+str+" TAN");
    pl_bn.draw_graph_dot("../../benchmarks/graphs/graph_bn__Classification_"+str+"_TAN.dot");

    bn_learned_TAN->setLearnObjectEAP(priorWeight);
    bn_learned_TAN->learnParameters(new_data);
    cout << "\n \n // ============  Finished Learning ============= // \n\n";
    bn_learned_TAN->weight = proba;
    this->vectorBN.push_back(bn_learned_TAN); //Changer nom bn avec str
  }
}
  //=============================================================================
plDistribution pmClassifierMultinet::query(plValues evidence1_new)
{
   map<string,double> Cproba;
   double total;
   plType type = this->classC.get_type();
   vector<string> vecVar =  type.get_values_as_strings();
   int i=0;
   for (string str : vecVar)
   {
      pmBayesianNetwork* bn = this->vectorBN[i];
      plJointDistribution jd = bn->getJointDistribution();

      plProbValue totalPart = 0.0;
      plDistribution pld_new = jd.ask(this->VarsWithoutClass);
      plProbValue val = jd.compute(evidence1_new);

      double proba = bn->weight;
      totalPart = val*proba;
      total = total + totalPart;
      Cproba[str] = totalPart;
      i++;
   }

   vector<plProbValue> probaDistribution;

   map<string, double>::iterator p;
   for(p = Cproba.begin(); p != Cproba.end(); p++)
   {
     plProbValue tot = double(p->second)/(total);
     probaDistribution.push_back(tot);
   }

   plDistribution jd(this->classC, probaDistribution);
   return jd;
}
  //=============================================================================
