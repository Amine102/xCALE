/*=============================================================================
 * Product        :
 * File           : AlgoMMPCbar_T
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef _AlgoMMPCbar_T_H
#define _AlgoMMPCbar_T_H

#include <pl.h>
#include <iostream>
#include <vector>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>

#include "pilgrim/general/pmCSVDataSet.h"





// Map iterator to store MaxMin variables along with assoc
typedef std::multimap<float, unsigned int> MaxMinVarMap;
typedef std::vector< std::vector <unsigned int>> SubSets;
typedef std::pair<bool, float> pair_t;

namespace PILGRIM{

class AlgoMMPCbar_T {
 public:
  //! A constructor.
  AlgoMMPCbar_T(pmCSVDataSet* _dataSet,          /*! Data set matrix */
    const unsigned int &_condVarSize = 10,    /*! Optional: Fix the maximum number of conditional variables in independence test. Default are 10  */
    bool _debugMode = false
    );

  //! Destructor
  ~AlgoMMPCbar_T(void);

  void setAlpha(float alpha);
  
  //! Compute CPC for a target variable
    /*!
      \param 1 Target variable as a const unsigned integer.
      \return  a vector of CPCs.
    */
  void computeMMPCbar_T(unsigned int&,                          ///< Target variable
                       std::vector<unsigned int>&         ///<  Return learned cpc
                      );

  //!
  void phase1_T(unsigned int&, int);  /// Target variable
  /**
  * \brief
  * \details
  * \param variables :
  */

  void phase2_T(unsigned int& vTarget, int);

  //! To calculate sub sets of size n.
  /*!
  \return  It will return list of subsets by using function computeSubsets().
  */
  void subs(const std::vector<unsigned int>& org, const std::vector<unsigned int>& pre, size_t k, size_t n, SubSets& c);

  //! To calculate sub sets of size n.
  /*!
  \return  It will return list of subsets by using function subs().
  */
  //void computeSubSets(SubSets& c, unsigned int& subSetSize);
  void computeSubSets(const std::vector<unsigned int>& org, size_t& n, SubSets& c);
  /**
  * \brief
  * \details
  * \param variables :
  */
  bool computeMaxMinHeuristic(unsigned int& vTarget, unsigned int& varF, float& assocF, MaxMinVarMap& mapMinAssoc);
  /**
  * \brief
  * \details
  * \param variables :
  */
  bool computeMinAssoc (const unsigned int& vTarget, const unsigned int& vMax, pair_t& minAssoc);
  /**
  * \brief
  * \details
  * \param variables :
  */
  bool isInList(const unsigned int& var, std::vector<unsigned int>& vList);
  /**
  * \brief
  * \details
  * \param variables :
  */
  bool isInList(const unsigned int& var, std::vector<unsigned int>& vList, std::vector<unsigned int>::iterator& position);
  /**
  * \brief
  * \details
  * \param variables :
  */
  bool isInList(const unsigned int& var, std::map <unsigned int, CPC_t>& mapList);

  static unsigned int varCounter;
  static unsigned int numVars;                  ///< Number of total vars
  static plVariablesConjunction all_variable;

  static int t;  // time slice number //add by ghada
  static std::map <unsigned int, std::vector<unsigned int> > cpcPath;  ///<  To store learning path for final CPC
  static std::map <unsigned int, std::vector<unsigned int>> CPC_tBar;  ///< to store cpc of target variable in time slice //add by ghada
  static std::vector<unsigned int> CPC;
  static std::vector<unsigned int> CC;
  static std::vector<unsigned int> PC;

 protected:
  float subsetTime;
  float alpha;
  float p;
  float MI;
  unsigned int df;
  unsigned int conditionalVariableSize;  ///< conditional variables limit for independence test.
  std::vector<unsigned int> varList;  //= new std::vector<unsigned int>();    ///< To store variable indexes
  std::vector<unsigned int> fullVarList;  // = new std::vector<unsigned int>();
  pmCSVDataSet* ds;                      ///< data set
  bool debugMode;                        ///< Debug mode by default off
  std::vector<unsigned int> *CPCBar;  ///< to store cpc of target variable
  std::vector<unsigned int>::iterator cpcItr;  ///< iterator for cpcbar
  int nFunctionCalls;

};
};
#endif

/**
 * Creation: ????
 * Last update: 2019/06/14
 * Adding setAlpha setter
 * Modif : Côme Vitis
 */