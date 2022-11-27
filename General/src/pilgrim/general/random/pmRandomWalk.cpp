#include <random>
#include <time.h>
#include <string>
#include <iostream>
#include <vector>
#include "pilgrim/general/random/pmRandomWalk.h"

using namespace boost;
using namespace std;
using namespace PILGRIM;


/*=====CONSTRUCTORS=====*/


// First constructor for chain

pmRandomWalk::pmRandomWalk(pmBayesianNetwork* originalBN, int nb_BNs, vector<int> nb_op, vector<int> nb_rows, double epsilon, bool print_BNs) {
    if (nb_BNs != nb_op.size() || nb_BNs != nb_rows.size()) {
        std::cout << "Error : the number of BNs asked is not equal to the size of operations and data rows vectors." << std::endl;
    }
    else if ((epsilon <= 0) || (epsilon >= 1)) {
        std::cout << "Error : Please enter an epsilon value between 0 and 1" << std::endl;
    }
    else {
        pmBayesianNetwork* firstBN = new pmBayesianNetwork(*originalBN);

        // Push the initial BN as the first BN in the chain
        computedBNs.push_back(firstBN);

        nb_operations = nb_op;
        nb_rows_data = nb_rows;

        string initial_file_name = "randomwalk0.csv";
        firstBN->generate_csvData(initial_file_name, 1000);
        file_names.push_back(initial_file_name);
        

        if (print_BNs) {
            cout << "Network 0 : \n" << endl;
            firstBN->summary();
            cout << "\n";
        }

        // Will be used in the second condition of rejection while loop
        int GS_1_to_0;

        // Run all the random walks
        for (unsigned int i = 0; i < nb_BNs; i++) {
            int GS_to_0;
            int GS_to_pred;
            std::string file_name = "randomwalk" + std::to_string(i + 1) + ".csv";
            pmBayesianNetwork* new_BN;

            // We accept a random walk if the similarity of the dataset generated with the previous one is bigger than the similarity to the initial one
            // to avoid stepping back to the first dataset
            do {
                // Remove the dataset generated in the last do-while loop iteration, which did not match conditions in the while (if it exists)
                std::remove(file_name.c_str());

                // Run a randomwalk starting from the last BN which has been validated
                new_BN = run_one_randomwalk(computedBNs[computedBNs.size() - 1], nb_operations[i]);
                new_BN->generate_csvData(file_name, nb_rows_data[i]);


                // Initial dataset, previous one and current one
                std::vector<std::string> datasets_to_compare = {"randomwalk0.csv", file_names[file_names.size() - 1], file_name};

                pmMTCSVDataSet csv_MTdata(datasets_to_compare);
                pmMTSimilarityMeasures sim_measures(csv_MTdata);

                // At the first iteration, initial file and previous file are both randomwalk0.csv, we don't need to compute global similarities
                if (i == 0) {
                    GS_1_to_0 = sim_measures.get_globsim(0, 2);
                    break;
                }

                // Global similarity between the task generated and initial task (0)
                GS_to_0 = sim_measures.get_globsim(0, 2);

                // Global similarity between the task generated and previous task
                GS_to_pred = sim_measures.get_globsim(1, 2);

            // First condition insure that we are still moving away from the initial dataset
            // Second condition is for not moving away too fast from the initial dataset
            } while ((GS_to_pred <= GS_to_0) || (GS_to_pred > GS_to_0 + (epsilon * GS_1_to_0)));

            if (print_BNs) {
                cout << "Network " << std::to_string(i+1) << " : \n" << endl;
                new_BN->summary();
                cout << "\n";
            }
            
            computedBNs.push_back(new_BN);
            file_names.push_back(file_name);
        }
    }

    // delete all temporary file data created to learn global similarity
    for (int i =0; i < file_names.size(); i++){
        std::remove(file_names[i].c_str());
    }
}



/* 
Second version of the first constructor (in chain) which insures that for each dataset generated, the most similar dataset is the previous one
This version is way more efficient than the previous one in terms of respect of chain principle, but has also probably more runtime 
*/

pmRandomWalk::pmRandomWalk(pmBayesianNetwork* originalBN, int nb_BNs, vector<int> nb_op, vector<int> nb_rows, bool print_BNs) {
    if (nb_BNs != nb_op.size() || nb_BNs != nb_rows.size()) {
        std::cout << "Error : the number of BNs asked is not equal to the size of operations and data rows vectors." << std::endl;
    }
    // else if ((epsilon <= 0) || (epsilon >= 1)) {
    //     std::cout << "Error : Please enter an epsilon value between 0 and 1" << std::endl;
    // }
    else {
        pmBayesianNetwork* firstBN = new pmBayesianNetwork(*originalBN);
        computedBNs.push_back(firstBN);
        nb_operations = nb_op;
        nb_rows_data = nb_rows;

        string initial_file_name = "randomwalk0.csv";
        firstBN->generate_csvData(initial_file_name, 1000);
        file_names.push_back(initial_file_name);

        if (print_BNs) {
            cout << "Network 0 : \n" << endl;
            firstBN->summary();
            cout << "\n";
        }
        
        int GS_1_to_0;

        // Run all the random walks
        for (unsigned int i = 0; i < nb_BNs; i++) {
            int GS_to_0;
            int GS_to_pred;
            int max_GS = 0;
            std::string file_name = "randomwalk" + std::to_string(i + 1) + ".csv";
            pmBayesianNetwork* new_BN;
            
            // We accept a random walk if the similarity of the dataset generated is bigger compared to the previous dataset than initial one
            // to avoid stepping back to the first dataset 
            do {
                max_GS = 0;

                // Remove the dataset generated in the last do-while loop iteration, which did not match conditions in the while (if it exists)
                std::remove(file_name.c_str());

                // Run a randomwalk starting from the last BN which has been validated
                new_BN = run_one_randomwalk(computedBNs[computedBNs.size() - 1], nb_operations[i]);
                new_BN->generate_csvData(file_name, nb_rows_data[i]);

                std::vector<std::string> datasets_to_compare = {"randomwalk0.csv"};

                for (unsigned int j=1; j<i+2; j++) {
                    std::string dataset_name = "randomwalk" + std::to_string(j) + ".csv";
                    datasets_to_compare.push_back(dataset_name);
                }
            

                pmMTCSVDataSet csv_MTdata(datasets_to_compare);
                pmMTSimilarityMeasures sim_measures(csv_MTdata);

                // At the first iteration, initial file and previous file are both randomwalk0.csv, we don't need to compute global similarities
                if (i == 0) {
                    GS_1_to_0 = sim_measures.get_globsim(0, 1);
                    break;
                }
                
                // Find the highest similarity value between current dataset and all previous ones,
                // to use it as a minimum value for the similarity between the current dataset and the previous one
                int sim;
                for (unsigned int j=0; j<i; j++) {
                    sim = sim_measures.get_globsim(j, datasets_to_compare.size() - 1);
                    if (sim > max_GS) max_GS = sim;
                }

                // Global similarity between the task generated and initial task (0)
                //GS_to_0 = sim_measures.get_globsim(0, datasets_to_compare.size() - 1);    // comment cuz already considered in the loop before

                // Global similarity between the task generated and previous task
                GS_to_pred = sim_measures.get_globsim(datasets_to_compare.size() - 2, datasets_to_compare.size() - 1);

            // First condition insure that we are still moving away from the initial dataset
            // Second condition is for not moving away too fast from the initial dataset
            // Third condition is to insure that the most similar dataset is the previous one
            } while (GS_to_pred <= max_GS); //(GS_to_pred <= GS_to_0) || (GS_to_pred > GS_to_0 + (epsilon * GS_1_to_0)) || 

            if (print_BNs) {
                cout << "Network " << std::to_string(i+1) << " : \n" << endl;
                new_BN->summary();
                cout << "\n";
            }

            computedBNs.push_back(new_BN);
            file_names.push_back(file_name);
        }
    }
} 


// Second constructor for clusters
pmRandomWalk::pmRandomWalk(pmBayesianNetwork* originalBN, vector<int> nb_op, vector<int> nb_rows, vector<pair<int, double>> clusters, vector<double> similarities_between_clusters, int delta_target, bool print_BNs) {
    int nb_BNs = 0;
    bool wrong_similarity = false;
    for (auto& cluster: clusters) {
        nb_BNs += cluster.first;

        if (cluster.second > 0.5) wrong_similarity = true;
    }
    for (auto& sim: similarities_between_clusters) {
        if (sim > 0.5) wrong_similarity = true;
    }
    if ((nb_BNs != nb_op.size() + 1) || (nb_BNs != nb_rows.size() + 1) ) {
        std::cout << "Error : the number of BNs asked is not equal to the size of operations and data rows vectors." << std::endl;
    }
    else if ((similarities_between_clusters.size() != clusters.size() -1)) {
        std::cout << "Error : The number of similarities values between clusters has to be equal to the size of clusters - 1." << std::endl;
    }
    else if (wrong_similarity) {
        std::cout << "Error : Similarity ratios values are too high, please enter values lower than 1." << std::endl;
    }

    else {
        // Creating the first BN on heap to be sure to delete it properly in the destructor
        pmBayesianNetwork* firstBN = new pmBayesianNetwork(*originalBN);

        // We also add networks and datasets in one dimension vectors if we want to save them without clusters
        computedBNs.push_back(firstBN);
        nb_operations = nb_op;
        nb_rows_data = nb_rows;

        string initial_file_name = "randomwalk0.csv";
        firstBN->generate_csvData(initial_file_name, 1000);
        file_names.push_back(initial_file_name);

        // Compute the maximum potential similarity to have a starting point for decreasing similarities
        pmMTCSVDataSet initial_data({"randomwalk0.csv", "randomwalk0.csv"});
        pmMTSimilarityMeasures initial_sim_measures(initial_data);
        int GS_max = initial_sim_measures.get_globsim(0, 1);

        int num_iteration = 0;
        int previous_similarity = GS_max;
        double similarity_target;


        // Iterating on the number of clusters
        for (unsigned int i = 0; i < clusters.size(); i++) {

            // Create a new cluster
            vector<pmBayesianNetwork*> cluster;
            vector<string> cluster_tasks;
            int index_starting = 0;

            if (print_BNs) {
                cout << "CLUSTER " << std::to_string(i) << endl;
            }

            // In the first cluster, we add the initial BN/dataset
            if (i == 0) {
                cluster.push_back(firstBN);
                cluster_tasks.push_back(initial_file_name);
                index_starting = 1;
                
                if (print_BNs) {
                    cout << "Network 0 : \n" << endl;
                    firstBN->summary();
                    cout << "\n";
                }
            }

            // Iterating on the number of tasks in each cluster
            for (unsigned int j = index_starting; j < clusters[i].first; j++) {
                num_iteration++;
                int GS_to_0;
                int GS_to_pred;
                std::string file_name = "randomwalk" + std::to_string(num_iteration) + ".csv";
                pmBayesianNetwork* new_BN;
                
                // Testing if we are into a cluster or between 2 clusters
                // We decrease similarity by a ratio of GS_max
                if (cluster.empty()) {
                    similarity_target = previous_similarity - (GS_max * similarities_between_clusters[i - 1]);
                } else {
                    similarity_target = previous_similarity - (GS_max * clusters[i].second);
                }

                do {
                    std::remove(file_name.c_str());

                    new_BN = run_one_randomwalk(computedBNs[computedBNs.size() - 1], nb_operations[i]);

                    new_BN->generate_csvData(file_name, nb_rows_data[i]);

                    std::vector<std::string> datasets_to_compare = {"randomwalk0.csv", file_name};

                    pmMTCSVDataSet csv_MTdata(datasets_to_compare);
                    pmMTSimilarityMeasures sim_measures(csv_MTdata);

                    //Global similarity between the task generated and initial task (0)
                    GS_to_0 = sim_measures.get_globsim(0, 1);

                // We want our dataset to have a similarity equal to target to initial dataset,
                // but we use delta_target as a margin to be more permissive
                } while ((GS_to_0 <= similarity_target - delta_target) || (GS_to_0 >= similarity_target + delta_target));

                previous_similarity = GS_to_0;
                computedBNs.push_back(new_BN);
                cluster.push_back(new_BN);
                cluster_tasks.push_back(file_name);
                file_names.push_back(file_name);

                if (print_BNs) {
                    cout << "Network " << std::to_string(num_iteration) << " : \n" << endl;
                    new_BN->summary();
                    cout << "\n";
                }
            }
            bn_clusters.push_back(cluster);
            task_clusters.push_back(cluster_tasks);
        }
    }
}




/*=====FUNCTIONS=====*/

void pmRandomWalk::update_parameters(pmBayesianNetwork *bn, std::vector<pmNode> modified_nodes, double epsilon_noise) {
    // Recompute randomly parameters of nodes which the number of parents has changed during the randomwalk
    for (auto &node : modified_nodes) {
        plComputableObject *comp_obj = bn->newComputableObject(node, true);
        bn->setComputableObject(node, comp_obj);
    }

    // Adding noise to all parameters
    // We generate a dataset with one row to let the parameter epsilon_noise only control the amount of noise we want to add
    bn->generate_csvData("datatemp.csv", 1);

    std::string file_name = "datatemp.csv";
    char* file_data = new char[file_name.length() + 1];
    strcpy(file_data, file_name.c_str());

    pmCSVDataSet data(file_data);
    bn->noisyConditionals(&data, epsilon_noise);
}



// Second randomwalk version : picks randomly an operation (add, remove, reverse) and finds a random pair of nodes which the operation can be applied on 
pmBayesianNetwork* pmRandomWalk::run_one_randomwalk(pmBayesianNetwork *originalBN, int nb_operations) {

    plVariablesConjunction vars = originalBN->getVariables();

    pmBayesianNetwork* computedBN = new pmBayesianNetwork(vars);

    // Initialize graph with the original one
    computedBN->get_graph() = originalBN->get_graph();
    
    // Number of nodes
    int numberOfNodes = num_vertices(computedBN->get_graph());

    // All possible pairs of nodes
    std::vector<pair<int, int>> all_pairsOfNodes;
    for (int i = 0; i < numberOfNodes; i++) {
        for (int j = 0; j < numberOfNodes; j++) {
            if (i != j) {
                pair<int, int> val(i, j);
                all_pairsOfNodes.push_back(val);
            }
        }
    }

    std::vector<pmGraphOp> operations;

    //Vector to store nodes which their number of parents has been modified during random walk, to update their parameters randomly
    std::vector<pmNode> nodes_to_update;

    // Initialize seed with time value to random 
    struct timeval time;
    gettimeofday(&time, NULL);
    srand((time.tv_sec * 1000) + (time.tv_usec / 1000));

    // Loop to do random operations while the number of desired operations is not achieved or all the possible pairs of nodes have been selected
    while (operations.size() != nb_operations && all_pairsOfNodes.size() != 0) {

        // Select a random operation to do, 0 <=> ADD, 1 <=> REM, 2 <=> REV
        int num_operation = rand() % 2; // 3; here exclude REV operation because wants to add or remove dependencies, by reversing an edge, the dependency still exists

        std::vector<pmGraphOp::op_type> operation_types = { pmGraphOp::SL_EDGE_ADD, pmGraphOp::SL_EDGE_REM, pmGraphOp::SL_EDGE_REV };

        bool testA, testB, adding_test, removing_test, reversing_test = false;
        std::vector<bool> tests = { false, false, false };

        int pair_idx;
        pair<int, int> currentNodePair;

        int node_idxA, node_idxB;
        pmNode nodeA, nodeB;
        pmEdge edgeA, edgeB;

        int iteration = 0;

        // Loop to find a pair of nodes which matches with the operation selected
        while (tests[num_operation] == false) {
            
            // We change the operation if the selected one is impossible
            // to avoid being stuck in the while loop, if there is no edge to delete for example
            if (iteration == all_pairsOfNodes.size()) {
                iteration = 0;
                num_operation = (num_operation + 1) % 3;
            }

            // Select random pair of nodes to apply a possible operation
            pair_idx = rand() % (all_pairsOfNodes.size());
            currentNodePair = all_pairsOfNodes[pair_idx];

            node_idxA = currentNodePair.first;
            node_idxB = currentNodePair.second;

            nodeA = vertex(node_idxA, computedBN->get_graph());
            nodeB = vertex(node_idxB, computedBN->get_graph());

            // Variables association to desired edges
            boost::tie(edgeA, testA) = edge(nodeA, nodeB, computedBN->get_graph());
            boost::tie(edgeB, testB) = edge(nodeB, nodeA, computedBN->get_graph());

            // Recomputing adding, removing and reversing tests
            // For adding and reversing tests, we need to test first if there is an edge before testing if it remains a DAG after adding an edge
            adding_test = ((testA == false) && (testB == false));
            removing_test = ((testA == true) || (testB == true));
            reversing_test = ((testA == true) || (testB == true));

            if (adding_test) {
                pmGraphOp op_temp(nodeA, nodeB, pmGraphOp::SL_EDGE_ADD);
                adding_test = (remainsADAG(computedBN->get_graph(), op_temp));
            }
            if (reversing_test) {
                pmGraphOp op_temp(nodeA, nodeB, pmGraphOp::SL_EDGE_REV);
                if (testB) {
                    op_temp.nodeA = nodeB;
                    op_temp.nodeB = nodeA;
                }
                reversing_test = (remainsADAG(computedBN->get_graph(), op_temp));
            }

            tests = { adding_test, removing_test, reversing_test };
            iteration++;
        }

        pmGraphOp op(nodeA, nodeB, operation_types[num_operation]);
        // Vector to temporarily store nodes to push to nodes_to_update
        std::vector<pmNode> nodes_with_modified_parents;

        // To decide which node we will update according to the operation
        switch(num_operation) {
            case 0:
                nodes_with_modified_parents.push_back(nodeB);
                break;

            case 1:
                if (testB) {
                    op.nodeA = nodeB;
                    op.nodeB = nodeA;
                    nodes_with_modified_parents.push_back(nodeA);
                } else {
                    nodes_with_modified_parents.push_back(nodeB);
                }
                break;

            case 2:
                if (testB) {
                    op.nodeA = nodeB;
                    op.nodeB = nodeA;
                }
                nodes_with_modified_parents.push_back(nodeA);
                nodes_with_modified_parents.push_back(nodeB);
                break;
        }
        
        // Add the nodes we want to update to the vector we will give to update_parameters function
        for (auto& node: nodes_with_modified_parents) {
            if (std::find(nodes_to_update.begin(), nodes_to_update.end(), node) == nodes_to_update.end()) nodes_to_update.push_back(node);
        }

        operations.push_back(op);
        op.apply(computedBN->get_graph());
        all_pairsOfNodes.erase(all_pairsOfNodes.begin() + pair_idx);
    }

    update_parameters(computedBN, nodes_to_update, 10000);
    return(computedBN);
}


/*=====DESTRUCTOR=====*/

pmRandomWalk::~pmRandomWalk() {
    for (auto &bn : computedBNs) {
        delete(bn);
    }
}


std::vector<std::vector<pmBayesianNetwork*>> pmRandomWalk::get_bn_clusters() {
    if (bn_clusters.empty()) {
        std::cout << "Error : You haven't specified any clusters before running pmRandomWalk";
    } else {
        return(bn_clusters);
    }
}

std::vector<std::vector<std::string>> pmRandomWalk::get_task_clusters() {
    if (bn_clusters.empty()) {
        std::cout << "Error : You haven't specified any clusters before running pmRandomWalk";
    } else {
        return(task_clusters);
    }
}

std::vector<pmBayesianNetwork*> pmRandomWalk::get_all_bns() {
    return(computedBNs);
}

std::vector<std::string> pmRandomWalk::get_all_tasks() {
    return(file_names);
}


/*
=====UTILS=====
*/
// Methods copied with no modification from AlgoGS
bool pmRandomWalk::remainsADAG(const pmNode& nodeInit, const pmNode& node, const pmGraph& graph) {
    graph_traits<pmGraph>::adjacency_iterator vi, vi_end;
    boost::tie(vi, vi_end) = adjacent_vertices(node, graph);
    for (; vi != vi_end; ++vi)
    {
        if ((*vi == nodeInit) || (!remainsADAG(nodeInit, *vi, graph)))
            return false;
    }

    return true;
}

bool pmRandomWalk::remainsADAG(pmGraph& graph, const pmGraphOp& op){

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


void pmRandomWalk::save_computedBNs(std::string dir_path, std::string bn_prefix_name){

    std::stringstream ss_out;

    for (int i = 1; i < computedBNs.size(); i++){

        ss_out.str("");
        ss_out << dir_path << "/" << bn_prefix_name << "_bn" << i-1 << ".xdsl";
        std::string bn_out = ss_out.str();
        computedBNs[i]->save_as_genie(bn_out);

    }

}