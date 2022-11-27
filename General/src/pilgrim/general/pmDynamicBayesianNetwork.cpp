/*=============================================================================
 * Product        : PILGRIM
 * File           : DynamicBayesianNetwork.h
 * Author         : GHADA TRABELSI
 * Creation       : Fev 01 2013

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM- all rights reserved
 *=============================================================================
 */
#include "pilgrim/general/pmDynamicBayesianNetwork.h"


//#include "pilgrim/general/DMMHC.h"
//#include "pilgrim/general/algorithms/AlgoDMMHC.h"
//#include "pilgrim/general/algorithms/AlgoMMHC.h"

#ifndef PILGRIM_DEBUG
    #define PILGRIM_DEBUG false
#endif

using namespace boost;
using namespace std;
using namespace PILGRIM;

//=============================================================================
PILGRIM::pmDynamicBayesianNetwork::pmDynamicBayesianNetwork() {}

PILGRIM::pmDynamicBayesianNetwork::
    pmDynamicBayesianNetwork(pmBayesianNetwork* G0, pmBayesianNetwork* G1, InferenceAlgorithms algo) {
    this->initial_G    = G0;
    this->transition_G = G1;
    this->algorithm    = algo;
}
//create a DBN from a CSV file
//pmDynamicBayesianNetwork::pmDynamicBayesianNetwork(char* filenameDBN_T0, char* filenameDBN, size_t entete_T0, size_t entete, string score_name_T0, string score_name, int algo){
//	//Fichier csv G0
//	plCSVDataDescriptor desc_T0(filenameDBN_T0, plVariablesConjunction(), false, false);
//	plVariablesConjunction varCSV_T0 = desc_T0.guess_variables(filenameDBN_T0);
//	size_t rowDBN_T0(varCSV_T0.size());
//	pmCSVDataSet* dataDBN_T0 = new pmCSVDataSet(filenameDBN_T0, entete_T0, rowDBN_T0, ';', varCSV_T0);
//	pmBayesianNetwork *empty_BNT0 = new pmBayesianNetwork(varCSV_T0);
//
//
//	//fichier csv G_T
//	plCSVDataDescriptor desc(filenameDBN, plVariablesConjunction(), false, false);
//	plVariablesConjunction varCSV = desc.guess_variables(filenameDBN);
//	size_t rowDBN(varCSV.size());
//	pmCSVDataSet* dataDBN = new pmCSVDataSet(filenameDBN, entete, rowDBN, ';', varCSV);
//	pmBayesianNetwork *empty_BNT = new pmBayesianNetwork(varCSV);
//
//	cout << "-------------------Algo MMHC-------------------" << endl;
//	AlgoMMHC construct_T0 = AlgoMMHC(dataDBN_T0);
//	construct_T0.setScoreName(score_name_T0);
//	pmBayesianNetwork *G0 = construct_T0.createMMHCGraph(-1, empty_BNT0);
//	pmCSVDataDescriptor* pmDD_T0 = new pmCSVDataDescriptor(filenameDBN_T0, G0->getVariables());
//	G0->learnParametersML(pmDD_T0);
//
//	cout << "-------------------Algo DMMHC-------------------" << endl;
//	//test algo DMMHC
//
//	DMMHC * construct = new DMMHC(dataDBN);
//	construct->setScoreName(score_name);
//	pmBayesianNetwork *G_t = construct->createDMMHCGraph(-1, empty_BNT, "test");
//
//	pmCSVDataDescriptor* pmDD = new pmCSVDataDescriptor(filenameDBN, G_t->getVariables());
//	G_t->learnParametersML(pmDD);
//
//	this->initial_G = G0;
//	this->transition_G = G_t;
//	this->algorithm = algo;
//}

/*pmDynamicBayesianNetwork::pmDynamicBayesianNetwork(const pmDynamicBayesianNetwork& other) {
  this->initial_G = other.initial_G;
  this->transition_G = other.transition_G;
  this->algorithm = other.algorithm;
}*/

PILGRIM::pmDynamicBayesianNetwork::
pmDynamicBayesianNetwork(const std::string& file_name, const std::string& jd0_label, const std::string& jdt_label, int algo) {
    // Maybe this works? Should try it
    //this = load_from_xml(file_name, jd0_label, jdt_label);

    plSerializer serial_load("serial_l");

    // Load bayesian network
    plJointDistribution loaded_jd0;
    plJointDistribution loaded_jdT;

    // Create and write in serialization file
    std::ifstream serialFile(file_name);
    if (serialFile)
    {
        try{
            serial_load.load(file_name);
            serial_load.get_object(jd0_label, loaded_jd0);
            serial_load.get_object(jdt_label, loaded_jdT);
        }

        catch (const std::exception & e)
        {
            std::cerr << " Error while loading from serialisation file : " << e.what();
        }
    }
    else
    {
        std::cerr << " Error : cannot read serialization file .\n ";
    }

    // Creation of the bayesian network
    this->initial_G     = new pmBayesianNetwork(loaded_jd0);
    this->transition_G  = new pmBayesianNetwork(loaded_jdT);

    if (algo == 0)
        this->algorithm = InferenceAlgorithms::Unroll_Static;
    else
        this->algorithm = InferenceAlgorithms::Interface;
}

pmDynamicBayesianNetwork::~pmDynamicBayesianNetwork() {
    //  this->initial_G->~pmBayesianNetwork();
    //  this->transition_G->~pmBayesianNetwork();
}

/*****************************************************************************/
const plVariablesConjunction& pmDynamicBayesianNetwork::getVariablesforG0() {
    return (initial_G->getVariables());
}
/*****************************************************************************/
const plVariablesConjunction& pmDynamicBayesianNetwork::
    getVariablesforGtransition() {
    return (transition_G->getVariables());
}
/*****************************************************************************/
plJointDistribution pmDynamicBayesianNetwork::getJointDistributionforG0() {
    plJointDistribution jd = initial_G->getJointDistribution();
    return jd;
}
/*****************************************************************************/
plJointDistribution pmDynamicBayesianNetwork::
    getJointDistributionforGtransition() {
    plJointDistribution jd = transition_G->getJointDistribution();
    return jd;
}
/*****************************************************************************/
pmBayesianNetwork& pmDynamicBayesianNetwork::getTransition_G() {
    return *transition_G;
}
/*****************************************************************************/
pmBayesianNetwork& pmDynamicBayesianNetwork::getInitial_G() {
    return *initial_G;
}
/*****************************************************************************/
void pmDynamicBayesianNetwork::setAlgorithm(InferenceAlgorithms algo) {
    this->algorithm = algo;
}
/*****************************************************************************/
pmBayesianNetwork* PILGRIM::pmDynamicBayesianNetwork::unroll(int t) {
    plVariablesConjunction vars              = initial_G->getVariables();
    plComputableObjectList computableObjects = initial_G
                                                   ->getComputableObjectList();
    for(int i = 1; i <= t; i++) {
        // Get all the variables of the transition graph
        vector<pmNode> nodes_topo = vector<pmNode>();
        transition_G->orderNodes(transition_G->get_graph(), nodes_topo, true);

        for(vector<pmNode>::iterator i_node = nodes_topo.begin();
            i_node != nodes_topo.end();
            ++i_node) {
            plVariable variable  = transition_G->getVariable(*i_node);
            string     node_name = variable.get_names()[0];
            // If variable is a node from the previous iteration, ignore it
            if(node_name.length() > 2 && node_name.substr(node_name.length() - 2) == "_t") {
                // get the computable object
                plComputableObject* tmp_compObj = transition_G
                                                      ->getComputableObject(*i_node);
                // Get the parents from the computable object
                vector<plVariable> parents = vector<plVariable>();
                tmp_compObj->get_right_variables().get_all_variables(parents);
                // Reconstruct a new plvariable for each parent
                plVariablesConjunction new_parents = plVariablesConjunction();
                for(vector<plVariable>::iterator i_parent = parents.begin();
                    i_parent != parents.end();
                    ++i_parent) {
                    // distinguish each parent type
                    string       parent_name = i_parent->get_names()[0];
                    stringstream ss;
                    if(parent_name.length() > 2 && parent_name.substr(parent_name.length() - 2) == "_t") {
                        ss << i;
                        parent_name += ss.str();
                    } else {
                        ss << i - 1;
                        parent_name += "_t" + ss.str();
                    }
                    // Find parents
                    new_parents = new_parents ^ vars.get_variable_with_name(parent_name);
                }
                // and the current node
                stringstream ss2;
                ss2 << i;
                string     new_name = node_name + ss2.str();
                plVariable new_node = plVariable(new_name, variable.get_type());
                // Get the probability distribution (tabulate)
                vector<plProbValue> values = vector<plProbValue>();
                tmp_compObj->tabulate(values);
                // Construct the new distribution table with the new plVariables and the
                // corresponding probabilities
                // add the new variable to the variable conjunction
                vars = vars ^ new_node;
                // distinguish probTable and distributionTable
                if(new_parents.size() == 0) {
                    plProbTable probsT = plProbTable(new_node, values);
                    // add the new distribution table to the computable object list
                    computableObjects = computableObjects * probsT;
                } else {
                    plDistributionTable probsD = plDistributionTable(new_node,
                                                                     new_parents,
                                                                     values);
                    // add the new distribution table to the computable object list
                    computableObjects = computableObjects * probsD;
                }
            }
        }
    }
    plJointDistribution jd(vars, computableObjects);
    pmBayesianNetwork*  bn = new pmBayesianNetwork(jd);
    // Creation and print of the bayesian network
    return bn;
}
/*****************************************************************************/
pmBayesianNetwork* PILGRIM::pmDynamicBayesianNetwork::unroll2(int t) {
    plVariablesConjunction vars = initial_G->getVariables();
    plComputableObjectList computableObjects;
    computableObjects *= initial_G->getComputableObjectList();

    // Get all the variables of the transition graph
    vector<pmNode> nodes_topo = vector<pmNode>();
    transition_G->orderNodes(transition_G->get_graph(), nodes_topo, true);

    for(int i = 1; i <= t; i++) {
        for(vector<pmNode>::iterator i_node = nodes_topo.begin(); i_node != nodes_topo.end(); ++i_node) {
            plVariable variable  = transition_G->getVariable(*i_node);
            string     node_name = variable.get_names()[0];
            // If variable is a node from the previous iteration, ignore it
            if(node_name.length() > 2 && node_name.substr(node_name.length() - 2) == "_t") {
                // get the computable object
                plComputableObject tmp_compObj(*(transition_G->getComputableObject(*i_node)));

                // Get the parents from the computable object
                vector<plVariable> parents = vector<plVariable>();

                tmp_compObj.get_right_variables().get_all_variables(parents);
                // Reconstruct a new plvariable for each parent
                plVariablesConjunction new_parents = plVariablesConjunction();
                for(vector<plVariable>::iterator i_parent = parents.begin();
                    i_parent != parents.end();
                    ++i_parent) {
                    // distinguish each parent type
                    string       parent_name = i_parent->get_names()[0];
                    stringstream ss;
                    if(parent_name.length() > 2 && parent_name.substr(parent_name.length() - 2) == "_t") {
                        ss << i;
                        parent_name += ss.str();
                    } else {
                        ss << i - 1;
                        parent_name += "_t" + ss.str();
                    }
                    // Find parents
                    new_parents = new_parents ^ vars.get_variable_with_name(parent_name);
                }
                // and the current node
                stringstream ss2;
                ss2 << i;
                string     new_name = node_name + ss2.str();
                plVariable new_node = plVariable(new_name, variable.get_type());

                //std::cout << tmp_compObj << std::endl;
                tmp_compObj.rename(new_node ^ new_parents);
                //std::cout << tmp_compObj << std::endl;
                //plComputableObject tmp_compObj3 (tmp_compObj2.rename(new_node^new_parents));

                // Construct the new distribution table with the new plVariables and the
                // corresponding probabilities
                // add the new variable to the variable conjunction
                vars                                    = vars ^ new_node;
                plComputableObjectList computableObject = plComputableObjectList(tmp_compObj);
                computableObjects *= computableObject;
            }
        }
    }
    plJointDistribution jd(vars, computableObjects);
    pmBayesianNetwork*  bn = new pmBayesianNetwork(jd);
    // Creation and print of the bayesian network
    return bn;
}
/*****************************************************************************/

plDistribution pmDynamicBayesianNetwork::
    query(plVariable&       target_variable,
          int               instant,
          vector<plValues>* evidenceVector) {
    bool debug         = false;
    int  evidence_size = evidenceVector->size();
    int  no_steps      = max(instant, evidence_size); //for how long we will unroll the model

    pmBayesianNetwork* unrolled = this->unroll(no_steps);

    if(debug) {
        std::cout << "Unrolled Bayesian Network" << std::endl;
        std::cout << "-------------------------" << std::endl;
        unrolled->summary();
    }

    plVariablesConjunction unrolledVarConj = unrolled
                                                 ->getJointDistribution()
                                                 .get_variables();

    // we identify in the unrolled network the plVariable corresponding to the target_variable
    stringstream target_step_suffix_ss;

    //target_variable name can finish (or not) with "_t" (is optional)
    string tgt_var_name        = target_variable.name();
    int    namesize_of_tgt_var = tgt_var_name.size();
    if(namesize_of_tgt_var > 2 && tgt_var_name.substr(tgt_var_name.length() - 2) == "_t") {
        target_step_suffix_ss << instant;
    } else {
        target_step_suffix_ss << "_t" << instant;
    }

    string     target_step_suffix = target_variable.name() + target_step_suffix_ss.str();
    plVariable unrolled_target_variable =
        unrolledVarConj.get_variable_with_name(target_step_suffix);
    if(debug) {
        std::cout << "unrolled_target_variable = "
                  << unrolled_target_variable.name()
                  << std::endl;
    }

    // we "translate" the evidence vector using the plVariables from the unrolled
    // network
    plValues unrolledEvidence = plValues();
    // for each instant t for which plValues contains at least a value
    int cstep = 0;
    for(vector<plValues>::iterator i_evidence = evidenceVector->begin();
        i_evidence != evidenceVector->end();
        ++i_evidence) {
        if(!i_evidence->is_empty()) {

            plVariablesConjunction cVarConj = i_evidence->get_variables();
            plDataValues           cValues  = i_evidence->values();
            vector<string>         cnames   = cVarConj.get_names();

            stringstream step_suffix_ss;

            int namesize_of_cnames_0 = cnames[0].size();
            if(namesize_of_tgt_var > 2 && cnames[0].substr(namesize_of_cnames_0 - 2) == "_t") {
                step_suffix_ss << cstep;
            } else {
                step_suffix_ss << "_t" << cstep;
            }
            string step_suffix = "" + step_suffix_ss.str();


            // for each value
            for(int i = 0; i < cnames.size(); i++) {
                string c_new_name = cnames[i] + step_suffix;
                // we identify the corresponding variable from the unrolled network
                plVariable c_variable = unrolledVarConj.get_variable_with_name(c_new_name);
                // we construct a new plValues with the variable from the unrolled
                // network and the value from the evidence
                plValues c_new_plValue    = plValues(c_variable);
                c_new_plValue[c_variable] = cValues[i];
                // we add the contructed plValues to the unrolled evidence
                unrolledEvidence = unrolledEvidence ^ c_new_plValue;
            }
        }
        cstep++;
    }

    if(debug) {
        std::cout << "unrolledEvidence = "
                  << unrolledEvidence.to_string()
                  << std::endl;
    }

    return unrolled->query(unrolled_target_variable, unrolledEvidence);
}

/*****************************************************************************/
plDistribution pmDynamicBayesianNetwork::query2(plVariable&       target_variable,
                                                int               instant,
                                                vector<plValues>* evidenceVector) {
    bool debug         = false;
    int  evidence_size = evidenceVector->size();
    int  no_steps      = max(instant, evidence_size); //for how long we will unroll the model

    pmBayesianNetwork* unrolled = this->unroll2(no_steps);

    if(debug) {
        std::cout << "Unrolled Bayesian Network" << std::endl;
        std::cout << "-------------------------" << std::endl;
        unrolled->summary();
    }

    plVariablesConjunction unrolledVarConj = unrolled
                                                 ->getJointDistribution()
                                                 .get_variables();

    // we identify in the unrolled network the plVariable corresponding to the target_variable
    stringstream target_step_suffix_ss;

    //target_variable name can finish (or not) with "_t" (is optional)
    string tgt_var_name        = target_variable.name();
    int    namesize_of_tgt_var = tgt_var_name.size();
    if(namesize_of_tgt_var > 2 && tgt_var_name.substr(tgt_var_name.length() - 2) == "_t") {
        target_step_suffix_ss << instant;
    } else {
        target_step_suffix_ss << "_t" << instant;
    }

    string     target_step_suffix = target_variable.name() + target_step_suffix_ss.str();
    plVariable unrolled_target_variable =
        unrolledVarConj.get_variable_with_name(target_step_suffix);
    if(debug) {
        std::cout << "unrolled_target_variable = "
                  << unrolled_target_variable.name()
                  << std::endl;
    }

    // we "translate" the evidence vector using the plVariables from the unrolled
    // network
    plValues unrolledEvidence = plValues();
    // for each instant t for which plValues contains at least a value
    int cstep = 0;
    for(vector<plValues>::iterator i_evidence = evidenceVector->begin();
        i_evidence != evidenceVector->end();
        ++i_evidence) {
        if(!i_evidence->is_empty()) {

            plVariablesConjunction cVarConj = i_evidence->get_variables();
            plDataValues           cValues  = i_evidence->values();
            vector<string>         cnames   = cVarConj.get_names();

            stringstream step_suffix_ss;

            int namesize_of_cnames_0 = cnames[0].size();
            if(namesize_of_tgt_var > 2 && cnames[0].substr(namesize_of_cnames_0 - 2) == "_t") {
                step_suffix_ss << cstep;
            } else {
                step_suffix_ss << "_t" << cstep;
            }
            string step_suffix = "" + step_suffix_ss.str();


            // for each value
            for(int i = 0; i < cnames.size(); i++) {
                string c_new_name = cnames[i] + step_suffix;
                // we identify the corresponding variable from the unrolled network
                plVariable c_variable = unrolledVarConj.get_variable_with_name(c_new_name);
                // we construct a new plValues with the variable from the unrolled
                // network and the value from the evidence
                plValues c_new_plValue    = plValues(c_variable);
                c_new_plValue[c_variable] = cValues[i];
                // we add the contructed plValues to the unrolled evidence
                unrolledEvidence = unrolledEvidence ^ c_new_plValue;
            }
        }
        cstep++;
    }

    if(debug) {
        std::cout << "unrolledEvidence = "
                  << unrolledEvidence.to_string()
                  << std::endl;
    }
    return unrolled->query(unrolled_target_variable, unrolledEvidence);
    //return unrolled->queryJT(unrolled_target_variable, unrolledEvidence);
}

/*****************************************************************************/

vector<plComputableObjectList> pmDynamicBayesianNetwork::prediction(vector<plVariablesConjunction> targets_lists, vector<int> instants_to_ask, vector<plValues> all_evidences) {

    vector<plComputableObjectList> result;
    const unsigned int             size_of_instants = instants_to_ask.size();
    const unsigned int             size_of_targets  = targets_lists.size();

    const std::string log_prefix = "[pmDynamicBayesianNetwork::prediction()] - ";

    // Check si on a autant de listes de targets que d'instants à questionner
    if(size_of_instants != size_of_targets) {
        std::string error_message = "The number of lists of targets and the number of asked instants are not the same : ";
        error_message += "size_of_instants is " + std::to_string(size_of_instants) + " and size_of_targets is " + std::to_string(size_of_targets);
        throw new std::invalid_argument(log_prefix + error_message);
    }
    // Check si les instants (et donc les targets) ne sont pas vides
    if(size_of_instants == 0) {
        throw new std::invalid_argument(log_prefix + "The number of questions can't be null : size_of_instants is " + std::to_string(size_of_instants));
    }

    if(PILGRIM_DEBUG) {
        std::cout << log_prefix << "Will apply inference with :" << std::endl;
        std::cout << "\t- Algorithm : " << std::to_string(this->algorithm) << " ;" << std::endl;
        std::cout << "\t- Instants : ";
        pmUtility::output_vector(instants_to_ask);
        std::cout << " ;" << std::endl;
        std::cout << "\t- Targets : ";
        pmUtility::output_vector(targets_lists);
        std::cout << " ;" << std::endl;
        std::cout << "\t- Evidences : ";
        pmUtility::output_vector(all_evidences);
        std::cout << " ;" << std::endl;
    }

    switch(this->algorithm) {
        case Unroll_Static: {
            // Create unrolled static network
            unsigned int       unrolled_length = instants_to_ask[size_of_instants - 1];
            pmBayesianNetwork* unrolled        = this->unroll(unrolled_length);

            // Initialize vectors for transformed evidences and targets
            vector<plValues>               sets_of_unrolled_evidences(unrolled_length + 1, plValues()); // Index start at 0
            vector<plVariablesConjunction> sets_of_unrolled_targets(size_of_targets, plVariablesConjunction());

            // Transform transition evidences (with or without _t) to evidences of the unrolled network
            for(unsigned int i = 0; i < all_evidences.size(); i++) {
                for(unsigned int j = 0; j < all_evidences[i].size(); j++) {
                    plVariable  current_ev_variable = all_evidences[i].get_variables()[j];
                    std::string evidence_static_name;

                    if(i == 0) {
                        evidence_static_name = current_ev_variable.name();
                    } else {
                        // Find index of _t to differentiate interface variables with _t variables
                        std::size_t index_t = current_ev_variable.name().rfind("_t");
                        // If end with _t
                        if(index_t == current_ev_variable.name().length() - 2) {
                            evidence_static_name = current_ev_variable.name() + to_string(i);
                        } else {
                            // Else, correspond to interface of transition model, meaning, t-1
                            evidence_static_name = current_ev_variable.name() + "_t" + to_string(i - 1);
                        }
                    }
                    plValues temp_value = plValues().add(unrolled->getVariables().get_variable_with_name(evidence_static_name), all_evidences[i][j]);
                    for(unsigned int t = i; t >= i && t < sets_of_unrolled_evidences.size(); t++) {
                        sets_of_unrolled_evidences[t] ^= temp_value;
                    }
                }
            }
            // Transform transition targets (with or without _t) to targets of the unrolled network
            for(int i = 0; i < targets_lists.size(); i++) {
                for(int j = 0; j < targets_lists[i].size(); j++) {
                    std::string target_static_name = targets_lists[i][j].name() + to_string(instants_to_ask[i]);
                    plVariable  temp_variable(unrolled->getVariables().get_variable_with_name(target_static_name));
                    sets_of_unrolled_targets[i] ^= temp_variable;
                }
            }

            // ------- Start of inference --------
            plJunctionTree junction_tree      = plJunctionTree(unrolled->getJointDistribution());
            unsigned int   next_instant_index = 0;

            // For each time step t = [0;T]
            for(unsigned int t = 0; t <= unrolled_length; t++) {
                // Results for the current time step
                plComputableObjectList result_t;

                // If we don't have any question at 't', we pass this step
                if(instants_to_ask[next_instant_index] != t)
                    continue;

                plVariablesConjunction targets_t = sets_of_unrolled_targets[next_instant_index];
                if(!all_evidences.empty()) {
                    plValues evidences_until_t = sets_of_unrolled_evidences[t];

                    // For each target of the current time step
                    for(auto target : targets_t) {
                        // Get original target from parameters with target of unrolled network
                        std::size_t found                = target.name().rfind("_t");
                        std::string original_target_name = target.name().substr(0, found) + "_t";
                        plVariable  original_target      = targets_lists[t].get_variable_with_name(original_target_name);

                        if(evidences_until_t.get_variables().contains(target)) {
                            // If the current target is also an evidence, recreate the table from the plValues
                            vector<plProbValue> table;
                            unsigned int        value_index;
                            target.get_type().get_value_index(value_index, evidences_until_t[target]);

                            for(unsigned int v = 0; v < target.get_type().cardinality(); v++) {
                                table.push_back(v == value_index);
                            }

                            result_t.push_back(plDistribution(original_target, table));
                        } else {
                            // This is an equivalent of pmBayesianNetwork::query() but with the Junction Tree algorithm
                            // Ask the model for the targets at step 't' knowing all evidences until 't'
                            plCndDistribution cnd_dist = junction_tree.ask(target, evidences_until_t.get_variables());

                            // Compute the distribution with the evidences
                            plDistribution instanciated_dist;
                            cnd_dist.instantiate(instanciated_dist, evidences_until_t);
                            plDistribution compiled_dist;
                            instanciated_dist.compile(compiled_dist);

                            // Push the results with the original target (_t)
                            vector<plProbValue> probas;
                            compiled_dist.tabulate(probas);
                            result_t.push_back(plDistribution(original_target, probas));
                        }
                    }

                } else {
                    // If no evidences AT ALL (from 0 to T) ask targets directly
                    // For each target of the current time step
                    for(auto target : targets_t) {
                        // Get original target from parameters with target of unrolled network
                        std::size_t found                = target.name().rfind("_t");
                        std::string original_target_name = target.name().substr(0, found) + "_t";
                        plVariable  original_target      = targets_lists[t].get_variable_with_name(original_target_name);

                        // Ask the Junction Tree and compute the probabilities
                        plDistribution dist = junction_tree.ask(target);
                        plDistribution compiled_dist;
                        dist.compile(compiled_dist);

                        // Push the results with the original target (_t)
                        vector<plProbValue> probas;
                        compiled_dist.tabulate(probas);
                        result_t.push_back(plDistribution(original_target, probas));
                    }
                }

                // Move cursor to next instant and push the results
                next_instant_index++;
                result.push_back(result_t);
            }
            break;
        }
        case Interface: {
            vector<plVariablesConjunction> targets_lists_from_t1   = targets_lists;
            vector<int>                    instants_to_ask_from_t1 = instants_to_ask;
            // Si on veut interroger le modèle à la tranche 0, il faut interroger un BN statique et non pas un 1.5DBN
            if(instants_to_ask[0] == 0) {
                pmBayesianNetwork      BN0           = this->getInitial_G();
                plVariablesConjunction variables_BN0 = BN0.getVariables();
                plComputableObjectList answers;

                // Transformation des evidences _t en _t0
                plValues evidences_t0;
                for(unsigned int i = 0; i < all_evidences[0].size(); i++) {
                    evidences_t0.push_back(plValues().add(all_evidences[0].get_variables()[i], all_evidences[0][i]));
                }

                for(auto target : targets_lists[0]) {
                    plVariable target_t0 = variables_BN0.get_variable_with_name(target.name() + "0");

                    if(evidences_t0.get_variables().contains(target_t0)) {
                        // If the current target is also an evidence, recreate the table from the plValues
                        vector<plProbValue> table;
                        unsigned int        value_index;
                        target_t0.get_type().get_value_index(value_index, evidences_t0[target_t0]);

                        for(unsigned int v = 0; v < target_t0.get_type().cardinality(); v++) {
                            table.push_back(v == value_index);
                        }

                        answers.push_back(plDistribution(target_t0, table));
                    } else {
                        answers.push_back(BN0.query(target_t0, evidences_t0));
                    }
                }
                result.push_back(answers);

                // Suppression des targets à t0 et de l'instant 0 des variables pour pm15dBN
                targets_lists_from_t1   = vector<plVariablesConjunction>(targets_lists.begin() + 1, targets_lists.end());
                instants_to_ask_from_t1 = vector<int>(instants_to_ask.begin() + 1, instants_to_ask.end());
            }

            // Création du 1.5DBN et application des évidences à t0
            pm15dBN* interfaceDBN = new pm15dBN(this, all_evidences[0]);

            vector<plValues>               all_evidences_from_t1 = vector<plValues>(all_evidences.begin() + 1, all_evidences.end());
            vector<plComputableObjectList> anwsers_from_t1       = interfaceDBN->predict(targets_lists_from_t1, instants_to_ask_from_t1, all_evidences_from_t1);

            result.insert(result.end(), anwsers_from_t1.begin(), anwsers_from_t1.end());
            break;
        }
        default: {
            throw new std::logic_error(log_prefix + "The selected algorithm (" + std::to_string(this->algorithm) + ") has not been implemented for this function yet.");
        }
    }
    return result;
}
/*****************************************************************************/
pmDynamicBayesianNetwork* pmDynamicBayesianNetwork::
    load_from_genie(const std::string& fileNameG0,
                    const std::string& fileNameG1) {
    pmDynamicBayesianNetwork* DBN = new pmDynamicBayesianNetwork();

    // create initial model
    plBayesianNetwork* BN0 = new plBayesianNetwork();
    BN0->load_from_genie(fileNameG0);
    DBN->initial_G = new pmBayesianNetwork(*BN0);
    // create transition model
    plBayesianNetwork* BN = new plBayesianNetwork();
    BN->load_from_genie(fileNameG1);
    DBN->transition_G = new pmBayesianNetwork(*BN);

    return DBN;
}
/*****************************************************************************/
pmDynamicBayesianNetwork* pmDynamicBayesianNetwork::
    load_from_netica(const std::string& fileNameG0,
                     const std::string& fileNameG1) {
    pmDynamicBayesianNetwork* DBN = new pmDynamicBayesianNetwork();

    // create initial model
    plBayesianNetwork* BN0 = new plBayesianNetwork();
    BN0->load_from_netica(fileNameG0);
    DBN->initial_G = new pmBayesianNetwork(*BN0);
    // create transition model
    plBayesianNetwork* BN = new plBayesianNetwork();
    BN->load_from_netica(fileNameG1);
    DBN->transition_G = new pmBayesianNetwork(*BN);

    return DBN;
}
/*****************************************************************************/
pmDynamicBayesianNetwork* pmDynamicBayesianNetwork::
    load_from_hugin(const std::string& fileNameG0,
                    const std::string& fileNameG1) {
    pmDynamicBayesianNetwork* DBN = new pmDynamicBayesianNetwork();

    // create initial model
    plBayesianNetwork* BN0 = new plBayesianNetwork();
    BN0->load_from_hugin(fileNameG0);
    // pmBayesianNetwork* BNs= new pmBayesianNetwork(*BN0);
    DBN->initial_G = new pmBayesianNetwork(*BN0);
    // create transition model
    plBayesianNetwork* BN = new plBayesianNetwork();
    BN->load_from_hugin(fileNameG1);
    DBN->transition_G = new pmBayesianNetwork(*BN);

    return DBN;
}

void pmDynamicBayesianNetwork::save_as_xml(const std::string& file,
					 std::string jd0_name,
					 std::string jdT_name) {
  
  plJointDistribution jd0 = this->getJointDistributionforG0();
  plJointDistribution jdT = this->getJointDistributionforGtransition();
  // Serialization
  std::string ser_save = "serial_s";
  std::string bn_jd0 = jd0_name;
  std::string bn_jdT = jdT_name;
  plSerializer serial_save(ser_save);

  // Add objects to save
  serial_save.add_object(bn_jd0, jd0);
  serial_save.add_object(bn_jdT, jdT);

  // Add the Learning Objects to save
  vector<plLearnObject *> vect_lo_d0 = this->getInitial_G().getLearnObjectList();
  if (vect_lo_d0[0] != 0)
  {
    std::string bn_lo_d0 = jd0_name + " Learn Object";
    bn_lo_d0[0] = std::toupper(bn_lo_d0[0]);
    // Bug ici: vect_lo_d0[LO, vide, vide, vide]
    // Pas content è_é
    plLearnDistributions lo_d0 = plLearnDistributions(vect_lo_d0, jd0.get_variables());
    serial_save.add_object(bn_lo_d0, lo_d0);
  }

  vector<plLearnObject *> vect_lo_dT = this->getTransition_G().getLearnObjectList();
  if (vect_lo_dT[0] != 0)
  {
    std::string bn_lo_dT = jdT_name + " Learn Object";
    bn_lo_dT[0] = std::toupper(bn_lo_dT[0]);
    plLearnDistributions lo_dT = plLearnDistributions(vect_lo_dT, jdT.get_variables());
    serial_save.add_object(bn_lo_dT, lo_dT);
  }

  std::ofstream serialFile(file);
  if (serialFile)
  {
      try {
          serial_save.save(serialFile);
      }
      catch (const std::exception & e)
      {
          std::cerr << " Error while writing in serialization file : " << e.what();
      }
  }
  else
  {
      std::cerr << " Error : cannot create serialization file (check your path).\n ";
  }
}
/*****************************************************************************/
void pmDynamicBayesianNetwork::save_as_genie(const std::string& fileNameG0,
                                             const std::string& fileNameG1) {
    // save initial model
    plJointDistribution jd0 = getJointDistributionforG0();
    plBayesianNetwork*  BN0 = new plBayesianNetwork(jd0, "GO");
    BN0->save_as_genie(fileNameG0);

    // save transition model
    plJointDistribution jd1 = getJointDistributionforGtransition();
    plBayesianNetwork*  BN  = new plBayesianNetwork(jd1, "G1");
    BN->save_as_genie(fileNameG1);
}
/*****************************************************************************/
void pmDynamicBayesianNetwork::save_as_netica(const std::string& fileNameG0,
                                              const std::string& fileNameG1) {
    // save initial model
    plJointDistribution jd0 = getJointDistributionforG0();
    plBayesianNetwork*  BN0 = new plBayesianNetwork(jd0, "GO");
    BN0->save_as_netica(fileNameG0);

    // save transition model
    plJointDistribution jd1 = getJointDistributionforGtransition();
    plBayesianNetwork*  BN  = new plBayesianNetwork(jd1, "G1");
    BN->save_as_netica(fileNameG1);
}
/*****************************************************************************/
void pmDynamicBayesianNetwork::save_as_hugin(const std::string& fileNameG0,
                                             const std::string& fileNameG1) {
    // save initial model
    plJointDistribution jd0 = getJointDistributionforG0();
    plBayesianNetwork*  BN0 = new plBayesianNetwork(jd0, "GO");
    BN0->save_as_hugin(fileNameG0);

    // save transition model
    plJointDistribution jd1 = getJointDistributionforGtransition();
    plBayesianNetwork*  BN  = new plBayesianNetwork(jd1, "G1");
    BN->save_as_hugin(fileNameG1);
}
/*****************************************************************************/
void pmDynamicBayesianNetwork::summary(bool displayLearnObject) {
  cout << endl << "---------------------- G initial: ----------------------" << endl;
  initial_G->summary(displayLearnObject);
  cout << endl << "---------------------- G transition: ----------------------" << endl;
  transition_G->summary(displayLearnObject);
}

vector<plVariable> pmDynamicBayesianNetwork::getOutgoingInterface(){

    vector<plVariable> outgoingInterface;
    plVariablesConjunction vars = this->transition_G->getVariables();
    vector<string> varsNames = vars.get_names();

    string cname;
    string ending = "_t";

    for(std::vector<string>::iterator it = varsNames.begin(); it != varsNames.end(); ++it) {
        cname = *it;
        //si le nom est plus long que "_t"
        if(cname.length() > 2) {
            //si le nom ne finit pas par "_t"
            if(0 != cname.compare(cname.length() - ending.length(), ending.length(), ending)) {
                //alors il fait partie de l'outgoing interface
                outgoingInterface.push_back(vars.get_variable_with_name(cname));
            }
        } else {
            outgoingInterface.push_back(vars.get_variable_with_name(cname));
        }
    }
    return outgoingInterface;
}

pmBayesianNetwork& pmDynamicBayesianNetwork::simpleGT()
{
    string cname;
    string ending = "_t";
    pmEdgeIterator ei, ei_end;
    pmEdge e_t;

    pmBayesianNetwork G_T = this->getTransition_G();
    plVariable c_Variable;
    pmNode c_Node;
    plVariablesConjunction temp_VC;
    vector<plComputableObject*> temp_vect_COL(0);
    plComputableObjectList      temp_COL;
    plComputableObjectList      COL_T = G_T.getComputableObjectList();

    for(unsigned int i = 0; i < G_T.getVariables().size(); i++) {
        c_Variable  = G_T.getVariables()[i];
        c_Node      = G_T.getNode(c_Variable);
        string name = c_Variable.name();
        if(G_T.getChildren(c_Node).size() > 0 || 0 == c_Variable.name().compare(c_Variable.name().length() - ending.length(), ending.length(), ending)) {
            temp_VC = temp_VC ^ c_Variable;
            temp_COL.push_back(COL_T[i]);
        }
    }

    plJointDistribution jdt(temp_VC, temp_COL);
    pmBayesianNetwork*  bn_tmp = new pmBayesianNetwork(jdt);
    return *bn_tmp;
}

void pmDynamicBayesianNetwork::serializeDBN(const string &file, string jd0_name, string jdT_name){

    plJointDistribution jd0 = this->getJointDistributionforG0();
    plJointDistribution jdT = this->getJointDistributionforGtransition();
    // Serialization
    std::string  ser_save = "serial_s";
    std::string  bn_jd0   = jd0_name;
    std::string  bn_jdT   = jdT_name;
    plSerializer serial_save(ser_save);

    // Add objects to save
    serial_save.add_object(bn_jd0, jd0);
    serial_save.add_object(bn_jdT, jdT);
    std::ofstream serialFile(file);

    if(serialFile) {
        try {
            serial_save.save(serialFile);
        } catch(const std::exception& e) {
            std::cerr << " Error while writing in serialization file : " << e.what();
        }
    } else {
        std::cerr << " Error : cannot create serialization file .\n ";
    }
}

void pmDynamicBayesianNetwork::serialize(plSerializer& serializer, const std::string& jd0_name, const std::string& jdT_name) {
    plJointDistribution jd0 = this->getJointDistributionforG0();
    plJointDistribution jdT = this->getJointDistributionforGtransition();

    // Serialization
    std::string ser_save = "serial_s";

    // Add objects to save
    serializer.add_object(jd0_name, jd0);
    serializer.add_object(jdT_name, jdT);
}

pmDynamicBayesianNetwork* pmDynamicBayesianNetwork::load_from_xml(
    const std::string& file_name,
    std::string        jd0,
    std::string        jdT) {
    // Create Serializer
    plSerializer serial_load("serial_l");

    // Load bayesian network
    plJointDistribution loaded_jd0;
    plJointDistribution loaded_jdT;
    std::string         jd0_name = jd0;
    std::string         jdT_name = jdT;
    // Create and write in serialization file
    std::ifstream serialFile(file_name);
    if(serialFile) {
        try {
            serial_load.load(file_name);
            serial_load.get_object(jd0_name, loaded_jd0);
            serial_load.get_object(jdT_name, loaded_jdT);
        }

        catch(const std::exception& e) {
            std::cerr << " Error while loading from zerialisation file : " << e.what();
        }
    } else {
        std::cerr << " Error : cannot read serialization file .\n ";
    }

    // Creation of the bayesian network
    pmBayesianNetwork* G_0_serialized = new pmBayesianNetwork(loaded_jd0);
    pmBayesianNetwork* G_T_serialized = new pmBayesianNetwork(loaded_jdT);

    pmDynamicBayesianNetwork* DBN_serialized = new pmDynamicBayesianNetwork(G_0_serialized, G_T_serialized);

    return DBN_serialized;
}

void pmDynamicBayesianNetwork::generate_csv_data(const std::string& dataset_filename, unsigned int t_max, int nbrOfSample, bool num_sequence, int num_time_slice) {
    plDistribution G0 = this->getInitial_G().getJointDistribution().ask(this->getInitial_G().getVariables());

    pm15dBN temp15DBN(this);

    temp15DBN.generate_csv_samples_Data(dataset_filename, t_max, nbrOfSample, G0, num_sequence, num_time_slice);
}

vector<vector<vector<plVariable>>> pmDynamicBayesianNetwork::get_Linked_Vars() {
    pmBayesianNetwork      TransitionG = this->getTransition_G();
    plVariablesConjunction AllNode     = TransitionG.getVariables();
    plVariablesConjunction parents;
    plVariablesConjunction children;
    string                 cname;
    vector<plVariable>     Var;
    vector<plVariable>     Var_t;
    plVariablesConjunction AlllinkedVar;
    //[parent1,parent2, ...]
    vector<plVariable> linkedList;
    //[node,parent_t1,parent_t2, ...]
    vector<plVariable> linkedList_t;
    //[[parentsList_t],[parentsList]]
    vector<vector<plVariable>> ordered_linked;
    //[[ordered_linked node1], [ordered_linked node2], ...]
    vector<vector<vector<plVariable>>> finalLinkedVars;
    plVariable                         current;
    for(int i = 0; i < AllNode.size(); i++) {
        current = AllNode[i];
        cname   = current.name();
        if(cname.length() > 2) {
            if(0 != cname.compare(cname.length() - 2, 2, "_t")) {
                Var.push_back(current);
            } else {
                Var_t.push_back(current);
            }
        } else {
            Var.push_back(current);
        }
    }
    for(int i = 0; i < Var_t.size(); i++) {
        current      = Var_t[i];
        parents      = TransitionG.getParents(TransitionG.getNode(current));
        children     = TransitionG.getChildren(TransitionG.getNode(current));
        AlllinkedVar = parents;
        linkedList.clear();
        linkedList_t.clear();
        ordered_linked.clear();
        linkedList_t.push_back(current);
        for(int j = 0; j < AlllinkedVar.size(); j++) {
            current = AlllinkedVar[j];
            cname   = current.name();
            if(cname.length() > 2) {
                if(0 != cname.compare(cname.length() - 2, 2, "_t")) {
                    linkedList.push_back(current);
                } else {
                    linkedList_t.push_back(current);
                }
            } else {
                linkedList.push_back(current);
            }
        }
        ordered_linked.push_back(linkedList_t);
        ordered_linked.push_back(linkedList);
        finalLinkedVars.push_back(ordered_linked);
    }
    return finalLinkedVars;
}


vector<vector<vector<plVariable>>> pmDynamicBayesianNetwork::get_max_clique(vector<vector<vector<plVariable>>> finalLinkedVars) {

    vector<vector<plVariable>> cliqueParents;
    //[[[clique1],[parentsClique1]],[[clique2],[parentsClique2]],...]
    vector<vector<vector<plVariable>>> all_cliquesParents;


    while(finalLinkedVars.size() != 0) {
        int max_size     = 0;
        int var_max_size = 0;
        cliqueParents.clear();
        for(int i = 0; i < finalLinkedVars.size(); i++) {
            if(max_size < finalLinkedVars[i][0].size()) {
                max_size     = finalLinkedVars[i][0].size();
                var_max_size = i;
            }
        }
        vector<vector<plVariable>> current_linked_var = finalLinkedVars[var_max_size];
        finalLinkedVars.erase(finalLinkedVars.begin() + var_max_size);
        plVariable current_var = current_linked_var[0][0];
        if(max_size != 1) {
            //cout << current_var.name() << endl;
            for(int i = 0; i < finalLinkedVars.size(); i++) {
                bool merge = false;
                for(int k = 0; k < finalLinkedVars[i][0].size(); k++) {
                    for(int j = 0; j < current_linked_var[0].size(); j++) {
                        cout << finalLinkedVars[i][0][k].name() << endl;
                        cout << current_linked_var[0][j].name() << endl;
                        if(finalLinkedVars[i][0][k].name() == current_linked_var[0][j].name()) {
                            merge = true;
                            k     = finalLinkedVars[i][0].size();
                            j     = current_linked_var.size();
                        }
                    }
                }
                if(merge) {
                    for(int k = 0; k < finalLinkedVars[i][0].size(); k++) {
                        bool inserer = true;

                        for(unsigned int l = 0; l < current_linked_var[0].size(); l++) {
                            if(current_linked_var[0][l].name() == finalLinkedVars[i][0][k].name()) {
                                inserer = false;
                            }
                        }
                        if(inserer) {
                            current_linked_var[0].push_back(finalLinkedVars[i][0][k]);
                        }
                    }
                    for(int k = 0; k < finalLinkedVars[i][1].size(); k++) {
                        bool inserer = true;

                        for(unsigned int l = 0; l < current_linked_var[1].size(); l++) {
                            if(current_linked_var[1][l].name() == finalLinkedVars[i][1][k].name()) {
                                inserer = false;
                            }
                        }
                        if(inserer) {
                            current_linked_var[1].push_back(finalLinkedVars[i][1][k]);
                        }
                    }
                    finalLinkedVars.erase(finalLinkedVars.begin() + i);
                    i--;
                }
            }
        }

        cliqueParents.push_back(current_linked_var[0]);
        cliqueParents.push_back(current_linked_var[1]);
        all_cliquesParents.push_back(cliqueParents);
    }
    return all_cliquesParents;
    //TransitionG.getParents(TransitionG.getNode(current));
}

void pmDynamicBayesianNetwork::clique_to_petri(vector<vector<vector<plVariable>>> all_cliquesParents) {
}
