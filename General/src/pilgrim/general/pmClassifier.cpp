/*=============================================================================
 * Product        : PILGRIM
 * File           : pmClassifier.h
 * Author         : PONSARD Soline
 * Creation       : Juillet 2020
 *=============================================================================
 *        (c) Copyright 2020, PILGRIM - all rights reserved
 *=============================================================================
 */
#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/pmClassifier.h"
#include <pilgrim/general/pmUtility.h>
#include <pilgrim/general/algorithms/AlgoTAN.h>
#include <pilgrim/general/algorithms/AlgoMWST.h>
#include <pilgrim/general/pmFrequencyCounter.h>
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
pmClassifier::pmClassifier() {}
//=============================================================================
pmClassifier::pmClassifier(plVariable classC)
{
  this->classC = classC;
}
//=============================================================================
pmClassifier::pmClassifier(plVariable& classC, plVariablesConjunction& VarsWithoutClass)
{
  this->classC = classC;
  this->VarsWithoutClass = VarsWithoutClass;
}
//=============================================================================

/*
=====DESTRUCTEUR=====
*/
pmClassifier::~pmClassifier() {}
//=============================================================================

/*
=====METHODES=====
*/
plVariable pmClassifier::get_class()
{
  return this->classC;
}
//=============================================================================
void pmClassifier::set_class(plVariable classC)
{
  this->classC = classC;
}
//=============================================================================
plVariablesConjunction pmClassifier::get_varsWithoutClass()
{
  return this->VarsWithoutClass;
}
//=============================================================================
void pmClassifier::set_varsWithoutClass(plVariablesConjunction VarsWithoutClass)
{
  this->VarsWithoutClass = VarsWithoutClass;
}
//=============================================================================
plDistribution pmClassifier::query(plValues evidence1_new)
{
  plDistribution pld_new;
  return pld_new;
}
//=============================================================================
map<map<plData, plData>,int> pmClassifier::confMatrix(pmCSVDataSet* data , bool print)
{
  map<map<plData, plData>,int> confMatrix;

  plCSVFileDataDescriptor* plDataDescriptor = data->get_data_descriptor();
  plValues data_values;

  plDataDescriptor->rewind();
  int nbRow = 0;

  while(plDataDescriptor->get_next_defined_values(data_values)){

    plVariablesConjunction variables = data_values.get_variables();
    plValues evidence = data_values.remove(this->classC);
    plDistribution pld = this->query(evidence);
    plValues bestVar = pld.best();

    plData CEstimate = bestVar[this->classC];
    plData CTheory = data_values[this->classC];

    std::map<plData, plData> mypair;
    mypair[CEstimate] = CTheory;
    if ( confMatrix.find(mypair) == confMatrix.end() ) {
      confMatrix[mypair] = 1;
    } else {
      confMatrix[mypair] +=1;
    }
    nbRow += 1;
  }

  if (print == true){
    // printing map
    cout << "Confusion matrix " << endl;

    std::map<plData,int> label;
    int j = 0;

    for( std::map<std::map<plData, plData>,int>::const_iterator ptr=confMatrix.begin();ptr!=confMatrix.end(); ptr++) {
      for( std::map<plData,plData>::const_iterator eptr=ptr->first.begin();eptr!=ptr->first.end(); eptr++){
        if ( label.find(eptr->first) == label.end() ) {
          label[eptr->first] = j;
          j++;
        } else if (label.find(eptr->second) == label.end()) {
          label[eptr->second] = j;
          j++;
        }
      }
    }

    cout << "legend : " << endl;
    cout << "label : num in the matrix" << endl;
    for( std::map<plData,int>::const_iterator ptr=label.begin();ptr!=label.end(); ptr++) {
      cout << ptr->first << " : " << ptr->second << endl;
    }

    int taille = label.size();

    cout << "   ";
    for (unsigned int i = 0; i < taille; i++) {
      cout << i << " ";
    }
    cout << endl;
    cout << endl;

    double rateClassification = 0;
    int nb = 0;

    for (unsigned int i = 0; i < taille; i++) {
      plData label_i;
      plData label_j;

      cout << i << "  ";
      for (unsigned int j = 0; j < taille; j++) {

        for( std::map<plData,int>::iterator it = label.begin(); it!=label.end(); it++){
          if(it->second == i){
            label_i = it->first;
          }
        }
        for( std::map<plData,int>::iterator it = label.begin(); it!=label.end(); it++){
          if(it->second == j){
            label_j = it->first;
          }
        }

        std::map<plData, plData> mypair;
        mypair[label_i] = label_j;
        if ( confMatrix.find(mypair) == confMatrix.end() ) {
          nb = 0;
          cout << nb << " ";
        } else {
          nb = confMatrix[mypair];
          cout << nb << " ";
        }
        if (i==j) {
          rateClassification += nb;
        }
      }
      cout << endl;
    }
    rateClassification /= nbRow;
    cout << "Rate of good classification : " << rateClassification << endl;
    return confMatrix;
  }
}
//=============================================================================
