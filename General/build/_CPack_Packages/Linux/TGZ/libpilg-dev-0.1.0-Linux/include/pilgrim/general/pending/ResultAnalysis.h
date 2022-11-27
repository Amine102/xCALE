/*=============================================================================
 * Product        : 
 * File           : ResultAnalysis.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
 #ifndef _ResultAnalysis_H
#define _ResultAnalysis_H


#include <fstream>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <pilgrim/general/algorithms/Algorithm.h>

#include "pl.h"

#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/graph/properties.hpp>


#include "pilgrim/general/pmBayesianNetwork.h"

using namespace std;
using namespace boost;
using namespace boost::filesystem;


typedef vector<vector<unsigned int>> Matrix_t;
typedef map<int, vector<int>> MapCPC;
typedef vector <MapCPC> CPCWindow;


namespace PILGRIM{

class ResultAnalysis
{
public:

	ResultAnalysis();
	~ResultAnalysis();    
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	Matrix_t readFile(string);
    /**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void splitFile(string, unsigned int);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	pmGraph convertDAGintoCPDAG(pmGraph graph);
	/**
	 * \brief  check in the parent list of a node and return edge if exist 
	 * \details 
	 * \param variables : 
	*/

	bool isParent( pmNode& w, pmNode& y, pmGraph& graph, pmEdge* wyEdge);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	bool isExistZYedgeAndZnotParentOfX( pmNode& x, pmNode& y, pmEdge* xyEdge, pmGraph& graph);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void labelEdges( pmEdge* xyEdge, pmNode& y, pmGraph& graph, string label, bool onlyUnknownEdges, bool incidentEdgesToo);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void orderEdges(pmGraph& graph, std::vector<pmNode>& ndList);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void getVstructuresAndInferedEdges(pmGraph& graph, std::vector<pmEdge>& vStructureAndInferedEdges);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/

    void getBayesianNetwork(Matrix_t&, pmBayesianNetwork*);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void compareBNs(pmGraph& origGraph, pmGraph& computedGraph, unsigned int& TP, unsigned int& FP, unsigned int& TN, unsigned int& FN, unsigned int& reversedEdges, unsigned int& strHammingDistance);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	CPCWindow ExtractCPCMap(Matrix_t);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void CompareCPC(CPCWindow& origNet, CPCWindow& batchWin, CPCWindow& incrWin, vector<Matrix_t> allData);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void CompareCPC(CPCWindow& batchWin, CPCWindow& incrWin, vector<Matrix_t> allData);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void CalculateResults(vector<int>& cpcOrig, vector<int>& cpcBatch, vector<int>& cpcIncr);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void CalculateResults(vector<int>& cpcBatch, vector<int>& cpcIncr);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void print(CPCWindow);
	/**
	 * \brief  
	 * \details 
	 * \param variables : 
	*/
	void print_CPDAG(pmGraph graph);

protected:
private:
	boost::property_map<pmGraph, boost::vertex_index_t>::type vertex_id;
	std::vector<pmNode> nodeList;
	
};
	};
#endif 