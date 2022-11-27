// WARINING /!\	  : STATUS ON PROGRESS

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include "pilgrim/general/pmMTCSVDataSet.h"
#include "plVariablesConjunction.h"

using namespace std;
using namespace PILGRIM;

/*=====CONSTRUCTORS=====*/

pmMTCSVDataSet::pmMTCSVDataSet(std::vector<std::string> file_names) {

	//number of input data files
	int n_datasets = file_names.size();

	//Push each data into vector all_data_vec
	for (int i = 0; i < n_datasets; i++) {
		push_data(file_names[i]);
	}

	// Push for each data variable names into vector all_var_vec then sort it and drop duplicates
	for (int i = 0; i < n_datasets; i++) {
		push_vars(i);
		sort_drop_var_duplicates();
	}

	// Localise variables positions within data
	localize_variables();
}

/*=====PUSHERS=====*/

void pmMTCSVDataSet::push_data(std::string file_name) {

	// convert file_name string to char*
	char* file_data = new char[file_name.length() + 1];
	strcpy(file_data, file_name.c_str());

	pmCSVDataSet* ds = new pmCSVDataSet(file_data);
	all_data_vec.push_back(ds);
}

void pmMTCSVDataSet::push_vars(int data_index) {

	plVariablesConjunction obs_vars = all_data_vec[data_index]->observed_variables();
	for (size_t i = 0; i < obs_vars.get_names().size(); i++) {
		all_vars_vec.push_back(obs_vars.get_names()[i]);
	}

}

/*=====PRINTERS=====*/

// Print all variable names
void pmMTCSVDataSet::print_varnames() {
	for (int i = 0; i < all_vars_vec.size(); i++) {
		cout << "Variable " << i << " : " << all_vars_vec[i] << endl;
	}
}

// Print Index_matrice
void pmMTCSVDataSet::print_var_positions() {
	cout << "\t";
	for (int i = 0; i < all_vars_vec.size(); i++) {
		cout << all_vars_vec[i] << " ";
	}
	cout << endl;
	for (int i = 0; i < all_data_vec.size(); i++) {
		cout << "data" << i << "\t";
		for (int j = 0; j < all_vars_vec.size(); j++) {
			cout << indx_mat[i][j] << "\t";
		}
		cout << endl;
	}

}

/*=====GETTERS=====*/

// Get the number of datasets
size_t pmMTCSVDataSet::get_nb_datasets() {
	return all_data_vec.size();
}

// Get the number of variables with all datasets (unique and sorted)
size_t pmMTCSVDataSet::get_nb_variables() {
	return all_vars_vec.size();
}

// Get all names of variables in all datasets, unique and sorted
std::vector<std::string> pmMTCSVDataSet::get_all_varnames() {
	return all_vars_vec;
}

// Get variables positions within datasets
std::vector<std::vector<int>> pmMTCSVDataSet::get_index_matrix() {
	return indx_mat;
}

// Get corresponding variable index for a specific dataset
int pmMTCSVDataSet::get_var_index(int data_indx, int var_indx) {

	std::vector<int>::iterator itr = std::find(indx_mat[data_indx].begin(), indx_mat[data_indx].end(), var_indx);

	if (itr != indx_mat[data_indx].cend()) {
		return std::distance(indx_mat[data_indx].begin(), itr);
	}
	else {
		return -1;
	}

}

/*=====UTILS=====*/

// Sort and drop duplicates of all_vars_vec vector
void pmMTCSVDataSet::sort_drop_var_duplicates() {
	// sort and drop duplicates from all_vars_vec
	sort(all_vars_vec.begin(), all_vars_vec.end());
	all_vars_vec.erase(unique(all_vars_vec.begin(), all_vars_vec.end()), all_vars_vec.end());
}

/*=====INIT=====*/

// for each data, gets the position of variables in concern within all variables in all_vars_vec vector
void pmMTCSVDataSet::localize_variables() {
	int r = all_data_vec.size(); // number of rows = number of datasets
	int c = all_vars_vec.size(); // number of columns = number of variables

	for (int i = 0; i < r; i++) {
		std::vector<int> RowVec;
		plVariablesConjunction current_obs_vars = all_data_vec[i]->observed_variables();
		for (int j = 0; j < c; j++) {
			int pos = current_obs_vars.get_variable_position(all_vars_vec[j]);
			RowVec.push_back(pos);
		}
		indx_mat.push_back(RowVec);
	}

}

