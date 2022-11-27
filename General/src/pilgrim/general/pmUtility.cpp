
#include <plRandom.h>
#include <pilgrim/general/pmUtility.h>
#include <pilgrim/general/random/GraphUtility.h>
#include <pilgrim/general/random/PMMixed.h>
#include <pilgrim/general/pmBayesianNetwork.h>
#include <vector>

using namespace boost;
using namespace std;
using namespace PILGRIM;
using namespace prm;

//plMt19937 plMt19937::generator_;

//=============================================================================
unsigned int pmUtility::getRandom(unsigned int superiorLimit)
{
	return plRandomInt(superiorLimit);
}

//=============================================================================
plProbValue pmUtility::getRandomProbability()
{
	return plRandomFloat(1.0);
}

//=============================================================================
void pmUtility::selectVariables(const plVariablesConjunction &from,
								const vector<bool> &selectedVars,
								plVariablesConjunction &to)
{
	plVariablesConjunction_const_iterator vi, vi_end;
	unsigned int i;
	for (i = 0, vi = from.begin(), vi_end = from.end(); vi != vi_end; ++vi, ++i)
		if (selectedVars[i])
			to ^= *vi;
}

//=============================================================================
// Added by F. Schnitzler, 2nd December 2010
void pmUtility::selectVariables(const plVariablesConjunction &from,
								const std::vector<unsigned int> &selectVariablesIndices,
								plVariablesConjunction &to)
{
	std::vector<unsigned int>::const_iterator vi, vi_end;
	for (vi = selectVariablesIndices.begin(),
		vi_end = selectVariablesIndices.end();
		 vi != vi_end; ++vi)
		to ^= from[*vi];
}

//=============================================================================
void pmUtility::getFileNames(vector<string> &listOfFiles)
{
	boost::filesystem::directory_iterator iterator(string("."));

	for (; iterator != boost::filesystem::directory_iterator(); ++iterator)
	{
		cout << (iterator->path().filename()) << endl;
		std::string fname = iterator->path().filename().string();
		boost::filesystem::path pth = iterator->path();
		listOfFiles.push_back(fname);
	}
}

//=============================================================================
void pmUtility::split(string file_name)
{
	char delim = ';';
	fstream myFile(file_name, ios::in);
	if (myFile)
	{
		unsigned int iterator(0);
		unsigned int nVars(0);
		string tuple;
		size_t pos = file_name.find('.');
		const string out_D0 = file_name.substr(0, pos) + "_t0.csv";
		const string out_Dt = file_name.substr(0, pos) + "_t.csv";
		ofstream out(out_D0, ios::out);
		ofstream out_T(out_Dt, ios::out);
		string memory_sequence;
		string prev_tuple;
		while (getline(myFile, tuple))
		{
			string element;

			// Cutting what is relative to sequence column/value/variable etc
			size_t pos = tuple.find_first_of(delim);

			// it becomes my line without the first column
			string cut_tuple = tuple.substr(pos + 1);

			// it becomes my number of sequence
			string sequence_number = tuple.substr(0, pos); // Keeping it just to know when I change index number

			if (iterator == 0)
			{
				// to store my variable either in t or t+1
				string variable;
				string variable_T;
				istringstream stream_tuple(cut_tuple);
				while (getline(stream_tuple, element, delim))
				{
					size_t found = element.find_last_of('/"');

					if (stream_tuple.eof()) // If Last word, go next line
					{
						// Directly writing in t0 file
						out << element.substr(0, found) + "_t0" + '\"' << endl;

						// Carefully adding delim for end of variable
						variable += (element.substr(0, found) + "" + '\"' + delim);
						variable_T += (element.substr(0, found) + "_t" + '\"');

						// Writting the variable and endline
						out_T << variable << variable_T << endl;
					}
					else // Else we write a delim character
					{
						// Directly writing in t0 file
						out << element.substr(0, found) + "_t0" + '\"' << delim;

						// Writting the t variable in either variable or variable_T
						variable += (element.substr(0, found) + "" + '\"' + delim);
						variable_T += (element.substr(0, found) + "_t" + '\"' + delim);
					}
					nVars++;
				}
				cout << nVars << endl;
			}
			else
			{ // If my sequence number isn't the same as in my
				if (sequence_number != memory_sequence)
				{
					// Store in memory my number of sequence
					memory_sequence = sequence_number;
					out << cut_tuple << endl;
					prev_tuple = cut_tuple;
				}
				else
				{
					out_T << prev_tuple << delim << cut_tuple << endl;
					prev_tuple = cut_tuple;
				}
			}
			iterator++;
		}
		myFile.close();
	}
	else
		cerr << "ERROR: failing to open the file" << endl;
}

void pmUtility::generatePMMixedDAG(pmGraph *g,
								   unsigned int nbNodes,
								   unsigned int maxInducedWidth,
								   unsigned int nbIterations,
								   unsigned int maxNodeDegree,
								   unsigned int maxNbEdges)
{

	prm::algo::PMMixed pmmixed;
	pmmixed.generateDAG(nbNodes, maxInducedWidth, nbIterations, maxNodeDegree, maxNbEdges);

	//Convert to pmGraph
	typedef boost::graph_traits<utils::DirectedGraph>::vertex_descriptor Vertex;

	boost::graph_traits<utils::DirectedGraph>::edge_iterator ei, ei_end;
	for (boost::tie(ei, ei_end) = boost::edges(pmmixed.graph); ei != ei_end; ++ei)
	{
		Vertex s = source(*ei, pmmixed.graph);
		Vertex t = target(*ei, pmmixed.graph);
		boost::add_edge(vertex(s, *g), vertex(t, *g), *g);
	}
}

//=============================================================================
void pmUtility::output_mixture(const plJointDistribution &mixture)
{
	std::cout << mixture.get_computable_object_list()[0] << std::endl; //Return the class distribution list
	std::cout << mixture.get_computable_object_list()[1] << std::endl; //Return X's distribution list
}


//=============================================================================
// float pmUtility::estimate_n_gaussians(int num_gaussians, plVariablesConjunction &var_continu, vector<int> &itterateur)
// {
// 	float bic;
// 	// EM Initial distribution on the class (kernel) variable: P(C)
// 	// num_gaussians++;
// 	const plVariable C("C_" + var_continu.name() + "AB", plIntegerType(0, num_gaussians - 1));
// 	const bool random_prob = true;
// 	const plProbTable pc_init(C, random_prob);
// 	plError e;
// 	e.ignore_this_message(122, true);
// 	// EM Initial Gaussians : P(var_continu | C)
// 	plDistributionTable px_init(var_continu, C);
// 	for (i = 0; i < num_gaussians; ++i)
// 		px_init.push(plNormal(var_continu[0], min + (i + (1 / 2)) * ((max - min) / num_gaussians), (max - min) / 10), int(i)); //initialisation des gaussienes

// 	plLearnHistogram LC(C);
// 	plCndLearnObject<plLearn1dNormal> LX(var_continu, C); // P(var_continu | C) is learnt as a set of gaussians (a gaussian for each value of C)
	
// 	std::vector<plLearnObject *> learn_objs(2);
// 	learn_objs[0] = &LC;
// 	learn_objs[1] = &LX;

// 	plEMLearner myEM(pc_init * px_init, learn_objs); // Creating the EM learner instance
// 	myEM.run(mysecondCSVdata, 0.0001); // Run until convergence
// 	nparams = myEM.get_n_parameters(); // Fill the output parameters

// 	llk = myEM.compute_loglikelihood(mysecondCSVdata);
// 	bic = llk - 0.5 * nparams * std::log(float(mysecondCSVdata.get_n_records()));
// 	model_temp = myEM.get_joint_distribution(); // create the model mixture
// 	// getting the mus & sigmas
// 	plDistributionTable temp(var_continu[0], C);
// 	plCndDistribution pxc(model_temp.get_computable_object_list()[1]);
// 	plDistribution pxci;
// 	float sigma_modele, mu_modele;
// 	for (unsigned int i = 0; i < num_gaussians; ++i)
// 	{
// 		pxci = pxc.instantiate(plValues().add(C, i)); //P(var_continu|c=i)
// 		mu_modele = plNormal(pxci).mean();
// 		sigma_modele = plNormal(pxci).standard_deviation();
// 		mu.push_back(mu_modele); // get the mean
// 		if (sigma_modele > std::numeric_limits<float>::epsilon())
// 			sig.push_back(sigma_modele); // get sigma
// 		else
// 		{
// 			sig.push_back(std::numeric_limits<float>::epsilon());
// 			sigma_modele = std::numeric_limits<float>::epsilon();
// 		}
// 		temp.push(plNormal(var_continu[0], mu_modele, sigma_modele), int(i));
// 		itterateur.push_back(i);
// 	}

// 	return bic;
// }

// void kmeans(int K, int total_points, int total_values, int max_iterations)
// {
// 	std::vector<int> prohibited_indexes;

// 	// initialization : choose K distinct values for the centers of the clusters
// 	for(int i = 0; i < K; i++)
// 	{
// 		while(true)
// 		{
// 			int index_point = rand() % total_points;

// 			if(find(prohibited_indexes.begin(), prohibited_indexes.end(),
// 					index_point) == prohibited_indexes.end())
// 			{
// 				prohibited_indexes.push_back(index_point);
// 				points[index_point].setCluster(i);
// 				Cluster cluster(i, points[index_point]);
// 				clusters.push_back(cluster);
// 				break;
// 			}
// 		}
// 	}

// 	int iter = 1;

// 	while(true)
// 	{
// 		bool done = true;

// 		// associates each point to the nearest center
// 		for(int i = 0; i < total_points; i++)
// 		{
// 			int id_old_cluster = points[i].getCluster();
// 			int id_nearest_center = getIDNearestCenter(points[i]);

// 			if(id_old_cluster != id_nearest_center)
// 			{
// 				if(id_old_cluster != -1)
// 					clusters[id_old_cluster].removePoint(points[i].getID());

// 				points[i].setCluster(id_nearest_center);
// 				clusters[id_nearest_center].addPoint(points[i]);
// 				done = false;
// 			}
// 		}

// 		// recalculating the center of each cluster
// 		for(int i = 0; i < K; i++)
// 		{
// 			for(int j = 0; j < total_values; j++)
// 			{
// 				int total_points_cluster = clusters[i].getTotalPoints();
// 				double sum = 0.0;

// 				if(total_points_cluster > 0)
// 				{
// 					for(int p = 0; p < total_points_cluster; p++)
// 						sum += clusters[i].getPoint(p).getValue(j);
// 					clusters[i].setCentralValue(j, sum / total_points_cluster);
// 				}
// 			}
// 		}

// 		if(done == true || iter >= max_iterations)
// 			break;

// 		iter++;
// 	}

// 	// shows elements of clusters
// 	for(int i = 0; i < K; i++)
// 	{
// 		int total_points_cluster =  clusters[i].getTotalPoints();

// 		// std::cout << "Cluster " << clusters[i].getID() + 1 << std::endl;
// 		// std::cout << "total nb points " << total_points_cluster + 1 << std::endl;

		
// 		// for(int j = 0; j < total_points_cluster; j++)
// 		// {
// 		// 	std::cout << "Point " << clusters[i].getPoint(j).getID() + 1 << ": ";
// 		// 	for(int p = 0; p < total_values; p++)
// 		// 		std::cout << clusters[i].getPoint(j).getValue(p) << " ";

// 		// 	std::string point_name = clusters[i].getPoint(j).getName();

// 		// 	if(point_name != "")
// 		// 		std::cout << "- " << point_name;

// 		// 	std::cout << std::endl;
// 		// }


// 		// std::cout << "Cluster values: ";
// 		// for(int j = 0; j < total_values; j++)
// 		// 	std::cout << clusters[i].getCentralValue(j) << " ";
// 		// std::cout << "\n\n";
// 	}
// }

// //=============================================================================
// vector<double> pmUtility::discretisation_kmeans(int range, string &file, string file_name, plJointDistribution &model, 
// 		plVariablesConjunction var_continu, bool modeVerbose /*=false */, std::pair<bool,int> force_gaussians)
// {
// 	int num_gaussians = force_gaussians.second;
// 	bool force_num_gaussians = force_gaussians.first;
// 	unsigned int nparams, i;
// 	double res = 0;
// 	float discriminant, bic, a, b, c, alpha = NULL, r1 = NULL, r2 = NULL;
// 	float bic_prev = NULL;
// 	double llk;
// 	// int num_gaussians = 1; // we start with one and increment untill optimizing a score
// 	double infinity_plus = std::numeric_limits<int>::max();
// 	double infinity_moins = -std::numeric_limits<int>::max();
// 	bool first = true;

// 	vector<double> valeur, tk, vect;
// 	vector<float> mu, sig, final_mu, final_sig, params, sol;
// 	vector<int> itterateur, it;
// 	plDistributionTable temp;
// 	std::vector<plFloat> vmin, vmax, column_vals;
// 	plJointDistribution &model_temp = model;

// 	ofstream fichier(file_name, ios::out | ios::app);

// 	//Open the CSVDataDescriptor to get the column values
// 	plCSVFileDataDescriptor myCSVdata(file, false);
// 	myCSVdata.get_column_values(range, column_vals);
// 	double max = *max_element(column_vals.begin() + 1, column_vals.end());
// 	double min = *min_element(column_vals.begin() + 1, column_vals.end());

// 	if (modeVerbose == true)
// 	{
// 		cout << "============== In column of variable of range :" << range << " and name : " << var_continu.name() << endl;
// 		cout << "Max value: " << max << endl;
// 		cout << "Min value: " << min << endl;
// 	}

// 	plCSVFileDataDescriptor mysecondCSVdata(file, var_continu, false);
// 	mysecondCSVdata.set_variables(var_continu);

// 	num_gaussians--;

// 	do
// 	{
// 		// bic = estimate_n_gaussians(num_gaussians, var_continu, itterateur);
// 		// EM Initial distribution on the class (kernel) variable: P(C)
// 		num_gaussians++;
// 		const plVariable C("C_" + var_continu.name() + "AB", plIntegerType(0, num_gaussians - 1));
// 		const bool random_prob = true;
// 		const plProbTable pc_init(C, random_prob);
// 		plError e;
// 		e.ignore_this_message(122, true);
// 		// EM Initial Gaussians : P(var_continu | C)
// 		plDistributionTable px_init(var_continu, C);
// 		for (i = 0; i < num_gaussians; ++i)
// 			px_init.push(plNormal(var_continu[0], min + (i + (1 / 2)) * ((max - min) / num_gaussians), (max - min) / 10), int(i)); //initialisation des gaussienes

// 		// plLearnHistogram LC(C);
// 		plLearnLaplace LC(C);
// 		plCndLearnObject<plLearn1dNormal> LX(var_continu, C); // P(var_continu | C) is learnt as a set of gaussians (a gaussian for each value of C)

// 		std::vector<plLearnObject *> learn_objs(2);
// 		learn_objs[0] = &LC;
// 		learn_objs[1] = &LX;

// 		plEMLearner myEM(pc_init * px_init, learn_objs); // Creating the EM learner instance
// 		myEM.run(mysecondCSVdata, 0.0001); // Run until convergence
// 		nparams = myEM.get_n_parameters(); // Fill the output parameters

// 		llk = myEM.compute_loglikelihood(mysecondCSVdata);
// 		bic = llk - 0.5 * nparams * std::log(float(mysecondCSVdata.get_n_records()));
// 		model_temp = myEM.get_joint_distribution(); // create the model mixture
// 		// getting the mus & sigmas
// 		plDistributionTable temp(var_continu[0], C);
// 		plCndDistribution pxc(model_temp.get_computable_object_list()[1]);
// 		plDistribution pxci;
// 		float sigma_modele, mu_modele;
// 		for (unsigned int i = 0; i < num_gaussians; ++i)
// 		{
// 			pxci = pxc.instantiate(plValues().add(C, i)); //P(var_continu|c=i)
// 			mu_modele = plNormal(pxci).mean();
// 			sigma_modele = plNormal(pxci).standard_deviation();
// 			mu.push_back(mu_modele); // get the mean
// 			if (sigma_modele > std::numeric_limits<float>::epsilon())
// 				sig.push_back(sigma_modele); // get sigma
// 			else
// 			{
// 				sig.push_back(std::numeric_limits<float>::epsilon());
// 				sigma_modele = std::numeric_limits<float>::epsilon();
// 			}
// 			temp.push(plNormal(var_continu[0], mu_modele, sigma_modele), int(i));
// 			itterateur.push_back(i);
// 		}

// 		// Gaussian model that we can ommit cause that doesnt really interest us
// 		model.replace(var_continu[0], C, temp);
// 		if (bic_prev == NULL || bic > bic_prev)
// 		{
// 			bic_prev = bic;
// 			final_mu = mu;
// 			final_sig = sig;
// 			it = itterateur;
// 			itterateur.clear();
// 			model = model_temp;
// 			model_temp.replace(var_continu[0], C, temp);
// 			mu = {};
// 			sig = {};
// 		}
// 	} while ((bic_prev <= bic) and (force_num_gaussians == false));

// 	// num_gaussians--;
// 	if (modeVerbose == true)
// 	{
// 		for (i = 0; i < final_mu.size(); i++)
// 		{
// 			cout << "[mu" << i << "] =" << final_mu[i] << endl;
// 		}
// 		for (i = 0; i < final_sig.size(); i++)
// 		{
// 			cout << "sig[" << i << "] =" << final_sig[i] << endl;
// 		}
// 		cout << "Optimal gaussian number is : " << num_gaussians << endl;
// 	}

// 	i = 0;
// 	int j = 0;
// 	int tmp = 0;
// 	for (i = 0; i < final_mu.size(); i++)
// 	{
// 		for (j = i; j < final_mu.size(); j++)
// 		{
// 			if (final_mu[it[j]] < final_mu[it[i]])
// 			{
// 				tmp = it[i];
// 				it[i] = it[j];
// 				it[j] = tmp;
// 			}
// 		}
// 	}

// 	//================================================= Computing Cutpoints ==============================//
// 	// descriminant
// 	for (unsigned int i = 0; i < num_gaussians - 1; ++i)
// 	{

// 		c = (pow(final_mu[it[i]], 2) / pow(final_sig[it[i]], 2)) - (pow(final_mu[it[i + 1]], 2) / pow(final_sig[it[i + 1]], 2)) - 2 * log(final_sig[it[i + 1]] / final_sig[it[i]]);
// 		b = 2 * ((final_mu[it[i + 1]] / pow(final_sig[it[i + 1]], 2)) - (final_mu[it[i]] / pow(final_sig[it[i]], 2)));
// 		a = (1 / pow(final_sig[it[i]], 2)) - (1 / pow(final_sig[it[i + 1]], 2));

// 		discriminant = ((b * b) - 4 * a * c);
// 		if (discriminant > 0)
// 		{
// 			r1 = (-1 * b + sqrt(discriminant)) / (2 * a);
// 			r2 = (-1 * b - sqrt(discriminant)) / (2 * a);
// 			if (r1 > final_mu[it[i]] && r1 < final_mu[it[i + 1]])
// 			{
// 				alpha = r1;
// 			}
// 			if (r2 > final_mu[it[i]] && r2 < final_mu[it[i + 1]])
// 			{
// 				alpha = r2;
// 			}
// 		}
// 		if (discriminant == 0)
// 		{
// 			r1 = alpha = (-1 * b) / (2 * a);
// 		}

// 		//Params vetor of mus and sigmas
// 		params.clear();
// 		params.push_back(final_mu[it[i]]);
// 		params.push_back(final_mu[it[i + 1]]);

// 		if (alpha != NULL && alpha >= final_mu[it[i]] && alpha <= final_mu[it[i + 1]])
// 		{
// 			params.push_back(alpha);
// 		}

// 		sol.clear();
// 		for (unsigned ii = 0; ii < params.size(); ii++)
// 		{
// 			//calculate the phi function and add alpha if it exists
// 			res = 1 + plPhi((params[ii] - final_mu[it[i + 1]]) / final_sig[it[i + 1]]) - plPhi((params[ii] - final_mu[it[i]]) / final_sig[it[i]]);
// 			sol.push_back(res);
// 		}

// 		//calculate the ArgMin
// 		tk.push_back(params[min_element(sol.begin(), sol.end()) - sol.begin()]);
// 		res = tk[i];
// 	}
// 	tk.insert(tk.begin(), infinity_moins);
// 	tk.push_back(infinity_plus);
// 	sort(tk.begin(), tk.end());
// 	fichier.close();
// 	tk.erase(unique(tk.begin(), tk.end()), tk.end());
// 	if (modeVerbose == true)
// 	{
// 		for (i = 0; i < tk.size(); i++)
// 			cout << "cutpoint[" << i << "]=" << tk[i] << endl;
// 	}
// 	return tk;
// }


//=============================================================================
vector<double> pmUtility::discretisation(int range, string &file, string file_name, plJointDistribution &model, 
		plVariablesConjunction var_continu, bool modeVerbose /*=false */, std::pair<bool,int> force_gaussians)
{
	int num_gaussians = force_gaussians.second;
	bool force_num_gaussians = force_gaussians.first;
	unsigned int nparams;
	double res = 0;
	float discriminant, bic, a, b, c, alpha = 0.0, r1 = 0.0, r2 = 0.0;
	float bic_prev = 0.0;
	double llk;
	// int num_gaussians = 1; // we start with one and increment untill optimizing a score
	double infinity_plus = std::numeric_limits<int>::max();
	double infinity_moins = -std::numeric_limits<int>::max();
	bool first = true;

	vector<double> valeur, tk, vect;
	vector<float> mu, sig, final_mu, final_sig, params, sol;
	vector<int> itterateur, it;
	plDistributionTable temp;
	// vector<int> it;
	std::vector<plFloat> vmin, vmax, column_vals;
	plJointDistribution &model_temp = model;

	ofstream fichier(file_name, ios::out | ios::app);

	//Open the CSVDataDescriptor to get the column values
	plCSVFileDataDescriptor myCSVdata(file, false);
	myCSVdata.get_column_values(range, column_vals);
	double max = *max_element(column_vals.begin() + 1, column_vals.end());
	double min = *min_element(column_vals.begin() + 1, column_vals.end());

	if (modeVerbose == true)
	{
		cout << "============== In column of variable of range :" << range << " and name : " << var_continu.name() << endl;
		cout << "Max value: " << max << endl;
		cout << "Min value: " << min << endl;
	}

	plCSVFileDataDescriptor mysecondCSVdata(file, var_continu, false);
	mysecondCSVdata.set_variables(var_continu);

	num_gaussians--;

	do
	{
		// bic = estimate_n_gaussians(num_gaussians, var_continu, itterateur);
		// EM Initial distribution on the class (kernel) variable: P(C)
		num_gaussians++;
		const plVariable C("C_" + var_continu.name() + "AB", plIntegerType(0, num_gaussians - 1));
		const bool random_prob = true;
		const plProbTable pc_init(C, random_prob);
		plError e;
		e.ignore_this_message(122, true);
		// EM Initial Gaussians : P(var_continu | C)
		plDistributionTable px_init(var_continu, C);
		for (int i = 0; i < num_gaussians; ++i)
			px_init.push(plNormal(var_continu[0], min + (i + (1 / 2)) * ((max - min) / num_gaussians), (max - min) / 10), int(i)); //initialisation des gaussienes

		// plLearnHistogram LC(C);
		plLearnLaplace LC(C);
		plCndLearnObject<plLearn1dNormal> LX(var_continu, C); // P(var_continu | C) is learnt as a set of gaussians (a gaussian for each value of C)

		std::vector<plLearnObject *> learn_objs(2);
		learn_objs[0] = &LC;
		learn_objs[1] = &LX;

		plEMLearner myEM(pc_init * px_init, learn_objs); // Creating the EM learner instance
		myEM.run(mysecondCSVdata, 0.0001); // Run until convergence
		nparams = myEM.get_n_parameters(); // Fill the output parameters

		llk = myEM.compute_loglikelihood(mysecondCSVdata);
		bic = llk - 0.5 * nparams * std::log(float(mysecondCSVdata.get_n_records()));
		model_temp = myEM.get_joint_distribution(); // create the model mixture
		// getting the mus & sigmas
		plDistributionTable temp(var_continu[0], C);
		plCndDistribution pxc(model_temp.get_computable_object_list()[1]);
		plDistribution pxci;
		float sigma_modele, mu_modele;
		for (unsigned int i = 0; i < num_gaussians; ++i)
		{
			pxci = pxc.instantiate(plValues().add(C, i)); //P(var_continu|c=i)
			mu_modele = plNormal(pxci).mean();
			sigma_modele = plNormal(pxci).standard_deviation();
			mu.push_back(mu_modele); // get the mean
			if (sigma_modele > std::numeric_limits<float>::epsilon())
				sig.push_back(sigma_modele); // get sigma
			else
			{
				sig.push_back(std::numeric_limits<float>::epsilon());
				sigma_modele = std::numeric_limits<float>::epsilon();
			}
			temp.push(plNormal(var_continu[0], mu_modele, sigma_modele), int(i));
			itterateur.push_back(i);
		}

		// Gaussian model that we can ommit cause that doesnt really interest us
		model.replace(var_continu[0], C, temp);
		if (bic_prev == 0.0 || bic > bic_prev)
		{
			bic_prev = bic;
			final_mu = mu;
			final_sig = sig;
			it = itterateur;
			itterateur.clear();
			model = model_temp;
			model_temp.replace(var_continu[0], C, temp);
			mu = {};
			sig = {};
		}
	} while ((bic_prev <= bic) and (force_num_gaussians == false));

	// num_gaussians--;
	if (modeVerbose == true)
	{
		for (size_t i = 0; i < final_mu.size(); i++)
		{
			cout << "[mu" << i << "] =" << final_mu[i] << endl;
		}
		for (size_t i = 0; i < final_sig.size(); i++)
		{
			cout << "sig[" << i << "] =" << final_sig[i] << endl;
		}
		cout << "Optimal gaussian number is : " << num_gaussians << endl;
	}

	int tmp = 0;
	for (size_t i = 0; i < final_mu.size(); i++)
	{
		for (size_t j = i; j < final_mu.size(); j++)
		{
			if (final_mu[it[j]] < final_mu[it[i]])
			{
				tmp = it[i];
				it[i] = it[j];
				it[j] = tmp;
			}
		}
	}

	//================================================= Computing Cutpoints ==============================//
	// descriminant
	for (size_t i = 0; i < num_gaussians - 1; ++i)
	{

		c = (pow(final_mu[it[i]], 2) / pow(final_sig[it[i]], 2)) - (pow(final_mu[it[i + 1]], 2) / pow(final_sig[it[i + 1]], 2)) - 2 * log(final_sig[it[i + 1]] / final_sig[it[i]]);
		b = 2 * ((final_mu[it[i + 1]] / pow(final_sig[it[i + 1]], 2)) - (final_mu[it[i]] / pow(final_sig[it[i]], 2)));
		a = (1 / pow(final_sig[it[i]], 2)) - (1 / pow(final_sig[it[i + 1]], 2));

		discriminant = ((b * b) - 4 * a * c);

		if (a == 0)
			r1 = alpha = (final_mu[it[i]] + final_mu[it[i + 1]]) / 2;

		else if (discriminant > 0)
		{
			r1 = (-1 * b + sqrt(discriminant)) / (2 * a);
			r2 = (-1 * b - sqrt(discriminant)) / (2 * a);
			if (r1 > final_mu[it[i]] && r1 < final_mu[it[i + 1]])
			{
				alpha = r1;
			}
			if (r2 > final_mu[it[i]] && r2 < final_mu[it[i + 1]])
			{
				alpha = r2;
			}
		}
		
		else if (discriminant == 0)
		{
			r1 = alpha = (-1 * b) / (2 * a);
		}

		//Params vetor of mus and sigmas
		params.clear();
		params.push_back(final_mu[it[i]]);
		params.push_back(final_mu[it[i + 1]]);

		if (alpha != NULL && alpha >= final_mu[it[i]] && alpha <= final_mu[it[i + 1]])
		{
			params.push_back(alpha);
		}

		sol.clear();
		for (unsigned ii = 0; ii < params.size(); ii++)
		{
			//calculate the phi function and add alpha if it exists
			res = 1 + plPhi((params[ii] - final_mu[it[i + 1]]) / final_sig[it[i + 1]]) - plPhi((params[ii] - final_mu[it[i]]) / final_sig[it[i]]);
			sol.push_back(res);
		}

		//calculate the ArgMin
		tk.push_back(params[min_element(sol.begin(), sol.end()) - sol.begin()]);
		res = tk[i];
	}
	tk.insert(tk.begin(), infinity_moins);
	tk.push_back(infinity_plus);
	sort(tk.begin(), tk.end());
	fichier.close();
	tk.erase(unique(tk.begin(), tk.end()), tk.end());
	if (modeVerbose == true)
	{
		for (size_t i = 0; i < tk.size(); i++)
			cout << "cutpoint[" << i << "]=" << tk[i] << endl;
	}
	return tk;
}
