/*=============================================================================
 * Product        : 
 * File           : PDAG.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef PM_GRAPHPDAG_H
#define PM_GRAPHPDAG_H

#include <pl.h>
#include <vector>
#include "pilgrim/general/pmBayesianNetwork.h"
#include <boost/graph/depth_first_search.hpp>

//#include "misc/DrawingBNWithGraphViz.h"

using namespace std;
using namespace boost;

namespace PILGRIM{
//!  To store the list of the nodes.
typedef std::vector < int > nodeList;

//!  To store the list of the edges.
typedef std::vector< pmEdge > edgeList;

//! To store triple nodes of the form a-b-c.
struct tripleNodes_t {
	unsigned int a;
	unsigned int b;
	unsigned int c;
};

//typedef std::vector < sepSet_t > sepSet;
typedef std::vector < tripleNodes_t > tripleNodes;


//! PDAG class contains PDAGraph related general methods.
class pmPDAG
{
public:
	
	pmPDAG(pmBayesianNetwork*);
	pmPDAG(void);
	~pmPDAG(void);

	/** 
	*	Input: number of variables n and to calculate cardinality of each variable range of min vals and max vals
	*	Return: it return plvariableconjunction type object
	*/
	plVariablesConjunction createVars(int n, vector <long double> min_vals, vector <long double> max_vals );

	/**
	*	Input: number of variables n and vector containing cardinality of each variable
	*	Return: it return plvariableconjunction type object
	*/
	plVariablesConjunction createVars(int n, vector <int> cards);
	
	/**
	*	\ parameterInput: Bayesian Network of given variables
	*	Return: Bayesian Network object containing complete connected undirected graph.
	*/
	pmBayesianNetwork* completeUndirectedGraph(pmBayesianNetwork*);

	/**
	*	\ parameter Integer type vertex index range 0 to n.
	*	\ Return nodelist type containing the list of neighbors of x.
	*/
	nodeList computeNeighbors(int );

	/**
	*	\ parameter Integer vertex index (0 to n) and slBayesianNetwork object
	*	\ Return total Degree of vertex x.
	*/
	int computeDegree(int, pmBayesianNetwork*);

	/**
	*	\ parameter Bayesian Network object.
	*	\ Return List of all edges. In the case of bidirectional, it will return only one edge. (e.g. A-B, B-A == A-B)
	*/
	edgeList getEdgeList(pmBayesianNetwork*);
	edgeList getEdgeList1(pmGraph);
	/**
	*	\ Return List of all edges. In the case of bidirectional, it will return only one edge. (e.g. A-B, B-A == A-B)
	*/
	edgeList getEdgeList();

	/**
	*	\ parameter Edge descriptor slEdge.
	*	\ Return Second parameter return integer type index of the "node" and third for "parent".
	*/
	void getEdgeNodesIndex (pmEdge, unsigned int&, unsigned int&);
	void getEdgeNodesIndex(pmBayesianNetwork*, pmEdge, unsigned int&, unsigned int&);
	/**
	*	\ parameter Edge list of type "typedef std::vector< slEdge > edgeList", vertex index A and vertex index B
	*	\ Return If there is an edge between A and B return true otherwise false 
	*/
	bool isInEdgeList( edgeList, unsigned int, unsigned int );

	/**
	*	\ parameter vertex index A and vertex index B
	*	\ Return If there is an edge between A and B return true otherwise false 
	*/	
	bool isInEdgeList( unsigned int, unsigned int );

	/**
	*	\ parameter Input Node index, Parent index.
	*	\ Return It will return the edge between two vertexes Node and Parent
	*            and a bool variable hasEdge true if there exist an edge otherwise false.
	*/
	pmEdge getEdge(unsigned int, unsigned int, bool&);

	/**
	*	\ parameter slBayesianNetwrok object, Node index, Parent index.
	*	\ Return It will return the edge between two vertexes Node and Parent
	*            and a bool variable hasEdge true if there exist an edge otherwise false.
	*/
	pmEdge getEdge(pmBayesianNetwork*, unsigned int, unsigned int, bool&);

	/**
	*	\ parameter slBayesianNetwrok object, Vertex index "A" and Vertex index "B".
	*	\ Return Integer type weight 
	*		weight = 0                           means A B (no edge, no weight).
	*		weight = 1 if  AB  == 1 and BA == 0, means A---->B (+ve direction).
	*		weight = -1 if AB  == 0 and BA == 1, means A<----B (-ve direction)
	*		weight = 2  if AB  == 0 and BA == 0, means A-----B (no direction).
	*		weight = 3  if AB  == 1 and BA == 1, means A<--->B (no direction).
	*		weight = 10  if AB  == ? and BA == ?, means A and b are linked by ambiguous.
	*/
	int getWeightPDAG(pmBayesianNetwork*, unsigned int, unsigned int );

	/**
	*	\ parameter Vertex index "A" and Vertex index "B".
	*	\ Return Integer type weight 
	*		weight = 0                           means A B (no edge, no weight).
	*		weight = 1 if  AB  == 1 and BA == 0, means A---->B (+ve direction).
	*		weight = -1 if AB  == 0 and BA == 1, means A<----B (-ve direction)
	*		weight = 2  if AB  == 0 and BA == 0, means A-----B (no direction).
	*		weight = 3  if AB  == 1 and BA == 1, means A<--->B (no direction).
	*		weight = 10  if AB  == ? and BA == ?, means A and b are linked by ambiguous.
	*/
	int getWeightPDAG(unsigned int, unsigned int );

	/**
	*	\ parameter slBayesianNetwrok object, slEdge Edge descriptor.
	*	\ Return Integer type weight 
	*		weight = 0                           means A B (no edge, no weight).
	*		weight = 1 if  AB  == 1 and BA == 0, means A---->B (+ve direction).
	*		weight = -1 if AB  == 0 and BA == 1, means A<----B (-ve direction)
	*		weight = 2  if AB  == 0 and BA == 0, means A-----B (no direction).
	*		weight = 3  if AB  == 1 and BA == 1, means A<--->B (no direction).
	*		weight = 10  if AB  == ? and BA == ?, means A and b are linked by ambiguous.
	*/
	int getWeightPDAG(pmBayesianNetwork*, pmEdge);

	/** 
	*	Input: Vertex index "A", Vertex index "B" and weight
	*		weight = 0                           means A B (no edge, no weight).
	*		weight = 1 if  AB  == 1 and BA == 0, means A---->B (+ve direction).
	*		weight = -1 if AB  == 0 and BA == 1, means A<----B (-ve direction)
	*		weight = 2  if AB  == 0 and BA == 0, means A-----B (no direction).
	*		weight = 3  if AB  == 1 and BA == 1, means A<--->B (no direction).
	*		weight = 10  if AB  == ? and BA == ?, means A and b are linked by ambiguous.
	*/
	void putWeightPDAG(unsigned int, unsigned int, int);

	//! Input edge BayesianNetwork object Edge discriptor and weight.
	void putWeightPDAG(pmBayesianNetwork*, pmEdge, int);

	/**
	*  To find directed path between two nodes
	*/
	bool hasDirectedCycle( pmBayesianNetwork*, int parent, int child);

	//////////////////////////////////////////////////////////////////////////

	//!  Compute all possible triplets of the form A-B-C
	/*!
	\return  List of all possible triplets.
	*/
	void computeTripleNodes(pmGraph& ,  /*!< Original graph. */
		edgeList&,	/*!< List of all edges in the graph. */
		tripleNodes& ); /*!< Returned all possible triple nodes. */


	//! PC rule A. If A -> B - C, and A, C are not adjacent, orient as B -> C.
	/*!
	\return  True if it applies the rule A else False .
	*/
	bool MeekRuleA();
	pmGraph MeekRuleA(pmGraph);
	//! PC rule B. If A -> B -> C and A - C orient as A -> C.
	/*!
	\return  True if it applies rule B else False .
	*/
	bool MeekRuleB();
	pmGraph MeekRuleB(pmGraph);

	//! PC rule C. If A -> B <- C, A - D - C, B - D, and A, C are not adjacent, orient B - D as D -> B.
	/*!
	\return  True if it applies the rule C else False .
	*/
	bool MeekRuleC();
	pmGraph MeekRuleC(pmGraph);
	//! for (each X ? Y ? Z -W ?X such that
	//! Y and W are linked, and Z and X are not linked)
	//!	orient W ? Z as W ? Z.
	/*!
	\return  True if it applies Step5 of construct DAG else False .
	*/
	bool MeekRuleD();
	pmGraph MeekRuleD(pmGraph);
	//! To find a directed cycle first 
	//! create a test graph by removing unoriented edges
	pmBayesianNetwork* createTestGraph(pmBayesianNetwork* );
	

	//! Given a DAG pattern, construct any DAG in the equivalence class
	//! represented by that pattern.
	void constructDAG( );
	void constructDAG( pmBayesianNetwork* );

	//////////////////////////////////////////////////////////////////////////

	void print_PDAG();
	void print_PDAG(edgeList);	
	void print_graph();
	void print_graph(pmBayesianNetwork *pBN);
	void print_graph(pmGraph& graph);


protected:
	pmBayesianNetwork *pBN;

private:
};
};

#endif 
