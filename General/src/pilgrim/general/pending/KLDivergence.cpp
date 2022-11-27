

double pmKLDivergence::computeLogLoss( pmBayesianNetwork *BN_Orig, pmBayesianNetwork *BN_computed, pmCSVDataDescriptor * DataDscptr )
	{

	// to do
	// Function is in test phase, not valid

	const rowDataType* pCurrentLine;
	const vector<bool>* pPresentValues;
	unsigned int i;
	plVariablesConjunction allVariables = DataDscptr->observed_variables();

	//long unsigned int idxLine;
	plValues completeLine(allVariables);

	plVariablesConjunction_const_iterator vi, vi_end;

	DataDscptr->rewind();

	plJointDistribution jointDistOfOrigBN, jointDistOfCompBN;

	// Read original network
	plBayesianNetworkFromXDSLfile * XDSlfile = new plBayesianNetworkFromXDSLfile();
	plBayesianNetwork * plBN_orig;
	plBN_orig = XDSlfile->getPlBayesianNetworkObject (DataDscptr, "E:\\DataSet\\Alarm\\alarm.xdpm");
	jointDistOfOrigBN = plBN_orig->get_joint_distribution ();

	// Computed BN
	jointDistOfCompBN = BN_computed->learnParametersML (DataDscptr);

	plProbValue p_orignalBN = 0;
	plProbValue sum_Info1 = 0;
	plProbValue sum_Info2 = 0;
	plProbValue sum_Info3 = 0;
	plProbValue p_computedBN = 0;
	plValues evidence;
	
	unsigned int counter =0;
	// to read line by line
	while((DataDscptr->get_data_record(pCurrentLine, pPresentValues) )) {
		counter++;
			for(i = 0, vi = allVariables.begin(), vi_end = allVariables.end(); vi != vi_end; ++vi, ++i)
				{
				if((*pPresentValues)[i]) {
					completeLine[i] = (*pCurrentLine)[i];
					}
				}
		evidence = completeLine;
		p_computedBN = jointDistOfCompBN.compute(evidence);
        p_orignalBN = jointDistOfOrigBN.compute(evidence);	

		/// check for infinity error log (0) = infinity
		if (p_computedBN !=0 && p_orignalBN != 0 )
			{
			sum_Info2+= /*p_computedBN **/ log10(p_computedBN/p_orignalBN);
			sum_Info1+= /*p_orignalBN * */log10(p_orignalBN/p_computedBN);
			sum_Info3+= p_orignalBN * log(p_computedBN); // Cross entropy
			}				
		}// endwhile

	cout<<"\t"<<sum_Info1/counter<<"\t "<<sum_Info2/counter<<"\t"<<sum_Info3;
 
	return (double)sum_Info1;
	}