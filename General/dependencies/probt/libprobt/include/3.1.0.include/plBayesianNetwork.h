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
#include <plPredictionPerformanceReport.h>
#include <plDLLExport.h>

SPL_DECL2(plBayesianNetwork);

class plVariable;
class plJointDistribution;
class plJunctionTree;
class plLearnDistributions;

class kplQuery;
class kplTimeSliceQuery;
class plEMLearner;
class plDataDescriptor;


// Doxygen bug: prevent broken link to plBayesianNetwork::set_inference_algorithm().
/** Inference algorithm to be used in %plBayesianNetwork::set_inference_algorithm(). */
enum plInferenceAlgorithm {
  PL_JT,  /*!<  "Junction Tree" algorithm. */
  PL_SR,  /*!<  "Successive Restrictions" algorithm (a sophisticated variant of variable elimination). */
  PL_MCMC /*!<  MCMC (Monte Carlo Markov Chain) approximate inference. */
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

/** plBayesianNetwork Import/Export formats */
enum plImportExportFormat {
  PL_FORMAT_GENIE,  /*!< Genie xdsl. */
  PL_FORMAT_NETICA, /*!< Netica dne. */
  PL_FORMAT_HUGIN   /*!< Hugin net. */
};

//================================

/** Class for building and/or learning (parameters and structure) a Bayesian
    network and using it for exact and approximate inference.  It provides
    also a support for one-time slice dynamic Bayesian networks.

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
 protected:

  typedef std::map<plVariable, plLearnObject*> learnable_map_t;
  typedef std::map<std::pair<plVariablesConjunction, plVariablesConjunction>, kplQuery*> query_cache_t;
  
 public:
  /** An edge list as a vector of node pairs (source, destination) */
  typedef std::vector< std::pair<plVariable, plVariable> > bn_edge_list_t;

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
  explicit plBayesianNetwork(const plComputableObjectList& decomposition,
                             const plVariablesConjunction& variables = plVariablesConjunction(),
                             const std::string& name = "untitled_BN");
  
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
  void set_nodes(const plVariablesConjunction& nodes);

  /** Clear the network and set its nodes */
  void set_nodes(const std::vector<plVariable>& nodes);

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
      
      You must call set_nodes() or add_node() to create the nodes before
      creating an edge between nodes. An exception will be raised if you do not
      do this.

      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()

      Changing the parents of a given node changes its default distribution as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)

      \sa plLearnObject::create_learner()
  */
  void add_edge(const plVariable& source, const plVariable& destination);

  /** Add an edge to the network

      You must call set_nodes() or add_node() to create the nodes before
      creating an edge between nodes. An exception will be raised if you do not
      do this.
      
      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()

      Changing the parents of a given node changes its default distribution as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)

      \sa plLearnObject::create_learner()
  */
  void add_edge(const std::string& source_name, const std::string& destination_name);

  /** Remove an edge from the network 
      
      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()
      
      Changing the parents of a given node changes its default distribution as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)

      \sa plLearnObject::create_learner()
  */
  void remove_edge(const plVariable& source, const plVariable& destination);

  /** Remove an edge from the network 
      
      Do not call add_edge() / remove_edge() if you want to customize the distributions later using set_distribution()
      \sa set_distribution()
      
      Changing the parents of a given node changes its default distribution as follows:
      - if the node is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(node, parents)
      - if the node is discrete and all its parents are continuous: plLearnSoftmax(node, parents)
      - if the node is discrete and its parents are mixed: plLearnSoftmaxFamily(node, continuous_parents, discrete_parents)
      - if the node is continuous and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(node, parents)
      - if the node is continuous and all its parents are continuous: plLearnLinearRegression(node, parents)
      - if the node is continuous and its parents are mixed: plLearnLinearRegressionFamily(node, continuous_parents, discrete_parents)

      If the node is a root (with no parents):
      - if the node is discrete: plLearnLaplace(node)
      - if the node is continuous: plLearn1dNormal(node)

      \sa plLearnObject::create_learner()
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
  plVariable get_node(const std::string& node_name)const;

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

  /** Adds evidence into the network. This adds the new evidence to the previously existing evidence.
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

#ifndef SWIG
  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, float val, bool check_validity=true);
#endif
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, long double val, bool check_validity=true);

  /** Same as add_evidence(const plValues&, bool) above but using a variable and its corresponding value */
  void add_evidence(const plVariable& evidence_variable, const std::string &val, bool check_validity=true);
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, int val, bool check_validity=true);

  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, double val, bool check_validity=true);

#ifndef SWIG
  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, float val, bool check_validity=true);
#endif
  
  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, long double val, bool check_validity=true);

  /** Same as add_evidence(const plValues&, bool) above but using a variable name and its corresponding value */
  void add_evidence(const std::string& evidence_variable_name, const std::string &val, bool check_validity=true);

  /** Get the current evidence */
  const plValues &get_evidence()const{return evidence_;}

  /** Clears evidence previously inserted in the network.
   */
  void clear_evidence();

  /**  Learn the parameters of the learnable distributions in of the network. There
       can be arbitrary missing data in the learning observations. If \em no_em
       is set to true (the default is \em false), only available values are
       used. Otherwise, EM (Expectation-Maximization) algorithm is used. This
       EM algorithm is run for \em em_iterations iterations if a positive value
       is provided. If \em em_iterations is not positive (the default value),
       the algorithm will be run until convergence.

       The learning data is made of observations on the network variables, in a
       data descriptor object (plDataDescriptor) \em data_set.

       \sa plDataDescriptor
       \sa learn_parameters_em()
       \sa learn_parameters_no_em()
  */
  void learn_parameters(plDataDescriptor& data_set, 
                        int em_iterations = -1,
                        bool no_em = false);
  
  /* Same as above but using a vector af plValues */
  void learn_parameters(const std::vector<plValues>& data_set, 
                        int em_iterations = -1,
                        bool no_em = false);

  /**  Learn the parameters of learnable distributions in the network using the
       EM algorithm.  There can be arbitrary missing data, including the case
       of latent variables, in the learning observations and EM
       (Expectation-Maximization) algorithm is used.

       \param[in,out] data_set The data set to be used. 

       \param[in] em_iterations The number of iterations to be performed. If
       the value is not positive (the default value), the algorithm will be run
       until convergence (see set_em_convergence_threshold()).

       \param[in] no_missing_data Set to true if the data set does not contain
       missing data values (i.e, the model contains latent variables
       exclusively).

       \param[in] use_most_probable_value If set to true (default is false), the most
       probable value of the missing variables is used instead of all the all possible
       values weights with their corresponding probabilities

       \return The log-likelihood of the learnt model if the algorithm is run
       until convergence, zero otherwise (i.e. when a positive value is
       provided for the parameter 'em_iterations').

       \sa plDataDescriptor
       \sa set_em_convergence_threshold()
       \sa learn_parameters()
       \sa learn_parameters_no_em()
  */
  plFloat learn_parameters_em(plDataDescriptor& data_set, 
                              int em_iterations = -1, 
                              bool no_missing_data = false,
                              bool use_most_probable_value = false);

  /** Same as above but using a vector af plValues */
  plFloat learn_parameters_em(const std::vector<plValues>& data_set, 
                              int em_iterations = -1, 
                              bool no_missing_data = false,
                              bool use_most_probable_value = false);

  /** Same as above but with initializing the model to the decomposition provided by 'init_model' 
   */
  plFloat learn_parameters_em(const plComputableObjectList& init_model,
                              plDataDescriptor& data_set,
                              int em_iterations = -1,
                              bool no_missing_data = false,
                              bool use_most_probable_value = false);

  /** Same as above but using a vector af plValues */
  plFloat learn_parameters_em(const plComputableObjectList& init_model,
                              const std::vector<plValues>& data_set,
                              int em_iterations = -1,
                              bool no_missing_data = false,
                              bool use_most_probable_value = false);
   

  /**  Learn the parameters of learnable distributions in the network. There
       can be arbitrary missing data in the learning observations. However,
       only available values are used.

       The learning data is made of observations on the network variables, in a
       data descriptor object (plDataDescriptor) \em data_set.

       \sa plDataDescriptor
       \sa learn_parameters()
       \sa learn_parameters_em()
  */
  void learn_parameters_no_em(plDataDescriptor& data_set);

  /** Same as above but using a vector af plValues */
  void learn_parameters_no_em(const std::vector<plValues>& data_set);

  /** Insert a learning point and adjust the parameters of the BN */
  bool learn_parameters_add_point(const plValues& data_row, plFloat weight=PL_ONE);

  /** Insert a learning point and adjust the parameters of the BN */
  bool learn_parameters_add_point(const plValues& data_row, const std::vector<bool>& definitions,
                                  plFloat weight=PL_ONE);

  /** Return the total weight of the data points inserted using learn_parameters_add_point() */
  plFloat learn_parameters_get_total_weight() const;

  /** Rescale the total weight of the data points inserted using learn_parameters_add_point()

      This allows, for example, to decrease the importance of the old data points compared to the new ones 
   */
  void learn_parameters_rescale_total_weight(plFloat s);
  
  /** Reset the learned distributions to their initial values */
  void reset_learned_parameters();

  /** Get the total number of parameters to be learnt */
  unsigned int get_n_parameters()const;

  /** Same as get_n_parameters() */
  inline unsigned int learned_parameters_number()const
  {
    return get_n_parameters();
  }
  
  /** Learn the structure of the network. It assumes that the nodes
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

      \param[in,out] data The data source to be used for learning the structure and then the parameters
      \param[in] use_current_structure_as_starting_point If 'true', the current structure is used as a starting point
      \param[in] keep_current_edges If 'true', all the current edges will be kept in the final output structure
      \param[in] edge_score The edge score to be used for the preliminary Directed Minimum Spanning Tree phase.
      This parameter (and the DMST algorithm) is used only when 'use_current_structure_as_starting_point=false'
      \param[in] node_score The node score to be used for the final Greedy Search phase.
      \param[in] root_node The root node if any
      \param[in] maxp The maximal number of parents allowed for a given node
      \param[in] maxpc The cardinality (numbers of discrete states) of parents allowed for a given node
      \param[in] edges_to_exclude The edges to mandatory exclude in the final output structure

      \return the score (node_score) of the output structure on the provided data

      \sa plDataDescriptor
  */
  plFloat learn_structure(plDataDescriptor& data,
                          bool use_current_structure_as_starting_point=true,
                          bool keep_current_edges=true, 
                          plStructureLearningScore edge_score=PL_MI,
                          plStructureLearningScore node_score=PL_AIC,
                          const plVariable& root_node=plVariable(),
                          unsigned int maxp=10, 
                          unsigned int maxpc=1000,
                          const bn_edge_list_t& edges_to_exclude=bn_edge_list_t());

  /** Same as above but using a vector af plValues */
  plFloat learn_structure(const std::vector<plValues>& data,
                          bool use_current_structure_as_starting_point=true,
                          bool keep_current_edges=true, 
                          plStructureLearningScore edge_score=PL_MI,
                          plStructureLearningScore node_score=PL_AIC,
                          const plVariable& root_node=plVariable(),
                          unsigned int maxp=10, 
                          unsigned int maxpc=1000,
                          const bn_edge_list_t& edges_to_exclude=bn_edge_list_t());
  

  /** Learn the structure of the network as a tree augmented naive bayes (TANB)
      \param[in,out] data The data source to be used for learning the structure and then the parameters
      \param[in] class_node The class node
      \param[in] edge_score The edge score to be used for evaluating edges.
      It will used as conditional regarding the class node for evaluating  'non class -> non class' edges, 
      and non conditional when evaluating  'class -> non class' edges
      \param[in] score_threshold The threshold to be used to decide if a given edge is to be added
      \param[in] keep_current_edges If 'true', all the current edges will be kept in the final output structure.
      It's the responsibility of the caller to check that the current edges are compatible with a TANB structure
      \param[in] edges_to_exclude The edges to mandatory exclude in the final output structure
  */
  void learn_structure_tree_augmented_naive_bayes(plDataDescriptor& data,
                                                  const plVariable& class_node,
                                                  plStructureLearningScore edge_score=PL_AIC,
                                                  plFloat score_threshold=PL_ZERO,
                                                  bool keep_current_edges=true,
                                                  const bn_edge_list_t& edges_to_exclude=bn_edge_list_t());

  /** Same as learn_structure_tree_augmented_naive_bayes() */
  void learn_structure_TANB(plDataDescriptor& data,
                            const plVariable& class_node,
                            plStructureLearningScore edge_score=PL_AIC,
                            plFloat score_threshold=PL_ZERO,
                            bool keep_current_edges=true,
                            const bn_edge_list_t& edges_to_exclude=bn_edge_list_t())
  {
    learn_structure_tree_augmented_naive_bayes(data, class_node, edge_score, score_threshold,
                                               keep_current_edges, edges_to_exclude);
  }

  /** Same as above but using a vector af plValues */
  void learn_structure_tree_augmented_naive_bayes(const std::vector<plValues>& data,
                                                  const plVariable& class_node,
                                                  plStructureLearningScore edge_score=PL_AIC,
                                                  plFloat score_threshold=PL_ZERO,
                                                  bool keep_current_edges=true,
                                                  const bn_edge_list_t& edges_to_exclude=bn_edge_list_t());

  /** Same as learn_structure_tree_augmented_naive_bayes() */
  void learn_structure_TANB(const std::vector<plValues>& data,
                            const plVariable& class_node,
                            plStructureLearningScore edge_score=PL_AIC,
                            plFloat score_threshold=PL_ZERO,
                            bool keep_current_edges=true,
                            const bn_edge_list_t& edges_to_exclude=bn_edge_list_t())
  {
    learn_structure_tree_augmented_naive_bayes(data, class_node, edge_score, score_threshold,
                                               keep_current_edges, edges_to_exclude);
  }

  
  /** Set the structure to naive bayes and learn the parameters using a data set
      while removing the edges having a value for score less than a given threshold

      \param[in,out] data The data source to be used for learning the structure and then the parameters
      \param[in] class_node The class node
      \param[in] edge_score The edge score to be used for evaluating the 'class
      -> non class' edges. This parameter is used iff 'score_threshold' below
      is not -infinity
      \param[in] score_threshold The threshold to be used for removing some 'class -> non class' edges. 
      The default value -infinity means that no 'class -> non class' edge will be removed
  */
  void learn_naive_bayes(plDataDescriptor& data, const plVariable& class_node,
                         plStructureLearningScore edge_score=PL_MI,
                         plFloat score_threshold=-std::numeric_limits<plFloat>::infinity());
  
  /** Same as learn_naive_bayes() */
  void learn_NB(plDataDescriptor& data, const plVariable& class_node,
                plStructureLearningScore edge_score=PL_MI,
                plFloat score_threshold=-std::numeric_limits<plFloat>::infinity())
  {
    learn_naive_bayes(data, class_node, edge_score, score_threshold);
  }

  /** Same as above but using a vector af plValues */
  void learn_naive_bayes(const std::vector<plValues>& data, const plVariable& class_node,
                         plStructureLearningScore edge_score=PL_MI,
                         plFloat score_threshold=-std::numeric_limits<plFloat>::infinity());

   /** Same as learn_naive_bayes() */
   void learn_NB(const std::vector<plValues>& data, const plVariable& class_node,
                 plStructureLearningScore edge_score=PL_MI,
                 plFloat score_threshold=-std::numeric_limits<plFloat>::infinity())
   {
     learn_naive_bayes(data, class_node, edge_score, score_threshold);
   }

  /** Return the belief associated to a given node with name @em node_name, knowing the currently inserted evidence.
      Belief is returned as a plDistribution object, that is a non-conditional probability distribution
      whose values can be inspected using plDistribution::compute() and plDistribution::tabulate().

      This belief can also be used as a building block for another plJointDistribution
      or plBayesianNetwork object (using plBayesianNetwork::set_distribution()).
  */
  const plDistribution &get_belief(const std::string& node_name)const;

  /** Same as above while setting the evidence temporally to \em evidence */
  const plDistribution &get_belief(const std::string& node_name, const plValues &evidence)const;

  /** Return the belief associated to a given conjunction of nodes, knowing the currently inserted evidence.
      Belief is returned as a plDistribution object, that is a non-conditional probability distribution
      whose values can be inspected using plDistribution::compute() and plDistribution::tabulate().

      This belief can also be used as a building block for another plJointDistribution
      or plBayesianNetwork object (using plBayesianNetwork::set_distribution()).
  */
  const plDistribution &get_belief(const plVariablesConjunction& node_conjunction) const;

  /** Same as above while setting the evidence temporally to \em evidence */
  const plDistribution &get_belief(const plVariablesConjunction& node_conjunction, const plValues& evidence) const;

  /** Returns the belief table associated to a given node with name @em node_name, knowing the currently inserted evidence.
   */
  const std::vector<plProbValue> &get_belief_table(const std::string& node_name) const;

  /** Same as above while setting the evidence temporally to \em evidence */
  const std::vector<plProbValue> &get_belief_table(const std::string& node_name, const plValues& evidence) const;

  /** Returns the belief table associated to a given conjunction of nodes, knowing the currently inserted evidence.
   */
  const std::vector<plProbValue> &get_belief_table(const plVariablesConjunction& node_conjunction) const;

  /** Same as above while setting the evidence temporally to \em evidence */
  const std::vector<plProbValue> &get_belief_table(const plVariablesConjunction& node_conjunction, const plValues& evidence) const;

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
  void save(const std::string& file_name)const;

  /** Loads the BN from a file */
  void load(const std::string& file_name);

  /** Return BN's name */
  const std::string &name()const{return name_;}

  /** Sets BN's name */
  void set_name(const std::string& name)
  {name_ = name;}

  /** Sets the inference algorithm to be used. 

      Possible values are: 
      - PL_JT for the "Junction Tree" exact inference algorithm, 
      - PL_SR for the "Successive Restrictions" exact inference
      algorithm (a sophisticated variant of variable elimination) and
      - PL_MCMC for Markov Chain Monte Carlo based approximate inference.

      The default initial value is PL_SR.

      When using PL_SR, the inference parameters could be customized using
      set_inference_optimization_criterion().
      
      When using PL_MCMC, the inference parameters could be customized using
      set_mcmc_nsamples()
  */
  void set_inference_algorithm(plInferenceAlgorithm algo);

  /** Set/change the optimality criterion when computing inference expressions
      using the Successive Restrictions algorithm (PL_SR).  

      This function has only an effect when using the Successive Restrictions
      algorithm (i.e., when using PL_SR in set_inference_algorithm())

      The parameter @em criterion should take a value among:

      1) PL_NO_OPTIMIZATION will not attempt to optimize the expression for any
      particular purpose.\n

      2) PL_OPTIMIZE_COMPILATION_TIME will optimize, for a given query 
      P(A | B), the expression so that the computation time for the 
      first inference P(A | B = b) is minimal.\n

      3) PL_OPTIMIZE_UPDATE_TIME will optimize, for a given query P(A | B), the
      expression for repeated inferences P(A | B = b1), P(A | B = b2), …, P(A |
      B = bn) (i.e, minimize the computation time needed for updating the
      query).\n

      4) PL_OPTIMIZE_MEMORY_USE will optimize the expression for minimal memory
      use.\n
  */
  void set_inference_optimization_criterion(plOptimizationCriterion criterion);

  /** Set/change the number of Monte Carlo sample points to be used for approximating marginalization 
      (i.e., computation of the sums/integrals) or for compiling a distribution.
      
      When mc_nsamples is negative, the actual number will be:
      - For marginalization: n = -mc_nsamples x free_vars_dim in which
      free_vars_dim is the number of variables on which we will marginalize.

      - For compilation: n = -mc_nsamples x distribution_dim in which
      distribution_dim is the dimension of the distribution to be compiled.

      The default value is -1000
  */
  void set_mcmc_nsamples(int mc_nsamples)
  {
    mcmc_nsamples_ = mc_nsamples;
  }
  
  /** Writes the drawing instructions to represent the graph of the
      net in @em file_name. The current implementation allows using xfig
      and graphviz dot.

      \sa draw_graph_dot
  **/
  void draw_graph(const std::string& file_name, 
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
  void draw_graph_dot(const std::string& file_name, 
		      const std::string& dot_node_shape="", 
		      const std::string& dot_node_color="",
		      const std::string& dot_node_fill_color="",
		      const std::string& dot_background_color="",
                      const std::string& dot_edge_color="",
		      const std::string& dot_rankdir="")const;


  /** Only for Python: Same as draw_graph_dot() above */
  void python_draw_graph()
  {
    THROW_PL_ERROR(89, "plBayesianNetwork.python_draw_graph() is only available for Python");
  }

#ifndef SWIG
  /** Same as draw_graph_dot(const std::string &file_name, ...) above but uses a stream instead of a file */
  void draw_graph_dot(std::ostream& out, 
                      const std::string& dot_node_shape="", 
		      const std::string& dot_node_color="",
		      const std::string& dot_node_fill_color="",
		      const std::string& dot_background_color="",
                      const std::string& dot_edge_color="",
		      const std::string& dot_rankdir="")const;
#endif
  
  /** Generate \em nsamples data samples in a CSV output file \em file according to the
      model defined by the net. 

      This uses forward sampling and does not take into account evidence that may have been inserted.
      Use gibbs_sample() if you want to take into account the inserted evidence.

      Simulating missing values is possible by providing the missing
      probability for each variable in the vector \em missing_probabilities.

      The generated CSV file will be structured as follows:
      - its first line (the header) gives the names of the variables (nodes)
      - each of the following lines gives the values of the variables
      for a given data instance
      - a value can be left empty to signify that it is missing.

      \sa gibbs_sample()
  */
  void sample(const std::string &file, unsigned int nsamples,
              const std::vector<plProbValue>& missing_probabilities = std::vector<plProbValue>() )const;

#ifndef SWIG
  /** Same as above but uses an ostream instead of a file name */
  void sample(std::ostream& os, unsigned int nsamples,
              const std::vector<plProbValue>& missing_probabilities = std::vector<plProbValue>() )const;
#endif

  /**  Return \em nsamples data samples according to the model defined by the net.

       This uses forward sampling and does not take into account evidence that may have been inserted.
       Use gibbs_sample() if you want to take into account the inserted evidence.

       \sa gibbs_sample()
   */
  std::vector<plValues> sample(unsigned int nsamples,
                               const std::vector<plProbValue>& missing_probabilities = std::vector<plProbValue>() )const;

  /** Run Gibbs sampling for \em n steps and return the last state 
      
      The difference with sample() above is that the current evidence is taken into
      account. Soft evidences are not taken into account

      \sa sample()
  */
  plValues gibbs_sample(size_t n=1) const;

  /** Run Gibbs sampling to get \em nsamples samples.
      for each sample, the sampler is run for a \em n steps and return the last state

      \sa sample()
  */
  std::vector<plValues> gibbs_samples(size_t nsamples, size_t n=1) const;

  
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
      \param[in] classif_prob_thresholds The probability thresholds to be applied for classification
      \param[in] nrows The number of data rows to be used for this prediction (starting from the current position in the input data source)

      \sa prediction()
  */
  void apply_prediction(const plVariablesConjunction& target_variables,
                        plDataDescriptor& data,
                        const std::string& output_file,
                        const std::vector<plProbValue>& classif_prob_thresholds=std::vector<plProbValue>(),
                        unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Same as above but using a vector of plValues */
  void apply_prediction(const plVariablesConjunction& target_variables,
                        const std::vector<plValues>& data,
                        const std::string& output_file,
                        const std::vector<plProbValue>& classif_prob_thresholds=std::vector<plProbValue>(),
                        unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Ask a query to the BN and apply it to a data set
      
      \param[in] target_variables The target variables to predict
      \param[in] data The input data set to be used for prediction
      \param[in] classif_prob_thresholds The probability thresholds to be applied for classification
      \param[in] nrows The number of data rows to be used for this prediction (starting from the current position in the input data source)
    
      \return for each data record, the value of @em target_variables that
      maximises the probability given the evidence provided by the data row
      P(target_variables | row_evidence). This value could be void (empty plValues) 
      if no prediction is provided (when using classif_prob_threshold).

      \sa prediction()

      \sa plCndDistribution::apply_prediction()
  */
  std::vector<plValues> apply_prediction(const plVariablesConjunction& target_variables,
                                         plDataDescriptor& data,
                                         const std::vector<plProbValue>& classif_prob_thresholds=std::vector<plProbValue>(),
                                         unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Same as above but using a vector of plValues */
  std::vector<plValues> apply_prediction(const plVariablesConjunction& target_variables,
                                         const std::vector<plValues>& data,
                                         const std::vector<plProbValue>& classif_prob_thresholds=std::vector<plProbValue>(),
                                         unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Same as  apply_prediction() returing std::vector<plValues> but returning the classification probabilities for each data row
      
      \sa prediction()

      \sa apply_prediction()
      
      \sa plCndDistribution::apply_prediction()
  */
  std::vector<std::vector<plProbValue> > apply_classification_proba(const plVariablesConjunction& target_variables,
                                                                    plDataDescriptor& data,
                                                                    unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Same as above but using a vector of plValues */
  std::vector<std::vector<plProbValue> > apply_classification_proba(const plVariablesConjunction& target_variables,
                                                                    const std::vector<plValues>& data,
                                                                    unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;
  
  /** Return the value of @em target_variables that maximises the probability given the evidence @em evidence 
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
  plValues prediction(const plVariablesConjunction& target_variables, const plValues& evidence,
                      const std::vector<plProbValue>& prob_thresholds=std::vector<plProbValue>()) const;
  
  /** Ask a query to the model, apply it to a data set, and provide validation statistics 
      
      \param[in] target_variables The target variables to predict
      \param[in] data The input data set to be used for validation
      \param[in] output_file The output file. It will include the columns (variables) of the input data set
      with additional columns corresponding the predicted target variables and the corresponding distribution table 
      \param[in] classif_prob_thresholds The probability thresholds to be applied for classification
      \param[in] nrows The number of data rows to be used for this validation (starting from the current position in the input data source)

      \return The validation results
      
      \sa plCndDistribution::validate_prediction()
  */
  plPredictionPerformanceReport validate_prediction(const plVariablesConjunction& target_variables,
                                                    plDataDescriptor& data,
                                                    const std::string& output_file="",
                                                    const std::vector<plProbValue>& classif_prob_thresholds=std::vector<plProbValue>(),
                                                    unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Same as above but using a vector of plValues */
  plPredictionPerformanceReport validate_prediction(const plVariablesConjunction& target_variables,
                                                    const std::vector<plValues>& data,
                                                    const std::string& output_file="",
                                                    const std::vector<plProbValue>& classif_prob_thresholds=std::vector<plProbValue>(),
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

  /** Compute the log-likelihood of a given record */
  plFloat compute_data_record_log_likelihood(const plValues& data_record) const;

  /** Compute the log-likelihood of a given data set
      
      \param data_set the data set for which the log-likelihood will be computed
      \param[in] nrows The number of data rows to be used 
      (starting from the current position in the input data source)

      \return log-likelihood of the data set
  */
  plFloat compute_data_set_log_likelihood(plDataDescriptor& data_set, 
                                          unsigned int nrows=std::numeric_limits<unsigned int>::max()) const;

  /** Compute the log-likelihood of a given vector of data records
      
      \param[in] data_records the vector of data records for which the log-likelihood will be computed

      \return log-likelihood of the data set
  */
  plFloat compute_data_set_log_likelihood(const std::vector<plValues>& data_records) const;

  /** Get the input distribution over the node @em node */
  plComputableObject get_node_distribution(const plVariable& node) const;
  
  /** Get the input distribution over the node @em node */
  plComputableObject get_node_distribution(const std::string& node_name) const
  {
    return get_node_distribution(get_node(node_name));
  }

  /** Get the input learnable distribution over the node @em node */
  const plLearnObject& get_node_learnable_distribution(const plVariable& node) const;

  /** Get the input learnable distribution over the node @em node */
  const plLearnObject& get_node_learnable_distribution(const std::string& node_name) const
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
  void add_time_transition_edge(const plVariable& node_prec_timeslice, 
                                const plVariable& node_current_timeslice);

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

      @sa next_time_slice()
  */
  void set_time_transition_distribution(const plCndDistribution& transition);

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

      @sa next_time_slice()
  */
  void set_time_transition_learnable_distribution(const plLearnObject& learnable_transition);
  
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

  /** Same as above while setting the evidence temporally to \em evidence */
  void next_time_slice(const plValues& evidence);

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
  void set_soft_evidence(const plDistribution& soft_evidence);

  /** Clear all soft evidences */
  void clear_soft_evidences();

  /** Set the convergence threshold for the EM algorithm (parameter learning).
      
      The provided threshold represents the relative log-likelihood change
      between two successive EM iterations (smaller value means more
      iterations). The default value is set to 0.0001
  */
  void set_em_convergence_threshold(plFloat em_convergence_threshold)
  {
    em_convergence_threshold_ = em_convergence_threshold;
  }

  /** Get an ancestral ordering of the variables */
  plVariablesConjunction ancestral_order_nodes() const
  {
    if(all_variables_.size() != ancestral_order_nodes_.size()) {
      ancestral_order_nodes_ = get_joint_distribution().ancestral_order_variables();
    }

    return ancestral_order_nodes_;
  }
  
#ifdef PL_ENABLE_CONVERTER

  /** Export to the format @em format.
      The allowed values are:
      - PL_FORMAT_GENIE: Genie xdsl
      - PL_FORMAT_NETICA: Netica dne
      - PL_FORMAT_HUGIN: Hugin net
  */
  void export_as(plImportExportFormat format, const std::string& file_name) const;

  /** Import from the format @em format.
      The allowed values are:
      - PL_FORMAT_GENIE: Genie xdsl
      - PL_FORMAT_NETICA: Netica dne
      - PL_FORMAT_HUGIN: Hugin net
  */
  void import_from(plImportExportFormat format, const std::string& file_name);
  
  /** Save as Genie xdsl file */
  void save_as_genie(const std::string& file_name) const;

  /** Load from a Genie xdsl file */
  void load_from_genie(const std::string& file_name);

  /** Save as Netica dne file */
  void save_as_netica(const std::string& file_name) const;

  /** Load from a Netica dne file */
  void load_from_netica(const std::string& file_name);

  /** Save as Hugin net file */
  void save_as_hugin(const std::string& file_name) const;

  /** Load from a Hugin net file */
  void load_from_hugin(const std::string& file_name);
#endif // PL_ENABLE_CONVERTER

 protected:
  void Output(std::ostream& os) const;
  
 private:
  
  const plDistribution &get_belief_internal(const plVariablesConjunction& node_conjunction,
                                            const plValues& evidence,
                                            bool force_no_compilation)const;
  
  const std::vector<plProbValue> &get_belief_table_internal(const plVariablesConjunction& node_conjunction, const plValues& evidence)const;
  void update_decomposition()const;
  
  void update_decomposition(const plComputableObjectList& dec)const;
  
  void decomposition(plComputableObjectList&) const;

  std::vector<plLearnObject*> get_learn_objects()const;

  void replace_distribution(const plVariable& node,
                            plLearnObject* learnable_distribution);

  void replace_learn_objects(const std::vector<plLearnObject*>& new_learn_objects);

  void replace_structure(const bn_edge_list_t& edges);

  bn_edge_list_t get_all_class_to_attrib_edges(const plVariable& class_node) const;

  plFloat compute_data_record_log_likelihood_internal_wo_check(const plValues& defined_data_record) const;

  void ask(const plVariablesConjunction& vars,
           plDistribution& response)const;
  
  void ask(const plVariablesConjunction& left_vars,
           const plVariablesConjunction& right_vars,
           plCndDistribution &cnd_response,
           bool ignore_normalization)const;

  const plDistribution &get_belief_internal(kplQuery& query,
                                            const plValues& evidence,
                                            bool force_no_compilation)const;

  const std::vector<plProbValue> &get_belief_table_internal(kplQuery &q, const plValues& evidence) const;

  void clear_query_cache() const;
  
  void invalidate_query_cache() const;
  
  void clear_time_slice_queries();

  void create_time_slice_query(const plVariable&node_prec, 
                               const plVariable &node_current);

  void create_time_slice_query_for_learnable(const plVariable&node_prec, 
                                             const plVariable &node_current);

  void finalize_time_slices_copy(bool delete_old=false);

  void set_from_decomposition(const plComputableObjectList &decomposition);

  void update_joint() const;

  plFloat learn_parameters_em_internal(plEMLearner &em,
                                       plDataDescriptor &data_set,
                                       int em_iterations,
                                       bool no_missing_data );
  template <typename T>
    void set_distribution_impl(const T& distribution);

  learnable_map_t::const_iterator
    get_node_learnable_iterator(const plVariable& node) const;

  learnable_map_t::iterator
    get_node_learnable_iterator(const plVariable& node);
  
  void output_table(std::ostream& outs,
                    const plVariablesConjunction& target_variables,
                    const plValues& actual_evidence) const;
  
  void output_results(std::ostream& outs,
                      const plValues* input_values, 
                      const std::vector<bool>* input_def,
                      const plValues& predicted,
                      const plVariablesConjunction& target_variables,
                      bool is_classif,
                      const plValues& actual_evidence) const;

  bool filter_by_threshold(const plValues& inferred_value,
                           const plVariablesConjunction& target,
                           bool is_classif,
                           const plVariableIndexer& target_indexer,
                           const std::vector<plProbValue>& prob_thresholds,
                           const plValues& actual_evidence) const;

  template <typename T>
    void add_evidence_impl(const plVariable& evidence_variable, T val, bool check_validity);

  template <typename T>
    void add_evidence_impl_name(const std::string& evidence_variable_name, T val, bool check_validity);

  void compile_mc(const plDistribution& dist,
                  plDistribution& compiled_dist,
                  plProbTable& compiled_response_as_probtable) const;

  void em_update_most_probable(plLearnDistributions &complete_data_learner,
                               plValues &all_variables_values,
                               const plValues &defined_data_values,
                               const plVariablesConjunction &missing_vars);

  void em_update_probability_weighted(plLearnDistributions &complete_data_learner,
                                      plValues &all_variables_values,
                                      const plValues &defined_data_values,
                                      const plVariablesConjunction &missing_vars);

  void em_update_mcmc(plLearnDistributions &complete_data_learner,
                      plValues &all_variables_values,
                      const plValues &defined_data_values,
                      const plVariablesConjunction &missing_vars);

  kplQuery *get_query_from_cache_or_create(const plVariablesConjunction& left,
                                           const plVariablesConjunction& right) const;
  template<typename T>
    void sample_impl(T &output, unsigned int nsamples,
                     const std::vector<plProbValue> &missing_probabilities)const;

  template<typename T>
    void sample_static_impl(const plJointDistribution &joint,
                            T &output, unsigned int nsamples,
                            const std::vector<plProbValue> &missing_probabilities)const;

  template<typename T>
    void sample_dynamic_impl(const plJointDistribution &joint,
                             T &output, unsigned int nsamples,
                             const std::vector<plProbValue> &missing_probabilities)const;
  
  template <typename DATA_T>
    void learn_parameters_no_em_impl(DATA_T &data_set);

  template <typename DATA_T>
    plFloat learn_parameters_em_impl(DATA_T &data_set,
                                     int em_iterations,
                                     bool no_missing_data,
                                     bool use_most_probable_value);

  bool next_defined_data_record(plDataDescriptor &data,
                                const plValues *&values,
                                size_t& /* idx */) const;

  inline bool next_defined_data_record(const std::vector<plValues> &data,
                                       const plValues *&values,
                                       size_t& idx) const
  {
    if(idx < data.size()) {
      values = &data[idx++];
      return true;
    }
    
    idx = 0;
    values = 0;
    return false;
  }

  template<typename T>
    plFloat learn_parameters_em_no_init(T &data_set,
                                        int em_iterations,
                                        bool no_missing_data,
                                        bool use_most_probable_value);
  
 protected:
  SPL_ACCESS2(plBayesianNetwork);
  friend class kplTimeSliceQuery;
  
  plValues evidence_;
  plVariablesConjunction all_variables_;
  learnable_map_t decomposition_learnable_;
  std::string name_;
  plInferenceAlgorithm inference_algorithm_;

  mutable plJointDistribution* joint_;
  mutable plJunctionTree* jt_;

  mutable bool invalidated_decomposition_;

  bool verbose_sl_;
  bool verbose_em_;

  bool compiled_belief_distribution_;

  std::vector<kplTimeSliceQuery*> time_slice_queries_;
  
  mutable query_cache_t query_cache_;
  unsigned int query_cache_max_size_;

  plLearnDistributions *non_em_learner_;

  bool enable_sanity_checking_;

  plComputableObjectList soft_evidences_;

  plOptimizationCriterion inference_optimization_criterion_;

  plFloat em_convergence_threshold_;

  int mcmc_nsamples_;

  // Used for multidimensional distributions:
  // The first variable of the conjunction is the representative of the remaining ones
  std::map<plVariable, plVariable> representative_;

  mutable plVariablesConjunction ancestral_order_nodes_;

  mutable plValues gibbs_current_state_;
  mutable plVariablesConjunction gibbs_variables_to_sample_;
  mutable plValues data_next_defined_values_;
};

#endif /* PLBAYESIANNETWORK_H */

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
