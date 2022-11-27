/*==============================================================================
 * Product        : PILGRIM
 * File           : pmRandomWalk.h
 * Author         :

 * Creation       : February 2021
 * WARINING /!\	  : STATUS ON PROGRESS



  *=============================================================================

 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *==============================================================================
 */

#ifndef _LoadRandomWalk_H
#define _LoadRandomWalk_H

#include "pilgrim/general/pmBayesianNetwork.h"
#include "pilgrim/general/pmPDAG.h"
#include "pilgrim/general/pmMTSimilarityMeasures.h"
#include <pilgrim/general/pmUtility.h>
#include <random>
#include <time.h>
#include <string>
#include <iostream>
#include <vector>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/dag_shortest_paths.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <chrono>
#include <ostream>
#include <pilgrim/general/algorithms/Algorithm.h>
#include <pilgrim/general/scores/DecomposableScore.h>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>

/* using namespace boost;
using namespace std;
using namespace PILGRIM; */

struct pmGraphOp
{
    pmNode nodeA;
    pmNode nodeB;
    enum op_type
    {
        SL_EDGE_INVALID = 0,
        SL_EDGE_ADD,
        SL_EDGE_REM,
        SL_EDGE_REV
    } op;

    pmGraphOp(pmNode nodeA_, pmNode nodeB_, op_type op_)
        : nodeA(nodeA_), nodeB(nodeB_), op(op_) {}

    pmGraphOp() : nodeA(), nodeB(), op(SL_EDGE_INVALID) {}

    void apply(pmGraph& graph) const
    {
        // added by Yasin
        pmEdge e;
        bool edgeAdded;
        property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);

        switch (op)
        {
        case SL_EDGE_ADD:
            boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
            put(weightMap, e, 2); // added by YASIN
            break;
        case SL_EDGE_REV:
            remove_edge(nodeA, nodeB, graph);
            boost::tie(e, edgeAdded) = add_edge(nodeB, nodeA, graph);
            put(weightMap, e, 2); // added by YASIN
            break;
        case SL_EDGE_REM:
            remove_edge(nodeA, nodeB, graph);
            break;
        default:
            throw std::runtime_error("invalid operation in Glutton Search");
        }
    }

    void unapply(pmGraph& graph) const
    {
        // added by Yasin
        pmEdge e;
        bool edgeAdded;
        property_map<pmGraph, edge_weight_t>::type weightMap = get(edge_weight, graph);

        switch (op)
        {
        case SL_EDGE_ADD:
            remove_edge(nodeA, nodeB, graph);
            break;
        case SL_EDGE_REV:
            remove_edge(nodeB, nodeA, graph);
            boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
            put(weightMap, e, 2); // added by YASIN
            break;
        case SL_EDGE_REM:
            boost::tie(e, edgeAdded) = add_edge(nodeA, nodeB, graph);
            put(weightMap, e, 2); // added by YASIN
            break;
        default:
            throw std::runtime_error("invalid operation in Glutton Search");
        }
    }

    std::ostream& print(std::ostream& out, const pmGraph& g) const
    {
        switch (op)
        {
        case SL_EDGE_ADD:
            out << "ADD";
            break;
        case SL_EDGE_REM:
            out << "REM";
            break;
        case SL_EDGE_REV:
            out << "REV";
            break;
        default:
            throw std::runtime_error("invalid operation in Glutton Search");
        }
        out << "(" << get(vertex_index, g)[nodeA]
            << "->" << get(vertex_index, g)[nodeB]
            << ")";
        return out;
    }
};

namespace PILGRIM {
    /**
     * @class pmRandomWalk
     * @brief Class for a Random Walk (generating several similars datasets)
     */

	class pmRandomWalk {

	public:
		/*=====Attributs=====*/
        std::vector<pmBayesianNetwork*> computedBNs;
        std::vector<std::string> file_names;
        std::vector<std::vector<pmBayesianNetwork*>> bn_clusters; // To store clusters of BNs if asked in the constructor call
        std::vector<std::vector<std::string>> task_clusters; // To store clusters of tasks if asked in the constructor call
        vector<int> nb_operations; // Number of operations to do on each BN generated
        vector<int> nb_rows_data; // Number of rows to generate for each data set

		/*
		=====CONSTRUCTORS=====
		*/


        /**
         * @brief First constructor which runs randomwalks to create networks and datasets from an initial network
         * 
         * @param originalBN the initial BN
         * @param nb_BNs the number of BNs we want to create
         * @param nb_op a vector which describes the number of operations we want to do to create each BN (in order)
         * @param nb_rows a vector which describes the number of rows we want in each dataset generated (in order)
         * @param epsilon a limit for decreasing similarity at each iteration (ratio from 0 to 1) : If epsilon is close to 1, pmRandomWalk will be very permissive. If it is close to 0, it will be very restrictive and the datasets generated will move away from initial dataset very slowly
         * @param print_BNs with a true value, prints summaries of networks generated during randomwalk
         */
        pmRandomWalk(pmBayesianNetwork* originalBN, int nb_BNs, vector<int> nb_op, vector<int> nb_rows, double epsilon, bool print_BNs=false);

        // Second version of the first constructor with more constraints in the similarity in the chain
        pmRandomWalk(pmBayesianNetwork* originalBN, int nb_BNs, vector<int> nb_op, vector<int> nb_rows, bool print_BNs=false);

        /**
         * @brief Second constructor which creates clusters of tasks
         * 
         * @param originalBN the initial BN
         * @param nb_op a vector which describes the number of operations we want to do to create each BN (in order)
         * @param nb_rows a vector which describes the number of rows we want in each dataset generated (in order)
         * @param clusters a vector which represents clusters of tasks we want to create : first number of the pair is the number of BNs in the cluster, second one represents the decrease of global similarity compared to initial dataset we want to have between the tasks of this cluster, given as a ratio between 0 and 0.5
         * @param similarities_between_clusters a vector which represents the decrease of similarity compared to initial dataset we want to have between the last task of a cluster and the first task of the next one, given as a ratio between 0 and 0.5
         * @param delta_target an integer which is the margin to enlarge or reduce the similarity target range : a high delta_target will be more permissive, and a lower value will be more restrictive
         * @param print_BNs with a true value, prints summaries of networks generated during randomwalk
         */
        pmRandomWalk(pmBayesianNetwork* originalBN, vector<int> nb_op, vector<int> nb_rows, vector<pair<int, double>> clusters, vector<double> similarities_between_clusters, int delta_target, bool print_BNs=false);


        /*
        =====DESTRUCTOR=====
        */
        ~pmRandomWalk();

        /*
        =====FUNCTIONS=====
        */

        /**
         * @brief a function called in both constructors which generates one bayesian network with randomWalk process : does random operations on the initial network, then recomputes randomly parameters of nodes with modified parents, and add some noise to all parameters
         * 
         * @param originalBN the previous network, on which we do some operations to create a new network
         * @param nb_operations the number of operations to apply on the network to create the new one
         */
        pmBayesianNetwork* run_one_randomwalk(pmBayesianNetwork *originalBN, int nb_operations);

        /**
         * @brief a function used to update parameters after doing operations on a network, recomputes randomly parameters tables of nodes with modified parents and use noisyConditionals to add noise to all parameters
         * 
         * @param bn the bn with parameters to update
         * @param nodes_with_modified_parents nodes with parameters tables to recompute randomly
         * @param epsilon_noise the second parameter given to noisyConditionals : as first parameter we give a dataset with one row, so the denominator of sc value in noisyConditionals function will be equal to this epsilon_noise. A high epsilon_noise value will add a little of noise, a low value will add more noise
         */
        void update_parameters(pmBayesianNetwork *bn, std::vector<pmNode> nodes_with_modified_parents, double epsilon_noise);


        /*
        =====GETTERS=====
        */
        std::vector<std::vector<pmBayesianNetwork*>> get_bn_clusters();
        std::vector<std::vector<std::string>> get_task_clusters();
        std::vector<pmBayesianNetwork*> get_all_bns();
        std::vector<std::string> get_all_tasks();
        
		/*
		=====UTILS=====
		*/
		// Methods copied with no modification from AlgoGS
		bool remainsADAG(const pmNode& nodeInit,
			const pmNode& node, const pmGraph& graph);

		bool remainsADAG(pmGraph& graph, const pmGraphOp& op);

        // Save computed bns in xdsl format
        void save_computedBNs(std::string dir_path, std::string bn_prefix_name);

	};
};


#endif