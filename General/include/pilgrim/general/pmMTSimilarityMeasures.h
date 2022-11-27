#ifndef _LoadMTSimilarity_H
#define _LoadMTSimilarity_H
#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include "pilgrim/general/pmMTCSVDataSet.h"
#include "pilgrim/general/scores/computeMI.h"


struct pmDataRef {
	int index;
	int v1; int v2;
	std::vector<unsigned int> subset;
	bool indep;
	float MI;
	float p_value;
	unsigned int df;
	float sc_value;
	float sl_value;
	int mostsim_task;
};

namespace PILGRIM {
	class pmMTSimilarityMeasures {
	public:
		/*=====Attributs=====*/
		std::vector<std::vector<bool>> indep_mat; // matrice to store independence test result between all pairs of variables for each data
		std::vector<int> glob_sim_vec; // 1d vector to store upper triangular matrix of global similarity between all pairs of data
		float comb_indep; // to return value of combined independence measure
		float comb_assoc; // to return value of combined association measure
		bool comb_indep_test;
		bool comb_assoc_test;
		pmDataRef target_task; // specificly to store single task independence test and association measure
		//std::vector<pmMI*> mt_pmMI;

		/*
		=====CONSTRUCTORS=====
		*/
		pmMTSimilarityMeasures(pmMTCSVDataSet& data_sets);
		/*
		=====DESTRUCTOR=====
		*/
		//~pmMTSimilarityMeasures(void);
		/*
		=====PUSHERS=====
		*/
		// Push all pmMI objects
		void push_pmMI(int data_indx, pmMTCSVDataSet& data_sets);
		// Push row vector of independence test result between all pairs of variables of data with data_indx into indep_mat
		void push_indep_pairs(int data_indx, pmMTCSVDataSet& data_sets);
		// Push global similarity measure between data1 and data2
		void push_global_similarity(int data_indx1, int data_indx2, pmMTCSVDataSet& data_sets);
		// Compute degree of freedom
		int compute_df(int data_index, int var1, int var2, std::vector<unsigned int> subset, pmMTCSVDataSet& data_sets);
		/*
		=====GETTERS=====
		*/
		// Get indepence matrice
		std::vector<std::vector<bool>> get_indep_mat();
		// Get vector of all global similarities;
		std::vector<int> get_globsim_vec();
		// Get value of global similarity between two specific datasets;
		int get_globsim(int data_indx1, int data_indx2);
		// Get combined independence measure
		float get_comb_indep();
		// Get combined association measure
		float get_comb_assoc();
		/*
		=====PRINTERS=====
		*/
		// Print indep_mat
		void print_indep_mat();
		// Print indep matrice for one data
		void print_single_indepmat(int data_indx, int nb_var);
		// Print global similarity matrix between all datasets
		void print_globsim_mat();
		// Print global similairty vector
		void print_globsim_vec();
		/*
		=====UTILS AND INIT=====
		*/
		// Get element corresponding to i , j position in upper triangular matrix
		int triangular_to_row(int i, int j, int dim);
		// Sign of I where: if sgn(I) = +1 when independence test is positive (I(X,Y|S) = true) and -1 otherwise
		int sgn(bool indep);
		// Init structure that will be passed as an argument in similarity measures with data index, var 1 and 2 indexes, and subset vector
		pmDataRef init_dataref_struct(int index, int var1, int var2, std::vector<unsigned int> subset, pmMTCSVDataSet& data_sets);
		/*
		=====PROTECTED METHODS=====
		*/
		// Local similarity between data_ref and data_aux
		void compute_local_similarity(pmDataRef data_ref, pmDataRef* data_aux, pmMTCSVDataSet& data_sets);
		// Most similar task using compute_local_similarity and glob_sim_vec
		pmDataRef most_similar_task(pmDataRef data_ref, pmMTCSVDataSet& data_sets);
		// Confidence measure
		float confidence_measure(pmDataRef data_ref, pmMTCSVDataSet& data_sets);
		// Combined independent measures (Combined independence function and combined association measure)
		void combined_measures(int index, int var1, int var2, std::vector<unsigned int> subset, pmMTCSVDataSet& data_sets);
	};
};
#endif
