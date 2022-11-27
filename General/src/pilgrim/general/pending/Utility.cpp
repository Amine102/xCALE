/* Algorithm DAG-to-CPDAG with Knowledge ,Meek(1995)
 * Input: DAG graph
 * OutPut: DAG graph with each edge labeled either "COMPELLED" or "REVERSIBLE"
 *Let knownEdges = (F Forbiden, R Required) be the background knowledge and let graph1 be the partially
 *directed graph obtained from DAG to CPDAG phase
 *S1: If there is an edge A-->B in F such that A-->B is in graph1 then FAIL.
 *S2: If there is an edge A-->B in R such that B-->A is in graph1 or A is not adjacent to B then FAIL.
 *S3: Randomly choose One edge A-->B from R and let R=R\{A-->B}
 *S4: Orient A-->B in graph1 and close orientations under R1, R2,R3 and R4 (meek rules)
 *S5: If R is not empty then go to S1.
 */
pmGraph pmUtility::convertDAGintoCPDAGwithKnowledge( pmBayesianNetwork* bn,edgeList1 knownEdges )
	{
// construction PDAG without knowledge
		
		pmGraph graph1=convertDAGintoCPDAG(bn->get_graph());
	
		boost::property_map<pmGraph, edge_name_t>::type  name1 = get(edge_name, graph1);
		boost::property_map<pmGraph, boost::vertex_index_t>::type vertex_id;
		property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph1);
		unsigned int nd, pr; 
		pmEdge e;
		pmEdgeIterator ei, ei_end;
		
		 pmPDAG *g=new pmPDAG(bn);
		
//insert knowledge in PDAG if it is required

		std::map <int, pmEdge>::const_iterator itrMap;
		
// Randomly choose One edge A-->B from R and let R=R\{A-->B}
		for(itrMap=knownEdges.begin(); itrMap != knownEdges.end(); ++itrMap)
			{
				if(itrMap->first>0)
				{
					
				e=itrMap->second;
				g->getEdgeNodesIndex(e, nd, pr);
				bool test=false;
	for(boost::tie(ei, ei_end) = edges(graph1); ei != ei_end; ++ei){
		
// Orient A-->B in graph1 and close orientations under R1, R2,R3 and R4 (meek rules)
		if((vertex_id[source(*ei, graph1)]==pr && vertex_id[target(*ei, graph1)]==nd && weightmap[*ei] ==-1 ))//&& weightmap[*ei] ==-1
		{
		
		weightmap[*ei] = 2;
		name1[*ei]=="COMPELLED";
       
       test=true;
      // knownEdges.erase(itrMap);
	  
				}
//If there is an edge A-->B in R such that B-->A is in graph1 then FAIL.
		else if((vertex_id[source(*ei, graph1)]==nd && vertex_id[target(*ei, graph1)]==pr && weightmap[*ei] ==2 ))
		{
		//cout<<" Fail then there is no complete causal explanation for this  consistent model with Knowledge.";
		test=false;		
		}
				}
//If there is an edge A-->B in R and A is not adjacent to B in graph1 then FAIL.
		if(test==false)
				{
cout<<" Fail then there is no complete causal explanation for this  consistent model with Knowledge.";
		}
		else
		{
			
			//Apply meek rules after addition of knowledge
		   
       graph1=g->MeekRuleA(graph1);
       graph1=g->MeekRuleB(graph1);
       graph1=g->MeekRuleC(graph1);
       graph1=g->MeekRuleD(graph1);
		}
		
				}
//If there is an edge A-->B in F such that A-->B is in graph1 then FAIL.
				else
				{
						
        e=itrMap->second;
				g->getEdgeNodesIndex(e, nd, pr);
	for(boost::tie(ei, ei_end) = edges(graph1); ei != ei_end; ++ei){
		if((vertex_id[source(*ei, graph1)]==pr && vertex_id[target(*ei, graph1)]==nd && weightmap[*ei] ==2))
		{
		cout<<" Fail then there is no complete causal explanation for this  consistent model with Knowledge.";
				}
		}
				}
			

		}
		
		return graph1;
	}

	
edgeList1 pmUtility::gettemporalEdgeList(pmBayesianNetwork* bn)
{
	unsigned int nd, pr;
	pmEdgeIterator e, e_end;
	edgeList elist;
	edgeList1 elist1;
	pmEdgeIterator edge_i,edge_end;
	edgeList1::iterator it;
	pmPDAG *g=new pmPDAG(bn);
	boost::tie(e, e_end) = edges(bn->get_graph()); 
	 it = elist1.begin();
	 int i=1;
	for(pmEdgeIterator ei = e; ei != e_end; ei++) 
	{
		g->getEdgeNodesIndex(*ei, nd, pr);
		if (nd != pr && !g->isInEdgeList(elist, pr, nd ))
		{
			if((nd> (bn->variables.dim()/3)-1)&&(pr< (bn->variables.dim()/3)))
			{
			
			elist.push_back(*ei);
			elist1.insert(it,make_pair(i,*ei));
			++it;
			i++;
			}
			else
			{
				
			}
			
		}
	}//endFor*/
	
return elist1;
}


//=============================================================================
void pmUtility::testIsWSymmetrical(pmBayesianNetwork *fullBN)
{
  pmNode node, parent;
  pmEdge e, inv;
  bool exists, isSym = true;
  property_map<pmGraph, edge_weight_t>::type weightMap 
    = get(edge_weight, fullBN->graph);
  unsigned int i, j, n = (unsigned int)num_vertices(fullBN->graph);
  long double val, valInv, diff;
  ios::fmtflags oflags;
  
  oflags = cout.flags();
  cout.flags(ios::fixed | ios::showpos | ios::left);
  for (i = 0; i < n; i++) {
    node = vertex(i, fullBN->graph);
    for (j = 0; j < n; j++) {
      if (i == j) {
        cout << (val = 0) << " ";
        continue;
      }
      parent = vertex(j, fullBN->graph);
      boost::tie(e, exists) = edge(parent, node, fullBN->graph);
      boost::tie(inv, exists) = edge(node, parent, fullBN->graph);
      val = (long double)weightMap[e];
      valInv = (long double)weightMap[inv];
      cout << val;
      diff = fabsl(val - valInv);
      if (diff > 0.000000000001) {
        isSym = false;
        cout << "!";
      }
      else
        cout << " ";
    }
    cout << endl;
  }
  cout << "pmUtility::testIsWSymmetrical(): matrix is "
       << ( (isSym)? "symmetrical" : "asymmetrical" ) << endl;
  cout.flags(oflags);
}



//=============================================================================
template <typename rowDataType>
void pmUtility::
learnParametersByEM(pmBayesianNetwork *pBN,
                    pmFrequencyCounter<rowDataType> *pFrequencyCounter,
                    unsigned int maxEMIterations,
                    bool updateMissingProbTable, 
                    pmScoreValueType bayesianPriorWeight)
{
 
}

template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< std::vector<unsigned int> >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< std::vector<int> >*,
                    unsigned int ,  bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< std::vector<float> >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< std::vector<double> >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< std::vector<long double> >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< unsigned int* >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< int* >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< float* >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< double* >*,
                    unsigned int , bool, pmScoreValueType);
template /*PL_DLL_API*/ void pmUtility::
learnParametersByEM(pmBayesianNetwork *, 
                    pmFrequencyCounter< long double* >*,
                    unsigned int , bool, pmScoreValueType);