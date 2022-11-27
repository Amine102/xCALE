#include "pilgrim/general/ResultAnalysis.h"

using namespace PILGRIM;

int TotalSameEdges=0,TotalExteraEdges=0, TotalMissingEdges=0, TotalEdges_incr=0, TotalEdges_batch=0;

int Total_TP_incr=0, Total_FP_incr=0, Total_FN_incr=0, Total_TP_batch=0, Total_FP_batch=0, Total_FN_batch=0;

Matrix_t ResultAnalysis::readFile(string fileName)
	{
	Matrix_t matVec;// 2d vector for storing all data read from file

	vector<unsigned int> lineVec;// for the entries on each line
	ifstream infile(fileName.c_str());
	if(infile)
		{
		unsigned int val = 0;
		while(!infile.eof())
			{
			infile >> val;
			lineVec.push_back(val);
			if( infile.peek() == '\n' )// last entry on the line has been read
				{
				matVec.push_back(lineVec);// push all of the lines entries at once
				lineVec.clear();// prepare to read the next line
				}		
			}
		infile.close();
		}
	else
		{
		cout << "\n Adjacency matrix file could not open \n";
		}

	cout<<"  data size: "<<matVec.size()<<" x "<<matVec[0].size();
	return matVec;
	}


CPCWindow ResultAnalysis::ExtractCPCMap(Matrix_t matCPCFile1)
	{
	vector<int> cpc;
	MapCPC cpcOfEachVar;
	CPCWindow cpcInEachWindow;
	int var;
	for(size_t n=0; n<matCPCFile1.size(); ++n)
		{
		for(size_t k=0; k<matCPCFile1[n].size(); ++k)
			{
			if (k==0) // separate each variable name
				var = matCPCFile1[n][k];
			else
				cpc.push_back(matCPCFile1[n][k]);
			}
		//  Separate each window. check if there is variable 0
		if (var == 0 && !cpcOfEachVar.empty())  // to separate each window
			{
			cpcInEachWindow.push_back(cpcOfEachVar); // store each window in a vector
			cpcOfEachVar.clear(); // reset vars for next window
			}
		cpcOfEachVar.insert(make_pair(var, cpc));			
		cpc.clear();
		}// end for

	cpcInEachWindow.push_back(cpcOfEachVar); // to store last window in a vector

	return cpcInEachWindow;
	}

void ResultAnalysis::CompareCPC( CPCWindow& OrigNet, CPCWindow& batchWin, CPCWindow& incrWin, vector<Matrix_t> allData )
	{
	int winNum, var;
	float sameEdges_Perc, exteraEdges_Perc, missingEdges_Perc;
	vector<int> cpcIncr, cpcBatch, cpcOrignet;
	MapCPC varMapIncr, varMapBatch, varMapOrigNet; 
	MapCPC::iterator mapIter, mapIterBatch, mapIterOrig;

	cout<<"\n Windows in batch and incremental files = "<<batchWin.size()<< " = "<<incrWin.size()<<endl;

	ofstream outFile("Results.txt",ios::app);
	outFile<<"\n WinNo. No.Instances TP_incr TP_batch FP_incr FP_batch FN_incr FN_batch";
	outFile<<" T_InrEdges T_BatchEdges T_SameEdges T_ExteraEdges T_MissingEdges SameEdges% ExteraEdges% MssingEdges%";
	//outFile<<" T_MissingEdgesOrig MissingEdgesOrig%";
	outFile<<" TOCOrevised TOCOnotRevised TOCOnotRevised% Inc_FCalls";
	outFile<<" Batch_FCalls";
	// allData[4][i][0]= incremental file execution time contains 2 column, number of instance and time taken
	outFile<<"  Inc_TimeTaken";
	// allData[5][i][0]= Batch file execution time contains 2 column, number of instance and time taken
	outFile<<" Batch_TimeTaken";
	outFile<<" CG% TG%";
	outFile.close();

	varMapOrigNet = OrigNet.at(0);

	for (size_t  i = 0; i< incrWin.size(); i++ )
		{
		winNum = i;
		varMapIncr = incrWin.at(i);
		varMapBatch = batchWin.at(i);
				
		for (mapIter = varMapIncr.begin(); mapIter !=varMapIncr.end(); ++mapIter)
			{
			var = mapIter->first;
			cpcIncr = mapIter->second;
			//find variable var in batch file then extract cpcBatch
			mapIterBatch = varMapBatch.find(var);
			mapIterOrig = varMapOrigNet.find(var);
			if ( mapIterBatch != varMapBatch.end( ) )
				cpcBatch = mapIterBatch->second;
			if ( mapIterOrig != varMapOrigNet.end() )
				cpcOrignet = mapIterOrig->second;

			CalculateResults(cpcOrignet, cpcBatch, cpcIncr);
			}	
		// calculations for each window
		sameEdges_Perc = (float)TotalSameEdges/TotalEdges_incr*100;
		exteraEdges_Perc = (float)TotalExteraEdges/TotalEdges_incr*100;
		missingEdges_Perc = (float)TotalMissingEdges/TotalEdges_incr*100;
cout<<"\n   1 ";
		ofstream outFile("Results.txt",ios::app);
		outFile<<"\n "<<i+1<<" "<<allData[3][i][0] /*number of instances*/ <<" "<<Total_TP_incr/2<<" "<<Total_TP_batch/2<<" "<<Total_FP_incr/2<<" "<<Total_FP_batch/2 <<" "<<Total_FN_incr/2<<" "<<Total_FN_batch/2;
		outFile<<" "<<TotalEdges_incr/2<<" "<<TotalEdges_batch/2<<" "<<TotalSameEdges/2<<" "<<TotalExteraEdges/2 <<" "<<TotalMissingEdges/2<<" "<<sameEdges_Perc<<" "<<exteraEdges_Perc<<" "<<missingEdges_Perc;
				// allData[3][i][0]= incremental file test summary contains 3 column Toco fired, non fired and total function calls
cout<<" 2 ";
		int sum = allData[3][i][3]+allData[3][i][2];
		outFile<<" "<<allData[3][i][2]<<" "<<allData[3][i][3]<<" "<<(float)(allData[3][i][3]/sum)*100 <<" "<<allData[3][i][4];
cout<<" 3 ";
		// allData[3][i][0]= batch file test summary contains 3 column Toco fired, non fired and total function calls
		outFile<<" "<<allData[4][i][4];// only function calls for batch
cout<<" 4 ";
		// allData[5][i][0]= incremental file execution time contains 2 column, number of instance and time taken
		outFile<<" "<<allData[3][i][1]; //time taken
cout<<" 5 ";
		// allData[6][i][0]= Batch file execution time contains 2 column, number of instance and time taken
		outFile<<" "<<allData[4][i][1];// time taken 
cout<<" 6 ";
		float callgain = 1-(float)allData[3][i][4]/allData[4][i][4];
cout<<" 7 ";
		outFile<<" "<<callgain*100;// Call gain
cout<<" 8 ";
		float timegain = 1-(float)allData[3][i][1]/allData[4][i][1];
cout<<" 9 ";
		outFile<<" "<<timegain*100;// time gain 

		TotalSameEdges = TotalExteraEdges = TotalMissingEdges = TotalEdges_incr = TotalEdges_batch =0;
		Total_TP_incr=0, Total_FP_incr=0, Total_FN_incr=0, Total_TP_batch=0, Total_FP_batch=0, Total_FN_batch=0;
		outFile.close();

		}
	// calculations overall

	}

void ResultAnalysis::CompareCPC( CPCWindow& batchWin, CPCWindow& incrWin, vector<Matrix_t> allData )
	{
	int winNum, var;
	float sameEdges_Perc, exteraEdges_Perc, missingEdges_Perc;
	vector<int> cpcIncr, cpcBatch;
	MapCPC varMapIncr, varMapBatch; 
	MapCPC::iterator mapIter, mapIterBatch;

	cout<<"\n Windows in batch and incremental files = "<<batchWin.size()<< " = "<<incrWin.size()<<endl;

	ofstream outFile("Results.txt",ios::app);
	outFile<<"\n WinNo. No.Instances T_InrEdges T_BatchEdges T_SameEdges T_ExteraEdges T_MissingEdges SameEdges% ExteraEdges% MssingEdges%";
	outFile<<" TOCOrevised TOCOnotRevised TOCOnotRevised% Inc_FCalls";
	outFile<<" Batch_FCalls";
	// allData[4][i][0]= incremental file execution time contains 2 column, number of instance and time taken
	outFile<<"  Inc_TimeTaken";
	// allData[5][i][0]= Batch file execution time contains 2 column, number of instance and time taken
	outFile<<" Batch_TimeTaken";
	outFile<<" CG% TG%";
	outFile.close();

	for (size_t  i = 0; i< incrWin.size(); i++ )
		{
		winNum = i;
		varMapIncr = incrWin.at(i);
		varMapBatch = batchWin.at(i);

		for (mapIter = varMapIncr.begin(); mapIter !=varMapIncr.end(); ++mapIter)
			{
			var = mapIter->first;
			cpcIncr = mapIter->second;
			//find variable var in batch file then extract cpcBatch
			mapIterBatch = varMapBatch.find(var);
			if ( mapIterBatch != varMapBatch.end( ) )
				cpcBatch = mapIterBatch->second;
	
			CalculateResults(cpcBatch, cpcIncr);
			}	
		// calculations for each window
		sameEdges_Perc = (float)TotalSameEdges/TotalEdges_incr*100;
		exteraEdges_Perc = (float)TotalExteraEdges/TotalEdges_incr*100;
		missingEdges_Perc = (float)TotalMissingEdges/TotalEdges_incr*100;
		cout<<"\n   1 ";
		ofstream outFile("Results.txt",ios::app);
		outFile<<"\n "<<i+1<<" "<<allData[5][i][0] /*number of instances*/ <<" "<<TotalEdges_incr<<" "<<TotalEdges_batch<<" "<<TotalSameEdges<<" "<<TotalExteraEdges <<" "<<TotalMissingEdges<<" "<<sameEdges_Perc<<" "<<exteraEdges_Perc<<" "<<missingEdges_Perc;
		// allData[3][i][0]= incremental file test summary contains 3 column Toco fired, non fired and total function calls
		cout<<" 2 ";
		int sum = allData[3][i][1]+allData[3][i][0];
		outFile<<" "<<allData[3][i][0]<<" "<<allData[3][i][1]<<" "<<(float)allData[3][i][1]/sum*100 <<" "<<allData[3][i][2];
		cout<<" 3 ";
		// allData[3][i][0]= batch file test summary contains 3 column Toco fired, non fired and total function calls
		outFile<<" "<<allData[4][i][2];// only function calls for batch
		cout<<" 4 ";
		// allData[5][i][0]= incremental file execution time contains 2 column, number of instance and time taken
		outFile<<" "<<allData[5][i][1]; //time taken
		cout<<" 5 ";
		// allData[6][i][0]= Batch file execution time contains 2 column, number of instance and time taken
		outFile<<" "<<allData[6][i][1];// time taken 
		cout<<" 6 ";
		float callgain = 1-(float)allData[3][i][2]/allData[4][i][2];
		cout<<" 7 ";
		outFile<<" "<<callgain*100;// Call gain
		cout<<" 8 ";
		float timegain = 1-(float)allData[5][i][1]/allData[6][i][1];
		cout<<" 9 ";
		outFile<<" "<<timegain*100;// time gain 

		TotalSameEdges = TotalExteraEdges = TotalMissingEdges = TotalEdges_incr = TotalEdges_batch =0;
	
		outFile.close();
		}
	}


void ResultAnalysis::CalculateResults(vector<int>& cpcOrig, vector<int>& cpcBatch, vector<int>& cpcIncr )
	{
	int sameEdges=0,exteraEdges=0, missingEdges=0;
	int TP_incr=0, FP_incr=0, FN_incr=0, TP_batch=0, FP_batch=0, FN_batch=0;
	
	// //compare incremental and batch cpc for a single variable
	for (size_t i=0; i<cpcIncr.size(); i++)
		for (size_t j=0; j<cpcBatch.size(); j++)
			{
			if (cpcIncr[i]==cpcBatch[j])
				{
				++sameEdges;
				}
			}
		TotalEdges_incr = TotalEdges_incr + cpcIncr.size();
		TotalEdges_batch = TotalEdges_batch + cpcBatch.size();
		exteraEdges = cpcIncr.size()-sameEdges;
		missingEdges = cpcBatch.size()-sameEdges;
				
		TotalSameEdges = TotalSameEdges + sameEdges;
		TotalExteraEdges = TotalExteraEdges + exteraEdges;
		TotalMissingEdges = TotalMissingEdges + missingEdges;

	// //compare incremental and orignal net cpc for a single variable
		for (size_t i=0; i<cpcIncr.size(); i++)
			for (size_t j=0; j<cpcOrig.size(); j++)
				{
				if (cpcIncr[i]==cpcOrig[j])
					{
					++TP_incr;
					}
				}
			FP_incr = cpcIncr.size()-TP_incr;
			FN_incr = cpcOrig.size()-TP_incr;
			Total_TP_incr += TP_incr;
			Total_FP_incr += FP_incr;
			Total_FN_incr += FN_incr;

	// //compare batch and orignal net cpc for a single variable
			for (size_t i=0; i<cpcBatch.size(); i++)
				for (size_t j=0; j<cpcOrig.size(); j++)
					{
					if (cpcBatch[i]==cpcOrig[j])
						{
						++TP_batch;
						}
					}
				FP_batch = cpcBatch.size()-TP_batch;
				FN_batch = cpcOrig.size()-TP_batch;
				Total_TP_batch += TP_batch;
				Total_FP_batch += FP_batch;
				Total_FN_batch += FN_batch;
	}

void ResultAnalysis::CalculateResults( vector<int>& cpcBatch, vector<int>& cpcIncr )
	{
	int sameEdges=0,exteraEdges=0, missingEdges=0;
	
	// //compare incremental and batch cpc for a single variable
	for (size_t i=0; i<cpcIncr.size(); i++)
		for (size_t j=0; j<cpcBatch.size(); j++)
			{
			if (cpcIncr[i]==cpcBatch[j])
				{
				++sameEdges;
				}
			}
		TotalEdges_incr = TotalEdges_incr + cpcIncr.size();
		TotalEdges_batch = TotalEdges_batch + cpcBatch.size();
		exteraEdges = cpcIncr.size()-sameEdges;
		missingEdges = cpcBatch.size()-sameEdges;

		TotalSameEdges = TotalSameEdges + sameEdges;
		TotalExteraEdges = TotalExteraEdges + exteraEdges;
		TotalMissingEdges = TotalMissingEdges + missingEdges;

	}

void ResultAnalysis::print(CPCWindow win)
	{
	MapCPC varMap;
	vector<int> cpcIncr;
	MapCPC::iterator mapIter;

	cout<<"\n Windows size = "<<win.size();

	for (size_t  i = 0; i< win.size(); i++ )
		{
		varMap = win.at(i);
		for (mapIter = varMap.begin(); mapIter !=varMap.end(); ++mapIter)
			{
			cout<<"\n print variable = "<<mapIter->first<<", ";
			cpcIncr = mapIter->second;
			for (int j=0; j<cpcIncr.size(); j++)
				cout<<" "<<cpcIncr[j];
			}			
		}	

	}

ResultAnalysis::ResultAnalysis()
	{

	};

ResultAnalysis::~ResultAnalysis()
	{

	};

void ResultAnalysis::getBayesianNetwork( Matrix_t& adjMatrix, pmBayesianNetwork* pBN)
	{

	int nodeA, nodeB, rowCounter=0, colCounter=0;
	pmEdge e;
	bool edgeAdded;
	pmGraph& graph = pBN->get_graph();
	int weight = 2;
	
	Matrix_t::iterator rowItr;
	std::vector<unsigned int>::iterator colItr;
	property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);

	for(rowItr = adjMatrix.begin(); rowItr != adjMatrix.end(); ++rowItr) 
		{
		nodeA = rowCounter;
		rowCounter++;
		
		colCounter = 0;
		for(colItr = rowItr->begin(); colItr!= rowItr->end(); ++colItr)
			{
			nodeB = colCounter;
			colCounter++;
			
			if(*colItr == 1){
				boost::tie(e, edgeAdded) = add_edge(vertex(nodeA, graph), vertex(nodeB, graph), graph);
				put(weightMap, e, weight);
				}
			}
		}

	}

void ResultAnalysis::compareBNs(pmGraph& origG, pmGraph& computedG, unsigned int& TP, unsigned int& FP, unsigned int& TN, unsigned int& FN, unsigned int& reversedEdges, unsigned int& strHammingDistance )
	{

	// To find the True positives, False positives, True negatives, False negatives, Reversed edges and structural hamming distance(see MMHC orignal paper).  
	TP = FP = TN = FN = reversedEdges = strHammingDistance = 0;
	
	pmGraph origGraph = convertDAGintoCPDAG(origG);
	pmGraph computedGraph = convertDAGintoCPDAG(computedG);
	
	/* Number of nodes within bayesian network */
	int numberOfNodes = num_vertices(origGraph);
	
	pmEdge e1,e2,e3,e4;
	
	property_map<pmGraph, edge_weight_t>::type weightMapOrig = get(edge_weight, origGraph);
	property_map<pmGraph, edge_weight_t>::type weightMapComp = get(edge_weight, computedGraph);
	
	std::vector<pair<unsigned int, unsigned int>> origGraphEdgeList; 
	std::vector<pair<unsigned int, unsigned int>> compGraphEdgeList;
	pmEdgeIterator ei, ei_end;

	// To compute SHD, first get the list of the edges for both graph, then delete the common/same edges, the remaining number of elements will be difference
	for(boost::tie(ei, ei_end) = edges(computedGraph); ei != ei_end; ++ei){
		compGraphEdgeList.push_back(make_pair(vertex_id[source(*ei, computedGraph)], vertex_id[target(*ei, computedGraph)]));		
		}
	
	// First delete the same edges and having the same weight in computed edge list
	// Then find if edge a->b is already exist in computed edge as reverse edge b->a then dont save it.
	// Finally sum of the size of both list will be the SHD.

	for(boost::tie(ei, ei_end) = edges(origGraph); ei != ei_end; ++ei){
		unsigned int src = vertex_id[source(*ei, origGraph)];
		unsigned int trgt = vertex_id[target(*ei, origGraph)];
		pair<unsigned int, unsigned int> edgeNodesPair = make_pair(src, trgt);

		std::vector<pair<unsigned int, unsigned int>>::iterator itr = find(compGraphEdgeList.begin(), compGraphEdgeList.end(), edgeNodesPair);
		if (itr != compGraphEdgeList.end() && weightMapOrig[*ei] == weightMapComp[*ei]){ // to delete only those edges which have the same direction (weight)
			compGraphEdgeList.erase(itr);
		}else { //if edge a->b is already exist in reverse direction in compGraphEdgelist then no need to store it again.
				itr = find(compGraphEdgeList.begin(), compGraphEdgeList.end(), make_pair(trgt, src));
				if (!(itr != compGraphEdgeList.end() && weightMapOrig[*ei] == weightMapComp[*ei]))
					origGraphEdgeList.push_back(edgeNodesPair);					
		   	}
		}//end for

	strHammingDistance = compGraphEdgeList.size() + origGraphEdgeList.size();

	//pmEdgeIterator testItr = find();
	
	/* Variables representing two nodes into the following loops */
	/* Scrolling through nodes */
	for (int i = 0; i < numberOfNodes - 1; i++) {
		
		/* Scrolling through remaining nodes */
		for (int j = i + 1; j < numberOfNodes; j++) {
			
			pmEdge edgeA, edgeB;
			bool testA = false, testB = false;

			/* Variables association to desired edges in original graph*/
			tie(edgeA, testA) = edge(vertex(i, origG), vertex(j, origG), origG);
			tie(edgeB, testB) = edge(vertex(j, origG), vertex(i, origG), origG);

			pmEdge compEdgeA, compEdgeB;
			bool compTestA = false, compTestB = false;

			/* Variables association to desired edges in computed graph*/
			tie(compEdgeA, compTestA) = edge(vertex(i, computedG), vertex(j, computedG), computedG);
			tie(compEdgeB, compTestB) = edge(vertex(j, computedG), vertex(i, computedG), computedG);

			if (( testA || testB) && ( compTestA || compTestB))
				++TP;
			else if((!testA && !testB) && (compTestA || compTestB)){
				++ FP;
				}
			else if((!testA && !compTestA) && (!testB && !compTestB))
				++TN;
			else if((testA || testB) && ( !compTestA && !compTestB)){
				++FN;	
				}			
			}
		}
		
	}

/* Algorithm DAG-to-CPDAG ,chickering(1995)
 * Input: DAG graph
 * OutPut: DAG graph with each edge labeled either "COMPELLED" or "REVERSIBLE"
 * 
 * Order the edges in Graph
 * Label every edge in Graph as "UNKNOWN"
 * While there are edges labeled "UNKNOWN" in Graph
 *      let x->y be the lowest ordered edge that is labeled "UNKNOWN"
 *      For every edge w->x labeled "COMPELED"
 *          IF w is not a parent of y then
 *             label x->y and every edge  incident into y with "COMPELLED"
 *             GO TO While;
 *          ELSE
 *             Label w-> with "COMPELLED"
 *             
 *  IF there exists an edge z->y such that z!=x and z is not a parent of x then
 *      Label x->y and all "UNKNOWN" edges incident into y with "COMPELLED"
 *  ELSE
 *      Label x->y and all "UNKNOWN" edges incident into y with "REVERSIBLE"
 **/
pmGraph ResultAnalysis::convertDAGintoCPDAG( pmGraph graph )
	{

	boost::property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph); 
	boost::property_map<pmGraph, edge_name_t>::type name = get(edge_name, graph);
	
	//1-order nodes using topological sort
	this->orderEdges(graph, nodeList);

	//2-label every edge of graph as "UNKNOWN"
	pmEdgeIterator ei, ei_end;
	for(tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei){
		name[*ei] = "UNKNOWN";
		weightmap[*ei] = -1;
		}//end for

	//3-
	std::vector<pmNode>::iterator nodeListItr;
	pmNode x, y, w, z;
	pmEdge *xyEdge = NULL;
	
	for(nodeListItr = nodeList.begin() ; nodeListItr != nodeList.end(); ++nodeListItr){
		bool flag = false;
		x = *nodeListItr;
		pmOutEdgeIterator outEdgeItr, outEdgeItrEnd;
	  for(boost::tie(outEdgeItr, outEdgeItrEnd)= out_edges(x, graph); outEdgeItr!= outEdgeItrEnd; ++outEdgeItr) // to get x-->y edges
			{
			y = target(*outEdgeItr, graph);
			xyEdge = &(*outEdgeItr); // out_edges, it is already sorted from lowest to highest

			name = get(edge_name, graph);
			if(name[*outEdgeItr].compare("UNKNOWN")==0){
				//for every edge w-->x labeled "compelled"
				pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
				for(boost::tie(inEdgeItr, inEdgeItrEnd)= in_edges(x, graph); inEdgeItr != inEdgeItrEnd; ++inEdgeItr){
					if(name[*inEdgeItr].compare("COMPELLED")==0){
						w = source(*inEdgeItr, graph);
						pmEdge* wyEdge=NULL;
						if (!isParent(w,y, graph, wyEdge)){
							labelEdges(xyEdge, y, graph, "COMPELLED", false, true);
							flag = true;
							break;
							}else
							{
							labelEdges(wyEdge, y, graph, "COMPELLED", false, false);
								}
						}//endif
					}//endfor
			if(!flag){
				if(isExistZYedgeAndZnotParentOfX(x, y, xyEdge, graph))
					labelEdges(xyEdge, y, graph, "COMPELLED", true, true);
				else
					labelEdges(xyEdge, y, graph, "REVERSIBLE", true, true);
				}
				}//endif		//cout<<"\n "<<vertex_id[*nodeListItr]<<" -->"<<vertex_id[target(*outEdgeItr, graph)];
			}//end for
	  name = get(edge_name, graph);
	}//end for	

	return graph;
	}

void ResultAnalysis::print_CPDAG( pmGraph graph )
	{
		//pmEdge e;
		property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph); 
		property_map<pmGraph, edge_name_t>::type edgeLabel = get(edge_name, graph); 

		pmEdgeIterator ei, ei_end;
		for(tie(ei, ei_end) = edges(graph); ei != ei_end; ++ei){
			if (weightmap[*ei] == 2)
				std::cout << vertex_id[source(*ei, graph)] << " --> " << vertex_id[target(*ei, graph)] << " " << std::endl;
			else 
				std::cout << vertex_id[source(*ei, graph)] << " ----  " << vertex_id[target(*ei, graph)] << " " << std::endl;
			}
						
	}

void ResultAnalysis::orderEdges( pmGraph& graph, std::vector<pmNode>& ndList )
	{
	// topological sort will gives the order from lowest to highest
	// we need the highest node toward lowest so, we will reverse the order and then we will check the first(highest) node towards lowest by using out_edges_list. 
	ndList.clear();
	
	try {
		topological_sort(graph, std::back_inserter(ndList));
		}catch(not_a_dag &e)
		{
		cout << " \n Cycle found" <<endl;
		}

		std::reverse(ndList.begin(), ndList.end());
	}

bool ResultAnalysis::isParent( pmNode& w, pmNode& y, pmGraph& graph, pmEdge* wyEdge)
	{
	pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
	for(boost::tie(inEdgeItr, inEdgeItrEnd)= in_edges(y, graph); inEdgeItr != inEdgeItrEnd; ++inEdgeItr){
		if (source(*inEdgeItr, graph) == w){ // if w is a parent of y
			wyEdge = &(*inEdgeItr);
			return true;
			}else
			{
			wyEdge = NULL;
			return false;
				}
		}
	}

void ResultAnalysis::labelEdges( pmEdge* xyEdge, pmNode& y, pmGraph& graph, string label, bool onlyUnknownEdges, bool incidentEdgesToo )
	{
	boost::property_map<pmGraph, edge_name_t>::type edgeNameMap = get(edge_name, graph);
	boost::property_map<pmGraph, edge_weight_t>::type weightmap = get(edge_weight, graph); 

	int w;
	
	if (label.compare("COMPELLED")==0)
		w = 2;
	else
		w= -1;
	
	
	if (xyEdge!= NULL){
		edgeNameMap[*xyEdge] = label;
		weightmap[*xyEdge] = w;
		}
	
	if (incidentEdgesToo){ 
		//find edges incident of y
		pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
		for(boost::tie(inEdgeItr, inEdgeItrEnd)= in_edges(y, graph); inEdgeItr != inEdgeItrEnd; ++inEdgeItr){
			if(onlyUnknownEdges){
				if (edgeNameMap[*inEdgeItr].compare("UNKNOWN")==0){
					edgeNameMap[*inEdgeItr] = label;
					weightmap[*inEdgeItr] = w;
					}
				}else{
					edgeNameMap[*inEdgeItr] = label;
					weightmap[*inEdgeItr] = w;
				     }
			}//endfor
		}
	}

bool ResultAnalysis::isExistZYedgeAndZnotParentOfX( pmNode& x, pmNode& y, pmEdge* xyEdge, pmGraph& graph )
	{
	pmInEdgeIterator inEdgeItr, inEdgeItrEnd;
	for(boost::tie(inEdgeItr, inEdgeItrEnd)= in_edges(y, graph); inEdgeItr != inEdgeItrEnd; ++inEdgeItr){
		pmNode w = source(*inEdgeItr, graph);
		if(*inEdgeItr != *xyEdge && !isParent(w, x, graph, NULL)){ //if z != x(means it is not the same edge) and z is not a parent of x
			return true;
			}
		}//endfor
	return false;
	}

