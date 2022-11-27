
#include "pilgrim/general/graph/PDAG.h"

//#include <pl.h>
using namespace std;
using namespace boost;
using namespace PILGRIM;

pmPDAG::pmPDAG(pmBayesianNetwork* bn) { this->pBN = bn; }
pmPDAG::pmPDAG() { this->pBN = NULL; }
pmPDAG::~pmPDAG() {  }
//===========================================================================
plVariablesConjunction pmPDAG::createVars(int n, std::vector<long double> min_vals, vector<long double> max_vals )
{
	plVariablesConjunction variables;
	vector< int > cards;
	// calculate cardinality of each variable and push_back in the vector
	for (int i = 0;i < n; i++)
	{
		int card = (int)(max_vals.at(i) - min_vals.at(i)) + 1;

		cards.push_back(card);		  
	}

	variables = this->createVars(n, cards);

return variables;
}//
//==============================================================================
plVariablesConjunction pmPDAG::createVars(int n, vector<int> cards)
{
	string name[128];

	plVariablesConjunction variables;

	for (int i = 0;i < n; i++)
	{
		plVariable *plS = NULL;
		plIntegerType* pType = NULL;
		pType = new plIntegerType(1, cards[i]);
		plS = new plVariable(name[i], *pType);
		variables ^= (*plS);
		delete plS;
		delete pType;
	}

return variables;
}//

//==============================================================================
pmBayesianNetwork* pmPDAG::completeUndirectedGraph(pmBayesianNetwork* bn)
{
	int weight = 0;
	int n;
	pmEdge e;
	bool edgeAdded;
	this->pBN = bn;
	pmGraph& graph = this->pBN->get_graph();
	property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);
	plVariablesConjunction variables;
	variables = this->pBN->getVariables();
	n = variables.dim();

	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++) {

			boost::tie(e, edgeAdded) = add_edge(vertex(i, graph), vertex(j, graph), graph);
			put(weightMap, e, weight);

		}
	}

return pBN;
}
//==============================================================================
nodeList pmPDAG::computeNeighbors(int x)
{
	pmNode targ;
	nodeList neighborX;
	pmGraph& graph = pBN->get_graph();
	pmOutNodeIterator eo, eo_end;

	boost::tie(eo, eo_end) = adjacent_vertices( vertex( x, graph), graph);

	for(pmOutNodeIterator iter = eo; iter != eo_end; ++iter) {
		int neighbor = pBN->getVariableIndex(*iter);
		if (x != neighbor){
			neighborX.push_back(neighbor);
		}
	}

	return neighborX;
}// end of computeNeibors
//==============================================================================
int pmPDAG::computeDegree(int i, pmBayesianNetwork *bn)
{
	int deg = 0;
	pmGraph& graph = bn->get_graph();

	 deg = (degree(vertex(i,graph), graph)/2)-1;
 
return deg;
}
//==============================================================================
pmEdge pmPDAG::getEdge(unsigned int n, unsigned int p, bool& hasEdge)
{
	return this->getEdge(this->pBN, n, p, hasEdge);
}
//==============================================================================
pmEdge pmPDAG::getEdge(pmBayesianNetwork* bn, unsigned int n, unsigned int p, bool& hasEdge)
{
	pmGraph& graph = bn->get_graph();
	unsigned int i, j;
	pmEdgeIterator edge_i,edge_end;
	pmEdge e;
	hasEdge = false;

	boost::tie(edge_i, edge_end) = edges( graph);
	for (pmEdgeIterator ei = edge_i; ei != edge_end; ei++)
	{
		this->getEdgeNodesIndex(*ei, i, j);

		if (n != p && (i == n && j == p))
		{
			e = (*ei);
			hasEdge = true;
			break;
		}
	}
return e;
}

//==============================================================================
edgeList pmPDAG::getEdgeList()
{
	return this->getEdgeList(this->pBN);
}


//==============================================================================
edgeList pmPDAG::getEdgeList(pmBayesianNetwork* bn)
{
	pmGraph& graph = bn->get_graph();
	unsigned int nd, pr;
	pmEdgeIterator e, e_end;
	edgeList elist;

	boost::tie(e, e_end) = edges(graph); 
	for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
		this->getEdgeNodesIndex(*ei, nd, pr);
		if (nd != pr && !this->isInEdgeList(elist, pr, nd ))
		{
			elist.push_back(*ei);
		}
	}//endFor

return elist;
}
//add by ghada==============================================================================
edgeList pmPDAG::getEdgeList1(pmGraph graph)
{
	unsigned int nd, pr;
	pmEdgeIterator e, e_end;
	edgeList elist;

	boost::tie(e, e_end) = edges(graph); 
	for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd != pr && !this->isInEdgeList(elist, pr, nd ))
		{
			elist.push_back(*ei);
		}
	}//endFor
	pmEdge e1;
		for(int i = 0; i < elist.size();i++)
		{
			e1=elist.at(i);
			getEdgeNodesIndex(e1, nd, pr);
			cout<<"\n"<< pr<<" -->"<<nd;
		}

return elist;
}

//==============================================================================
void pmPDAG::getEdgeNodesIndex(pmEdge e, unsigned int& nodeIndex, unsigned int& parentIndex)
{
	pmGraph& graph = pBN->get_graph();
	pmNode nodeA , parentA;

	nodeA = target(e, graph);
	parentA = source(e, graph);
	nodeIndex = pBN->getVariableIndex(nodeA);
	parentIndex = pBN->getVariableIndex(parentA);
} 
//===============================================================================
void pmPDAG::getEdgeNodesIndex(pmBayesianNetwork* bn, pmEdge e, unsigned int& nodeIndex, unsigned int& parentIndex)
{
	pmGraph& graph = bn->get_graph();
	pmNode nodeA , parentA;

	nodeA = target(e, graph);
	parentA = source(e, graph);
	nodeIndex = bn->getVariableIndex(nodeA);
	parentIndex = bn->getVariableIndex(parentA);
}
//==============================================================================
bool pmPDAG::isInEdgeList( unsigned int n, unsigned int p)
{
	return	this->isInEdgeList(this->getEdgeList(), n, p);
}
//==============================================================================
bool pmPDAG::isInEdgeList(edgeList e , unsigned int nB, unsigned int pB)
{
	unsigned int nA, pA;
	bool flag = false;

	for(edgeList::iterator ei = e.begin(); ei != e.end(); ++ei) 
	{
		this->getEdgeNodesIndex(*ei, nA, pA);
		if ( (nA == nB && pA == pB) || (nA == pB && pA == nB) )
		{
			flag = true; 
			break;
		}
		else
		{
			flag = false;
		}
	}//end for
	
	return flag;

}

//==============================================================================
int pmPDAG::getWeightPDAG(pmBayesianNetwork* bn, pmEdge e)
{
	unsigned int a, b;
	
	getEdgeNodesIndex(e, a, b);

	return this->getWeightPDAG(bn, a, b);
}
//==============================================================================
int pmPDAG::getWeightPDAG(unsigned int a, unsigned int b)
{
	return this->getWeightPDAG(this->pBN, a, b );
}
//==============================================================================
int pmPDAG::getWeightPDAG(pmBayesianNetwork* bn, unsigned int a, unsigned int b)
{
	pmGraph& graph = bn->get_graph();
	property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);

	///property_map<slGraph, edge_t>::Map edgeTypeMap = get(edge_t, graph);
	int weight = NULL;

	if (a != b)
	{
		bool hasEdge = true;
		pmEdge a_b, b_a;
		a_b = this->getEdge(bn, a, b, hasEdge);
		b_a = this->getEdge(bn, b, a, hasEdge);

		if (hasEdge)
		{
			int weight_a_b, weight_b_a;
			weight_a_b = get(edge_weight, graph, a_b);
			weight_b_a = get(edge_weight, graph, b_a);

			if ( weight_a_b == 0 && weight_b_a== 0)
				weight = 2; // a-b

			else if ( weight_a_b == 0 && weight_b_a == 1)
				weight = -1; // a<-b

			else if ( weight_a_b == 1 && weight_b_a == 0)
				weight = 1; // a->b

			else if ( weight_a_b == 1 && weight_b_a == 1)
				weight = 10; // a and b marked as ambiguous for algorithm CPC

			else if ( weight_a_b == 1 && weight_b_a == 2) // have to change because it should not be same as for ambiguous links.
				weight = 3; // a<->b

		  }

		}
		else
			weight = 0; // a b means there is no edge and no weight

		
return weight;
}//

//==============================================================================
void pmPDAG::putWeightPDAG(pmBayesianNetwork* bn, pmEdge e, int w)
{
	unsigned int a, b;
	this->pBN = bn;
	this->getEdgeNodesIndex(e, a, b);
	this->putWeightPDAG(a, b, w);
}

void pmPDAG::putWeightPDAG(unsigned int a, unsigned int b, int weight)
{
	pmGraph& graph = this->pBN->get_graph();
	property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);

	pmEdge a_b, b_a;
	bool hasEdge = true;
	a_b = this->getEdge(a, b, hasEdge);
	b_a = this->getEdge(b, a, hasEdge);

	switch ( weight )
	{
	case 2: // a-b
		{
			put(weightMap, a_b, 0);
			put(weightMap, b_a, 0);
		}
		break;

	case -1: // a<-b
		{
			put(weightMap, a_b, 0);
			put(weightMap, b_a, 1);
		}
		break;
	
	case 1: // a->b
		{
			put(weightMap, a_b, 1);
			put(weightMap, b_a, 0);
		}
		break;

	case 10: // a and b are ambiguous
		{
			put(weightMap, a_b, 1);
			put(weightMap, b_a, 1);
		}
		break;
	
	case 3: // a<->b
		{
			put(weightMap, a_b, 1);
			put(weightMap, b_a, 1);
		}
	
		break;
	
	//default:
		
	}

}

//=============================Depth First Search===========================
/// Depth First Search 
class DFSVisitor: public default_dfs_visitor
{
protected:
	pmGraph testG;
	int nodeA, nodeC;
	bool* exist;
	int counter;

public:
	DFSVisitor(pmGraph testG, int nodeA, int nodeC, bool& k);
	~DFSVisitor();
	void discover_vertex(pmNode u, const pmGraph &g);
	void resetCounter();
	void examine_edge(pmEdge u, const pmGraph& g);

};
//--------------------------------------------------------------------------
DFSVisitor::DFSVisitor(pmGraph g, int nodeA, int nodeC, bool& k)
{
	this->testG = g;
	this->nodeA = nodeA;
	this->nodeC = nodeC;
	this->exist = &k;
	this->resetCounter();
}
//--------------------------------------------------------------------------
DFSVisitor::~DFSVisitor() { }
//--------------------------------------------------------------------------
void DFSVisitor::resetCounter()
{
	this->counter = 0;

}
//--------------------------------------------------------------------------

void DFSVisitor::discover_vertex(pmNode u, const pmGraph &g)
{
this->counter++; 
boost::property_map<pmGraph, boost::vertex_index_t>::type vertex_id;
unsigned int varIdx = vertex_id[u];

cout<<" Vertex visited "<< varIdx+1;

if ((varIdx == this->nodeC ) && ((this->counter) > 2))
{
*this->exist = true;
}
}
//--------------------------------------------------------------------------
void DFSVisitor::examine_edge(pmEdge u, const pmGraph &g)
{
unsigned int a, b;
pmPDAG p;
p.getEdgeNodesIndex(u,a,b);
cout<<" \n "<< a+1<<" ---- "<<b+1;
}

//=============================================================================

/// Depth First Search method to detect directed path
struct cycle_detector : public dfs_visitor<>
{
	cycle_detector( bool& has_cycle) 
		: _has_cycle(has_cycle) { }

	template <class Edge, class Graph>
	void back_edge(Edge, Graph&) {
		_has_cycle = true;
	}
protected:
	bool& _has_cycle;
};

//=============================================================================
//===========================================================================
bool pmPDAG::hasDirectedCycle(pmBayesianNetwork* testBN, int parent, int child)
{
	/* This function will check whether a directed cycle or path exist between two nodes.
	*  Graph g is a directed graph. 
	*  To check whether all the edges are in the same direction, save current edge weight and
	*  further compare it with the next edge weight. If they are not same then break the process,
	*  means all the edges are not in the same direction.
	*/

	pmGraph g = testBN->get_graph();
	plVariablesConjunction variables;
	variables = testBN->getVariables();
	bool has_cycle = false;
	bool cycle = false;
	
	// To calculate edge list of the graph
	edgeList elist;
	elist = getEdgeList(testBN);

	cycle_detector vis(has_cycle);
	DFSVisitor DFSvis(g, child, parent, cycle );
	boost::depth_first_search(g, visitor(vis));
	
	return false;
}


//===========================================================================
bool pmPDAG::MeekRuleA()
{
	bool flag = false; 
	edgeList eList = this->getEdgeList();
	tripleNodes ABCform; // to store A-B-C forms
	this->computeTripleNodes(this->pBN->get_graph(), eList, ABCform);

	for (int i = 0; i < ABCform.size();i++){
		unsigned int a,b,c;

		a = ABCform[i].a;
		b = ABCform[i].b;
		c = ABCform[i].c;

		if (!this->isInEdgeList(eList, a, c))
		{
			int w1 ,w2;
			for(edgeList::iterator ei = eList.begin(); ei != eList.end(); ++ei) 
			{
				unsigned int node, parent;
				this->getEdgeNodesIndex( *ei, node, parent);
				if ( (node == a && parent == b) || (node == b && parent == a) )
				{
					w1 = this->getWeightPDAG(a, b);

				}else if ( (node == c && parent == b) || (node == b && parent == c) )
				{
					w2 = this->getWeightPDAG(b, c);
				}					
			}//endfor
			if ( w1 == 1 && w2 == 2)
			{
				flag = true;
				this->putWeightPDAG(b, c, 1);// b->c
			}
		}//endif

	}//endfor
	return flag;
}//
// added by ghada
pmGraph pmPDAG::MeekRuleA(pmGraph graph)
{
	bool flag = false; 
	
	unsigned int nd, pr;
	pmEdgeIterator e, e_end;
	edgeList elist;
	property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);
	boost::property_map<pmGraph, edge_name_t>::type  name1 = get(edge_name, graph);
	boost::tie(e, e_end) = edges(graph); 
	for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd != pr && !this->isInEdgeList(elist, pr, nd ))
		{
			elist.push_back(*ei);
		}
	}//endFor
	

	tripleNodes ABCform; // to store A-B-C forms
	this->computeTripleNodes(graph, elist, ABCform);
	
	for (int i = 0; i < ABCform.size();i++){
		unsigned int a,b,c;

		a = ABCform[i].a;
		b = ABCform[i].b;
		c = ABCform[i].c;

		if (!this->isInEdgeList(elist, a, c))
		{
			int w1 ,w2;

			for(edgeList::iterator ei = elist.begin(); ei != elist.end(); ++ei) 
			{
				unsigned int node, parent;
				
				this->getEdgeNodesIndex( *ei, node, parent);
				if ( (node == a && parent == b) || (node == b && parent == a) )
				{
					cout<<"\n meek rules1 \n"<<weightmap[*ei]<<"node "<<node<<"parent "<<parent;
					w1 = weightmap[*ei];
					

				}else if ( (node == c && parent == b) || (node == b && parent == c) )
				{
					cout<<"\n meek rules2 \n"<<weightmap[*ei]<<"node "<<node<<"parent "<<parent;
					w2 = weightmap[*ei];//this->getWeightPDAG(b, c);
					
				}					
			}//endfor
			if ( w1 == 2 && w2 == -1)
			{
				cout<<"\n b-->c \n"<<"c "<<c<<"b "<<b;
				flag = true;
		for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd==c && pr==b)
		{
			weightmap[*ei]=2;
			name1[*ei]=="COMPELLED";
		}
	}
			}
		}//endif
	}//endfor
	cout<<"flag"<<flag;
	return graph;
}//

bool pmPDAG::MeekRuleB()
{
	/*

	*/
	unsigned int a, c;
	bool flag =  false, isPathExist = false;
	edgeList eList = this->getEdgeList();
	tripleNodes triple; // to store A-B-C forms
	this->computeTripleNodes(this->pBN->get_graph(), eList, triple);

	for(edgeList::iterator ei = eList.begin(); ei != eList.end(); ++ei) 
	{
		int w;
		this->getEdgeNodesIndex(*ei, a, c);
		w = this->getWeightPDAG(a, c);
		if (w == 2) // Pick up undirected edge to check for directed cycle
		{
			cout<<"\n-- a == "<< a+1<< " c == " <<c+1 <<" weight = "<<w;
			
			pmBayesianNetwork* testBN = this->createTestGraph(this->pBN);  // create a test graph by removing undirected edges to find directed path
						
			if (a > c)
			{
				isPathExist = this->hasDirectedCycle( testBN, a, c);	
				if (isPathExist)
				{
					this->putWeightPDAG(a, c, -1);
					flag = true;
					//for testing 
					cout<<"\n Rule B1  executed a= "<< a<<" ---- c = "<<c;
				}
			}
			else
			{
				isPathExist = this->hasDirectedCycle( testBN, c, a);
				if (isPathExist)
				{
					this->putWeightPDAG(a, c, 1);
					flag = true;

					//for testing 
					cout<<"\n Rule B2 executed a= "<< a<<" ---- c = "<<c;

				}
			}//end else								
		}//end if
	}//end if

	return flag;
}//
// added by ghada
pmGraph pmPDAG::MeekRuleB(pmGraph graph)
{
	/*
	*/
	int w1 ,w2,w3;
	unsigned int nd, pr;
	pmEdgeIterator e, e_end;
	edgeList eList;
	property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);
	boost::property_map<pmGraph, edge_name_t>::type  name1 = get(edge_name, graph);
	boost::tie(e, e_end) = edges(graph); 
	for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd != pr && !this->isInEdgeList(eList, pr, nd ))
		{
			eList.push_back(*ei);
		}
	}//endFor
	
	tripleNodes ABCform; // to store A-B-C forms
	
	
	this->computeTripleNodes(graph, eList, ABCform);
for (int i = 0; i < ABCform.size();i++){
	unsigned int a,b,c;

		a = ABCform[i].a;
		b = ABCform[i].b;
		c = ABCform[i].c;

		if (this->isInEdgeList(eList, a, c) && this->isInEdgeList(eList, a, b) && this->isInEdgeList(eList, b, c))
		{
			
			for(edgeList::iterator ei = eList.begin(); ei != eList.end(); ++ei) 
			{
				unsigned int node, parent;
				this->getEdgeNodesIndex( *ei, node, parent);
				if ( (node == b && parent == a) )//(node == a && parent == b) || 
				{
					w1 = weightmap[*ei];

				}else if ( (node == c && parent == b)  )//|| (node == b && parent == c)
				{
					w2 = weightmap[*ei];
				}	
				else if ( (node == c && parent == a) || (node == a && parent == c) )
				{
					w3 = weightmap[*ei];
				}	
			}//endfor
			if ( w1 == 2 && w2 == 2 && w3 ==-1)
			{
				cout<<"test "<<a<<b<<c;
		for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd==c && pr==a)
		{
			weightmap[*ei]=2;// a->c
			name1[*ei]=="COMPELLED";

		}
	}			
			}
		}//endif

	}//endfor
	return graph;
}//

pmBayesianNetwork* pmPDAG::createTestGraph(pmBayesianNetwork* BN)
{
	plVariablesConjunction variables;
	variables = this->pBN->getVariables();

	//-------------
	pmBayesianNetwork* computeBN = new pmBayesianNetwork(variables);
	pmNode node, parent;
	//slEdge e;
	unsigned int a, b;

	property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, computeBN->get_graph());

	//-----------
	edgeList elist = getEdgeList(BN); // First get the edge list
	pmBayesianNetwork* testBN = new pmBayesianNetwork(*BN); // copy BN network//
	pmEdge e;

	for (int i = 0; i < elist.size(); i++ ) // Find undirected edges to remove from the test gragph
	{
		e = elist[i];
		int w = getWeightPDAG(BN, e);
		getEdgeNodesIndex(e, a, b);

		if ( w == 2)
		{
			remove_edge(vertex(a, testBN->get_graph()), vertex(b, testBN->get_graph()), testBN->get_graph());
			remove_edge(vertex(b, testBN->get_graph()), vertex(a, testBN->get_graph()), testBN->get_graph());
//			cout << " \n edge removed ... ";
		}

	}
	//--------------
	
	return testBN;
}
//////////////////////////////////////////////////////////////////////////

void pmPDAG::computeTripleNodes(pmGraph& graph, edgeList& elist, tripleNodes& triple)
{
	edgeList tempEdgeList;
	unsigned int nd1, nd2, pr1, pr2;
	bool flag = false; 
	tripleNodes_t abcForm;

	for(edgeList::iterator ei = elist.begin(); ei != elist.end(); ++ei) 
	{
		tempEdgeList.push_back(*ei);
		this->getEdgeNodesIndex(*ei, nd1, pr1);

		for(edgeList::iterator eIt = elist.begin(); eIt != elist.end(); ++eIt) 
		{
			this->getEdgeNodesIndex(*eIt, nd2, pr2);

			if ( (pr1 != pr2 && nd1 == nd2) && this->isInEdgeList(tempEdgeList , nd2, pr2) == false) // check isInEdgeList to avoid repetition
			{
				abcForm.a = pr1;
				abcForm.b = nd1; //= nd2
				abcForm.c = pr2;
				flag = true;

			}else if ((pr1 == pr2 && nd1 != nd2) && this->isInEdgeList(tempEdgeList , nd2, pr2) == false)
			{
				abcForm.a = nd1;
				abcForm.b = pr1; // = pr2
				abcForm.c = nd2;
				flag = true;

			}else if ( nd1 == pr2 )
			{
				abcForm.a = pr1;
				abcForm.b = nd1; // = pr2
				abcForm.c = nd2;
				flag = true;

			}else
				flag = false;

			// if triplet found then push into the vector along with edges	
			if (flag)
			{	
				triple.push_back(abcForm);
			}
		}// endfor

	}// endfor

}//

//////////////////////////////////////////////////////////////////////////
bool pmPDAG::MeekRuleC()
{
	/*!
	* Rule: for (each uncoupled meeting X ? Z ? Y
	*			such that X ? W, Y ? W, and Z ?W)
	*			orient Z ?W as Z ? W;
	*
	* Logic used: Iterate all triplets of form A-B-C and find two (a1-b1-c1 , a2-b2-c2)triplets which have 
	* same a1 = a2 and c1 = c2 but b1 != b2 . Then find if there edge exist between b1 and b2 or not.
	* then check each edge wieght to find its direction.
	*/
	bool flag = false , edgExist1, edgExist2;
	//slGraph& graph = this->pBN->get_graph();
	edgeList eList = this->getEdgeList();
	tripleNodes triple; // to store A-B-C forms
	this->computeTripleNodes(this->pBN->get_graph(), eList, triple);
	
	for ( int i = 0; i < triple.size(); i++ ) // 
	{
		unsigned int a1, b1, c1;
		a1 = triple[i].a;
		b1 = triple[i].b;
		c1 = triple[i].c;

		for ( int j = i + 1; j < triple.size(); j++)
		{
			unsigned int a2, b2, c2;
			a2 = triple[j].a;
			b2 = triple[j].b;
			c2 = triple[j].c;

			if (a1 == a2 && b1 != b2 && c1 == c2)
			{
				edgExist1 = this->isInEdgeList(eList, a1, c1);
				edgExist2 = this->isInEdgeList(eList, b1, b2);

				if ( edgExist1 == false && edgExist2 == true ) // to test uncoupled meeting of a-b-c
				{
					unsigned int wAB1, wAB2, wBC1, wBC2;
					wAB1 = this->getWeightPDAG(a1, b1);
					wAB2 = this->getWeightPDAG(a2, b2);
					wBC1 = this->getWeightPDAG(b1, c1);
					wBC2 = this->getWeightPDAG(b2, c2);

					if ( wAB1 == 2 && wBC1 == 2 &&  // means no direction
						 wAB2 == 1 && wBC2 == -1 ) // means v structure
					{
						this->putWeightPDAG(b1, b2, 1);
						flag = true;
					} 
					else if ( wAB1 == 1 && wBC1 == -1 &&  // means v structure
						      wAB2 == 2 && wBC2 == 2 )    // means no direction
					{
						this->putWeightPDAG(b1, b2, -1);
						flag = true;
					}
				}
				
			}
		}
	}

	return flag;

}
//
pmGraph pmPDAG::MeekRuleC(pmGraph graph)
{
	/*!
	* Rule: for (each uncoupled meeting X ? Z ? Y
	*			such that X ? W, Y ? W, and Z ?W)
	*			orient Z ?W as Z ? W;
	*
	* Logic used: Iterate all triplets of form A-B-C and find two (a1-b1-c1 , a2-b2-c2)triplets which have 
	* same a1 = a2 and c1 = c2 but b1 != b2 . Then find if there edge exist between b1 and b2 or not.
	* then check each edge wieght to find its direction.
	*/
	bool edgExist1, edgExist2;
	
	unsigned int nd, pr;
	pmEdgeIterator e, e_end;
	edgeList eList;
	property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);
	boost::property_map<pmGraph, edge_name_t>::type  name1 = get(edge_name, graph);
	boost::tie(e, e_end) = edges(graph); 
	for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd != pr && !this->isInEdgeList(eList, pr, nd ))
		{
			eList.push_back(*ei);
			//cout<< "\n "<<nd <<"---"<<pr;

		}
	}//endFor
	
	tripleNodes triple; // to store A-B-C forms
	this->computeTripleNodes(graph, eList, triple);
	
	for ( int i = 0; i < triple.size(); i++ ) // 
	{
		unsigned int a1, b1, c1;
		a1 = triple[i].a;
		b1 = triple[i].b;
		c1 = triple[i].c;

		for ( int j = i + 1; j < triple.size(); j++)
		{
			unsigned int a2, b2, c2;
			a2 = triple[j].a;
			b2 = triple[j].b;
			c2 = triple[j].c;

			if (a1 == a2 && b1 != b2 && c1 == c2)
			{
				edgExist1 = this->isInEdgeList(eList, a1, c1);
				edgExist2 = this->isInEdgeList(eList, b1, b2);

				if ( edgExist1 == true && edgExist2 == false ) // to test uncoupled meeting of a-b-c
				{
					unsigned int wAB1, wAB2, wBC1, wBC2, wAC2;
					
					unsigned int node, parent;
					for(edgeList::iterator ei = eList.begin(); ei != eList.end(); ++ei) 
			{
				this->getEdgeNodesIndex( *ei, node, parent);
				if ( (node == b1 && parent == a1) || (node == a1 && parent == b1) ) 
				{
					wAB1 = weightmap[*ei];//-1

				}else if ( (node == b2 && parent == a2)|| (node == a2 && parent == b2)   )
				{
					wAB2 = weightmap[*ei];//-1
				}	
				else if ( (node == c1 && parent == b1) )//|| (node == b1 && parent == c1) )
				{
					wBC1 = weightmap[*ei];//2
				}
				else if ( (node == c2 && parent == b2) )//|| (node == b2 && parent == c2) )
				{
					wBC2 = weightmap[*ei];//2
				}
				else if ( (node == c1 && parent == a1) || (node == c2 && parent == a2) )
				{
					wAC2 = weightmap[*ei];//-1
				}
					
			}//endfor
					
					
					if ( wAB1 !=2 && wBC1 == 2 &&  // means no direction
						 wAB2 !=2 && wBC2 == 2 && wAC2!=2 ) // means v structure
					{
						//this->putWeightPDAG(b1, b2, 1);
						for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd==c1 && pr==a1)
		{
			weightmap[*ei]=2;// a1->c1
			//cout<< "\n "<<nd <<"---"<<pr;
			name1[*ei]=="COMPELLED";

		}
	}
	
					} 
					
				}
				
			}
		}
	}

	return graph;

}

//////////////////////////////////////////////////////////////////////////
bool pmPDAG::MeekRuleD()
{
	/*
	Iterate all triplets of form A-B-C and find two (a1-b1-c1 , a2-b2-c2)triplets which have 
	same a1 = a2 and c1 = c2 but b1 != b2 . Then find if there edge exist between A and C or not.	
	*/
	bool flag = false , edgExist1, edgExist2;
	edgeList eList = this->getEdgeList();
	tripleNodes triple; // to store A-B-C forms
	this->computeTripleNodes(this->pBN->get_graph(), eList, triple);

	for ( int i = 0; i < triple.size(); i++ ) // 
	{
		unsigned int a1, b1, c1;
		a1 = triple[i].a;
		b1 = triple[i].b;
		c1 = triple[i].c;
		
		for ( int j = i + 1; j < triple.size(); j++)
		{
			unsigned int a2, b2, c2;
			a2 = triple[j].a;
			b2 = triple[j].b;
			c2 = triple[j].c;

			if (a1 == a2 && b1 != b2 && c1 == c2)
			{
				unsigned int wAB1, wAB2, wBC1, wBC2;
				edgExist1 = this->isInEdgeList(eList, a1, c1);
				edgExist2 = this->isInEdgeList(eList, b1, b2);
				wAB1 = this->getWeightPDAG(a1, b1);
				wAB2 = this->getWeightPDAG(a2, b2);
				wBC1 = this->getWeightPDAG(b1, c1);
				wBC2 = this->getWeightPDAG(b2, c2);

				if ( edgExist1 == true && edgExist2 == false )  // First possible orientation
				{
					if ( wAB1 == 1 && wAB2 == -1 &&  // means edges in series
						 wBC1 == 2 && wBC2 == 2 ) // means no direction
					{
						this->putWeightPDAG(b1, c1, -1);
						flag = true;
					} 
					else if ( wAB1 == 0 && wAB2 == 0 &&  //means no direction
						      wBC1 == 1 && wBC2 == -1 ) //  means edges in series
					{
						this->putWeightPDAG(a2, c2, 1);
						flag = true;
					}
				}
				else if ( edgExist1 == false && edgExist2 == true ) // Second possible orientation
				{
					if ( wAB1 == 1 && wBC1 == 1 &&  // means edges in series
						 wBC2 == 0 && wAB2 == 0 ) // means no direction
					{
						this->putWeightPDAG(b2, c2, 1);
						flag = true;
					} 
					else if ( wAB1 == 0 && wBC1 == 0 &&  //means no direction
						      wBC2 == 1 && wAB2 == 1 ) //  means edges in series
					{
						this->putWeightPDAG(a1, b1, 1);
						flag = true;
					}

				}

			}
		}
	}

	return flag;

}
//add by ghada
pmGraph pmPDAG::MeekRuleD(pmGraph graph)
{
	/*
	Iterate all triplets of form A-B-C and find two (a1-b1-c1 , a2-b2-c2)triplets which have 
	same a1 = a2 and c1 = c2 but b1 != b2 . Then find if there edge exist between A and C or not.	
	*/
	bool edgExist1, edgExist2;
	unsigned int nd, pr;
	pmEdgeIterator e, e_end;
	edgeList eList;
	property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph);
	boost::property_map<pmGraph, edge_name_t>::type  name1 = get(edge_name, graph);
	boost::tie(e, e_end) = edges(graph); 
	for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd != pr && !this->isInEdgeList(eList, pr, nd ))
		{
			eList.push_back(*ei);
		}
	}//endFor
	
	tripleNodes triple; // to store A-B-C forms
	this->computeTripleNodes(graph, eList, triple);

	for ( int i = 0; i < triple.size(); i++ ) // 
	{
		unsigned int a1, b1, c1;
		a1 = triple[i].a;
		b1 = triple[i].b;
		c1 = triple[i].c;
		
		for ( int j = i + 1; j < triple.size(); j++)
		{
			unsigned int a2, b2, c2;
			a2 = triple[j].a;
			b2 = triple[j].b;
			c2 = triple[j].c;

			if (a1 == a2 && b1 != b2 && c1 == c2)
			{
				unsigned int wAB1, wAB2, wBC1, wBC2, wAC2;
				edgExist1 = this->isInEdgeList(eList, a1, c1);
				edgExist2 = this->isInEdgeList(eList, b1, b2);
				unsigned int node, parent;
					for(edgeList::iterator ei = eList.begin(); ei != eList.end(); ++ei) 
			{
				this->getEdgeNodesIndex( *ei, node, parent);
				if ( (node == b1 && parent == a1) || (node == a1 && parent == b1) ) 
				{
					wAB1 = weightmap[*ei];//-1

				}else if ( (node == b2 && parent == a2)|| (node == a2 && parent == b2)   )
				{
					wAB2 = weightmap[*ei];//-1
				}	
				else if ( (node == c1 && parent == b1) )//|| (node == b1 && parent == c1) )
				{
					wBC1 = weightmap[*ei];//2
				}
				else if ( (node == b2 && parent == c2) )//|| (node == b2 && parent == c2) )
				{
					wBC2 = weightmap[*ei];//2
				}
				else if ( (node == c1 && parent == a1) || (node == c2 && parent == a2) )
				{
					wAC2 = weightmap[*ei];//-1
				}
					
			}//endfor
					
				if ( edgExist1 == true && edgExist2 == false )  // First possible orientation
				{
					cout<<"test1 "<<a1<<a2<<b1<<b2;
						if ( wAB1 !=2 && wBC1 == 2 &&  // means no direction
						 wAB2 !=2 && wBC2 == 2 && wAC2!=2 ) // means v structure
					{
						//this->putWeightPDAG(b1, b2, 1);
						for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
	
		this->getEdgeNodesIndex(*ei, nd, pr);
		
		if (nd==b2 && pr==a1)
		{
			cout<<"test2 "<<a1<<"-->"<<b2;
			weightmap[*ei]=2;// a1->c1
			name1[*ei]=="COMPELLED";

		}
	}
	
					} 
					

				}

			}
		}
	}

	return graph;

}
//////////////////////////////////////////////////////////////////////////

/*! Method constructDAG works as
*  while there are unoriented edges
*     step 1: Orient A-B as A->B
*     step 2: apply Meek rules
*/

void pmPDAG::constructDAG( )
{
	this->constructDAG(this->pBN);
}

void pmPDAG::constructDAG( pmBayesianNetwork* pBN)
{
	edgeList elist;
	
	elist = this->getEdgeList(pBN); // get edge list of the graph
	
	for (int i = 0; i < elist.size(); i++) // check all edges whether it is oriented or not
	{
		int weight = this->getWeightPDAG(pBN, elist[i]);
		if (weight == 2) // if it is unoriented then orient it and then apply further rules
		{
			this->putWeightPDAG(pBN, elist[i], 1);  // step 1: Orient A-B as A->B
			this->MeekRuleA();
			this->MeekRuleB();
			this->MeekRuleC();
			this->MeekRuleD();

		}

	}

}

//////////////////////////////////////////////////////////////////////////
void pmPDAG::print_PDAG()
{
	edgeList eList = this->getEdgeList();
	this->print_PDAG(eList);

}
void pmPDAG::print_PDAG(edgeList e)
{
	//slGraph& graph = pBN->get_graph();
	unsigned int node, parent;

	cout<< "\n *********** Each edge direction***********";
	for(edgeList::iterator ei = e.begin(); ei != e.end(); ++ei) 
	{
		this->getEdgeNodesIndex(*ei, node, parent);
		cout<<"\n"<<parent+1;

		if (this->getWeightPDAG(parent, node) == 1)
		{
			cout<<" ---> ";
		}else if (this->getWeightPDAG(parent, node) == 2)
		{
			cout<<" --- ";
		}else if (this->getWeightPDAG(parent, node) == -1)
		{
			cout<<" <--- ";
		}else if (this->getWeightPDAG(parent, node) == 10)
		{
			cout<<" ==== ";
		}
		cout << node+1;
	}

	cout<<endl;

}//
//*************************
void pmPDAG::print_graph()
{
	pmGraph& graph = this->pBN->get_graph();
	this->print_graph(graph);
}

void pmPDAG::print_graph(pmGraph& graph)
{
	unsigned int i, j, n;
	bool exists;
	pmEdge e;

	n = (unsigned int)num_vertices(graph);
	cout<<"\n ********* Undirected graph **********"<<endl;
	for(i = 0; i < n; i++) {
		cout << i + 1 << " ---  ";
		for(j = 0; j < n; j++) {
			if(j == i)
				continue;
			boost::tie(e, exists) = edge(vertex(i, graph), vertex(j, graph), graph);
			if(exists)
				cout << j + 1 << " ";
		}
		cout << endl;
	}
}