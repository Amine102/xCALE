/*=============================================================================
 * Product        :
 * File           : Utility
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef pm_UTILITY_H
    #define pm_UTILITY_H

    #include <pilgrim/general/pmBayesianNetwork.h>
    #include <pilgrim/general/random/GraphUtility.h>
    #include <pilgrim/general/random/PMMixed.h>
    #include <pl.h>

    #include <boost/filesystem.hpp>
    #include <boost/filesystem/path.hpp>
    #include <boost/graph/adjacency_list.hpp>
    #include <boost/graph/graph_utility.hpp>
    #include <boost/graph/properties.hpp>
    #include <boost/graph/topological_sort.hpp>
    #include <string>
    #include <vector>

    #include "pilgrim/general/pmCommonTypeDefinitions.h"
//=============================================================================
/*!
    *  \brief pmUtility class
    *
    *  \details This class contains the utility functions. These can be accessed with creating object of the class.
    *  \author  BRISCAN Cristian
    *  \author  Amanullah YASIN
    *  \version   PILGRIM 0.1
    *  \date      March 11, 2013
    *  \pre       If there are initialization conditions or configuration parameters then write here.
    *  \bug       If there is known bug, please put here
    *  \warning   There is no warning.
    *  \todo      This class can be updated using smart pointers.
    */
namespace PILGRIM {
    class pmBayesianNetwork;
    class pmUtility {
    public:
        /**
     * \brief  Gets a random value between 0 and superiorLimit - 1.
     * \details
     * \param variables :
     */
        static unsigned int getRandom(unsigned int superiorLimit);

        /**
     * \brief  Gets a random value between 0 and 1.
     * \details
     * \param variables :
     */
        static plProbValue getRandomProbability();

        /**
     * \brief  Selects a subset of variables from a variable conjunction.
     * \details
     * \param variables :
     */
        static void selectVariables(const plVariablesConjunction &from,
                                    const std::vector<bool> &     selectedVars,
                                    plVariablesConjunction &      to);

        /**
     * \brief
     * \details
     * \param variables
     * \author F. Schnitzler
     * \date  2nd December 2010
     */
        static void selectVariables(const plVariablesConjunction &   from,
                                    const std::vector<unsigned int> &selectVariablesIndices,
                                    plVariablesConjunction &         to);

        /**
     * \brief
     * \details
     * \param fileName of a dynamic data csv format
     * \out generate two files named <fileName>_t0.csv and <fileName>_t.csv
     * \date  2019/06/12
     */
        static void split(std::string fileName);

        void     getFileNames(std::vector<std::string> &listOfFiles);
        Matrix_t readFile(std::string fileName);
        void     getBayesianNetwork(Matrix_t &adjMatrix, pmBayesianNetwork *pBN);

        void generatePMMixedDAG(pmGraph *    g,
                                unsigned int nbNodes,
                                unsigned int maxInducedWidth,
                                unsigned int nbIterations,
                                unsigned int maxNodeDegree,
                                unsigned int maxNbEdges);

        void output_mixture(const plJointDistribution &mixture);

        /**
     * \Discretize the continuous variables in the pmCSVDataset in its creation 
     * date  2020/07/01
     */

        std::vector<double> discretisation(int range, std::string &file, std::string file_name, plJointDistribution &model, plVariablesConjunction var_continu, bool modeVerbose /*=false */, std::pair<bool, int> num_gaussians = std::make_pair(false, 2));
        // float estimate_n_gaussians(int num_gaussians, plVariablesConjunction &var_continu);

        // kmeans

        /**
         * @brief Transform a vector to a string like : "Vector<0, 1, 2>".
         * 
         * @param vector The vector to output
         */
        template <typename T>
        static void output_vector(const T &vector, std::ostream& os = std::cout) {
            std::copy(vector.cbegin(), vector.cend(), std::ostream_iterator<typename T::value_type>(os, ", "));
        }
    };
}     // namespace PILGRIM
#endif // pm_UTILITY_H

/**
 * Creation: ????
 * Last update: 2020/07/22
 * Adding discretization
 * Modif : Houda Abouchamala
 */