/*=============================================================================
 * Product        : PILGRIM
 * File           : pmClassifier.h
 * Author         : PONSARD Soline
 * Creation       : JUILLET 2020
 *=============================================================================
 *        (c) Copyright 2020, PILGRIM - all rights reserved
 *=============================================================================
 */

#ifndef PMCLASSIFIER_H
#define PMCLASSIFIER_H

#include <pl.h>
#include "pilgrim/general/pmUtility.h"
#include "pilgrim/general/pmBayesianNetwork.h"
#include <string>
#include <iostream>
#include <vector>

/*!
 *  \brief pmClassifier class
 *
 *  \details   pmClassifier : the mother-class
 *  \author    PONSARD Soline
 *  \date      Juillet, 2020
 *  \bug
 *  \todo
 */

namespace PILGRIM {
class pmClassifier {
 protected:
   plVariable classC;
   plVariablesConjunction VarsWithoutClass;

 public:
   //Constructeurs
   /*!
    *  \brief Default constructor.
    */
   pmClassifier();

   /**
     * \brief Constructor from plVariable
     * \details
     * \param[in] classC plVariable
     */
   pmClassifier(plVariable classC);

   /**
     * \brief Constructor from plVariable and plVariablesConjunction
     * \details
     * \param[in] classC plVariable, VarsWithoutClass plVariablesConjunction
     */
   pmClassifier(plVariable& classC, plVariablesConjunction& VarsWithoutClass);

   //Destructeur
   /**
     * \brief Destructor.
     * \details It destroys the classifier Multinet.
     */
   ~pmClassifier();

   //Méthodes
   void set_class(plVariable classC);
   plVariable get_class();
   void set_varsWithoutClass(plVariablesConjunction VarsWithoutClass);
   plVariablesConjunction get_varsWithoutClass();

   /**
     * \brief       To do a query
     * \param[in]   evidence1_new - create after.
     */
   virtual plDistribution query(plValues evidence1_new);

   /**
     * \brief       Create and print confusion matrix
     * \param[in]   data - create with the constructor pmCSVDataSet().
     * \param[in]   print - boolean to print or not the matrix.
     */
   std::map<std::map<plData, plData>,int> confMatrix(pmCSVDataSet* data , bool print);
};
};  // namespace PILGRIM

#endif
