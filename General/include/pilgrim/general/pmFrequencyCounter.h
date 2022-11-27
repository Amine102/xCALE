/*=============================================================================
 * Product        :
 * File           : FrequencyCounter.h
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef pm_FREQUENCY_COUNTER_H
#define pm_FREQUENCY_COUNTER_H

#include <pl.h>
#include <plConfig.h>
#include <plDataDescriptor.h>

#include <vector>
#include <iostream>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/filesystem/path.hpp>
#include <pilgrim/general/pmUtility.h>
#include "pilgrim/general/pmCommonTypeDefinitions.h"
//#include "pilgrim/general/pmBayesianNetwork.h"

//=============================================================================

namespace PILGRIM
{
    /*!
    *  \brief pmFrequencyCounter class
    *
    *  \details Base-class for frequency counting objects. An object of this type
    iterates on data in order to find the number of occurrences of a subset of variables in a given configuration. There must be
    coherency between the data set and the order of the variables.
    *  \author  BRISCAN Cristian
    *  \author  Amanullah YASIN
    *  \version   PILGRIM 0.1
    *  \date      March 11, 2013
    *  \pre       If there are initialization conditions or configuration parameters then write here.
    *  \bug       If there is known bug, please put here
    *  \warning   There is no warning.
    *  \todo      This class can be updated using smart pointers.
    */

    class pmFrequencyCounter
    {
    public:
        enum enumEstimationMethod
        {
            JointDistribution,
            ProbabilityTable,
            Uniform,
            IgnoreLine,
            UniformExtrapolation
        };

        /**
       Constructs a new frequency object counter giving the data set and
       the associated variable list.
    */
        pmFrequencyCounter(plDataDescriptor *pDataDesc, const plVariablesConjunction &allVariables);

        pmFrequencyCounter(pmCSVDataSet *data, const plVariablesConjunction &allVariables, bool useSlWind = false, float SW_forgettingCoeff = 1, bool AllowUpdates=false);
        /**
    Copy constructor
    */
        pmFrequencyCounter(pmFrequencyCounter &other);

        /** Destructor. */
        // virtual ~pmFrequencyCounter();

        /**
     * \brief  Changes the data set descriptor.
     * \details
     * \param variables :
     */
        virtual void setDataDescriptor(plDataDescriptor * /*<rowDataType>*/ pDataDesc);

        /**
     * \brief Returns the data set descriptor.
     * \details
     * \param variables :
     */
        plDataDescriptor * /*<rowDataType>**/ getDataDescriptor();

        /**
     * \brief
     * \details
     * \param variables :
     */
        virtual void setVariables(const plVariablesConjunction &allVariables);

        /**
     * \brief
     * \details
     * \param variables :
     */
        const plVariablesConjunction &getVariables();

        /**
     * \brief
     * \details
     * \param variables :
     */
        void setEstimationMethod(enumEstimationMethod estimationMethod);

        /**
     * \brief
     * \details
     * \param variables :
     */
        enumEstimationMethod getEstimationMethod();

        /**
     * \brief
     * \details
     * \param variables :
     */
        virtual void setMissingProbabilityTable(
            const std::vector<std::vector<plProbValue>> *
                missing_probability_table,
            bool autoDelete = false);

        /**
     * \brief
     * \details
     * \param variables :
     */
        const std::vector<std::vector<plProbValue>> *getMissingProbabilityTable();

        /**
     * \brief
     * \details
     * \param variables :
     */
        void setJointDistribution(plJointDistribution *pJD,
                                  bool autoDelete = false);

        /**
     * \brief
     * \details
     * \param variables :
     */
        plJointDistribution *getJointDistribution();

        /**
     * \brief
     * \details
     * \param variables :
     */
        virtual void computeFrequencies();
        virtual void frequencyCount(const std::vector<bool> &searchedVars,
                                    std::vector<plFloat> &freq);
        virtual void frequencyCount(
            const std::vector<unsigned int> &SearchedVariablesIndices,
            std::vector<plFloat> &freq);
        // added by F.schnitzler, 2nd December 2010

        /**
     * \brief
     * \details
     * \param variables :
     */
        virtual long unsigned int getExempleCount();

        /**
     * \brief
     * \details
     * \param variables :
     */
        virtual long unsigned int learnDistributions(
            plLearnDistribVector &learnDistribVect);
        virtual long unsigned int learnDistributions(
            plLearnDistribVector &learnDistribVect,
            const std::vector<bool> &searchedVars);
        virtual long unsigned int learnDistributions(
            plLearnDistribVector &learnDistribVect,
            const std::vector<unsigned int> &SearchedVariablesIndices);
        // added by F.schnitzler, 2nd December 2010

        virtual plJointDistribution *learnAndBuildDistribution(
            plLearnDistribVector &learnDistribVect,
            const std::vector<bool> &searchedVars);
        std::vector<plFloat> weights;

        void updateSW(pmBayesianNetwork bn);
        void printSW_Summary();

    protected:
        /**
     * \brief
     * \details
     * \param variables :
     */
        void constructSearchedVariableList(
            const std::vector<plLearnObject *> &learnObjects,
            std::vector<bool> &searchedVars);

        /**
     * \brief
     * \details
     * \param variables :
     */
        bool isMissingData(const std::vector<bool> &presentValues,
                           const std::vector<bool> &searchedVars);

        /**
     * \brief
     * \details
     * \param variables :
     */
        bool isMissingData(const std::vector<bool> &presentValues,
                           const std::vector<unsigned int> &searchedVars);
        // added by F.schnitzler, 2nd December 2010

        /**
     * \brief
     * \details
     * \param variables :
     */
        virtual const std::vector<plProbValue> *computeCurrentMissingProbabilities(
            long unsigned int idxLine,
            const plValues &line,
            const plVariablesConjunction &search,
            const plVariablesConjunction &known);

    protected:
        plVariablesConjunction allVariables;
        // changed to a reference by fschnitzler 14/12
        plDataDescriptor * /*<rowDataType>**/ pDataDesc;
        pmCSVDataSet *data;
        long unsigned int exempleCount;

        enumEstimationMethod estimationMethod;

        const std::vector<std::vector<plProbValue>> *missing_probability_table;
        bool freeMissingProbTable;
        bool usingSlidingWindow;
        plJointDistribution *pJointDistribution;
        bool freeJointDistribution;
        plCndDistribution jdeCndDistribution;
        plDistribution jdeDistribution;
        plDistribution jdeCompiledDistribution;

        std::vector<plProbValue> currentMissingProbTable;
    };
}; // namespace PILGRIM

    //namespace PILGRIM {
    //
    //template <typename T>
    //class pmVectorFrequencyCounter : public pmFrequencyCounter<std::vector<T> > {
    // public:
    //    /**
    //     Constructs a new frequency object counter giving the data set and
    //     the associated variable list.
    //     */
    //    pmVectorFrequencyCounter(plDataDescriptor*/*<std::vector<T> >**/ pDataDesc,
    //               const plVariablesConjunction& allVariables,
    //               bool ifreeDataDesc=false);
    //    pmVectorFrequencyCounter(pmVectorFrequencyCounter<T> & other);
    //    virtual ~pmVectorFrequencyCounter();
    //
    //    /**
    //     Generates a new frequencyCounter countaining a bagged version of the first
    //     data set.
    //     NbrBaggedSamples is the number of samples in the bagged set
    //     */
    //
    //    pmVectorFrequencyCounter<T> * Bagging();
    //    pmVectorFrequencyCounter<T> * Bagging(int NbrBaggedSamples);
    //
    //    virtual void SetWeight(unsigned int index,plFloat weight);
    //    pmVectorFrequencyCounter<T> * CopyAndSetUniformWeights(
    //                                                        plFloat inWeight = 1);
    //    virtual void DisplayWeights(std::ostream & strea);
    // protected:
    //    bool freeDataDesc;
    //};
    //}; /* PILGRIM*/

    //******************************************************************************
    //namespace PILGRIM {
    //
    //class pmValuesFrequencyCounter : public pmFrequencyCounter<plValues> {
    // public:
    //    /**
    //     Constructs a new frequency object counter giving the data set and
    //     the associated variable list.
    //     */
    //    pmValuesFrequencyCounter(plDataDescriptor*/*<plValues>**/ pDataDesc,
    //               const plVariablesConjunction& allVariables,
    //               bool ifreeDataDesc=false);
    //    pmValuesFrequencyCounter(pmValuesFrequencyCounter & other);
    //
    //    virtual ~pmValuesFrequencyCounter();
    //
    //    /**
    //     Generate a new frequencyCounter countaining a bagged version of the first
    //     data set.
    //     NbrBaggedSamples is the number of samples in the bagged set
    //     */
    //    virtual pmValuesFrequencyCounter * Bagging();
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //     **/
    //    virtual pmValuesFrequencyCounter * Bagging(int NbrBaggedSamples);
    //
    //    // This function modifies the sample weights in order to provide a bagged
    //    // sample.
    //    void Baggweights(unsigned int NbrBaggedSamples);
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //     **/
    //    void Baggweights(unsigned int NbrBaggedSamples,
    //                     std::vector<plFloat>& weightvector);
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //     **/
    //    virtual long unsigned int learnDistributions(
    //                                      plLearnDistribVector & learnDistribVect,
    //                                      const std::vector< bool >& searchedVars);
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //     **/
    //    virtual long unsigned int learnDistributions(
    //                  plLearnDistribVector& learnDistribVect,
    //                  const std::vector< unsigned int >& SearchedVariablesIndices);
    //    // added by F.schnitzler, 2nd December 2010
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //    **/
    //    virtual plJointDistribution* learnAndBuildDistribution(
    //                                     plLearnDistribVector & learnDistribVect,
    //                                     const std::vector< bool >& searchedVars);
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //    **/
    //    virtual void SetWeight(unsigned int index, plFloat weight);
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //    **/
    //    virtual pmValuesFrequencyCounter* CopyAndSetUniformWeights(
    //                                                        plFloat inWeight = 1);
    //
    //    /**
    //     * \brief  Run the MI test
    //     * \details
    //     * \param variables :
    //    **/
    //    virtual void DisplayWeights(std::ostream & strea);
    // protected:
    //    bool freeDataDesc;
    //};
    //};  // namespace PILGRIM

#endif
