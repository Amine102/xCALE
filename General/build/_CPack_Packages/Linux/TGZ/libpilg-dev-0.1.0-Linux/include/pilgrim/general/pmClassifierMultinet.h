/*=============================================================================
 * Product        : PILGRIM
 * File           : pmClassifierMultinet.h
 * Author         : PONSARD Soline
 * Creation       : JUILLET 2020
 *=============================================================================
 *        (c) Copyright 2020, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef PMCLASSIFIERMULTINET_H
#define PMCLASSIFIERMULTINET_H

#include <string>
#include <vector>
#include <map>
#include "pilgrim/general/pmClassifier.h"
#include <pl.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <iostream>

/*!
 *  \brief pmClassifierMultinet class
 *
 *  \details   pmClassifierMultinet : the daughter-class of the pmClassifier
 *  \author    PONSARD Soline
 *  \date      Juillet, 2020
 *  \bug
 *  \todo
 */

namespace PILGRIM {
class pmClassifierMultinet : public pmClassifier {
 public:
   //Constructeurs
   /*!
    *  \brief Default constructor.
    */
   pmClassifierMultinet();

   /**
     * \brief Constructor from plVariable
     * \details
     * \param[in] classC plVariable
     */
   pmClassifierMultinet(plVariable& classC);

   /**
     * \brief Constructor from plVariable and plVariablesConjunction
     * \details
     * \param[in] classC plVariable, VarsWithoutClass plVariablesConjunction
     */
   pmClassifierMultinet(plVariable& classC, plVariablesConjunction& VarsWithoutClass);

   //Destructor
   /**
     * \brief Destructor.
     * \details It destroys the classifier Multinet.
     */
   ~pmClassifierMultinet();

   //Méthodes
   std::vector<pmBayesianNetwork*> get_vectorbn();

  /**
    * \brief       Create different files according to the value of the class
    * \param[in]   data - create with the constructor pmCSVDataSet().
    */
   void createFiles(pmCSVDataSet* data);

   /**
     * \brief       Learn bayesien network with MWST algo
     * \param[in]   data - create with the constructor pmCSVDataSet().
     */
   void learnBN(pmCSVDataSet* data);

   /**
     * \brief       To do a query
     * \param[in]   evidence1_new - create after.
     */
   plDistribution query(plValues evidence1_new);

 private:
   std::vector<pmBayesianNetwork*> vectorBN;
};
};  // namespace PILGRIM

/**
  * \brief  Function to change string to vector<string>
  *         27;2;3;3;1;1;3;4;0  -->  [27][2][3][3][1][1][3][4][0]
  * \param[in] str - the initial string .
  * \param[in] delimiter - the delimiter that separate the elements.
  * \param[in] elem - vector to store the string.
  */
void split2(const std::string &str, char delimiter, std::vector<std::string> &elem);
std::vector<std::string> split1(const std::string &str, char delimiter);

#endif
