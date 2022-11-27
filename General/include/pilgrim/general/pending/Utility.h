
					 /**
					* \brief  
					* \details 
					* \param variables : 
				    **/ 
					pmGraph convertDAGintoCPDAGwithKnowledge(pmBayesianNetwork* bn,edgeList1 knownEdges);
					/**
					* \brief   To convert DAG into CPDAG with knowledge
					* \details 
					* \param variables : 
					*  \author   Ghada Trabelsi
				  **/ 
					edgeList1 gettemporalEdgeList(pmBayesianNetwork* bn);
						/**
					* \brief fonctions used for debugging
					* \details Test if the costs associated with the edges of this full bayesian network
								(all the nodes are linked) form a symmetrical matrix (ie. cost(i,j)
								equals cost(j,i).  This matrix is printed having a '!' after each asymmetrical cost. 
					* \param variables : 
					**/
				
				  static void testIsWSymmetrical(pmBayesianNetwork *fullBN);
				  
				  template <typename rowDataType>
				  static void learnParametersByEM(pmBayesianNetwork *pBN,
					  pmFrequencyCounter<rowDataType> *pFrequencyCounter,
					  unsigned int maxEMIterations,
					  bool updateMissingProbTable = false,
					  pmScoreValueType bayesianPriorWeight = 0);