/*==============================================================================
 * Product : PILGRIM
 * File : pmMTCSVDataSet.h
 * Author :
 * Creation : January 2021
 * WARINING /!\ : STATUS ON PROGRESS
  *=============================================================================
 * (c) Copyright 2013, PILGRIM - all rights reserved
 *==============================================================================
 */
#ifndef _LoadMTDataSet_H
#define _LoadMTDataSet_H
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstring>
#include "pilgrim/general/pmCommonTypeDefinitions.h"
#include "pilgrim/general/pmCSVDataSet.h"

namespace PILGRIM {
	class pmMTCSVDataSet {
	public:
		/*=====Attributs=====*/
		std::vector<pmCSVDataSet*> all_data_vec; // vector of all data sets
		std::vector<std::string> all_vars_vec; // vector of all variables names within all data sets
		std::vector<std::vector<int>> indx_mat; // matrix with variable position within data sets (to be read like this: variable 1 is in dataset1 and dataset2)
		/*
		=====CONSTRUCTORS=====
		*/
		pmMTCSVDataSet(std::vector<std::string> file_names);
		/*
		=====DESTRUCTOR=====
		*/
		//~pmMTCSVDataSet(void);
		/*
		=====PUSHERS=====
		*/
		// Push new data into the datasets vector all_data_vec (without sorting and droping duplicates)
		void push_data(std::string file_name);
		// Push new vars into the variable vector all_vars_vec
		void push_vars(int data_index);
		/*
		=====GETTERS=====
		*/
		// Get number of datasets (ie tasks);
		size_t get_nb_datasets();
		// Get number of all variables in all datasets
		size_t get_nb_variables();
		// Get all names of variables in all datasets, unique and sorted
		std::vector<std::string> get_all_varnames();
		// Get variables positions within datasets
		std::vector<std::vector<int>> get_index_matrix();
		// Get variable index for a specific dataset
		int get_var_index(int data_indx, int var_indx);
		/*
		=====PRINTERS=====
		*/
		// Print all variable names
		void print_varnames();
		// Print Index_matrice
		void print_var_positions();
		/*
		=====UTILS=====
		*/
		// Sort and drop duplicates of all_vars_vec vector
		void sort_drop_var_duplicates();
		/*
		=====PROTECTED ATTRIBUTES AND INIT METHOD====
		*/
		// Get variables positions in each data set
		void localize_variables();
	};
};
#endif
