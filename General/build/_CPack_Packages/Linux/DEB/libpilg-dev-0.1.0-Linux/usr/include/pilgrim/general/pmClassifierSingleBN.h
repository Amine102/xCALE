/*=============================================================================
 * Product        : PILGRIM
 * File           : pmClassifierSingleBN.h
 * Author         : PONSARD Soline
 * Creation       : JUILLET 2020
 *=============================================================================
 *        (c) Copyright 2020, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef PMCLASSIFIERSINGLEBN_H
#define PMCLASSIFIERSINGLEBN_H

#include <string>
#include <map>
#include "pilgrim/general/pmClassifier.h"
#include <pilgrim/general/pmBayesianNetwork.h>
#include <pl.h>
#include <iostream>
#include <vector>

/*!
 *  \brief pmClassifierSingleBN class
 *
 *  \details   pmClassifierSingleBN : the daughter-class of the pmClassifier
 *  \author    PONSARD Soline
 *  \date      Juillet, 2020
 *  \bug
 *  \todo
 */

namespace PILGRIM {
class pmClassifierSingleBN : public pmClassifier {
 public:
   //Constructeurs
   /*!
    *  \brief Default constructor.
    */
   pmClassifierSingleBN();

   /**
     * \brief Constructor from plVariable
     * \details
     * \param[in] classC plVariable
     */
   pmClassifierSingleBN(plVariable& classC);

   /**
     * \brief Constructor from plVariable and plVariablesConjunction
     * \details
     * \param[in] classC plVariable, VarsWithoutClass plVariablesConjunction
     */
   pmClassifierSingleBN(plVariable& classC, plVariablesConjunction& VarsWithoutClass);

   /**
     * \brief Constructor from plVariable and pmBayesianNetwork
     * \details
     * \param[in] classC plVariable, bn pmBayesianNetwork
     */
   pmClassifierSingleBN(plVariable& classC, pmBayesianNetwork* bn);

   //Destructeur
   /**
     * \brief     Destructor.
     * \details   It destroys the classifier Multinet.
     */
   ~pmClassifierSingleBN();

   //Méthodes
   pmBayesianNetwork* get_bn();
   void set_bn(pmBayesianNetwork* bn);

   /**
     * \brief       to do a query
     * \param[in]   evidence1_new - create after.
     */
   plDistribution query(plValues evidence1_new);

   /**
     * \brief      learn BN with TAN algo
     * \param[in]  data - create with the constructor pmCSVDataSet().
     */
   void learnBN_TAN(pmCSVDataSet* data);

   /**
     * \brief       learn BN with naive bayesien
     * \param[in]   data - create with the constructor pmCSVDataSet().
     */
   void learnBN_NB(pmCSVDataSet* data);

 private:
   pmBayesianNetwork* bn;
};
};  // namespace PILGRIM
#endif
