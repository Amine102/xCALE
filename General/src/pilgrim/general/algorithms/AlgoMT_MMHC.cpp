// WARINING /!\	  : STATUS ON PROGRESS

#include <iostream>
#include <ostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <boost/lexical_cast.hpp>
#include "pilgrim/general/pmMTSimilarityMeasures.h"
#include "pilgrim/general/algorithms/AlgoMT_MMHC.h"

using namespace std;
using namespace PILGRIM;
using namespace boost;

/*=====CONSTRUCTORS=====*/

AlgoMT_MMHC::AlgoMT_MMHC(pmMTCSVDataSet& csv_data, pmMTpScore& mt_Score) {

    int nb_dataset = csv_data.get_nb_datasets();

    // set attribut values
    std::vector<double> set_priors(nb_dataset, 0.5);
    individual_priors = set_priors;

    bool empty = true;
    initgraphs(this->all_CPCgraphs, csv_data, empty);
    initgraphs_forScore(csv_data, mt_Score);

    // create a log file 
    std::ofstream logfile;
    logfile.open("log.csv");
    logfile.close();
    
    //compute all possible operations
    compute_allMTop(csv_data);

    //compute all possible pairs of nodes
    compute_allMTpairs(csv_data);
    
}


/*
=====PUSHERS=====
*/

/*
=====GETTERS=====
*/
void AlgoMT_MMHC::set_allgraphs(std::vector<pmGraph> configuration){
    this->all_graphs = configuration;
}

void AlgoMT_MMHC::set_allCPCgraphs(std::vector<pmGraph> CPCconfiguration){
    this->all_CPCgraphs = CPCconfiguration;
}

void AlgoMT_MMHC::set_Z(int factor){
    this->z = factor;
}

void AlgoMT_MMHC::set_delta(double param_delta){
    this->delta = param_delta;
}

void AlgoMT_MMHC::set_individual_priors(std::vector<double> indiv_priors){
    this->individual_priors = indiv_priors;
}

/*
=====PRINTERS=====
*/

void AlgoMT_MMHC::print_allgraphs(){

    int nb_tasks = this->all_graphs.size();

    for(int t = 0; t < nb_tasks; t++){
        std::cout << "Graph " << t << endl;
        print_graph(this->all_graphs[t]);
    }

}

/*
=====UTILS AND INIT=====
*/

// Get all subsets of CPC set
std::vector<std::vector<unsigned int>> AlgoMT_MMHC::compute_subsets(std::vector<unsigned int> set) {

    int set_size = set.size();
    int output_size = pow(2, set_size);

    std::vector<std::vector<unsigned int>> output;

    for (int counter = 0; counter < output_size; counter++) {
        std::vector<unsigned int> subset;
        for (int j = 0; j < set_size; j++) {
            if ((counter & (1 << j)) != 0) {
                subset.push_back(set[j]);
            }
        }
        output.push_back(subset);
    }

    return output;

}

// initialze graphs (empty or initiated with one and only one random edge, this doesn't take into account CPCsets)
void AlgoMT_MMHC::initgraphs(std::vector<pmGraph>& set_graphs, pmMTCSVDataSet csv_data, bool empty) {

    int nb_dataset = csv_data.get_nb_datasets();

    for (int i = 0; i < nb_dataset; i++) {
        pmBayesianNetwork* tempBN = new pmBayesianNetwork(csv_data.all_data_vec[i]->observed_variables());
        set_graphs.push_back(tempBN->get_graph());
        delete tempBN;
    }

    // if not empty means in here initialized with a random edge (only 1 edge)
    if (!empty) {

        // Initialize seed with time value to random 
        struct timeval time;
        gettimeofday(&time, NULL);
        srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

        for (int i = 0; i < nb_dataset; i++) {
            int numberOfNodes = num_vertices(set_graphs[i]);

            int indx_nodeA = rand() % (numberOfNodes);
            int indx_nodeB = rand() % (numberOfNodes);
            int iter = 0;

            while ((indx_nodeB == indx_nodeA) && (iter < numberOfNodes)) {
                indx_nodeB = rand() % (numberOfNodes);
                iter++;
            }

            pmNode nodeA = vertex(indx_nodeA, set_graphs[i]);
            pmNode nodeB = vertex(indx_nodeB, set_graphs[i]);
            add_edge(nodeA, nodeB, set_graphs[i]);
        }
    }

}

// init graphs with object pmScore;
void AlgoMT_MMHC::initgraphs_forScore(pmMTCSVDataSet& csv_data, pmMTpScore& mt_Score){

    int nb_dataset = csv_data.get_nb_datasets();

    for (int i = 0; i < nb_dataset; i++){
        this->all_graphs.push_back(mt_Score.all_bn_vec[i]->get_graph());
    }

}

// update CPC graphs (convert CPC sets to graphs)
void AlgoMT_MMHC::update_CPCgraphs(int data_indx, std::vector<std::vector<unsigned int>> CPC_set) {

    for (int i = 0; i < CPC_set.size(); i++) {

        for (int j = 0; j < CPC_set[i].size(); j++) {

            pmNode nodeI = vertex(i, this->all_CPCgraphs[data_indx]);
            pmNode nodeJ = vertex(CPC_set[i][j], this->all_CPCgraphs[data_indx]);

            pmEdge edgeI, edgeJ;
            bool testI, testJ = true;

            boost::tie(edgeI, testI) = edge(nodeI, nodeJ, this->all_CPCgraphs[data_indx]);
            boost::tie(edgeJ, testJ) = edge(nodeJ, nodeI, this->all_CPCgraphs[data_indx]);

            if (testI == false) add_edge(nodeI, nodeJ, this->all_CPCgraphs[data_indx]);
            if (testJ == false) add_edge(nodeJ, nodeI, this->all_CPCgraphs[data_indx]);
        }
    }
}

// Methods copied with no modification from AlgoGS
bool AlgoMT_MMHC::remainsADAG(const pmNode& nodeInit, const pmNode& node, const pmGraph& graph) {
    graph_traits<pmGraph>::adjacency_iterator vi, vi_end;
    boost::tie(vi, vi_end) = adjacent_vertices(node, graph);
    for (; vi != vi_end; ++vi)
    {
        if ((*vi == nodeInit) || (!remainsADAG(nodeInit, *vi, graph)))
            return false;
    }

    return true;
}

bool AlgoMT_MMHC::remainsADAG(pmGraph& graph, const pmGraphOp& op) {

    bool result = true;
    switch (op.op)
    {
    case pmGraphOp::SL_EDGE_ADD:
        add_edge(op.nodeA, op.nodeB, graph);
        result = remainsADAG(op.nodeA, op.nodeB, graph);
        remove_edge(op.nodeA, op.nodeB, graph);
        break;

    case pmGraphOp::SL_EDGE_REM:
        break;

    case pmGraphOp::SL_EDGE_REV:
        remove_edge(op.nodeA, op.nodeB, graph);
        add_edge(op.nodeB, op.nodeA, graph);
        result = remainsADAG(op.nodeB, op.nodeA, graph);
        remove_edge(op.nodeB, op.nodeA, graph);
        add_edge(op.nodeA, op.nodeB, graph);
        break;

    default:
        throw std::runtime_error("invalid operation in Glutton Search");
    }

    return result;
}

// Compute number of edges that exists in graph 1 and not in graph2
int AlgoMT_MMHC::oneway_edge_distance(int d1, int d2, pmGraph graph1, pmGraph graph2, pmMTCSVDataSet csv_data) {

    int n_nodes1 = num_vertices(graph1);

    int counts = 0; // number of edges in graph1 and not in graph2

    for (int i1 = 0; i1 < n_nodes1 - 1; i1++) {

        pmNode nodeA1 = vertex(i1, graph1);

        for (int j1 = i1 + 1; j1 < n_nodes1; j1++) {

            pmNode nodeB1 = vertex(j1, graph1);

            pmEdge edgeA1, edgeB1;
            bool testA1, testB1;

            boost::tie(edgeA1, testA1) = edge(nodeA1, nodeB1, graph1);
            boost::tie(edgeB1, testB1) = edge(nodeB1, nodeA1, graph1);

            //std::cout << "..........................................." << endl;
            //std::cout << "node i1 " << i1 << " node j1 " << j1 << endl;
            //std::cout << "testA1 " << testA1 << " testB1 " << testB1 << endl;

            if (testA1 == true) {

                int locA = csv_data.get_var_index(d1, i1);
                int locB = csv_data.get_var_index(d1, j1);

                int i2 = csv_data.indx_mat[d2][locA];
                int j2 = csv_data.indx_mat[d2][locB];

                //std::cout << "node i2 " << i2 << " node j2 " << j2 << endl;

                if ((i2 != -1) && (j2 != -1)) {
                    pmNode nodeA2 = vertex(i2, graph2);
                    pmNode nodeB2 = vertex(j2, graph2);

                    pmEdge edgeA2;
                    bool testA2;

                    boost::tie(edgeA2, testA2) = edge(nodeA2, nodeB2, graph2);

                    //std::cout << "testA2 " << testA2 << endl;

                    if (testA2 == false) counts += 1;
                    //std::cout << "counts apres testA2 " << counts << endl;
                }
                else counts += 1;
                //std::cout << "counts apres fail i2 j2 " << counts << endl;
            }

            if (testB1 == true) {

                int locA = csv_data.get_var_index(d1, i1);
                int locB = csv_data.get_var_index(d1, j1);

                int i2 = csv_data.indx_mat[d2][locA];
                int j2 = csv_data.indx_mat[d2][locB];

                //std::cout << "node i2 " << i2 << " node j2 " << j2 << endl;

                if ((i2 != -1) && (j2 != -1)) {
                    pmNode nodeA2 = vertex(i2, graph2);
                    pmNode nodeB2 = vertex(j2, graph2);

                    pmEdge edgeB2;
                    bool testB2;

                    boost::tie(edgeB2, testB2) = edge(nodeB2, nodeA2, graph2);

                    //std::cout << "testA2 " << testB2 << endl;

                    if (testB2 == false) counts += 1;
                    //std::cout << "counts apres testA2 " << counts << endl;
                }
                else counts += 1;
                //std::cout << "counts apres fail i2 j2 " << counts << endl;
            }
        }
    }

    return counts;
}

// Compute symmetric distance (number of edges that exists in one graph but not in the other)
int AlgoMT_MMHC::symMT_distance(int d1, int d2, pmGraph graph1, pmGraph graph2, pmMTCSVDataSet csv_data) {

    int counts_1 = oneway_edge_distance(d1, d2, graph1, graph2, csv_data); //number of edges in graph1 and not in graph2
    int counts_2 = oneway_edge_distance(d2, d1, graph2, graph1, csv_data); //number of edges in graph2 and not in graph1

    int counts = 0; //value to be returned ie symmetric distance btw graph1 and graph2 = counts_1 + counts_2

    counts = counts_1 + counts_2;

    return counts;
}

// Convert pmGraphOp_MT structure to pmGraphOp
void AlgoMT_MMHC::adapt_operation(pmGraphOp& adaptedOp, pmGraphOp_MT operation_MT, int data_index, pmMTCSVDataSet csv_data) {

    adaptedOp = pmGraphOp(vertex(0, this->all_graphs[data_index]), vertex(1, this->all_graphs[data_index]), pmGraphOp::SL_EDGE_ABS); // fake operation, only to store the information in case at least one of the two nodes doesn't exist in the graph, so it is not concerned by operation

    int i_nodeA = csv_data.indx_mat[data_index][operation_MT.nodeA_indx];
    int i_nodeB = csv_data.indx_mat[data_index][operation_MT.nodeB_indx];

    //std::cout << "initial vars index, A : " << operation_MT.nodeA_indx << " B : " << operation_MT.nodeB_indx << endl;
    //std::cout << "corresponding vars index, A : " << i_nodeA << " B : " << i_nodeB << endl;

    if ((i_nodeA != -1) && (i_nodeB != -1)) {

        pmNode nodeA_d = vertex(i_nodeA, this->all_graphs[data_index]);
        pmNode nodeB_d = vertex(i_nodeB, this->all_graphs[data_index]);

        pmEdge edgeA_d, edgeCPC_d;
        bool testA, testCPC = true;

        boost::tie(edgeA_d, testA) = edge(nodeA_d, nodeB_d, this->all_graphs[data_index]);
        boost::tie(edgeCPC_d, testCPC) = edge(vertex(i_nodeA, this->all_CPCgraphs[data_index]), vertex(i_nodeB, this->all_CPCgraphs[data_index]), this->all_CPCgraphs[data_index]);

        //std::cout << "testA " << testA << " testCPC " << testCPC << endl;

        switch (operation_MT.op) {

        case pmGraphOp_MT::SL_EDGE_NOT:
            adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_NOT);
            break;

        case pmGraphOp_MT::SL_EDGE_ADD:
            if (testCPC == true) {
                if (testA == false) { // if CPC allows adding edge and edge doesn't exists, so try add edge
                    pmGraphOp op(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_ADD);
                    if (remainsADAG(this->all_graphs[data_index], op)) adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_ADD);
                    else adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_NOT); //if graph doesn't remain DAG then do nothing
                }
                else adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_NOT); // else if CPC allows adding edge but edge already exists, then do nothing
            }
            else adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_NOT); // else if CPC doesn't allow adding this edge, then do nothing
            break;

        case pmGraphOp_MT::SL_EDGE_REV:
            if (testA == true) { // means edge already exists in graph so it has been allowed by CPC in previous processes
                pmGraphOp op(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_REV);
                if (remainsADAG(this->all_graphs[data_index], op)) adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_REV);
                else adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_NOT); //if graph doesn't remain DAG then do nothing
            }
            else adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_NOT); // means edge doesn't exist, so reversing it is not an option, then the actual graph is not concerned by this operation (ABS(1) or NOT(0) ?)
            break;

        case pmGraphOp_MT::SL_EDGE_REM:
            if (testA == true) { //if the edge already exists so there is sense in removing it
                adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_REM);
            }
            else adaptedOp = pmGraphOp(nodeA_d, nodeB_d, pmGraphOp::SL_EDGE_NOT); // means edge doesn't exist, so removing it is not an option, then the actual graph is not concerned by this operation (ABS(1) or NOT(0) ?)
            break;
        }
    }

}

// Compute all possible operations considering all variables
void AlgoMT_MMHC::compute_allMTop(pmMTCSVDataSet csv_data) {

    int nb_vars = csv_data.get_nb_variables();

    for (int iA = 0; iA < nb_vars - 1; iA++) {
        for (int iB = iA + 1; iB < nb_vars; iB++) {

            // Do nothing
            all_possible_MTop.push_back(pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_NOT));

            // Add edge
            all_possible_MTop.push_back(pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_ADD));
            all_possible_MTop.push_back(pmGraphOp_MT(iB, iA, pmGraphOp_MT::SL_EDGE_ADD));

            // Reverse edge
            all_possible_MTop.push_back(pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_REV));
            all_possible_MTop.push_back(pmGraphOp_MT(iB, iA, pmGraphOp_MT::SL_EDGE_REV));

            // Remove edge
            all_possible_MTop.push_back(pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_REM));
            all_possible_MTop.push_back(pmGraphOp_MT(iB, iA, pmGraphOp_MT::SL_EDGE_REM));
        }
    }
}

// Compute all possible pairs of variables within all data
void AlgoMT_MMHC::compute_allMTpairs(pmMTCSVDataSet csv_data) {

    int nb_vars = csv_data.get_nb_variables();

    for (int iA = 0; iA < nb_vars - 1; iA++) {
        for (int iB = iA + 1; iB < nb_vars; iB++) {

            pair<int, int> val(iA, iB);
            pair<int, int> valOp(iB, iA);
            all_poss_pairsOfNodes.push_back(val);
            all_poss_pairsOfNodes.push_back(valOp);
        }
    }

}

// Save all graphs in empty bns
void AlgoMT_MMHC::save_as_emptybns(std::string dir_path, std::string name, pmMTCSVDataSet csv_data){

    int nb_tasks = this->all_graphs.size();

    for (int t = 0; t < nb_tasks; t++){

        pmBayesianNetwork* tempBN = new pmBayesianNetwork(csv_data.all_data_vec[t]->observed_variables());
        tempBN->get_graph() = this->all_graphs[t];

        std::stringstream ss_out;
        ss_out << dir_path << name << "_bn" << t << ".xdsl";
        std::string p_out = ss_out.str();
        tempBN->save_as_genie(p_out);

        delete tempBN;

    }

}

// Save all CPCgraphs in empty bns (ie with no parameters)
void AlgoMT_MMHC::save_CPCgraphs(std::string dir_path, std::string name, pmMTCSVDataSet csv_data){

    int nb_tasks = this->all_CPCgraphs.size();

    for (int t = 0; t < nb_tasks; t++){

        pmBayesianNetwork* tempBN = new pmBayesianNetwork(csv_data.all_data_vec[t]->observed_variables());
        tempBN->get_graph() = this->all_CPCgraphs[t];

        int nb_nodes = num_vertices(tempBN->get_graph());

        for (int i = 0; i < nb_nodes - 1; i++){
            for (int j = i+1; j < nb_nodes; j++){

                pmNode nodeI = vertex(i, tempBN->get_graph());
                pmNode nodeJ = vertex(j, tempBN->get_graph());

                pmEdge edgeI, edgeJ;
                bool testI, testJ = true;

                boost::tie(edgeI, testI) = edge(nodeI, nodeJ, tempBN->get_graph());

                if (testI == true) remove_edge(nodeI, nodeJ, tempBN->get_graph());
                
            }
        }

        std::stringstream ss_out;
        ss_out << dir_path << name << "_CPCbn" << t << ".xdsl";
        std::string p_out = ss_out.str();
        tempBN->save_as_genie(p_out);

        //std::cout << "graph " << t << endl;
        //print_graph(tempBN->get_graph());

        delete tempBN;

    }

}

/*
===== METHODS=====
*/

/*=================================== MT_MMHC CPCs identification part ===================================*/

// Compute mininum combined association between var1 and var2 knowing CPC_set and return minimum association value with its independent test result
std::pair<bool, float> AlgoMT_MMHC::computeMinAssoc(int data_indx, int var1, int var2, std::vector<unsigned int> CPC_set, pmMTCSVDataSet& csv_data, pmMTSimilarityMeasures& sim_measures) {

    std::vector<std::vector<unsigned int>> allCPCsubsets = compute_subsets(CPC_set);
    //std::cout << "Subsets computed and allsubsets size is " << allCPCsubsets.size() << endl;

    /*std::ofstream logfile;
    logfile.open("log.csv", std::ios_base::app);

    logfile << "===========================================================" << endl;
    logfile << "ComputeMinAssoc data " << data_indx << " Var_T " << var1 << " var_X " << var2 << " CPC size " << CPC_set.size() << " subsets size " << allCPCsubsets.size() << endl;
    logfile << "===========================================================" << endl;*/

    float min_assoc_val = 0;
    bool indep_assoc = true;
    bool first_iteration = true;

    /*logfile << "CPC  : ";
    for (int l = 0; l < CPC_set.size(); l++)
        logfile << CPC_set[l] << " ";
    logfile << endl;*/

    //std::cout << "Debut de boucle " << endl;

    for (int i = 0; i < allCPCsubsets.size(); i++) {

        //std::cout << "................................. Iteration n " << i << endl;
        /*logfile << "................................. Iteration n " << i << " subset ";
        for (int k = 0; k < allCPCsubsets[i].size(); k++)
            logfile << allCPCsubsets[i][k] << " ";
        logfile << endl;*/
        //std::cout << "MinAssoc data " << data_indx << " var_T " << var1 << " var2 " << var2 << std::endl;
        sim_measures.combined_measures(data_indx, var1, var2, allCPCsubsets[i], csv_data);
        float assoc_val = sim_measures.comb_assoc;

        if ((assoc_val) == 0 && (sim_measures.comb_assoc_test == false)){
            continue;
        }

        //logfile << "assoc_val " << assoc_val << endl;
        //logfile << "min_assoc " << min_assoc_val << endl;

        if (first_iteration) {
            //logfile << "premiere iteration...................." << endl;
            //logfile << "assoc_val " << assoc_val << endl;
            min_assoc_val = assoc_val;
            indep_assoc = sim_measures.comb_assoc_test;
            first_iteration = false;
            //logfile << "min assoc apres 1ere iteration....................... " << min_assoc_val << endl;
        }
        else {
            if (assoc_val < min_assoc_val) {
                min_assoc_val = assoc_val;
                indep_assoc = sim_measures.comb_assoc_test;
            }
        }
    }

    //logfile.close();

    std::pair<bool, float> result;

    result.first = indep_assoc;
    result.second = min_assoc_val;

    return result;
}

// Compute max min heuristic for var_T in data data_indx and return the maximum value and argmax
F_assocF AlgoMT_MMHC::compute_MT_MaxMinHeuristic(int data_indx, int var_T, std::vector<unsigned int> CPC_set, pmMTCSVDataSet& csv_data, pmMTSimilarityMeasures& sim_measures) {

    int nb_vars = csv_data.all_data_vec[data_indx]->get_num_fields(); //numbre of variables in data data_indx

    bool maxMinAssoc_test = true;
    float maxMinAssoc = 0;
    int F_indx = -1;

    bool first_compute = true;

    /*std::ofstream logfile;
    logfile.open("log.csv", std::ios_base::app);

    logfile << "==================================================" << endl;
    logfile << "MaxminHeuristic data " << data_indx << " Var_T " << var_T << " CPC size " << CPC_set.size() << endl;
    logfile << "==================================================" << endl;

    //std::cout << "Avant boucle F " << F_indx << " maxAssoc " << maxMinAssoc << endl;
    logfile << "Avant boucle F " << F_indx << " maxAssoc " << maxMinAssoc << endl;

    logfile.close();
    */
    for (int l = 0; l < varList.size(); l++) {

        int i = varList[l];

        //std::cout << "Iteration de la variable " << i << "---------------------" << endl;

        if (std::find(CPC_set.begin(), CPC_set.end(), i) != CPC_set.end()) {
            //std::cout << "Warning, the variable " << i << " has been found twice in MaxMinHeuristic (in I(T,X|CPC) )" << endl;
        }

        if ((i != var_T) && (std::find(CPC_set.begin(), CPC_set.end(), i) == CPC_set.end())) {
            std::pair<bool, float> min_assoc = computeMinAssoc(data_indx, var_T, i, CPC_set, csv_data, sim_measures);
            //std::cout << "min_assoc " << min_assoc.second << endl;

            if (first_compute) {
                maxMinAssoc = min_assoc.second;
                maxMinAssoc_test = min_assoc.first;
                F_indx = i;
                first_compute = false;
            }

            if (min_assoc.second > maxMinAssoc) {
                maxMinAssoc = min_assoc.second;
                maxMinAssoc_test = min_assoc.first;
                F_indx = i;
            }

            // for optimisation
            if (min_assoc.first){
                varList.erase(std::remove(varList.begin(), varList.end(), i), varList.end());
                l -= 1;
                if (l < -1) l = varList.size()+1;
            }
        }
    }

    //std::cout << "Apres boucle F " << F_indx << " maxAssoc " << maxMinAssoc << endl;
    /*logfile.open("log.csv", std::ios_base::app);
    logfile << "Apres boucle F " << F_indx << " maxAssoc " << maxMinAssoc << endl;
    logfile << "==================== Fin MaxMinheuristic ===================" << endl;
    logfile.close();*/

    F_assocF result;
    result.F = F_indx;
    result.assocF = maxMinAssoc;
    result.assoc_Test = maxMinAssoc_test;

    return result;

}

// Corresponding to forward phase in MMHC algorithm
void AlgoMT_MMHC::Phase1(int data_indx, int var_T, std::vector<unsigned int>& CPC_T, pmMTCSVDataSet& csv_data, pmMTSimilarityMeasures& sim_measures) {

    /*std::ofstream logfile;
    logfile.open("log.csv", std::ios_base::app);

    logfile << "==================================================" << endl;
    logfile << "Phase1 data " << data_indx << " Var_T " << var_T << " CPC size " << CPC_T.size() << endl;
    logfile << "==================================================" << endl;

    logfile.close();*/

    CPC_T = {}; // initialization of CPC(T) to empty set
    bool isChangeCPC = true;
    //std::cout << "Initiated to empty and size is " << CPC_T.size() << endl;

    int nb_vars = csv_data.all_data_vec[data_indx]->get_num_fields(); //numbre of variables in data data_indx

    int sizeCPC_T = 1; //indicator to stop while loop, (while loop ends when size of CPC doesn't change), initiated to 1 so that first loop can occur

    int iter = 0;

    //std::vector<unsigned int> varList;

    //varList.push_back(1);
    varList.clear();

    for (int i = 0; i < nb_vars; i++){
        if (i != var_T) varList.push_back(i);
    }

    // std::vector<std::pair<float, int>> mi_vars;
    // std::vector<unsigned int> empty_CPC = {};

    // for (int i = 0; i < nb_vars; i++){
    //     if (i != var_T){
    //         sim_measures.combined_measures(data_indx, var_T, i, empty_CPC, csv_data);
    //         mi_vars.push_back( make_pair(sim_measures.comb_assoc,i) );
    //     }
    // }

    // sort(mi_vars.rbegin(), mi_vars.rend());

    // int max_degree = nb_vars-1;
    // int lim = min(nb_vars-1,max_degree);

    // for (int k = 0; k < lim; k++){
    //     if (mi_vars[k].first != 0){
    //         varList.push_back(mi_vars[k].second);
    //     }
    //     else break;
    // }

    ////////////////////

    int i = 0;
    while (i < CPC_T.size()){
        remove(varList.begin(), varList.end(), CPC_T.at(i));
		++i;
    }

    while (isChangeCPC && !varList.empty()) { // && (CPC_T.size() < 10)

        //std::cout << "Iteration while loop n " << iter << endl;

        sizeCPC_T = CPC_T.size(); //size of CPC at the begining of the loop

        //std::cout << "sizeCPC " << sizeCPC_T << " allCPC size " << CPC_T.size() << endl;

        // if (this->df_jaug > 27000){
        //     this->df_jaug = 1;
        //     break;
        // }

        F_assocF assoc = compute_MT_MaxMinHeuristic(data_indx, var_T, CPC_T, csv_data, sim_measures);

        /*logfile.open("log.csv", std::ios_base::app);
        //std::cout << "F " << assoc.F << " assocF " << assoc.assocF << " assocF_Test " << assoc.assoc_Test << endl;
        logfile << "Phase 1 : while loop " << iter << " F " << assoc.F << " assocF " << assoc.assocF << " assocF_Test " << assoc.assoc_Test << endl;*/

        if (assoc.assoc_Test != true) { // assoc_value != 0  <=> assoc_Test = false
            CPC_T.push_back(assoc.F);  
            isChangeCPC = true;          
        }
        else {
            isChangeCPC = false;
            continue;
        }

        //std::cout << "Apres if condition sizeCPC " << sizeCPC_T << " allCPC size " << CPC_T.size() << endl;
        //logfile << "Phase 1 : Apres if condition sizeCPC " << sizeCPC_T << " allCPC size " << CPC_T.size() << endl;
        //logfile.close();

        iter++;
    }

    /*logfile.open("log.csv", std::ios_base::app);
    logfile << "========================================== Fin Phase1 ========================================" << endl;
    logfile.close();*/
    //std::cout << "Fin phase 1 pour variable " << var_T << " taille cpc " << CPC_T.size() << endl;

}

// Corresponding to backward phase in MMHC algorithm
void AlgoMT_MMHC::Phase2(int data_indx, int var_T, std::vector<unsigned int>& CPC_T, pmMTCSVDataSet& csv_data, pmMTSimilarityMeasures& sim_measures) {

    //std::cout << "Debut Phase2 pour variable " << var_T << "et CPC_T size " << CPC_T.size() << " ............................. " << endl;

    std::vector<std::vector<unsigned int>> allCPCsubsets = compute_subsets(CPC_T);

    std::vector<unsigned int> initialCPC_T = CPC_T; // to use for the loop as long as we access by index and CPC_T size might change inside the loop

    for (int i = 0; i < initialCPC_T.size(); i++) {

        //std::cout << "Variable n " << i << " de CPC_T qui est " << CPC_T[i] << endl;

        bool exist_Ind = false;
        //std::cout << "exist_Ind initialisé à " << exist_Ind << endl;

        for (int s = 0; s < allCPCsubsets.size(); s++) {

            //std::cout << "subset num " << s << " contenant ";
            //for (int j = 0; j < allCPCsubsets[s].size(); j++) std::cout << allCPCsubsets[s][j] << " ";
            //std::cout << endl;

            bool indep = false;

            if (std::find(allCPCsubsets[s].begin(), allCPCsubsets[s].end(), initialCPC_T[i]) == allCPCsubsets[s].end()) {
                //std::cout << "la variable n'est en cours pas dans le subset" << endl;
                //std::cout << "Phase2 data " << data_indx << " var_T " << var_T << " var2 " << initialCPC_T[i] << std::endl;
                sim_measures.combined_measures(data_indx, var_T, initialCPC_T[i], allCPCsubsets[s], csv_data);
                indep = sim_measures.comb_assoc_test;
                //std::cout << "Mise à jour de indep à " << indep << endl;
            }
            if (indep) {
                exist_Ind = true;
                break;
            }
        }

        //std::cout << "exist_Ind après boucle sur les subsets " << exist_Ind << endl;

        if (exist_Ind) {
            CPC_T.erase(std::remove(CPC_T.begin(), CPC_T.end(), initialCPC_T[i]), CPC_T.end());
        }
    }

    //std::cout << "Fin Phase2 pour variable " << var_T << "et CPC_T size " << CPC_T.size() << " ............................. " << endl;
}

// Identification of CPC_set of var_T
void AlgoMT_MMHC::MT_MMPCbar(int data_indx, int var_T, std::vector<unsigned int>& CPC_T, pmMTCSVDataSet& csv_data, pmMTSimilarityMeasures& sim_measures) {

    //std::cout << "Début phase 1 dans MMPCbar pour var " << var_T << " taille CPC_T " << CPC_T.size() << endl;
    Phase1(data_indx, var_T, CPC_T, csv_data, sim_measures);
    //std::cout << "Début phase 2 dans MMPCbar pour var " << var_T << " taille CPC_T " << CPC_T.size() << endl;
    Phase2(data_indx, var_T, CPC_T, csv_data, sim_measures);

}

// Exclude false positives in CPC_set of var_T
void AlgoMT_MMHC::MT_MMPC(int data_indx, pmMTCSVDataSet& csv_data, pmMTSimilarityMeasures& sim_measures) {

    allCPC_T.clear();

    int nb_vars = csv_data.all_data_vec[data_indx]->get_num_fields();

    for (int var_T = 0; var_T < nb_vars; var_T++){

        std::vector<unsigned int> CPC_T;
        // Compute CPC of variable T
        MT_MMPCbar(data_indx, var_T, CPC_T, csv_data, sim_measures);
        allCPC_T.push_back(CPC_T);

    }

    // Delete false positives

    for (int var = 0; var < allCPC_T.size(); var++){

        std::vector<unsigned int> initialCPC_T = allCPC_T[var]; // to use for the loop because we access by index and CPC_T size might change inside the loop

        for (int i = 0; i < initialCPC_T.size(); i++) {

            if (std::find(allCPC_T[initialCPC_T[i]].begin(), allCPC_T[initialCPC_T[i]].end(), var) == allCPC_T[initialCPC_T[i]].end()) {
                allCPC_T[var].erase(std::remove(allCPC_T[var].begin(), allCPC_T[var].end(), initialCPC_T[i]), allCPC_T[var].end());
            }
        }

    }
}

// Gathering all phases and computing all CPC sets for all variables in data data_indx
/*void AlgoMT_MMHC::MT_MMHC_CPC(int data_indx, pmMTCSVDataSet csv_data, pmMTSimilarityMeasures sim_measures) {

    // allCPC_T stores all CPC(T) of data with index data_indx, allCPC_T[i] = cpc set of variable i
    allCPC_T.clear(); // to assure that each CPC will be corresponding to its index
    //std::cout << "tache n " << data_indx << " allCPC_T size after clear " << allCPC_T.size() << endl;

    int nb_vars = csv_data.all_data_vec[data_indx]->get_num_fields(); //numbre of variables in data data_indx
    //std::cout << "nb vars de la tache " << data_indx << " est " << nb_vars << endl;

    for (int i = 0; i < nb_vars; i++) {

        std::vector<unsigned int> CPC_i;
        MT_MMPC(data_indx, i, CPC_i, csv_data, sim_measures);
        allCPC_T.push_back(CPC_i);

    }

}*/

// Running MT_MMHC_part1 (identification of CPC sets) to all tasks (datasets)
void AlgoMT_MMHC::run_MT_MMHC_ph1(pmMTCSVDataSet csv_data) {

    auto start = std::chrono::high_resolution_clock::now();
    //Creating pmMTSimilarityMeasures object
    pmMTSimilarityMeasures sim_measures(csv_data);

    int nb_dataset = csv_data.get_nb_datasets();

    for (int d = 0; d < nb_dataset; d++) {

        std::cout << "Boucle pour data " << d << "........................" << endl;

        MT_MMPC(d, csv_data, sim_measures);
        update_CPCgraphs(d, allCPC_T);

        std::cout << "print_ graph" << endl;
        std::cout << "allCPC_T size " << allCPC_T.size() << endl;
        print_graph(this->all_CPCgraphs[d]);
    }

    auto finish = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "MT-MMHC time execution: " << elapsed.count() << " s\n";    
}

/*=================================== MT_Greedy search part ===================================*/

// Compute priors as in MT-GS/Niculescu, if used outside class don't forget to set Z, Delta and individual priors
double AlgoMT_MMHC::compute_prior(int level, std::vector<pmGraphOp_MT> operations_MT, pmMTCSVDataSet& csv_data) {

    int nb_tasks = csv_data.get_nb_datasets();

    double prior = this->z;

    double exponent = 1 / (1 + (nb_tasks - 1) * this->delta);

    for (int i = 0; i < level; i++) {
        prior *= pow(this->individual_priors[i], exponent);
    }

    for (int i = 0; i < level - 1; i++) {

        // Adapt MT operation to current task
        pmGraphOp adaptedOpI(vertex(0, this->all_graphs[i]), vertex(1, this->all_graphs[i]), pmGraphOp::SL_EDGE_ABS); // fake operation, only to store the information in case at least one of the two nodes doesn't exist in the graph, so it is not concerned by operation
        adapt_operation(adaptedOpI, operations_MT[i], i, csv_data);
        // Step to avoid ununderstood bad_alloc error from using directly adaptedOp
        int iA = get(vertex_index, this->all_graphs[i])[adaptedOpI.nodeA];
        int iB = get(vertex_index, this->all_graphs[i])[adaptedOpI.nodeB];
        pmGraphOp resultOpI(vertex(iA, this->all_graphs[i]), vertex(iB, this->all_graphs[i]), adaptedOpI.op);

        resultOpI.apply(this->all_graphs[i]);

        for (int j = i + 1; j < level; j++) {

            // Adapt MT operation to current task
            pmGraphOp adaptedOpJ(vertex(0, this->all_graphs[j]), vertex(1, this->all_graphs[j]), pmGraphOp::SL_EDGE_ABS); // fake operation, only to store the information in case at least one of the two nodes doesn't exist in the graph, so it is not concerned by operation
            adapt_operation(adaptedOpJ, operations_MT[j], j, csv_data);
            // Step to avoid ununderstood bad_alloc error from using directly adaptedOp
            int jA = get(vertex_index, this->all_graphs[j])[adaptedOpJ.nodeA];
            int jB = get(vertex_index, this->all_graphs[j])[adaptedOpJ.nodeB];
            pmGraphOp resultOpJ(vertex(jA, this->all_graphs[j]), vertex(jB, this->all_graphs[j]), adaptedOpJ.op);

            resultOpJ.apply(this->all_graphs[j]);

            int distance = symMT_distance(i, j, this->all_graphs[i], this->all_graphs[j], csv_data);
            prior *= pow(1 - delta, distance / (nb_tasks - 1));

            resultOpJ.unapply(this->all_graphs[j]);
        }

        resultOpI.unapply(this->all_graphs[i]);
    }

    return prior;
}

// Compute individual scores
pmScoreValueType AlgoMT_MMHC::compute_indiv_score(pmGraphOp& operation, int task_id, pmMTpScore& mt_Score) {

    pmScoreValueType resultScore = 0; //in case nothing is done
    //std::cout << "premier resultScore initie " << endl;

    pmScoreValueType score1, score2, score3, score4;

    switch (operation.op) {

    case pmGraphOp::SL_EDGE_ABS:
        resultScore = 0;
        // means nothing will be done in the actual graph
        //std::cout << "cas ABS score = " << resultScore << endl;
        break;

    case pmGraphOp::SL_EDGE_NOT:
        resultScore = 0; // nothing is done so the score will not change
        //std::cout << "cas NOT score = " << resultScore << endl;
        break;

    case pmGraphOp::SL_EDGE_ADD:
        score2 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeB);
        operation.apply(this->all_graphs[task_id]);
        score1 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeB);
        operation.unapply(this->all_graphs[task_id]);
        resultScore = score1 - score2;
        break;

    case pmGraphOp::SL_EDGE_REV:
        score2 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeB);
        score4 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeA);
        remove_edge(operation.nodeA, operation.nodeB, this->all_graphs[task_id]);
        score1 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeB);
        add_edge(operation.nodeB, operation.nodeA, this->all_graphs[task_id]);
        score3 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeA);
        operation.unapply(this->all_graphs[task_id]);
        resultScore = score1 - score2 + score3 - score4;
        break;

    case pmGraphOp::SL_EDGE_REM:
        score2 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeB);
        operation.apply(this->all_graphs[task_id]);
        score1 = mt_Score.all_pScores[task_id]->computeNode(operation.nodeB);
        operation.unapply(this->all_graphs[task_id]);
        resultScore = score1 - score2;
        break;
    }

    return resultScore;
}

// Compute best delta scores to actual configuration (this->allgraphs) for selected pair of nodes
// If used outside scope, don't forget to set allgraphs and set or compute CPCgraphs
std::vector<pmScoreValueType> AlgoMT_MMHC::compute_bestq(pair<int, int> pair_of_MTNodes, pmMTpScore& mt_Score, pmMTCSVDataSet& csv_data) {

    int nb_tasks = csv_data.get_nb_datasets(); // if previous steps has been calculated correctly, numbre tasks = number data_sets = nb graphs (size of the configuration all_graphs)

    std::vector<pmScoreValueType> bestscores; // initiation of the vector that will be returned storing all best scores

    for (int d = 0; d < nb_tasks; d++) {

        // in case nodeA or/and nodeB doesn't exist in task d, the best operation to do is nothing. Supposing that each task has at least node 0 and node 1
        pmGraphOp op_Nothing(vertex(0, this->all_graphs[d]), vertex(1, this->all_graphs[d]), pmGraphOp::SL_EDGE_NOT);
        pmScoreValueType bestScore = compute_indiv_score(op_Nothing, d, mt_Score); 

        // Conversion of mt indexes to each corresponding data
        int i_nodeA = csv_data.indx_mat[d][pair_of_MTNodes.first];
        int i_nodeB = csv_data.indx_mat[d][pair_of_MTNodes.second];

        if ((i_nodeA != -1) && (i_nodeB != -1)) {
            //std::cout << "........................noeud I " << i_nodeA << endl;
            pmNode nodeI = vertex(i_nodeA, this->all_graphs[d]);

            //std::cout << "........................noeud J " << i_nodeB << endl;
            pmNode nodeJ = vertex(i_nodeB, this->all_graphs[d]);

            pmEdge edgeI, edgeJ, edgeCPC;
            bool testI, testJ, testCPC = true;

            boost::tie(edgeI, testI) = edge(nodeI, nodeJ, this->all_graphs[d]);
            boost::tie(edgeJ, testJ) = edge(nodeJ, nodeI, this->all_graphs[d]);
            boost::tie(edgeCPC, testCPC) = edge(vertex(i_nodeA, this->all_CPCgraphs[d]), vertex(i_nodeB, this->all_CPCgraphs[d]), this->all_CPCgraphs[d]); // as CPC graph is symetric one test is sufficient

            // In case there is no longer options, the best operation to do is nothing
            pmGraphOp do_Nothing(vertex(i_nodeA, this->all_graphs[d]), vertex(i_nodeB, this->all_graphs[d]), pmGraphOp::SL_EDGE_NOT);
            pmScoreValueType currentbestScore = compute_indiv_score(do_Nothing, d, mt_Score);

            pmScoreValueType score1, score2, score3, score4;

            //testing adding an edge if it exists in CPC
            if (testCPC == true) {
                if (testI == false) {
                    pmGraphOp op(nodeI, nodeJ, pmGraphOp::SL_EDGE_ADD);
                    if (remainsADAG(this->all_graphs[d], op)) {
                        currentbestScore = compute_indiv_score(op, d, mt_Score);
                        //std::cout << "deltaScore dans CPCtrue TestI false " << deltaScore << endl;
                        if (currentbestScore > bestScore) bestScore = currentbestScore;
                    }
                }

                if (testJ == false) {
                    pmGraphOp op(nodeJ, nodeI, pmGraphOp::SL_EDGE_ADD);
                    if (remainsADAG(this->all_graphs[d], op)) {
                        currentbestScore = compute_indiv_score(op, d, mt_Score);
                        //std::cout << "deltaScore dans CPCtrue TestJ false " << deltaScore << endl;
                        if (currentbestScore > bestScore) bestScore = currentbestScore;
                    }
                }
            }

            if (testI == true) {

                //Reversing//
                pmGraphOp op(nodeI, nodeJ, pmGraphOp::SL_EDGE_REV);
                if (remainsADAG(this->all_graphs[d], op)) {
                    currentbestScore = compute_indiv_score(op, d, mt_Score);
                    if (currentbestScore > bestScore) bestScore = currentbestScore;
                }

                //Removing//
                op = pmGraphOp(nodeI, nodeJ, pmGraphOp::SL_EDGE_REM);
                currentbestScore = compute_indiv_score(op, d, mt_Score);
                if (currentbestScore > bestScore) bestScore = currentbestScore;

            }

            if (testJ == true) {

                //Reversing//
                pmGraphOp op(nodeJ, nodeI, pmGraphOp::SL_EDGE_REV);
                if (remainsADAG(this->all_graphs[d], op)) {
                    currentbestScore = compute_indiv_score(op, d, mt_Score);
                    if (currentbestScore > bestScore) bestScore = currentbestScore;
                }

                //Removing//
                op = pmGraphOp(nodeJ, nodeI, pmGraphOp::SL_EDGE_REM);
                currentbestScore = compute_indiv_score(op, d, mt_Score);
                if (currentbestScore > bestScore) bestScore = currentbestScore;

            }
        }

        double exponent = 1 / (1 + (nb_tasks - 1) * delta);
        bestScore += exponent * log(individual_priors[d]); //bestScore *= pow(individual_priors[d], exponent);

        bestscores.push_back(bestScore);
    }


    return bestscores;

}



// Compute global score when applying operation in all_graphs
double AlgoMT_MMHC::compute_score(int level, std::vector<pmGraphOp_MT> operations_MT, pmMTCSVDataSet& csv_data, pmMTpScore& mt_Score) {

    double score = 0; //the score to be returned  0 or 1 (sum or product)

    int nb_tasks = csv_data.get_nb_datasets();

    //first term in product : prior
    double prior = compute_prior(level, operations_MT, csv_data);
    score += log(prior);

    //second term in product : product of individual scores
    double s_indiv = 0;
    for (int t = 0; t < level; t++) {

        // Adapt MT operation to current task
        pmGraphOp adaptedOp(vertex(0, this->all_graphs[t]), vertex(1, this->all_graphs[t]), pmGraphOp::SL_EDGE_ABS); // fake operation, only to store the information in case at least one of the two nodes doesn't exist in the graph, so it is not concerned by operation
        adapt_operation(adaptedOp, operations_MT[t], t, csv_data);
        // Step to avoid ununderstood bad_alloc error from using directly adaptedOp
        int iA = get(vertex_index, this->all_graphs[t])[adaptedOp.nodeA];
        int iB = get(vertex_index, this->all_graphs[t])[adaptedOp.nodeB];
        pmGraphOp resultOp(vertex(iA, this->all_graphs[t]), vertex(iB, this->all_graphs[t]), adaptedOp.op);
        //std::cerr << " Operation ";
        //resultOp.print(cerr, this->all_graphs[t]);
        //std::cout << endl;

        // Individual score
        double ind_score = compute_indiv_score(resultOp, t, mt_Score);
        //std::cout << "ind_score " << t << " : " << ind_score << endl;
        s_indiv += ind_score;
    }
    score += s_indiv;

    //third term in product in case of partial configuration : product of bestq
    pair<int, int> pair_mt(operations_MT[0].nodeA_indx, operations_MT[0].nodeB_indx);
    std::vector<pmScoreValueType> bestq = compute_bestq(pair_mt, mt_Score, csv_data);

    double s_bestq = 0;
    for (int i = level; i < nb_tasks; i++) {

        s_bestq += bestq[i];
    }

    score += s_bestq;

    return score;
}

// Generate neighbourhood and cuting branches that have lower score that actual best score
double AlgoMT_MMHC::branch_and_bound(std::vector<pmGraphOp_MT>& held_operations, pmMTCSVDataSet& csv_data, pmMTpScore& mt_Score) {

    // Number of tasks = number of learning datasets
    int nb_tasks = csv_data.get_nb_datasets();

    // The initial score is the score of the actual configuration (ie the score with "do nothing" operations)
    pmGraphOp_MT do_nothing_opMT(0, 1, pmGraphOp_MT::SL_EDGE_NOT);
    std::vector<pmGraphOp_MT> nothing_operations(nb_tasks, do_nothing_opMT);
    double bestScore = compute_score(nb_tasks, nothing_operations, csv_data, mt_Score);

    // Parameters only for test
    std::vector<pmGraphOp_MT> previous_held_ops(nb_tasks, do_nothing_opMT);
    //double previous_bestScore = bestScore;

    std::ofstream logfile;
    logfile.open("log.csv", std::ios_base::app);

    for (int i = 0; i < all_poss_pairsOfNodes.size(); i++) {

        int iA = all_poss_pairsOfNodes[i].first;
        int iB = all_poss_pairsOfNodes[i].second;

        logfile << "Boucle pour variable " << iA << " et " << iB << std::endl;

        std::vector<pmGraphOp_MT> possible_MTop = {
        pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_NOT),
        pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_ADD),
        pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_REV),
        pmGraphOp_MT(iA, iB, pmGraphOp_MT::SL_EDGE_REM)
        };

        int nb_possible_op = possible_MTop.size();

        std::vector<int> oPxLevel(nb_tasks); //to store operation history in levels
        std::vector<double> scRxLevel(nb_tasks + 1); //to store score history in levels;
        std::vector<pmGraphOp_MT> current_operations(nb_tasks, do_nothing_opMT); //to store current selected operations

        //scRxLevel[0] = bestScore;
        //bestScore = previous_bestScore;
        int DeltaScore; // int bestDeltaScore;

        int level = 1;

    profondeur:

        int id_op = 0;

    largeur:

        int d = level - 1; // a level corresponds to the position of a tasks, and as level goes from 1 to nb_tasks+1 so d = level - 1
        oPxLevel[d] = id_op;
        current_operations[d] = possible_MTop[id_op];
        //std::cout << "just before compute_score level : " << level << " id_op : " << id_op << endl;
        double score = compute_score(level, current_operations, csv_data, mt_Score);

        //std::cout << "this score " << score << " actual bestScore to compare " << scRxLevel[level - 1] << " delta " << score - scRxLevel[level - 1] << endl;

        logfile << "level " << level << " id_op " << id_op << " score " << lexical_cast<string>(score) << " bestScore to compare with " << lexical_cast<string>(bestScore) << endl;

        DeltaScore = score - bestScore; //scRxLevel[level - 1];

        logfile << "DeltaScore " << lexical_cast<string>(DeltaScore) << endl;

        if (DeltaScore > 0){
            //scRxLevel[level - 1] = score;
            //held_operations[level-1] = possible_MTop[id_op];
            level += 1;
            if (level <= nb_tasks){
                goto profondeur;
            }
            else {
                bestScore = score;
                held_operations = current_operations;
                id_op += 1;
                if (id_op < nb_possible_op){
                    level -= 1;
                    goto largeur;
                }
                else {
                backward1: 
                    level -= 1;
                    if (level == 0)
                        goto breakpoint;
                    else {
                        id_op = oPxLevel[level - 1] + 1; // go back to previous level and try the next operation to try in this level
                        if (id_op < nb_possible_op)
                            goto largeur;
                        else goto backward1;
                    }
                }
            }
        }
        else {
            id_op += 1;
            if (id_op < nb_possible_op)
                goto largeur; // stay in the same level and try the next operation
            else {
            backward2: 
                level -= 1;
                if (level == 0)
                    goto breakpoint;
                else {
                    id_op = oPxLevel[level - 1] + 1; // go back to previous level and try the next operation to try in this level
                    if (id_op < nb_possible_op)
                        goto largeur;
                    else goto backward2;
                }
            }        
        }


    breakpoint:
        continue;

    }

    logfile.close();

    return bestScore;
}

// Multi-task Greedy search
void AlgoMT_MMHC::MT_Greedy(pmMTCSVDataSet& csv_data, pmMTpScore& mt_Score) {

    int nb_tasks = csv_data.get_nb_datasets();

    double bestScore_init;
    double bestScore_current;

    //double delta_score_best = 1;
    double delta_score_current = 1; //to pass the first iteration

    int iter = 0;

    auto start = std::chrono::high_resolution_clock::now();

    while (delta_score_current > 0) {

        std::cout << "======================================================================================" << endl;
        std::cout << "                              iter " << iter << endl;
        std::cout << "======================================================================================" << endl;

        std::ofstream logfile;
        logfile.open("log.csv", std::ios_base::app);
        logfile << "======================================================================================" << endl;
        logfile << "                              iter " << iter << endl;
        logfile << "======================================================================================" << endl;

        //std::cout << "tous les graphs au début de l'iteration" << endl;
        //print_allgraphs();

        pmGraphOp_MT do_nothing_opMT(0, 1, pmGraphOp_MT::SL_EDGE_NOT);
        std::vector<pmGraphOp_MT> best_operations(nb_tasks, do_nothing_opMT);

        bestScore_init = compute_score(nb_tasks, best_operations, csv_data, mt_Score);

        //std::cout << "BestScore init en dehors de branch and bound " << bestScore_init << endl;
        logfile << "BestScore init en dehors de branch and bound " << bestScore_init << endl;

        logfile << "Debut branch and bound ........................................................" << endl;
        logfile.close();

        std::cout << "branch and bound " << endl;
        bestScore_current = branch_and_bound(best_operations, csv_data, mt_Score);
        std::cout << "iter " << iter << " branch and bound DONE !!!" << endl;

        logfile.open("log.csv", std::ios_base::app);
        logfile << "iter " << iter << " branch and bound DONE !!!" << endl;

        std::cout << "Pour verif, bestScore_current: " << bestScore_current << endl;
        logfile << "Pour verif, bestScore_current: " << lexical_cast<string>(bestScore_current) << endl;

        //std::cout << " compute score with best " << mt.compute_score(nb_dataset, best_operations, all_graphs, bestqsrc, csv_data, score_str) << endl;

        delta_score_current = round(bestScore_current - bestScore_init);

        if (delta_score_current <= 0) {
            std::cout << "Le delta score est négatif, c'est fini!" << endl;
            break;
        }
        else {
            std::cout << "............Voici les operations retenues :" << endl;

            for (int t = 0; t < nb_tasks; t++) {

                // Adapt MT operation to current task
                pmGraphOp adaptedOp(vertex(0, this->all_graphs[t]), vertex(1, this->all_graphs[t]), pmGraphOp::SL_EDGE_ABS); // fake operation, only to store the information in case at least one of the two nodes doesn't exist in the graph, so it is not concerned by operation
                this->adapt_operation(adaptedOp, best_operations[t], t, csv_data);
                // Step to avoid ununderstood bad_alloc error from using directly adaptedOp
                int tA = get(vertex_index, this->all_graphs[t])[adaptedOp.nodeA];
                int tB = get(vertex_index, this->all_graphs[t])[adaptedOp.nodeB];
                pmGraphOp resultOp(vertex(tA, this->all_graphs[t]), vertex(tB, this->all_graphs[t]), adaptedOp.op);
                std::cerr << " Operation pour tache " << t << " ";
                resultOp.print(cerr, this->all_graphs[t]);
                std::cout << endl;

                resultOp.apply(this->all_graphs[t]);
            }

        }

        iter++;

    }

    auto finish = std::chrono::high_resolution_clock::now();
    
    std::ofstream logfile;
    logfile.open("log.csv", std::ios_base::app);

    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "MT-GS time execution: " << elapsed.count() << " s\n";
    logfile << "MT-GS Search time execution: " << elapsed.count() << " s\n";

    logfile.close();


}

// Run both MT MMHC and Greedy search 
void AlgoMT_MMHC::run(pmMTCSVDataSet& csv_data, pmMTpScore& mt_Score) {

    run_MT_MMHC_ph1(csv_data);
    MT_Greedy(csv_data, mt_Score);

}



