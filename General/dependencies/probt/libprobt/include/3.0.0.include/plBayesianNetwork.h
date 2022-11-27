/// \file plBayesianNetwork.h
/*===========================================================================
 * Product        : ProBT
 * File           : plBayesianNetwork.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLBAYESIANNETWORK_H
#define PLBAYESIANNETWORK_H

#include "plConfig.h"

#include <utility>
#include <vector>

#include <plValues.h>
#include <plVariablesConjunction.h>
#include <plDistribution.h>
#include <plCndDistribution.h>
#include <plComputableObject.h>
#include <plLearnObject.h>
#include <plProbTable.h>
#include <plVariableIndexer.h>

SPL_DECL2(plBayesianNetwork);

class plVariable;
class plJointDistribution;
class plJunctionTree;
class plLearnDistributions;

class plQuery;
class plTimeSliceQuery;

class plPredictionPerformanceReport;

// Doxygen bug: prevent broken link to plBayesianNetwork::set_inference_algorithm().
/** Inference algorithm to be used in %plBayesianNetwork::set_inference_algorithm(). */
enum plInferenceAlgorithm {
  PL_JT, /*!<  "Junction Tree" algorithm. */
  PL_SR /*!<   "Successive Restrictions" algorithm (a kind of variable elimination). */
};

/** Scores to be used for structure learning */
enum plStructureLearningScore {
  PL_MI,      /*!< Mutual Information edge score. */
  PL_MI_DIST, /*!< Normalized metric variant of the Mutual Information edge score: D(X,Y) = I(X,Y) / H(X,Y). */
  PL_ENTROPY, /*!< Entropy edge score. */
  PL_BIC,     /*!< Bayesian Information Criterion (node and edge score). */
  PL_AIC,     /*!< Akaike's Information Criterion (node and edge score). */
  //PL_BDEU,    /*!< Bayesian Dirichlet Equivalent Uniform (node and edge score). */
  PL_MDL      /*!< Minimum Description Length (node and edge score). */
};

//================================

/** Class for building a Bayesian network and using it for exact inference using discrete
    variables (continuous variables are allowed only when used as evidence ones).

    This class provides a simple and high-level API for building/querying Bayesian networks.
    More advanced functionalities are provided by the low-level ProBT API.

    Transitioning from this class to the low-level ProBT API is easy.
    Consider the following example using plBayesianNetwork:
    @code
    double a_vals[] = {1, 3.6, 5};
    plVariable a("A", plDiscreteIntervalType(3, a_vals));
    plVariable b("B", plIntegerType(0, 10));
    plBayesianNetwork net;
    net.add_node(a);
    net.add_node(b);
    net.set_distribution(plUniform(a));
    net.set_distribution(plCndNormal(b, 5, a));
    plValues evidence(b);
    evidence[b] = 2;
    net.set_evidence(evidence);
    // P(A = 5 | B = 2)
    std::cout << net.get_belief(a).compute(5) << std::endl;
    @endcode

    This example can be replicated with the low-level and
    more powerful ProBT API:
    @code
    double a_vals[] = {1, 3.6, 5};
    plVariable a("A", plDiscreteIntervalType(3, a_vals));
    plVariable b("B", plIntegerType(0, 10));
    plJointDistribution j(a ^ b,
    plUniform(a) * plCndNormal(b, 5, a));
    plValues evidence(b);
    evidence[b] = 2;
    // P(A = 5 | B = 2)
    std::cout << j.ask(a, b).instantiate(evidence).compute(5) << std::endl;   
    @endcode
*/
class PL_DLL_API plBayesianNetwork : public plObject
{
 public:
  /** An edge list as a vector of node pairs (source, destination) */
  typedef std::vector< std::pair<plVariable, plVariable> > bn_edge_list_t;

 protected:
  SPL_ACCESS2(plBayesianNetwork);
  plValues evidence_;
  plVariablesConjunction all_variables_;

  std::map<plVariable, plLearnObject*> decomposition_learnable_;
  std::string name_;
  plInferenceAlgorithm inference_algorithm_;

  mutable plJointDistribution* joint_;
  mutable plJunctionTree* jt_;

  mutable bool invalidated_decomposition_;

  bool verbose_sl_;
  bool verbose_em_;

  bool compiled_belief_distribution_;

  std::vector<plTimeSliceQuery*> time_slice_queries_;
  mutable std::map<plVariablesConjunction, plQuery*> query_cache_;
  unsigned int query_cache_max_size_;

  plLearnDistributions *non_em_learner_;

  bool enable_sanity_checking_;

  plComputableObjectList soft_evidences_;

  plOptimizationCriterion inference_optimization_criterion_;

  plFloat em_convergence_threshold_;
  
 protected:
  const plDistribution &get_belief_internal(plVariablesConjunction const & node_conjunction,
                                            bool force_no_compilation)const;
  const std::vector<plProbValue> &get_belief_table_internal(plVariablesConjunction const & node_conjunction)const;
  void update_decomposition()const;
  void decomposition(plComputableObjectList&) const;

  std::vector<plLearnObject*> get_learn_objects()const;

  void replace_distribution(const plVariable &node,
                            plLearnObject *learnable_distribution);

  void replace_learn_objects(const std::vector<plLearnObject*> &new_learn_objects);

  /** Outputs a human-readable representation of the network.
   */
  void Output(std::ostream & os) const;

  void replace_structure(const bn_edge_list_t &edges);

  bn_edge_list_t get_all_class_to_attrib_edges(const plVariable &class_node) const;

  plFloat compute_data_record_log_likelihood_internal_wo_check(const plValues &defined_data_record) const;

  void set_evidence_internal(const plValues& evidence);

  void ask(const plVariablesConjunction &vars,
           plDistribution &response)const;
  
  void ask(const plVariablesConjunction &left_vars,
           const plVariablesConjunction &right_vars,
           plCndDistribution &cnd_response,
           bool ignore_normalization)const;

  const plDistribution &get_belief_internal(plQuery &query,
                                            bool force_no_compilation)const;

  const std::vector<plProbValue> &get_belief_table_internal(plQuery &q) const;

  void clear_query_cache() const;
  void clear_time_slice_queries();

  void create_time_slice_query(const plVariable&node_prec, 
                               const plVariable &node_current);

  void create_time_slice_query_for_learnable(const plVariable&node_prec, 
                                             const plVariable &node_current);

  void finalize_time_slices_copy(bool delete_old=false);

  void set_from_decomposition(const plComputableObjectList &decomposition);

  void update_joint() const;

 public:

  /** Builds an empty Bayesian network.
      Add nodes to it using add_node().
      Simultaneously add links and set the corresponding
      probability distribution using set_distribution().
      Insert evidence using set_evidence().
      Get a belief corresponding to this last evidence
      using get_belief() (inference/propagation in the network is
      made automatically for you when needed).
  */
  explicit plBayesianNetwork(const std::string &name = "untitled_BN");

  /** Builds a Bayesian network from a plJointDistribution object.
   */
  explicit plBayesianNetwork(const plJointDistribution& joint,
                             const std::string &name = "untitled_BN");
  
  /** Builds a Bayesian network from a plComputableObjectList object.
      'variables' must be the concatenation of all the variables used in the passed computable object list. 
      If empty, it will be constructed from the passed computable object list
  */
  explicit plBayesianNetwork(const plComputableObjectList &decomposition,
                             const plVariablesConjunction &variables = plVariablesConjunction(),
                             const std::string &name = "untitled_BN");
  
  /** Copy constructor */
  plBayesianNetwork(const plBayesianNetwork& other);

  /** Destructor */
  ~plBayesianNetwork();
  
  
  /** Assignment operator */
  plBayesianNetwork& operator=(const plBayesianNetwork& other);

  /** Same as operator=() */
  plBayesianNetwork& assign_from(const plBayesianNetwork& other)
  {
    return operator=(other);
  }


  /** Enable/disable sanity checking
     
      Disabling sanity checking allows speeding up the computation (for huge networks for example).
      Sanity checking is enabled by default. It is always enabled in debug mode (DEBUG defined)

      \sa sanity_checking_enabled()
  */
  void enable_sanity_checking(bool enable_it)
  {
    enable_sanity_checking_ = enable_it;
  }

  /** Get sanity checking status
      
      Return true iif:
      - enable_sanity_checking() has been set to true OR
      - DEBUG is defined

      \sa enable_sanity_checking()
  */
  inline bool sanity_checking_enabled() const
  {
#ifdef DEBUG
    return true;
#else
    return enable_sanity_checking_;
#endif
  }


  /** Clear the network and set its nodes */
  void set_nodes(const plVariablesConjunction &nodes);

  /** Clear the network and set its nodes */
  void set_nodes(const std::vector<plVariable> &nodes);

  /** Add a node with name \em name and of type \em type */
  void add_node(const std::string& name, const plType& type);

  /** Declares a node in the network. The node is described
      by a plVariable object, which represents a variable.
  */
  void add_node(const plVariable& node);

  /** Removes the node @em node if exists */
  void remove_node(const plVariable& node);
  
  /** Removes the node with the name @em name if exists */
  void remove_node(const std::string& name);

  /** Sets the distribution associated to a given node.

      The distribution itself specifies the network link:
      - its left variable is the node to which the distribution is attached
      - its right variables are the parent nodes.

      If the distribution is non-conditional (in ProBT terminology, a plDistribution),
      this also establishes the fact that the node has no parents.

      For instance,
      @code
      net.set_distribution(plUniform(a));
      @endcode
      specifies that node @em a has no parents, and that the prior distribution
      on @em a is a uniform one.

      If the distribution is conditional (a plCndDistribution), its right variables
      become the parents of the node.

      For instance,
      @code
      net.set_distribution(plCndNormal(b, a, 1));
      @endcode
      specifies that node @em b has @em a for only parent, and that its associated
      conditional probability distribution is a Normal (Gaussian)
      centered on the value of @em a, and of constant standard deviation equal to one.
  */
  void set_distribution(const plComputableObject& distribution);

  /** Sets the distribution associated to a given node to be a learnable distribution.
      You may learn all learnable distributions of a network using learn_parameters().
      Within the same network, you may freely mix learnable and non-learnable distributions.
      
      For instance:
      @code
      plVariable a("A", plIntegerType(0, 42));
      plVariable b("B", plIntegerType(-2, 2));
      plVariable c("C", plIntegerType(-1, 1));
      plBayesianNetwork net;
      net.add_node(a);
      net.add_node(b);
      net.add_node(c);
      plLearnLaplace learn_a(a);
      net.set_distribution(learn_a);
      plCndLearnObject<plLearnNormal> learn_b_k_a(b, a);
      net.set_distribution(learn_b_k_a);
      plCndLearnObject<plLearnNormal> learn_c_k_b(c, b);
      net.set_distribution(learn_c_k_b);
      net.learn_parameters("data.csv");
      std::cout << net << std::endl;
      @endcode
  */
  void set_distribution(const plLearnObject& learnable_distribution);

    /** Add an edge to the network
      
      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()

      Changing the parents of a given node changes its default distribution of the node as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)
  */
  void add_edge(const plVariable& source, const plVariable& destination);

  /** Add an edge to the network
      
      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()

      Changing the parents of a given node changes its default distribution of the node as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)
  */
  void add_edge(const std::string& source_name, const std::string& destination_name);


  /** Remove an edge from the network 
      
      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()
      
      Changing the parents of a given node changes its default distribution of the node as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)
  */
  void remove_edge(const plVariable& source, const plVariable& destination);

  /** Remove an edge from the network 
      
      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()
      
      Changing the parents of a given node changes its default distribution of the node as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)
  */
  void remove_edge(const std::string& source_name, const std::string& destination_name);
  
  /** Get the BN nodes */
  const plVariablesConjunction &nodes() const
  {
    return all_variables_;
  }
  
  /** Get the BN edges as a set of pairs (source, destination) */
  bn_edge_list_t edges() const;

  /** Get the number of nodes */
  size_t get_n_nodes()const
  {
    return all_variables_.dim();
  }

  /** Get the nth node name */
  std::string get_nth_node_name(unsigned int n)const
  {
    return get_node(n).name();
  }

  /** Get the nth node */
  plVariable get_node(unsigned int n)const;

  /** Get the node having \em node_name as name */
  plVariable get_node(const std::string &node_name)const;

  /** Clear all BN's nodes and the associated distributions/learnable_distributions */
  void clear();

  /** Sets the evidence in the network. This replaces previously existing evidence.
      Evidence is contained in a plValues object. For instance, to insert evidence
      over nodes @em a and @em c, that a = 2 and c = 42:
      @code
      plValues evidence(a ^ c);
      evidence[a] = 2;
      evidence[c] = 42;
      net.set_evidence(evidence);
      @endcode
  */
  void set_evidence(const plValues& evidence, bool check_validity=true);


  /** Addes a evidence in the network. This adds the new evidence to the previously existing evidence.
      Evidence is contained in a plValues object. For instance, to add an evidence
      over nodes @em a and @em c, that a=2 and c = 42:
      @code
      plValues evidence(a ^ c);
      evidence[a] = 2;
      evidence[c] = 42;
      net.add_evidence(evidence);
      @endcode
  */
  void add_evidence(const plValues& added_evidence, bool check_validity=true);

  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, int val, bool check_validity=true);

  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, double val, bool check_validity=true);
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, float val, bool check_validity=true);
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, long double val, bool check_validity=true);

  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, const std::string &val, bool check_validity=true);
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, int val, bool check_validity=true);

  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, double val, bool check_validity=true);
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, float val, bool check_validity=true);
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, long double val, bool check_validity=true);

    /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, const std::string &val, bool check_validity=true);

  /** Get the current evidence */
  const plValues &get_evidence()const{return evidence_;}

  /** Clears evidence previously inserted in the network.
   */
  void clear_evidence();
  
  /** Learns the parameters of learnable distributions in the network. There
      can be arbitrary missing data in the learning observations. If \em no_em
      is set to true (the default is \em false), only available values are
      used. Otherwise, EM (Expectation-Maximization) algorithm is used. This
      EM algorithm is implemented using plEMLearner and is run for \em
      em_iterations iterations if a positive value is provided. If \em
      em_iterations is not positive (the default value), the algorithm will be
      run until convergence.

      The learning data is made of observations on the network variables,
      in a CSV (comma separated values) file.

      The CSV files consists in a series of lines in text. Each line
      is made of a comma or semicolon-separated list of fields. Each field
      can be non-quoted, or quoted using single (') or double quotes (&quot;). However,
      fields containing a comma or semicolon must be quoted. Spaces around quoted
      fields are ignored. You may include a quote inside a quoted field by doubling it.

      The CSV file must be structured in the following way:
      - its first line (the header) gives the names of the observed variables
      - each of the following lines gives the values of the observed variables
      for a given observation
      - a value can be left empty to signify that it was not observed.

      For instance, we can build a network with variables A, B, and C :
      @code
      plVariable a("A", plIntegerType(0, 42));
      plVariable b("B", plIntegerType(-2, 2));
      plVariable c("C", plIntegerType(-1, 1));
      plBayesianNetwork net;
      net.add_node(a);
      net.add_node(b);
      net.add_node(c);
      plLearnLaplace learn_a(a);
      net.set_distribution(learn_a);
      plCndLearnObject<plLearn1dNormal> learn_b_k_a(b, a);
      net.set_distribution(learn_b_k_a);
      plCndLearnObject<plLearn1dNormal> learn_c_k_b(c, b);
      net.set_distribution(learn_c_k_b);
      //net.learn_parameters("data.csv", 4);
      net.learn_parameters("data.csv");
      std::cout << net << std::endl;
      @endcode

      The CSV file "data.csv" may have the following contents:
      @code
      C,A
      0,41
      1,
      ,7
      0,2
      @endcode

      In this file, the header announces that we will have values
      for C, then A. No values for B will be given (they are always unknown).
      The first observation is C=0, A=41.
      The next is C=1, A: unknown.
      The next is C: unknown, A=7.
      The last is C=0, A=2.
  */
  void learn_parameters(const std::string& csv_file, int em_iterations = -1,
                        bool csv_file_has_header=true,
                        const std::string &csv_file_undefined_field=" ",
                        const std::string &csv_file_field_separators=",;",
                        const std::string &csv_file_quotes="'\"",
                        bool no_em=false);

  /**  Learns the parameters of learnable distributions in the network. There
       can be arbitrary missing data in the learning observations. If \em no_em
       is set to true (the default is \em false), only available values are
       used. Otherwise, EM (Expectation-Maximization) algorithm is used. This
       EM algorithm is implemented using plEMLearner and is run for \em
       em_iterations iterations if a positive value is provided. If \em
       em_iterations is not positive (the default value), the algorithm will be
       run until convergence.

       The learning data is made of observations on the network variables, in a
       data descriptor object (plDataDescriptor) \em data_set.

       \sa plDataDescriptor
  */
  void learn_parameters(plDataDescriptor &data_set, 
                        int em_iterations = -1,
                        bool no_em = false);

  /**  Learns the parameters of learnable distributions in the network. There
       can be arbitrary missing data in the learning observations and EM
       (Expectation-Maximization) algorithm is used. This EM algorithm is
       implemented using plEMLearner and is run for \em em_iterations
       iterations if a positive value is provided. If \em em_iterations is not
       positive (the default value), the algorithm will be run until
       convergence.

       The learning data is made of observations on the network variables, in a
       data descriptor object (plDataDescriptor) \em data_set.

       \return The log-likelihood of the learnt model

       \sa plDataDescriptor
  */
  plFloat learn_parameters_em(plDataDescriptor &data_set, 
                              int em_iterations = -1, 
                              bool same_missing_variables = false);


  /**  Learns the parameters of learnable distributions in the network. There
       can be arbitrary missing data in the learning observations. However,
       only available values are used.

       The learning data is made of observations on the network variables, in a
       data descriptor object (plDataDescriptor) \em data_set.

       \sa plDataDescriptor
  */
  void learn_parameters_no_em(plDataDescriptor &data_set);

  /** Insert a learning point and adjust the parameters of the BN */
  bool learn_parameters_add_point(const plValues& data_row, plFloat weight=PL_ONE);

  /** Insert a learning point and adjust the parameters of the BN */
  bool learn_parameters_add_point(const plValues& data_row, const std::vector<bool> &definitions,
                                  plFloat weight=PL_ONE);
  
  /** Reset the learned distributions to their initial values */
  void reset_learned_parameters();

  /** Get the total number of parameters to be learnt */
  unsigned int get_n_parameters()const;
  
  /** Learns the structure of the network. It assumes that the nodes
      (variables) of the net have been already inserted.

      The learning data is made of observations on the network variables, in a
      data descriptor object (plDataDescriptor) \em data_set.

      For instance, we can build a network with variables A, B, and C :
      @code
      plVariable a("A", plIntegerType(0, 42));
      plVariable b("B", plIntegerType(-2, 2));
      plVariable c("C", plIntegerType(-1, 1));
      plBayesianNetwork net;
      net.add_node(a);
      net.add_node(b);
      net.add_node(c);
   
      plCSVDataDescriptor cvs_data("data.csv", a^b^c);
      net.learn_structure(cvs_data);
      std::cout << net << std::endl;
      @endcode

      \param     data The data source to be used for learning the structure and then the parameters
      \param[in] use_current_structure_as_starting_point If 'true', the current structure is used as a starting point
      \param[in] keep_current_edges If 'true', all the current edges will be kept in the final output structure
      \param[in] edge_score The edge score to be used for the preliminary Directed Minimum Spanning Tree phase.
      This parameter (and the DMST algorithm) is used only when 'use_current_structure_as_starting_point=false'
      \param[in] node_score The node score to be used for the final Greedy Search phase.
      \param[in] root_node The root not if any
      \param[in] maxp The maximal number of parents allowed for a given node
      \param[in] maxpc The cardinality (numbers of discrete states) of parents allowed for a given node
      \param[in] edges_to_exclude The edges to mandatory exclude in the final output structure

      \return the score (node_score) of the output structure on the provided data

      \sa plDataDescriptor
  */
  plFloat learn_structure(plDataDescriptor &data,
                          bool use_current_structure_as_starting_point=true,
                          bool keep_current_edges=true, 
                          plStructureLearningScore edge_score = PL_MI,
                          plStructureLearningScore node_score = PL_AIC,
                          const plVariable &root_node=plVariable(),
                          unsigned int maxp=10, 
                          unsigned int maxpc=1000,
                          const bn_edge_list_t &edges_to_exclude=bn_edge_list_t());

  /** Learns the structure of the network as a tree augmented naive bayes (TANB)
      \param     data The data source to be used for learning the structure and then the parameters
      \param[in] class_node The class node
      \param[in] edge_score The edge score to be used for evaluating edges.
      It will used as conditional regarding the class node for evaluating  'non class -> non class' edges, 
      and non conditional when evaluating  'class -> non class' edges
      \param[in] score_threshold The threshold to be used to decide if a given edge is to be added
      \param[in] keep_current_edges If 'true', all the current edges will be kept in the final output structure.
      It's the responsibility of the caller to check that the current edges are compatible with a TANB structure
      \param[in] edges_to_exclude The edges to mandatory exclude in the final output structure
  */
  void learn_structure_tree_augmented_naive_bayes(plDataDescriptor &data,
                                                  const plVariable &class_node,
                                                  plStructureLearningScore edge_score=PL_AIC,
                                                  plFloat score_threshold=PL_ZERO,
                                                  bool keep_current_edges=true,
                                                  const bn_edge_list_t &edges_to_exclude=bn_edge_list_t());

  /** Set the structure to naive bayes and learn the parameters using a data set
      while removing the edges having a value for score less than a given threshold

      \param     data The data source to be used for learning the structure and then the parameters
      \param[in] class_node The class node
      \param[in] edge_score The edge score to be used for evaluating the 'class -> non class' edges. This parameter is used iff 'score_threshold' below is not -infinity
      \param[in] score_threshold The threshold to be used for removing some 'class -> non class' edges. 
      The default value -infinity means that no 'class -> non class' edge will be removed
  */
  void learn_naive_bayes(plDataDescriptor &data, const plVariable &class_node,
                         plStructureLearningScore edge_score=PL_MI,
                         plFloat score_threshold=-std::numeric_limits<plFloat>::infinity());

  /** Returns the belief associated to a given node with name @em node_name, knowing the currently inserted evidence.
      Belief is returned as a plDistribution object, that is a non-conditional probability distribution
      whose values can be inspected using plDistribution::compute() and plDistribution::tabulate().

      This belief can also be used as a building block for another plJointDistribution
      or plBayesianNetwork object (using plBayesianNetwork::set_distribution()).
  */
  const plDistribution &get_belief(const std::string& node_name)const;

  /** Returns the belief associated to a given conjunction of nodes, knowing the currently inserted evidence.
      Belief is returned as a plDistribution object, that is a non-conditional probability distribution
      whose values can be inspected using plDistribution::compute() and plDistribution::tabulate().

      This belief can also be used as a building block for another plJointDistribution
      or plBayesianNetwork object (using plBayesianNetwork::set_distribution()).
  */
  const plDistribution &get_belief(const plVariablesConjunction& node_conjunction) const;

  /** Returns the belief table associated to a given node with name @em node_name, knowing the currently inserted evidence.
   */
  const std::vector<plProbValue> &get_belief_table(const std::string& node_name) const;

  /** Returns the belief table associated to a given conjunction of nodes, knowing the currently inserted evidence.
   */
  const std::vector<plProbValue> &get_belief_table(const plVariablesConjunction& node_conjunction) const;

  /** Returns a const reference plJointDistribution object built from this Bayesian network.
   */
  const plJointDistribution &get_joint_distribution() const;
  
  /** Returns a reference plJointDistribution object built from this Bayesian network.
   */
  plJointDistribution &get_joint_distribution();

  /** Returns a const reference plJunctionTree object built from this Bayesian network.
   */
  const plJunctionTree &get_junction_tree() const;

  /** Saves the BN in a file */
  void save(const std::string &file_name)const;

  /** Loads the BN from a file */
  void load(const std::string &file_name);

  /** Return BN's name */
  const std::string &name()const{return name_;}

  /** Sets BN's name */
  void set_name(const std::string &name)
  {name_ = name;}

  /** Sets the inference algorithm to be used. 
      Possible values are \em PL_JT for the "Junction Tree" algorithm
      and \em PL_SR for the "Successive Restrictions" algorithm (a kind of variable elimination).
      The initial value is \em PL_SR.
  */
  void set_inference_algorithm(plInferenceAlgorithm algo);

  /** Set/change the optimality criterion when computing inference expressions.
      This function has no effect when using the Junction Tree inference algorithm 
      (i.e., when using PL_JT in set_inference_algorithm())

      The parameter @em optimization should take a value among:

      1) PL_NO_OPTIMIZATION will not attempt to optimize the expression for any
      particular purpose.\n

      2) PL_OPTIMIZE_COMPILATION_TIME will optimize, for a given query 
      P(A | B), the expression so that the computation time for the 
      first inference P(A | B = b) is minimal.\n

      3) PL_OPTIMIZE_UPDATE_TIME Optimize, for a given query P(A | B), the
      expression for repeated inferences P(A | B = b1), P(A | B = b2), …, P(A |
      B = bn) (i.e, minimize the computation time needed for updating the query).\n


      4) PL_OPTIMIZE_MEMORY_USE will optimize the expression for minimal memory
      use.\n
  */
  void set_inference_optimization_criterion(plOptimizationCriterion criterion);

  /** Writes the drawing instructions to represent the graph of the
      net in @em file_name. The current implementation allows using xfig
      and graphviz dot.

      \sa draw_graph_dot
  **/
  void draw_graph(const std::string &file_name, 
		  plDrawingLanguage drawing_language=PL_XFIG_GRAPH)const;


  /** Writes the graphviz dot drawing instructions to represent the graph
      of the net in @em file_name.

      The parameters dot_node_shape, dot_node_color, dot_node_fill_color,
      dot_background_color, and dot_rankdir correspond to graphviz dot ones (see
      http://www.graphviz.org/pdf/dotguide.pdf). There values will be inserted
      in the generated dot file without checking their validity.

      To generate an image file from the generated graphvis dot @em file_name,
      you can use "dot -T'img_format' 'file_name' -o file_name.'img_format' "
      in which 'img_format' could be "pdf", "png",...  
      The supported image formats could be found in
      http://www.graphviz.org/pdf/dotguide.pdf
  **/
  void draw_graph_dot(const std::string &file_name, 
		      const std::string &dot_node_shape="", 
		      const std::string &dot_node_color="",
		      const std::string &dot_node_fill_color="",
		      const std::string &dot_background_color="",
                      const std::string &dot_edge_color="",
		      const std::string &dot_rankdir="")const;

#ifndef SWIG
  /** Same as draw_graph_dot(const std::string &file_name, ...) above but uses a stream instead of a file */
  void draw_graph_dot(std::ostream &out, 
                      const std::string &dot_node_shape="", 
		      const std::string &dot_node_color="",
		      const std::string &dot_node_fill_color="",
		      const std::string &dot_background_color="",
                      const std::string &dot_edge_color="",
		      const std::string &dot_rankdir="")const;
#endif
  
  /** Generate \em nsamples data in a CSV output file \em file according to the
      model defined by the net. 

      Simulating missing values is possible by providing the missing
      probability for each variable in the vector \em missing_probabilities.

      The generated CSV file will be structured as follows:
      - its first line (the header) gives the names of the variables (nodes)
      - each of the following lines gives the values of the variables
      for a given data instance
      - a value can be left empty to signify that it is missing.
  */
  void sample(const std::string &file, unsigned int nsamples,
              const std::vector<plProbValue> &missing_probabilities = std::vector<plProbValue>() )const;


  /** Return @em true iff the node exists */
  bool exists_node(const plVariable& node)const;

  /** Return @em true iff a node with name @em name exists */
  bool exists_node(const std::string& name)const;
  
  /** Return node's parents */
  plVariablesConjunction node_parents(const plVariable& node)const;
  
  /** Return the parents of the node having the name @em name*/
  plVariablesConjunction node_parents(const std::string& name)const;


  /** Return node's children */
  plVariablesConjunction node_children(const plVariable& node)const;
  
  /** Return the children of the node having the name @em name*/
  plVariablesConjunction node_children(const std::string& name)const;
  
  /** Ask a query to the BN and apply it to a data set
      
      \param[in] target_variables The target variables to predict
      \param[in] data The input data set to be used for prediction
      \param[in] output_file The output file. It will include the columns (variables) of the input data set
      with additional columns corresponding the predicted target variables and the corresponding distribution table
      \param[in] classif_prob_thresholds The probability thresholds to be applyed for classification
      \param[in] nrows The number of rows to be used for this prediction (starting from the current position in the input data source)

      \sa prediction()
  */
  void apply_prediction(const plVariablesConjunction& target_variables,
                        plDataDescriptor &data,
                        const std::string &output_file,
                        const std::vector<plProbValue> &classif_prob_thresholds=std::vector<plProbValue>(),
                        unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Ask a query to the BN and apply it to a data set
      
      \param[in] target_variables The target variables to predict
      \param[in] data The input data set to be used for prediction
      \param[in] nrows The number of rows to be used for this prediction (starting from the current position in the input data source)
      \param[in] classif_prob_thresholds The probability thresholds to be applyed for classification
    
      \return for each data record, the value of @em target_variables that
      maximises the probabiliy given the evidence provided by the data row
      P(target_variables | row_evidence). This value could be void (empty plValues) 
      if no prediction is provided (when using classif_prob_threshold).

      \sa prediction()
  */
  std::vector<plValues> apply_prediction(const plVariablesConjunction& target_variables,
                                         plDataDescriptor &data,
                                         unsigned int nrows=std::numeric_limits<unsigned int>::max(),
                                         const std::vector<plProbValue> &classif_prob_thresholds=std::vector<plProbValue>()) const;
  
  /** Return the value of @em target_variables that maximises the probabilité given the evidence @em evidence 
      P(target_variables | evidence).
      
      Equivalent to:
      \code
      plBayesianNetwork net(...);
      .
      .
      .
      net.set_evidence(evidence.remove(target_variables));
      plValues result = net.get_belief(target_variables).best();
      \endcode
  */
  plValues prediction(const plVariablesConjunction& target_variables, const plValues &evidence,
                      const std::vector<plProbValue> &prob_thresholds=std::vector<plProbValue>()) const;
  
  /** Ask a query to the model, apply it to a data set, and provide validation statistics 
      
      \param[in] target_variables The target variables to predict
      \param[in] data The input data set to be used for validation
      \param[in] output_file The output file. It will include the columns (variables) of the input data set
      with additional columns corresponding the predicted target variables and the corresponding distribution table 
      \param[in] classif_prob_thresholds The probability thresholds to be applyed for classification
      \param[in] nrows The number of rows to be used for this validation (starting from the current position in the input data source)

      \return The validation results
  */
  plPredictionPerformanceReport validate_prediction(const plVariablesConjunction& target_variables,
                                                    plDataDescriptor &data,
                                                    const std::string &output_file="",
                                                    const std::vector<plProbValue> &classif_prob_thresholds=std::vector<plProbValue>(),
                                                    unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;


  /** Return Markov blanket for a given node */
  plVariablesConjunction get_markov_blanket(const plVariable& node) const;

  /** Set/unset verbose mode for structure learning.
      If true, the structure learning algorithms will display intermediate results
  */
  void set_verbose_structure_learning(bool verbose=true)
  {
    verbose_sl_ = verbose;
  }

  /** Set/unset verbose mode for parameter learning using EM.
      If true, the parameter learning algorithm will display intermediate results
  */
  void set_verbose_em_learning(bool verbose=true)
  {
    verbose_em_ = verbose;
  }

  /** If set to 'true' (the default value), the distributions returned by get_belief() are compiled.
      Set it to 'false' if you do not need compiled distributions
  */
  void set_compiled_belief_distribution(bool compile_it);

  /** Get the number of parameters to learn */
  unsigned int learned_parameters_number() const;

  /** Compute the log-likelihood of a given record */
  plFloat compute_data_record_log_likelihood(const plValues &data_record) const;

  /** Compute the log-likelihood of a given data set
      
      \param data_set the data set for which the log-likelihood will be computed
      \param[in] nrows The number of rows to be used for this validation
      (starting from the current position in the input data source)

      \return log-likelihood of the data set
  */
  plFloat compute_data_set_log_likelihood(plDataDescriptor &data_set, 
                                          unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Compute the log-likelihood of a given vector of data records
      
      \param[in] data_records the vector of data records for which the log-likelihood will be computed

      \return log-likelihood of the data set
  */
  plFloat compute_data_set_log_likelihood(const std::vector<plValues> &data_records) const;

  /** Get the input distribution over the node @em node */
  plComputableObject get_node_distribution(const plVariable &node) const;
  
  /** Get the input distribution over the node @em node */
  plComputableObject get_node_distribution(const std::string &node_name) const
  {
    return get_node_distribution(get_node(node_name));
  }

  /** Get the input learnable distribution over the node @em node */
  plLearnObject* get_node_learnable_distribution(const plVariable &node) const;

  /** Get the input learnable distribution over the node @em node */
  plLearnObject* get_node_learnable_distribution(const std::string &node_name) const
  {
    return get_node_learnable_distribution(get_node(node_name));
  }

  /** Add a time-transition edge.
      For example, to add a time transition edge 
      \f[ P(X_{t} | X_{t-1} ) \f], one can write:
      @code
      plBayesianNetwork bn;
      const plVariable x("X", PL_BINARY_TYPE);
      const plVariable x_("X_", PL_BINARY_TYPE);
      
      bn.add_node(x);
      bn.add_node(x_);

      bn.add_time_transition_edge( x_, x );

      @endcode
  */
  void add_time_transition_edge(const plVariable &node_prec_timeslice, 
                                const plVariable &node_current_timeslice);

  /** Set a time-transition edge. 
      For example, to set a time-transition distribution \f[ P(X_{t} | X_{t-1} ) \f], one can write:

      @code
      plBayesianNetwork bn; 
      const plVariable x("X", PL_BINARY_TYPE); 
      const plVariable x_("X_", PL_BINARY_TYPE);
      
      bn.add_node(x);
      bn.add_node(x_);

      const plCndDistribution transition( x, x_);

      bn.set_time_transition_distribution( transition );

      @endcode
  */
  void set_time_transition_distribution(const plCndDistribution &transition);

  /** Set a time-transition learnable distribution. 
      For example, to set a learnable time transition distribution \f[ P(X_{t} | X_{t-1} ) \f], one can write:

      @code
      plBayesianNetwork bn; 
      const plVariable x("X", PL_BINARY_TYPE); 
      const plVariable x_("X_", PL_BINARY_TYPE);
      
      bn.add_node(x);
      bn.add_node(x_);

      plCndLearnObject<plLearnHistogram> learnable_transition( x, x_);

      bn.set_time_transition_learnable_distribution( learnable_transition );

      @endcode
  */
  void set_time_transition_learnable_distribution(const plLearnObject &learnable_transition);
  
  /** Update all the time-slice distributions if the BN is dynamic.

      A typical use is as follows:

      @code
      plBayesianNetwork bn; 
      const plVariable x("X", PL_BINARY_TYPE); 
      const plVariable x_("X_", PL_BINARY_TYPE);
      const plVariable o("O", plRealType(-100000., 100000.));
      
      bn.add_node(x);
      bn.add_node(x_);
      bn.add_node(o);

      const plCndDistribution transition( x, x_);

      bn.set_time_transition_distribution( transition );

      // Dynamic BN joint distribution: P( X X_ O) = P(X_) P(X | X_) P(O | X)

      plValues observation(o);
      
      for(unsigned int i = 0; i < 5; ++i) {
      observation[o] = -10. + plRandom::uniform_float(10.);
      bn.set_evidence(observation);
      std::cout << bn << std::endl;
      bn.next_time_slice();
      }
      @endcode
  */
  void next_time_slice();

  /** 
      Get the time-slice edges as pairs of (var_{t-1}, (var_{t}).
  */
  bn_edge_list_t get_time_slice_edges() const;

  /** Set the maximum number of the cached target distributions.
      The default value is 10.
  */
  void set_query_cache_max_size(unsigned int new_size)
  {
    query_cache_max_size_ = new_size;
  }

  /** Set a soft evidence */
  void set_soft_evidence(const plDistribution &soft_evidence);

  /** Clear all soft evidences */
  void clear_soft_evidences();

  /** Set the convergence_threshold for the EM algorithm 
      The default value is 0.0001
  */
  void set_em_convergence_threshold(plFloat em_convergence_threshold)
  {
    em_convergence_threshold_ = em_convergence_threshold;
  }

 private:
  void output_results(std::ostream &outs,
                      const plValues *input_values, 
                      const std::vector<bool> *input_def,
                      const plValues &predicted,
                      const plVariablesConjunction &target_variables,
                      bool is_classif) const;

  bool filter_by_threshold(const plValues &inferred_value,
                           const plVariablesConjunction &target,
                           bool is_classif,
                           const plVariableIndexer &target_indexer,
                           const std::vector<plProbValue> &prob_thresholds ) const;

  template <typename T>
    void add_evidence_impl(const plVariable& evidence_variable, T val, bool check_validity);

  template <typename T>
    void add_evidence_impl_name(const std::string& evidence_variable_name, T val, bool check_validity);
    
 public:

#ifdef PL_ENABLE_CONVERTER
  /** Save as Genie xdsl file */
  void save_as_genie(const std::string &file_name) const;

  /** Load from a Genie xdsl file */
  void load_from_genie(const std::string &file_name);

  /** Save as Netica dne file */
  void save_as_netica(const std::string &file_name) const;

  /** Load from a Netica dne file */
  void load_from_netica(const std::string &file_name);

  /** Save as Hugin net file */
  void save_as_hugin(const std::string &file_name) const;

  /** Load from a Hugin net file */
  void load_from_hugin(const std::string &file_name);
#endif // PL_ENABLE_CONVERTER

  friend class plTimeSliceQuery;
};

class plQuery
{
public:
  plQuery()
    : variables()
    , discretized_variables(false)
    , dist()
    , compiled_dist()
    , cnd_dist()
    , compiled_response_as_probtable()
    , first_computation(true)
    , last_evidence()
    , compile_distribution(true)
    , invalidated_compiled_distribution(true)
  {}
  
  plQuery(const plVariablesConjunction &vars, bool compile_it)
    : variables(vars)
    , discretized_variables(vars.is_discretized())
    , dist()
    , compiled_dist()
    , cnd_dist()
    , compiled_response_as_probtable()
    , first_computation(true)
    , last_evidence()
    , compile_distribution(compile_it)
    , invalidated_compiled_distribution(true)
  {}
  
  plVariablesConjunction variables;
  bool discretized_variables;
  plDistribution dist;
  plDistribution compiled_dist;
  plCndDistribution cnd_dist;
  plProbTable compiled_response_as_probtable;
  bool first_computation;
  plValues last_evidence;
  bool compile_distribution;
  bool invalidated_compiled_distribution;
};

class plTimeSliceQuery : public plQuery
{
private:
  plTimeSliceQuery(const plTimeSliceQuery&);// NI
  plTimeSliceQuery& operator=(const plTimeSliceQuery&);// NI

public:
  plTimeSliceQuery()
    : plQuery()
    , mutable_dist(0)
    , prec_variable()
  {}

  plTimeSliceQuery(const plVariable &node_prec, 
                   const plVariable &node_current,
                   const plDistribution &init_dist)
    : plQuery(node_current, true)
    , mutable_dist(new plMutableDistribution( init_dist ) )
    , prec_variable(node_prec)
  {}

  plTimeSliceQuery(const plVariable &node_prec, 
                   const plVariable &node_current)
    : plQuery(node_current, true)
    , mutable_dist(0 )
    , prec_variable(node_prec)
  {}

  ~plTimeSliceQuery()
  {
    delete mutable_dist;
  }

  void set_mutable(plBayesianNetwork &bn)
  {
    assert(  mutable_dist == 0 );

    mutable_dist = new  plMutableDistribution(  plDistribution(bn.get_node_distribution(prec_variable) ) );

    plLearnFrozenDistribution *frozen_l = new plLearnFrozenDistribution(*mutable_dist);
    bn.replace_distribution(prec_variable, frozen_l);
  }

  plMutableDistribution *mutable_dist;
  plVariable prec_variable;
};

/** Prediction Performance Report returned by plBayesianNetwork::validate_prediction().
    Provides confusion matrix in case of classification, and R2, RMSE, SSE, MBE, MAE, MRE in case of regression
*/
class PL_DLL_API plPredictionPerformanceReport : public plObject
{
 public:
  /** Construction for a given target */
  explicit plPredictionPerformanceReport(const plVariablesConjunction& target_vars);

  /** Update prediction statistics */
  void update(const plValues &actual_value, const plValues &inferred_value);

  /** Get the number of validation examples */
  unsigned int get_n() const;

  /** Return True is classification results and False if regression */
  bool is_classification() const{return is_classif_;}

  /** Add another result to this */
  void add(const plPredictionPerformanceReport &other);
  
  /** Get a given entry of the confusion matrix.
      Applicable only for classification
  */
  unsigned int get_confusion_matrix_entry(size_t real, size_t predicted) const;

  /** Get the number of real validation examples for a given class index.
      Applicable only for classification
  */
  unsigned int get_n_real(size_t class_index) const;

  /** Get the number of prediction validation examples for a given class index.
      Applicable only for classification
  */
  unsigned int get_n_predicted(size_t class_index) const;

  /** Get the precision for a given class.
      Applicable only for classification
  */
  plFloat get_precision(size_t class_index) const;

  /** Get the recall for a given class.
      Applicable only for classification
  */
  plFloat get_recall(size_t class_index) const;

  /** Get classification accuray.
      Applicable only for classification
  */
  plFloat get_accuracy() const;

  /** Get the number of classification errors.
      Applicable only for classification
  */
  unsigned int get_n_errors() const;

  /** Get the number of good classification responses.
      Applicable only for classification
  */
  unsigned int get_n_non_errors() const;
 
  const plVariableIndexer& variable_indexer() const { return target_indexer_;}
    
  /** Get the R-squared score for a given dimension.
      Applicable only for regression
  */
  plFloat get_r2(size_t dim=0) const;

  /** Get the Root Mean Squared Error for a given dimension.
      Applicable only for regression
  */
  plFloat get_rmse(size_t dim=0) const;

  /** Get the Sum of Squared Error for a given dimension.
      Applicable only for regression
  */
  plFloat get_sse(size_t dim=0) const;

  /** Get the Mean Bias Error for a given dimension.
      Applicable only for regression
  */
  plFloat get_mbe(size_t dim=0) const;

  /** Get the Mean Absolute Error for a given dimension.
      Applicable only for regression
  */
  plFloat get_mae(size_t dim=0) const;

  /** Get the Mean Relative Error for a given dimension.
      Applicable only for regression
  */
  plFloat get_mre(size_t dim=0) const;

 protected:
  void update_regression(const plValues &actual_value, const plValues &inferred_value);
  void update_classification(const plValues &actual_value, const plValues &inferred_value);
  void print_regression_report(std::ostream &outs) const;
  void print_classification_report(std::ostream &outs) const;
  void increment_confusion_matrix_entry(size_t real_index, size_t predicted_index, unsigned int inc);
  
  /**  Print the performance report: confusion matrix in case of classification, 
       and R2, RMSE, SSE, MBE, MAE, MRE in case of regression 
  */
  virtual void Output(std::ostream &out) const;

 private:
  plVariablesConjunction target_;
  bool is_classif_;

  // Classification
  std::vector<unsigned int> confusion_matrix_;
  plVariableIndexer target_indexer_;
  unsigned int target_variables_cardinality_;

  // Regression
  std::vector<plFloat> sum_y_;
  std::vector<plFloat> sum_y2_;
  std::vector<plFloat> sum_err2_;
  std::vector<plFloat> sum_err_;
  std::vector<plFloat> sum_abs_err_;
  std::vector<plFloat> sum_abs_rel_err_;
    
  unsigned int n_;
};

#endif /* PLBAYESIANNETWORK_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
