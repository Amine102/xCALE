/*
 * T_HMM_simple.cpp
 *
 *  Created on: Feb 25, 2009
 *      Author: palu
 */

#include <iostream>
#include <stdlib.h>

#include "mocapy.h"
using namespace mocapy;
using namespace std;

int main() {

	//User-defined parameters
	double P_INITIAL = 0;
	double PA_GUESS = 0.1;
	double PB_GUESS = 0.3;
	double PC_GUESS = 0.2;
	double PA_SLIP = 0.15;
	double PB_SLIP = 0.3;
	double PC_SLIP = 0.2;
	double PA_LEARN = 0.5;
	double PB_LEARN = 0.8;
	double PC_LEARN = 0.7;
	double P_FORGET = 0;
	double P1 = 0.4;
	double P2 = 0.5;
	double P_LR = 0.6;
	double P_L2= 0.7;

	// T_HMM hidden and observed node dimensions (len of the domain)
	uint H_SIZE = 2;
	uint O_SIZE = 2;
	bool init_random = false;

	//=============== Definitions of slice1 nodes of the target DBN ===============
	// Define the CPDs of T_SkillA
	CPD T_SkillA_cpd;
	T_SkillA_cpd.set_shape(2);
	T_SkillA_cpd.set_values(vec(1-P_INITIAL, P_INITIAL));

	// Define the CPDs of T_SkillB
	CPD T_SkillB_cpd;
	T_SkillB_cpd.set_shape(2);
	T_SkillB_cpd.set_values(vec(1-P_INITIAL, P_INITIAL));

	// Define the CPDs of T_ZA
	CPD T_ZA_cpd;
	T_ZA_cpd.set_shape(2,2);
	T_ZA_cpd.set_values(vec(1.0, 0.0, 1-P1, P1));

	// Define the CPDs of T_ZB
	CPD T_ZB_cpd;
	T_ZB_cpd.set_shape(2,2);
	T_ZB_cpd.set_values(vec(1.0, 0.0, 1-P2, P2));

	// Define the CPDs of T_PRC
	CPD T_PRC_cpd(vec(2,2,2));
	// T_PRC_cpd.set_shape(2, 4);
	const std::vector<double> noisyOR = {1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0};
	T_PRC_cpd.set_values(noisyOR);

	// Define the CPDs of T_ZPRC
	CPD T_ZPRC_cpd;
	T_ZPRC_cpd.set_shape(2,2);
	T_ZPRC_cpd.set_values(vec(1.0, 0.0, 1-P_LR, P_LR));

	// Define the CPDs of T_ZSRC
	CPD T_ZSRC_cpd;
	T_ZSRC_cpd.set_shape(2);
	T_ZSRC_cpd.set_values(vec(1-P_INITIAL, P_INITIAL));

	// Define the CPDs of T_SkillC
	CPD T_SkillC_cpd(vec(2,2,2));
	// T_SkillC_cpd.set_shape(2, 4);
	const std::vector<double> noisyOR2 = {1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0};
	T_SkillC_cpd.set_values(noisyOR2);

	// Define the CPDs of T_ResA
	CPD T_ResA_cpd;
	T_ResA_cpd.set_shape(2,2);
	T_ResA_cpd.set_values(vec(1-PA_GUESS, PA_GUESS, PA_SLIP, 1-PA_SLIP));

	// Define the CPDs of T_ResB
	CPD T_ResB_cpd;
	T_ResB_cpd.set_shape(2,2);
	T_ResB_cpd.set_values(vec(1-PB_GUESS, PB_GUESS, PB_SLIP, 1-PB_SLIP));

	// Define the CPDs of T_ResC
	CPD T_ResC_cpd;
	T_ResC_cpd.set_shape(2,2);
	T_ResC_cpd.set_values(vec(1-PC_GUESS, PC_GUESS, PC_SLIP, 1-PC_SLIP));

  //=============== Definitions of slice2 nodes of the target DBN ===============
	// Define the CPDs of T_SkillA_t
	CPD T_SkillA_t_cpd;
	T_SkillA_t_cpd.set_shape(2,2);
	T_SkillA_t_cpd.set_values(vec(1-PA_LEARN, PA_LEARN, P_FORGET, 1-P_FORGET));

	// Define the CPDs of T_SkillB_t
	CPD T_SkillB_t_cpd;
	T_SkillB_t_cpd.set_shape(2,2);
	T_SkillB_t_cpd.set_values(vec(1-PB_LEARN, PB_LEARN, P_FORGET, 1-P_FORGET));

	// Define the CPDs of T_ZSRC_t
	CPD T_ZSRC_t_cpd;
	T_ZSRC_t_cpd.set_shape(2,2);
	T_ZSRC_t_cpd.set_values(vec(1-P_L2, P_L2, 0.0, 1.0));


	// Define the Nodes of the target DBN
	Node* T_SkillA = NodeFactory::new_discrete_node(H_SIZE, "T_SkillA", init_random, T_SkillA_cpd);
	Node* T_SkillB = NodeFactory::new_discrete_node(H_SIZE, "T_SkillB", init_random, T_SkillB_cpd);
	Node* T_SkillC = NodeFactory::new_discrete_node(H_SIZE, "T_SkillC", init_random, T_SkillC_cpd);
	Node* T_ZA = NodeFactory::new_discrete_node(H_SIZE, "T_ZA", init_random, T_ZA_cpd);
	Node* T_ZB = NodeFactory::new_discrete_node(H_SIZE, "T_ZB", init_random, T_ZB_cpd);
	Node* T_PRC = NodeFactory::new_discrete_node(H_SIZE, "T_PRC", init_random, T_PRC_cpd);
	Node* T_ZPRC = NodeFactory::new_discrete_node(H_SIZE, "T_ZPRC", init_random, T_ZPRC_cpd);
	Node* T_ZSRC = NodeFactory::new_discrete_node(H_SIZE, "T_ZSRC", init_random, T_ZSRC_cpd);
	Node* T_ResA = NodeFactory::new_discrete_node(O_SIZE, "T_ResA", init_random, T_ResA_cpd);
	Node* T_ResB = NodeFactory::new_discrete_node(O_SIZE, "T_ResB", init_random, T_ResB_cpd);
	Node* T_ResC = NodeFactory::new_discrete_node(O_SIZE, "T_ResC", init_random, T_ResC_cpd);

	Node* T_SkillA_t = NodeFactory::new_discrete_node(H_SIZE, "T_SkillA_t", init_random, T_SkillA_t_cpd);
	Node* T_SkillB_t = NodeFactory::new_discrete_node(H_SIZE, "T_SkillB_t", init_random, T_SkillB_t_cpd);
	Node* T_ZSRC_t = NodeFactory::new_discrete_node(H_SIZE, "T_ZSRC_t", init_random, T_ZSRC_t_cpd);
	
	// Create the target DBN(which will generate the data)

  DBN T_HMM;
	const std::vector<Node*> T_nodes = { T_SkillA, T_ResA, T_SkillB, T_ResB, T_ZA, T_ZB, T_PRC, T_ZPRC, T_ZSRC, T_SkillC, T_ResC };
	const std::vector<Node*> T_nodes_t = { T_SkillA_t, T_ResA, T_SkillB_t, T_ResB, T_ZA, T_ZB, T_PRC, T_ZPRC, T_ZSRC_t, T_SkillC, T_ResC };
	T_HMM.set_slices(T_nodes, T_nodes_t);

	T_HMM.add_intra("T_SkillA", "T_ResA");
	T_HMM.add_intra("T_SkillA", "T_ZA");
	T_HMM.add_intra("T_ZA","T_PRC");
	T_HMM.add_intra("T_SkillB", "T_ResB");
	T_HMM.add_intra("T_SkillB",  "T_ZB");
	T_HMM.add_intra("T_ZB","T_PRC");
	T_HMM.add_intra("T_PRC", "T_ZPRC");
	T_HMM.add_intra("T_ZPRC","T_SkillC");
	T_HMM.add_intra("T_ZSRC","T_SkillC");
	T_HMM.add_intra("T_SkillC","T_ResC");

	T_HMM.add_inter("T_SkillA","T_SkillA_t");
	T_HMM.add_inter("T_SkillB","T_SkillB_t");
	T_HMM.add_inter("T_SkillC","T_ZSRC_t");
  T_HMM.construct();
	std::cout << "debug" << std::endl;

	cout << "=============== My variable tables of the target DBN ===============" << endl;
	cout << *T_SkillA << endl;
	cout << *T_ResA << endl;
	cout << *T_SkillB << endl;
	cout << *T_ResB << endl;
	cout << *T_ZA << endl;
	cout << *T_ZB << endl;
	cout << *T_PRC << endl;
	cout << *T_ZPRC << endl;
	cout << *T_ZSRC << endl;
	cout << *T_SkillC << endl;
	cout << *T_ResC << endl;

	cout << *T_SkillA_t << endl;
	cout << *T_SkillB_t << endl;
	cout << *T_ZSRC_t << endl;

	// Define the Nodes of the model DBN
	// For M_SkillA, get the CPD from T_SkillA and fix parameters
	Node* M_SkillA = NodeFactory::new_discrete_node(H_SIZE, "M_SkillA", init_random);
	Node* M_SkillB = NodeFactory::new_discrete_node(H_SIZE, "M_SkillB", init_random);
	Node* M_SkillC = NodeFactory::new_discrete_node(H_SIZE, "M_SkillC", init_random, CPD(), T_SkillC, true);
	Node* M_ZA = NodeFactory::new_discrete_node(H_SIZE, "M_ZA", init_random, T_ZA_cpd);
	Node* M_ZB = NodeFactory::new_discrete_node(H_SIZE, "M_ZB", init_random, T_ZB_cpd);
	Node* M_PRC = NodeFactory::new_discrete_node(H_SIZE, "M_PRC", init_random, CPD(), T_PRC, true);
	Node* M_ZPRC = NodeFactory::new_discrete_node(H_SIZE, "M_ZPRC", init_random, T_ZPRC_cpd);
	Node* M_ZSRC = NodeFactory::new_discrete_node(H_SIZE, "M_ZSRC", init_random, T_ZSRC_cpd);
	Node* M_ResA = NodeFactory::new_discrete_node(O_SIZE, "M_ResA", init_random);
	Node* M_ResB = NodeFactory::new_discrete_node(O_SIZE, "M_ResB", init_random);
	Node* M_ResC = NodeFactory::new_discrete_node(O_SIZE, "M_ResC", init_random, T_ResC_cpd);

	Node* M_SkillA_t = NodeFactory::new_discrete_node(H_SIZE, "M_SkillA_t", init_random);
	Node* M_SkillB_t = NodeFactory::new_discrete_node(H_SIZE, "M_SkillB_t", init_random);
	Node* M_ZSRC_t = NodeFactory::new_discrete_node(H_SIZE, "M_ZSRC_t", init_random, T_ZSRC_t_cpd);

	// Create the model DBN(which will be trained with the generate data)

  DBN M_HMM;
	const std::vector<Node*> M_nodes = { M_SkillA, M_ResA, M_SkillB, M_ResB, M_ZA, M_ZB, M_PRC, M_ZPRC, M_ZSRC, M_SkillC, M_ResC };
	const std::vector<Node*> M_nodes_t = { M_SkillA_t, M_ResA, M_SkillB_t, M_ResB, M_ZA, M_ZB, M_PRC, M_ZPRC, M_ZSRC_t, M_SkillC, M_ResC };
	M_HMM.set_slices(M_nodes, M_nodes_t);

	M_HMM.add_intra("M_SkillA", "M_ResA");
	M_HMM.add_intra("M_SkillA", "M_ZA");
	M_HMM.add_intra("M_ZA","M_PRC");
	M_HMM.add_intra("M_SkillB", "M_ResB");
	M_HMM.add_intra("M_SkillB", "M_ZB");
	M_HMM.add_intra("M_ZB","M_PRC");
	M_HMM.add_intra("M_PRC", "M_ZPRC");
	M_HMM.add_intra("M_ZPRC","M_SkillC");
	M_HMM.add_intra("M_ZSRC","M_SkillC");
	M_HMM.add_intra("M_SkillC","M_ResC");

	M_HMM.add_inter("M_SkillA","M_SkillA_t");
	M_HMM.add_inter("M_SkillB","M_SkillB_t");
	M_HMM.add_inter("M_SkillC","M_ZSRC_t");
  M_HMM.construct();

	cout << "=============== My variable tables of the model DBN ===============" << endl;
	cout << *M_SkillA << endl;
	cout << *M_ResA << endl;
	cout << *M_SkillB << endl;
	cout << *M_ResB << endl;
	cout << *M_ZA << endl;
	cout << *M_ZB << endl;
	cout << *M_PRC << endl;
	cout << *M_ZPRC << endl;
	cout << *M_ZSRC << endl;
	cout << *M_SkillC << endl;
	cout << *M_ResC << endl;

	cout << *M_SkillA_t << endl;
    cout << *M_SkillB_t << endl;
	cout << *M_ZSRC_t << endl;


	// Generate the data
	int nb_seq = 100;       // number of training sequences
	int seq_len = 1000;      // sequences lengths

	cout << "==================== Generating data ====================" << endl;

	vector<Sequence> seq_list;
	vector< MDArray<eMISMASK> > mismask_list;

	// Creating my missing value mask (no missing here)
	const std::vector<eMISMASK> mismask_va = { MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED, MOCAPY_OBSERVED };
	MDArray<eMISMASK> mismask;
	mismask.repeat(seq_len, mismask_va);

	double sum_LL(0);
	for (int i=0; i<nb_seq; i++) {
		pair<Sequence, double>  seq_ll = T_HMM.sample_sequence(seq_len);
		sum_LL += seq_ll.second;
		seq_list.push_back(seq_ll.first);
		mismask_list.push_back(mismask);
	}
	cout << "Average LL: " << sum_LL/nb_seq << endl;	

	// Start EM loop    
	GibbsRandom mcmc = GibbsRandom(&M_HMM);
	EMEngine em = EMEngine(&M_HMM, &mcmc, &seq_list, &mismask_list); // cout the number of sequences and mismasks

	InfEngineMCMC inf = InfEngineMCMC(&M_HMM, &mcmc, &(seq_list[0]), mismask_list[0]);
	inf.initialize_viterbi_generator(5, 5);

	for (uint i=0; i<6; i++) {
		Sample s = inf.viterbi_next();
		cout << "Viterbi LL= " << s.ll << endl;
	}
	cout << "Ending Viterbi" << endl;

	cout << "=============== Starting EM loop ===============" << endl;
	double bestLL=-1000;
    int MCMC_BURN_IN = 10; // Gibbs sampling parameters
	uint it_no_improvement(0);
	uint i(0);
	while (it_no_improvement<100) {
	em.do_E_step(1, MCMC_BURN_IN, false);

	double ll = em.get_loglik();

	cout << "LL= " << ll;

	if (ll > bestLL) {
		cout << " * saving model *" << endl;
		M_HMM.save("create_M_HMM.dbn");
		bestLL = ll;
		it_no_improvement=0;
	}
	else { it_no_improvement++; cout << endl; }
	cout << it_no_improvement << endl;

	i++;
	em.do_M_step();
}

	cout << "The number of iteration:" << i << endl;
M_HMM.load("create_M_HMM.dbn");

	// Output the results
	cout << "=============== My variable tables of the target DBN ===============" << endl;
	cout << *T_SkillA << endl;
	cout << *T_ResA << endl;
	cout << *T_SkillB << endl;
	cout << *T_ResB << endl;
	cout << *T_ZA << endl;
	cout << *T_ZB << endl;
	cout << *T_PRC << endl;
	cout << *T_ZPRC << endl;
	cout << *T_ZSRC << endl;
	cout << *T_SkillC << endl;
	cout << *T_ResC << endl;

	cout << *T_SkillA_t << endl;
	cout << *T_SkillB_t << endl;
	cout << *T_ZSRC_t << endl;

	cout << "=============== My variable tables of the model DBN ===============" << endl;
	cout << *M_SkillA << endl;
	cout << *M_ResA << endl;
	cout << *M_SkillB << endl;
	cout << *M_ResB << endl;
	cout << *M_ZA << endl;
	cout << *M_ZB << endl;
	cout << *M_PRC << endl;
	cout << *M_ZPRC << endl;
	cout << *M_ZSRC << endl;
	cout << *M_SkillC << endl;
	cout << *M_ResC << endl;

	cout << *M_SkillA_t << endl;
  cout << *M_SkillB_t << endl;
	cout << *M_ZSRC_t << endl;
}