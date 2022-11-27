#include "pilgrim/general/plBayesianNetworkFromXDSLfile.h"

using namespace PILGRIM;
using namespace std;
plBayesianNetworkFromXDSLfile::plBayesianNetworkFromXDSLfile() {}

plBayesianNetworkFromXDSLfile::~plBayesianNetworkFromXDSLfile() {}

bool plBayesianNetworkFromXDSLfile::computeAdjMatrix(string fileName) {
  string fileExtension;
  ifstream fileStream;
  bool isFileOK = this->readFile(fileName, fileExtension, fileStream);

  if (!isFileOK) {
    return false;
  } else {
    if (fileExtension == "xdsl")
      this->readDotXDSLFile(fileStream);
    else
      this->readDotNetFile(fileStream);

    this->generateMatrix();
    this->writeNodesList();
    this->writeCPCList();
    this->writeAdjMatrix();
  }  // end if

  return true;
}

bool plBayesianNetworkFromXDSLfile::getNodeIndex(string nodeName, int &ind) {
  nodeItr1 = nodes.find(nodeName);
  if (nodeItr1 != nodes.end()) {
    ind = nodeItr1->second;
    return true;
  } else {
    return false;
  }
}

bool plBayesianNetworkFromXDSLfile::getNodeParents(string nodeName, std::vector<string> &parentList) {
  matrixTempItr1 = parentsMap.find(nodeName);
  if (matrixTempItr1 != parentsMap.end()) {
    parentList = matrixTempItr1->second;
    return true;
  } else {
    return false;
  }
}

std::string plBayesianNetworkFromXDSLfile::readExtension(string fileName) {
  // separate file extension and file path and name without extension.
  string ext = "";
  string strEx1 = fileName.substr(fileName.size()-3);
  string strEx2 = fileName.substr(fileName.size()-4);

  if (strEx1 == "net") {
    ext = strEx1;
    outFileName = fileName.substr(0, fileName.size()-4);
  } else if (strEx2 == "xdsl") {
    ext = strEx2;
    outFileName = fileName.substr(0, fileName.size()-5);
  } else if (strEx1 == "txt") {
    ext = strEx1;
    outFileName = fileName.substr(0, fileName.size()-4);
  }

  return ext;
}

void plBayesianNetworkFromXDSLfile::writeAdjMatrix() {
  string fileType = "_AdjMatrix.txt";
  string fName = outFileName + fileType;
  ofstream adjMatrixFile(fName.c_str());
  // if the element in the ith row and jth column of adjacency matrix
  // is equal to 1, this means that variable i is a parent of variable j.
  for (size_t i = 0; i < Adj_matrix.size(); i++) {
    for (size_t j = 0; j < Adj_matrix[i].size(); j++) {
      adjMatrixFile << " " << Adj_matrix[i][j];
    }
    adjMatrixFile << endl;
  }
  adjMatrixFile.close();
}

void plBayesianNetworkFromXDSLfile::writeCPCList() {
  string fileType = "_cpcList.txt";
  string fName = outFileName + fileType;
  ofstream edgeListFile(fName.c_str());
  // print cpc list of each node
  for (size_t i = 0; i < cpc_matrix.size(); i++) {
    edgeListFile << i;
    for (size_t j = 0; j < cpc_matrix[i].size(); j++) {
      if (cpc_matrix[i][j] == 1)
        edgeListFile << " " << j;
    }
    edgeListFile << endl;
  }
  edgeListFile.close();
}

void plBayesianNetworkFromXDSLfile::writeNodesList() {
  string fileType = "_NodesList.txt";
  string fName = outFileName + fileType;
  ofstream nodeListFile(fName.c_str());

  nodeListFile << "Node Name:  ----  Node Label:  ----  Node Index: \n";
  for (nodeItr = nodes.begin(); nodeItr != nodes.end(); ++nodeItr) {
    int column;
    column = nodeItr->second;
    std::cout << " \n  " << column;
    nodeListFile << nodesName[nodeItr->second] << " ---- " << nodeItr->first
                 << " ---- " << nodeItr->second << "\n";
  }  // end for

  nodeListFile.close();
}

bool plBayesianNetworkFromXDSLfile::readFile(std::string fileName,
                                             std::string &fileExt,
                                             std::ifstream &file) {
  file.open(fileName.c_str());
  if (!file) {
    cout << "\n Error: unable to read " << fileName;
    return false;
  } else {
    fileExt = this->readExtension(fileName);
    if (fileExt == "net") {
       cout << "\n Input file extension is ." << fileExt << "\n";
    } else if (fileExt == "xdsl") {
       cout << " \n Input file extension is ." << fileExt << "\n";
    } else {
       cout << "\n Unknown file type, it supports only .net/.xdsl file "
               "types.\n";
       return false;
    }
  }  // end if
  return true;
}

bool plBayesianNetworkFromXDSLfile::readFile(std::string fileName, std::ifstream &file) {
  file.open(fileName.c_str());
  if (!file) {
    cout << "\n Error: unable to read " << fileName;
    return false;
  } else {
    return true;
  }
}

void plBayesianNetworkFromXDSLfile::generateMatrix() {
  size_t n = nodes.size();
  std::vector<std::vector<int>> Adj_matrix_temp(n, vector<int>(n, 0));
  std::vector<std::vector<int>> cpc_matrix_temp(n, vector<int>(n, 0));
  Adj_matrix = Adj_matrix_temp;
  cpc_matrix = cpc_matrix_temp;

  for (nodeItr = nodes.begin(); nodeItr != nodes.end(); ++nodeItr) {
    int column;
    int row;
    column = nodeItr->second;
    std::vector<string> parentList;
    if (getNodeParents(nodeItr->first, parentList)) {
      for (size_t i = 0; i < parentList.size(); i++)
        // iterate all parents and put 1 in adj matrix.
        if (getNodeIndex(parentList[i], row)) {
          Adj_matrix[row][column] = 1;
          cpc_matrix[row][column] = 1;
          cpc_matrix[column][row] = 1;
        }
    }  // endif
  }  // end for
}

void plBayesianNetworkFromXDSLfile::readDotNetFile(std::ifstream &file) {
  string line;
  int counter = 0;
  string strNode;

  while (getline(file, line)) {
    // Extract nodes
    size_t pos0, pos1, pos2;
    pos1 = line.find("potential (");
    pos2 = line.find("|");

    if (pos1 != string::npos) {
      strNode = line.substr(pos1+11, pos2-pos1-12);
      cout << " " << strNode << " = ";
      nodes.insert(pair<string, int>(strNode, counter));
      counter++;
    }

    // extract parents of a node
    pos0 = line.find("| ");
    pos2 = line.find(")");
    string parents;
    if (pos0 != string::npos) {
      std::vector<std::string> strs;
      parents = line.substr(pos0+2, pos2-pos0-2);
      boost::split(strs, parents, boost::is_any_of(" "));
      cout << " " << parents << ", " << strs.size() << "\n";
      parentsMap.insert(pair<string, vector<string>>(strNode, strs));
      continue;
    }

    // extract nodes names
    size_t posN1 = line.find("label = ");
    string name;
    if (posN1!=string::npos) {
      name = line.substr(posN1+9);  // , posN2-posN1-4);
      nodesName.push_back(name);
    }
  }  // end while
}

void plBayesianNetworkFromXDSLfile::readDotXDSLFile(std::ifstream &file) {
  string line;
  int counter = 0;
  string strNode;

  while (getline(file, line)) {
    // Extract nodes
    size_t pos0, pos1, pos2, pos3, pos4;
    pos1 = line.find("<cpt id=");
    pos2 = line.find(">");

    if (pos1 != string::npos) {
      strNode = line.substr(pos1+9, pos2-pos1-10);
      std::pair<string, int> prNode(strNode, counter);
      nodes.insert(prNode);
      counter++;
      continue;
    }

    // Extract nodes <deterministic id=
    pos1 = line.find("<deterministic id=");
    pos2 = line.find(">");

    if (pos1 != string::npos) {
      strNode = line.substr(pos1+19, pos2-pos1-20);
      cout << " \n " << counter << "\t" << strNode << " = ";
      nodes.insert(pair<string, int>(strNode, counter));
      counter++;
      continue;
    }

    // extract parents of a node
    pos0 = line.find("<parents>");
    pos2 = line.find("</parents>");
    string parents;
    if (pos0 != string::npos) {
      std::vector<std::string> strs;
      parents = line.substr(pos0+9, pos2-pos0-9);
      // 9 is size of string "<parents>"
      boost::split(strs, parents, boost::is_any_of(" "));
      parentsMap.insert(pair<string, vector<string>>(strNode, strs));
      continue;
    }

    // extract prob table of a node
    // Known Error for "deterministic" node as it does not contain probability
    // table
    pos3 = line.find("<probabilities>");
    pos4 = line.find("</probabilities>");
    string probabilites;
    if (pos3 != string::npos) {
      std::vector<std::string> strs;
      probabilites = line.substr(pos3+15, pos4-pos3-15);
      boost::split(strs, probabilites, boost::is_any_of(" "));
      // type casting from string to plProbValues
      typedef std:: vector<std::string>::iterator itr;
      itr it = strs.begin();
      itr end = strs.end();
      std::vector<plProbValue> probTable;
      for (; it < end; ++it) {
        probTable.push_back(boost::lexical_cast<plProbValue>(*it));
      }
      mapProbTableOfEachNode.insert(pair<string,
                                    std::vector<plProbValue>>(strNode,
                                                              probTable));
      continue;
    }

    // extract nodes names
    size_t posN1, posN2;
    posN1 = line.find("<name>");
    posN2 = line.find("</name>");
    string name;
    if (posN1 != string::npos) {
      name = line.substr(posN1+6, posN2-posN1-6);
      nodesName.push_back(name);
    }

  }  // end while
}

void plBayesianNetworkFromXDSLfile::readAdjMatrixFile(std::ifstream &file) {
  string line;
  std::vector<std::vector<std::string>> Adj_StrMatrix;

  while (getline(file, line)) {
    std::vector<std::string> strs;
    boost::split(strs, line, boost::is_any_of(" "));
    Adj_StrMatrix.push_back(strs);
  }  // end while

  // to generate cpc list
  std::vector<std::vector<int>> cpc_matrix_temp(Adj_StrMatrix.size(),
                                             vector<int>(Adj_StrMatrix.size(),
                                                         0));

  for (size_t i = 0; i < Adj_StrMatrix.size(); i++)
    for (size_t j = 0; j < Adj_StrMatrix.size(); j++) {
      if (Adj_StrMatrix[i][j] == "1") {
        cpc_matrix_temp[i][j] = 1;
        cpc_matrix_temp[j][i] = 1;
      }
    }
  cpc_matrix = cpc_matrix_temp;
}

bool plBayesianNetworkFromXDSLfile::computeCPCfromAdjMatrix(string fileName) {
  ifstream fileStream;
  bool isFileOK = this->readFile(fileName, fileStream);
  string ext = this->readExtension(fileName);

  if (!isFileOK) {
    return false;
  } else {
    this->readAdjMatrixFile(fileStream);
    this->writeCPCList();
  }

  return true;
}

plBayesianNetwork * plBayesianNetworkFromXDSLfile::
getPlBayesianNetworkObject(pmCSVDataDescriptor * DataSet, string fileName) {
  plBayesianNetwork *plBN = new plBayesianNetwork("Theoretical Graph");
  plVariablesConjunction variables = DataSet->observed_variables();

  // Read file
  string fileExtension;
  ifstream fileStream;
  bool isFileOK = this->readFile(fileName, fileExtension, fileStream);

  if (!isFileOK || fileExtension != "xdsl") {
    cout << "\n Unable to extract plBayesianNetowrk object from files of type "
         << fileExtension;
    return plBN;
  } else {
    this->readDotXDSLFile(fileStream);

    std::vector<plVariable> ListOfVariables;
    std::vector<plVariable>::iterator variableItr;
    variables.get_all_variables(ListOfVariables);

    // Adding nodes in a BN
    for (variableItr = ListOfVariables.begin ();
         variableItr != ListOfVariables.end ();
         ++variableItr) {
      plBN->add_node (*variableItr);
    }

    // Adding Edges in a BN
    int parentNodeIndex;
    for (nodeItr = nodes.begin(); nodeItr != nodes.end(); ++nodeItr) {
      // node list in XDSL file
      std::vector<string> parentList;
      if (getNodeParents(nodeItr->first, parentList)) {  // if node has parents
        plVariablesConjunction parents;
        for (size_t i = 0; i < parentList.size(); i++) {
          // iterate all parents and put 1 in adj matrix.
          if (getNodeIndex(parentList[i], parentNodeIndex)) {
            plVariable x = ListOfVariables[parentNodeIndex];
            plVariable y = ListOfVariables[nodeItr->second];
            plBN->add_edge(x, y);  // source --> destination
            parents ^= x;
          }
          plError::ignore_this_message(62, true);
          plCndDistribution probTable(ListOfVariables[nodeItr->second],
                                      parents,
                                      mapProbTableOfEachNode[nodeItr->first]);
          // Assigning distribution to each node P(node|parents)
          plBN->set_distribution(probTable);
          parents.clear();
        }
      } else {  // if node has no parents
        plError::ignore_this_message(62, true);
        plProbTable probTable(ListOfVariables[nodeItr->second],
                              mapProbTableOfEachNode[nodeItr->first]);
        // Assigning distribution to each node P(node|parents)
        plBN->set_distribution(probTable);
      }  // endif
    }  // end for
  }  // end if

  return plBN;
}
