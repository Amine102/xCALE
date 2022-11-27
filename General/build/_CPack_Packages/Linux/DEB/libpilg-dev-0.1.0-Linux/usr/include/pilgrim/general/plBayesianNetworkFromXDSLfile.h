/*=============================================================================
 * Product        :
 * File           : plBayesianNetworkFromXDSLfile.h
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

#include "pl.h"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <vector>
#include <map>

#include "pilgrim/general/pmCommonTypeDefinitions.h"




namespace PILGRIM {
class plBayesianNetworkFromXDSLfile {
 public:
     plBayesianNetworkFromXDSLfile();
     ~plBayesianNetworkFromXDSLfile();
     /**
     * \brief
     * \details
     * \param variables :
     */
     bool computeAdjMatrix(std::string fileName);
     /**
     * \brief
     * \details
     * \param variables :
     */
     bool computeCPCfromAdjMatrix(std::string fileName);
     /**
     * \brief
     * \details
     * \param variables :
     */
     bool getNodeIndex(std::string nodeName, int &ind);
     /**
     * \brief
     * \details
     * \param variables :
     */
     bool getNodeParents(std::string nodeName, std::vector<std::string> &parentList);
     /**
      * \brief
      * \details
      * \param variables :
      */
     std::string readExtension(std::string fileName);
     /**
      * \brief
      * \details
      * \param variables :
      */
     void generateMatrix();
     /**
      * \brief
      * \details
      * \param variables :
      */
     bool readFile(std::string fileName, std::string &fileExt, std::ifstream &file);
     /**
      * \brief
      * \details
      * \param variables :
      */
     bool readFile(std::string fileName, std::ifstream &file);
     /**
      * \brief
      * \details
      * \param variables :
      */
     void readDotNetFile(std::ifstream &file);
     /**
      * \brief
      * \details
      * \param variables :
      */
     void readAdjMatrixFile(std::ifstream &file);
     /**
      * \brief
      * \details
      * \param variables :
      */
     void readDotXDSLFile(std::ifstream &file);
     /**
      * \brief
      * \details
      * \param variables :
      */
     void writeAdjMatrix();
     /**
      * \brief
      * \details
      * \param variables :
      */
     void writeCPCList();
     /**
      * \brief
      * \details
      * \param variables :
      */
     void writeNodesList();
     /**
      * \brief
      * \details
      * \param variables :
      */
     plBayesianNetwork * getPlBayesianNetworkObject(
                                                pmCSVDataDescriptor * DataSet,
                                                std::string fileName);

 protected:
    std::map<std::string, int> nodes;
    std::map<std::string, std::vector<std::string>> parentsMap;
    std::map<std::string, std::vector<plProbValue>> mapProbTableOfEachNode;
    std::vector<std::string> nodesName;
    std::map<std::string, int>::iterator nodeItr, nodeItr1;
    std::vector<std::vector<int>> Adj_matrix;
    std::vector<std::vector<int>> cpc_matrix;
    std::map<std::string, std::vector<std::string>>::iterator matrixTempItr,
                                                    matrixTempItr1;
    std::string outFileName;
 private:
};
}; // namespace PILGRIM
