#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include "pilgrim/general/pmMTSimilarityMeasures.h"
#include "plVariablesConjunction.h"

using namespace std;
using namespace PILGRIM;

/*=====CONSTRUCTORS=====*/

pmMTSimilarityMeasures::pmMTSimilarityMeasures(pmMTCSVDataSet& data_sets) {

    // number of data_sets
    int nb_dataset = data_sets.get_nb_datasets();

    // For each data set compute MI test to all pairs of variables and push to in indep_mat
    for (int d = 0; d < nb_dataset; d++) {
        push_indep_pairs(d, data_sets);
    }

    // For each pair of data sets, compute global similarity and push to glob_sim_vec
    for (int d1 = 0; d1 < nb_dataset - 1; d1++) {
        for (int d2 = d1 + 1; d2 < nb_dataset; d2++) {
            push_global_similarity(d1, d2, data_sets);
        }
    }

}

/*=====PUSHERS=====*/

// Push row vector of independence test result between all pairs of variables of data with data_indx into indep_mat
void pmMTSimilarityMeasures::push_indep_pairs(int data_indx, pmMTCSVDataSet& data_sets) {

    //Current data
    pmCSVDataSet* data = data_sets.all_data_vec[data_indx];
    plVariablesConjunction vars = data->observed_variables();
    int nb_var = vars.dim();

    //Mutual information
    pmMI* mi = new pmMI(data, &vars, true);
    std::vector<bool> indep_mat_row;
    for (int i = 0; i < nb_var - 1; i++) {
        for (int j = i + 1; j < nb_var; j++) {
            indep_mat_row.push_back(mi->computeMI(i, j));
        }
    }
    indep_mat.push_back(indep_mat_row);
    pmMI::clearCache();
    delete mi;
}

// Push global similarity measure between data1 and data2
void pmMTSimilarityMeasures::push_global_similarity(int data_indx1, int data_indx2, pmMTCSVDataSet& data_sets) {

    // number of variables
    int nb_all_var = data_sets.get_nb_variables();
    int n1 = data_sets.all_data_vec[data_indx1]->get_num_fields();
    int n2 = data_sets.all_data_vec[data_indx2]->get_num_fields();
    // global similarity initiated to 0
    int s_g = 0;
    // Loop within pairs of variables to update s_g
    for (int v1 = 0; v1 < nb_all_var - 1; v1++) {
        for (int v2 = v1 + 1; v2 < nb_all_var; v2++) {
            int var1_d1 = data_sets.indx_mat[data_indx1][v1];
            int var2_d1 = data_sets.indx_mat[data_indx1][v2];
            int var1_d2 = data_sets.indx_mat[data_indx2][v1];
            int var2_d2 = data_sets.indx_mat[data_indx2][v2];
            if (var1_d1 != -1 && var2_d1 != -1 && var1_d2 != -1 && var2_d2 != -1) {
                int k1 = triangular_to_row(var1_d1, var2_d1, n1);
                int k2 = triangular_to_row(var1_d2, var2_d2, n2);
                bool indep_d1 = indep_mat[data_indx1][k1];
                bool indep_d2 = indep_mat[data_indx2][k2];
                if (indep_d1 == indep_d2) {
                    s_g += 1;
                }
            }
        }
    }
    // Stock s_g between data1 and data2 in glob_sim_vec which is an upper triangular matrix converted to 1D vector
    glob_sim_vec.push_back(s_g);
}
/*=====GETTERS=====*/

// Get indepence matrice: row i of indep_mat corresponds to vector of independence test results between pairs of variables in data i 
std::vector<std::vector<bool>> pmMTSimilarityMeasures::get_indep_mat() {
    return indep_mat;
}
// Get vector of all global similarities;
std::vector<int> pmMTSimilarityMeasures::get_globsim_vec() {
    return glob_sim_vec;
}

// Get value of global similarity between two specific datasets;
int pmMTSimilarityMeasures::get_globsim(int data_indx1, int data_indx2) {

    int k = triangular_to_row(data_indx1, data_indx2, indep_mat.size());
    int s_g = glob_sim_vec[k];
    return s_g;
}

// Get combined independence measure
float pmMTSimilarityMeasures::get_comb_indep(){
    return comb_indep;
}

// Get combined association measure
float pmMTSimilarityMeasures::get_comb_assoc(){
    return comb_assoc;
}


/*=====PRINTERS=====*/

// Print indep_mat
void pmMTSimilarityMeasures::print_indep_mat() {

    cout << "Flattened independence matrix for each data" << endl;
    for (int i = 0; i < indep_mat.size(); i++) {
        cout << "data" << i << " ";
        for (int j = 0; j < indep_mat[i].size(); j++) {
            cout << indep_mat[i][j] << " ";
        }
        cout << endl;
    }
}

// Print indep matrix for one data (indep matrix of data i corresponds to row i in indep_mat)
void pmMTSimilarityMeasures::print_single_indepmat(int data_indx, int nb_var) {

    cout << "\t";
    for (int i = 0; i < nb_var; i++) { cout << "Var" << i << "\t"; }
    cout << endl;
    std::string decal = "\t*\t"; // To fill lower triangular matrix
    for (int i = 0; i < nb_var - 1; i++) {
        cout << "Var" << i << decal;
        for (int j = i + 1; j < nb_var; j++) {
            int k = triangular_to_row(i, j, nb_var);
            cout << indep_mat[data_indx][k] << "\t";
        }
        cout << endl;
        decal.append("*\t");
    }
    cout << "Var" << nb_var - 1 << decal << endl;
}

// Print global similarity matrix between all datasets
void pmMTSimilarityMeasures::print_globsim_mat() {

    int nb_data = indep_mat.size();

    cout << "\t";
    for (int i = 0; i < nb_data; i++) { cout << "data" << i << " "; }
    cout << endl;

    std::string decal = "\t*\t";
    for (int i = 0; i < nb_data - 1; i++) {
        cout << "data" << i << " ";
        cout << decal;
        for (int j = i + 1; j < nb_data; j++) {
            int k = triangular_to_row(i, j, nb_data);
            cout << glob_sim_vec[k] << "\t";
        }
        cout << endl;
        decal.append("*\t");
    }
    cout << "data" << nb_data - 1 << decal << endl;
}

// Print global similairty vector
void pmMTSimilarityMeasures::print_globsim_vec() {
    for (int i = 0; i < glob_sim_vec.size(); i++) {
        cout << glob_sim_vec[i] << " ";
    }
    cout << endl;
}

/*=====UTILS AND INIT=====*/

// Get element corresponding to i , j position in upper triangular matrix
int pmMTSimilarityMeasures::triangular_to_row(int i, int j, int dim) {

    int k = (dim * (dim - 1)) / 2 - (((dim - std::min(i, j)) * (dim - std::min(i, j) - 1)) / 2) + std::max(i, j) - std::min(i, j) - 1;
    return k;

}

// Sign of I where if sgn(I) = +1 when independence test is positive (I(X,Y|S) = true) and -1 otherwise
int pmMTSimilarityMeasures::sgn(bool indep) {

    int sign = 0;

    if (indep == true) {
        sign = 1;
    }
    else {
        sign = -1;
    }

    return sign;
}

// Init structure that will be passed as an argument in similarity measures with data index, var 1 and 2 indexes, and subset vector, and compute MI 
pmDataRef pmMTSimilarityMeasures::init_dataref_struct(int index, int var1, int var2, std::vector<unsigned int> subset, pmMTCSVDataSet& data_sets) {

    // output structure
    pmDataRef output;
    output.index = index;
    output.v1 = var1;
    output.v2 = var2;
    output.subset = subset;

    // Load data
    pmCSVDataSet* dataset_ref = data_sets.all_data_vec[index];

    // Get independence test result for v1_ref and v2_ref given subset
    plVariablesConjunction vars_ref = dataset_ref->observed_variables();
    pmMI* mi_ref = new pmMI(dataset_ref, &vars_ref, true);

    if (mi_ref->isDataSufficientToProceedIndTest(var1, var2, subset)){
        output.indep = mi_ref->computeMI(var1, var2, subset);
        output.MI = mi_ref->getMI();
        output.p_value = mi_ref->getPValue();
        output.df = mi_ref->getDF();
    }
    else {
        output.indep = false;
        output.MI = 0;
        output.p_value = 0;
        output.df = mi_ref->getDF();
    }

    // output.indep = mi_ref->computeMI(var1, var2, subset);
    // output.MI = mi_ref->getMI();
    // output.p_value = mi_ref->getPValue();
    // output.df = mi_ref->getDF();

    // std::cout << "output mi " << output.MI << endl;
    // std::cout << "output df " << output.df << endl;
    // std::cout << "mi_reg getDF " << mi_ref->getDF() << endl;

    pmMI::clearCache();
    delete mi_ref;

    return output;

}


/*
=====PROTECTED METHODS=====
*/

// Local similarity between data_ref and data_aux
void pmMTSimilarityMeasures::compute_local_similarity(pmDataRef data_ref, pmDataRef* data_aux, pmMTCSVDataSet& data_sets) {

    // Localize variable 1 and 2
    int v1 = data_sets.get_var_index(data_ref.index, data_ref.v1);
    int v2 = data_sets.get_var_index(data_ref.index, data_ref.v2);

    // Update data_aux structure with its corresponding indexes of var1 and var2
    data_aux->v1 = data_sets.indx_mat[data_aux->index][v1];
    data_aux->v2 = data_sets.indx_mat[data_aux->index][v2];

    if (data_aux->v1 != -1 && data_aux->v2 != -1) {
        // Get corresponding indexes of subset variables in data_aux
        for (int i = 0; i < data_ref.subset.size(); i++) {
            int corresp = data_sets.indx_mat[data_aux->index][data_sets.get_var_index(data_ref.index, data_ref.subset[i])];
            if (corresp == -1) {
                // In case there is a variable that is not in common between data_ref and data_aux, exclude data_aux from most similar task
                data_aux->index = -1;
                data_aux->v1 = -1;
                data_aux->v2 = -1;
                data_aux->indep = false;
                data_aux->sl_value = 0;
                data_aux->sc_value = 0;
                data_aux->MI = 0;
                data_aux->p_value = 0;
                break;
            }
            else {
                data_aux->subset.push_back(corresp);
            }
        }
        // Test if all variables in subset_ref have beew localized in data_aux, then computeMI and compare results will have sens
        if (data_ref.subset.size() == data_aux->subset.size()) {
            pmCSVDataSet* data = data_sets.all_data_vec[data_aux->index];
            plVariablesConjunction vars = data->observed_variables();
            pmMI* mi = new pmMI(data, &vars, true);
            if (mi->isDataSufficientToProceedIndTest(data_aux->v1, data_aux->v2, data_aux->subset)){
                data_aux->indep = mi->computeMI(data_aux->v1, data_aux->v2, data_aux->subset);
                data_aux->MI = mi->getMI();
                data_aux->p_value = mi->getPValue();
                if (data_ref.indep == data_aux->indep) {
                    data_aux->sl_value = 1;
                }
                else {
                    data_aux->sl_value = 0.5;
                }
            }
            else {
                data_aux->indep = false;
                data_aux->MI = 0;
                data_aux->p_value = 0;
                data_aux->sl_value = 0;
            }
            pmMI::clearCache();
            delete mi;
        }
    }
    else {
        // In case there is a variable that is not in common between data_ref and data_aux, exclude data_aux from most similar task
        data_aux->index = -1;
        data_aux->v1 = -1;
        data_aux->v2 = -1;
        data_aux->indep = false;
        data_aux->sl_value = 0;
        data_aux->sc_value = 0;
        data_aux->MI = 0;
        data_aux->p_value = 0;
    }
}

// Most similar task using local similarity and global similarity
pmDataRef pmMTSimilarityMeasures::most_similar_task(pmDataRef data_ref, pmMTCSVDataSet& data_sets) {

    // Result to be returned initiated in such a way that if no similar task found, it would not interfer in combined measures
    pmDataRef result;
    result.index = -1; result.v1 = -1; result.v2 = -1; result.sc_value = 0; result.indep = true; result.MI = 0;

    for (int d = 0; d < data_sets.all_data_vec.size(); d++) {

        if (d != data_ref.index) {
            pmDataRef currentdata;
            currentdata.index = d;
            compute_local_similarity(data_ref, &currentdata, data_sets);
            float s_c = get_globsim(d, data_ref.index) * currentdata.sl_value;

            if (result.sc_value < s_c) {
                result.index = d; // update index of most similar data to the current one
                result.sc_value = s_c; // update s_c measure
                result.sl_value = currentdata.sl_value; // update s_l value
                result.indep = currentdata.indep; // update indep test
                result.MI = currentdata.MI; // update MI value
                result.p_value = currentdata.p_value; // update p_value
                result.v1 = currentdata.v1; // update v1 index
                result.v2 = currentdata.v2; // update v2 index
                result.subset = currentdata.subset; // update subset indexes
            }
        }
    }

    return result;

}

// Confidence measure
float pmMTSimilarityMeasures::confidence_measure(pmDataRef data_ref, pmMTCSVDataSet& data_sets) {

    if (data_ref.index != -1) {

        // load data_ref
        pmCSVDataSet* dataset_ref = data_sets.all_data_vec[data_ref.index];
        plVariablesConjunction vars_ref = dataset_ref->observed_variables();

        int n = dataset_ref->get_n_records();
        int card_v1 = vars_ref[data_ref.v1].cardinality();
        int card_v2 = vars_ref[data_ref.v2].cardinality();
        int card_s;
        if (data_ref.subset.size() == 0) { card_s = 0; }
        else { card_s = 1; }
        for (int i = 0; i < data_ref.subset.size(); i++) {
            card_s *= vars_ref[data_ref.subset[i]].cardinality();
        }

        float alpha = 1 - ((log(n) / (2 * n)) * card_v1 * card_v2 * card_s);

        if (alpha <= 0) {
            alpha = 0.005;
        }

        return alpha;
    }
    else {
        return 0; // This has been added to deal with the case when no similar task has been found
    }

}


// Combined independent measure
void pmMTSimilarityMeasures::combined_measures(int index, int var1, int var2, std::vector<unsigned int> subset, pmMTCSVDataSet& data_sets) {

    // Data_ref structure (which is to be considered as the current target task)
    pmDataRef data_ref = init_dataref_struct(index, var1, var2, subset, data_sets);

    // Find most similar task and its measures
    pmDataRef sim_task = most_similar_task(data_ref, data_sets);

    // Calculate confindence measure
    float alpha_ref = confidence_measure(data_ref, data_sets);
    float alpha_mostsim = confidence_measure(sim_task, data_sets);

    // Combined measues
    comb_indep = alpha_ref * sgn(data_ref.indep) + sim_task.sc_value * alpha_mostsim * sgn(sim_task.indep);
    comb_assoc = (alpha_ref * data_ref.MI + sim_task.sc_value * alpha_mostsim * sim_task.MI) / (alpha_ref + sim_task.sc_value * alpha_mostsim);

    // Combined tests
    if (comb_indep > 0) { comb_indep_test = true; }
    else { comb_indep_test = false; }

    float comb_p_value = (alpha_ref * data_ref.p_value + sim_task.sc_value * alpha_mostsim * sim_task.p_value) / (alpha_ref + sim_task.sc_value * alpha_mostsim);
    if (comb_p_value > 0.05) { comb_assoc_test = true; }
    else { comb_assoc_test = false; }

    //if (comb_assoc == 0) comb_assoc_test = true;

    // Update attribute target task
    target_task = data_ref;
    target_task.mostsim_task = sim_task.index;
    target_task.sc_value = sim_task.sc_value;
    target_task.sl_value = sim_task.sl_value;

    // std::cout << "data ref df " << data_ref.df << endl;
    // std::cout << "target task df " << target_task.df << endl;
}

