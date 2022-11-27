/*=============================================================================
 * Product        :
 * File           : pmBayesianNetwork.h
 * Author         :
 * Creation       :

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */
#ifndef pm_BAYESIAN_NETWORK_H
#define pm_BAYESIAN_NETWORK_H
#include <pl.h>

#include <random>
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/filesystem/path.hpp>
#include <pilgrim/general/pmCommonTypeDefinitions.h>
#include <pilgrim/general/pmCSVDataSet.h>
#include <stdio.h>
#include <plSerializer.h>
#include <plExternalFunctionFromC.h>
#include <boost/graph/graph_traits.hpp>

#include <pilgrim/general/random/GraphUtility.h>
#include <pilgrim/general/random/PMMixed.h>

//=============================================================================
/**
 *  Can be consider boost graph bundled properties, an easy and efficient way.
 */
namespace boost
{
   struct computable_object_t
   {
      typedef vertex_property_tag kind;
   };

   struct learn_object_t
   {
      typedef vertex_property_tag kind;
   };

   struct cycle_detector : public dfs_visitor<>
   {
      cycle_detector(bool &has_cycle)
          : _has_cycle(has_cycle) {}

      template <class Edge, class Graph>
      void back_edge(Edge, Graph &)
      {
         _has_cycle = true;
      }

   protected:
      bool &_has_cycle;
   };
} // namespace boost

namespace PILGRIM
{
   /**
 *  Definition of pmGraph using boost library
 *  \typedef
 */
   typedef boost::property<boost::vertex_index_t, unsigned int,
                           boost::property<boost::computable_object_t, plComputableObject *,
                                           boost::property<boost::learn_object_t, plLearnObject *>>>
       pmVertexProperty;

   typedef boost::property<boost::edge_weight_t,
                           pmScoreValueType,
                           boost::property<boost::edge_name_t,
                                           std::string>>
       pmEdgeProperty;
   // edge_name property added by YASIN for edge label
   typedef boost::adjacency_list<boost::vecS,
                                 boost::listS,
                                 boost::bidirectionalS,
                                 pmVertexProperty,
                                 pmEdgeProperty>
       pmGraph;

   /**
 *  Different objects of pmGraph
 *  \typedef
 */
   typedef pmGraph::vertex_descriptor pmNode;
   typedef pmGraph::edge_descriptor pmEdge;
   typedef pmGraph::vertex_iterator pmNodeIterator;
   typedef pmGraph::edge_iterator pmEdgeIterator;
   typedef pmGraph::out_edge_iterator pmOutEdgeIterator;
   typedef pmGraph::in_edge_iterator pmInEdgeIterator;
   typedef pmGraph::adjacency_iterator pmOutNodeIterator;
   typedef pmGraph::inv_adjacency_iterator pmInNodeIterator;

   // std::map<pmGraph::vertex_descriptor, int> vertex_intMap;

   //=============================================================================
   /*!
   *  \brief pmBayesianNetwork class
   *
   *  \details This class header represents a Bayesian Network model. It uses Boost graph for the definition of Bayesian networks graph.
   *  \author  BRISCAN Cristian
   *  \author  Amanullah YASIN
   *  \version   PILGRIM 0.1
   *  \date      March 11, 2013
   *  \pre       If there are initialization conditions or configuration parameters then write here.
   *  \bug       If there is known bug, please put here
   *  \warning   There is no warning.
   *  \todo      This class can be updated using smart pointers. An interface to
   *  plBayesianNetwork, using plJointDistribution. Specially to read different file formates and to write too.
   *  \todo      Interface to boost library can be added as Mr. F. Schnitzler did.
   */

   class pmBayesianNetwork
   {
   public:
      /*!
     *  \brief Default constructor.
     */
      pmBayesianNetwork();

      /**
     * \brief Main constructor
     * \details Constructs a new bayesian network, giving the set of
     * variables {Xi} which will compose it. A bayesian network is Boost graph object.
     * Every node i can have an attached probability law P(Xi), in the form of a
     * computable object. An arc from node j to node i represents the fact that
     * the probability law of the variable i is dependant of the variable j
     * P(Xi | ... Xj, ...).
     *
     *  \param variables : The plVariablesConjunction containing the variables that compose Bayesian network.
     */
      pmBayesianNetwork(const plVariablesConjunction &variables);

      /**
     * \brief Copy constructor.
     * \details Copies a Bayesian network into another one.
     * \param[in] bn Bayesian Network object
     * \param[in] copyComputableObjects Copy the ComputableObjects in the new Bayesian Network (default: true).
     * \param[in] copyLearnObjects Copy the LearnObjects in the new Bayesian Network (default: false).
     */
      pmBayesianNetwork(pmBayesianNetwork &bn, bool copyComputableObjects = true, bool copyLearnObjects = false);

      /**
     * \brief Copy constructor.
     * \details Copies a Bayesian network into another one.
     * \param[in] jd plJointDistribution
     */
      pmBayesianNetwork(const plJointDistribution &jd);
      /**
     * \brief Copy constructor.
     * \details It copies a Bayesian network into another one.
     * \param[in] jd plJointDistribution
     */
      pmBayesianNetwork(plJointDistribution &jd, pmGraph &gr);
      /**
     * \brief Copy constructor for plBayesianNetwork type object
     * \details It copies a plBayesiannetwork into pmBayesianNetwork.
     * \param[in] plBN plByesianNetwork
     */
      pmBayesianNetwork(plBayesianNetwork &plBN);

      /**
     * \brief Constructor from file
     * \details Supported files are: genie
     * \param[in] file_name name of the instance file
     */
      pmBayesianNetwork(const std::string &file_name, std::string jd_label = "jd");

      /**
     * \brief Destructor
     * \details It destroys the Bayesian network and the associated computable objects.
    */
      virtual ~pmBayesianNetwork();

      /**
     * \brief plVariablesConjunction
     * \return a constant reference to the variables that compose the network.
     */
      const plVariablesConjunction &getVariables();
      const plVariablesConjunction &getVariables() const;

      /**
     * \brief To get single variable as plVariable
     * \details It returns the symbol number n from the list of variables that compose the network.
     * \param[in] n index of the variable
     * \return plVariable
     */
      const plVariable getVariable(const unsigned int n) const;

      double getForgettingFactor();

      pmGraph getCPDAG();

      void setForgettingFactor(double forgettingFactor);

      /**
     * \brief
     * \param[in] n number of variables
     * \param[in] min_vals vector of minimum values (min range) to compute cardinality of each variable
     * \param[in] max_vals vector of maximum values (max range) to compute cardinality of each variable
     * \return plvariableconjunction type object
     */
      plVariablesConjunction createVars(int n,
                                        std::vector<long double> min_vals,
                                        std::vector<long double> max_vals);

      /**
     * Input: number of variables n and vector containing cardinality of each variable
     * Return: it return plvariableconjunction type object
     */
      plVariablesConjunction createVars(int n, std::vector<int> cards);

      /**
      It returns the node corresponding the variable var.
     */
      pmNode getNode(const plVariable &var);

      /**
      Returns a pair of iterator (begin, end) to the vertices of the underlying graph
     */
      std::pair<pmNodeIterator, pmNodeIterator> vertices() const;

      /**
      It returns the symbol associated with the given node of the network.
     */
      const plVariable getVariable(const pmNode &node) const;

      /**
      It returns the index of the given node.
     */
      unsigned int getVariableIndex(const pmNode &node) const;

      /**
      It returns the index of the given symbol.
     */
      unsigned int getVariableIndex(const plVariable &var) const;

      /**
      It returns the set of parents of the given node.
     */
      plVariablesConjunction getParents(const pmNode &node) const;

      /**
      Only for trees or forests.
      It returns the parent of the given node.
     */
      plVariable getParent(const pmNode &node);

      /**
      It returns the set of children (variables) of the given node.
     */
      plVariablesConjunction getChildren(const pmNode &node);

      /**
      It returns the set of children (nodes) of the given node.
     */
      std::vector<pmNode> getChildNodes(const pmNode &node);

      /**
      It returns the law of probability of the given node.
     */
      plComputableObject *getComputableObject(const pmNode &node) const;

      /**
      It returns the object used for parameter learning.
     */
      plLearnObject *getLearnObject(const pmNode &node) const;

      /**
      It sets the law of probability of the given node. The existing plComputableObject
      associated with this node will be destroyed. The given computable object
      must be a new one, and it will be taken in the charge of the bayesian network,
      which will be responsable for it's destruction.
     */
      void setComputableObject(const pmNode &node, plComputableObject *pCompObj);

      /**
      It returns the list of computable objects. The list contains a computable
      object for every node of the network, even for nodes that don't have an
      associated one.
     */
      plComputableObjectList getComputableObjectList() const;

      std::vector<plLearnObject *> getLearnObjectList() const;
      /**
     *  It sets to the graph the parameter learning objects defined by the vector passed in parameters
     */
      void setLearnObject(std::vector<plLearnObject *> &learnObjects);

      void setLearnObjectML();

      void setLearnObjectMLbis();

      void setLearnObjectEAP(pmScoreValueType priorWeight);

      /**
      It adds one learn object for every node of the network in the given list.
      The learn objects are pushed at the end of the list.
     */
      void setLearnObjectList(std::vector<plLearnObject *> &learnObjectList);

      void setLearnObjectListbis(std::vector<plLearnObject *> &learnObjectList);

      /**
      It adds one learn object for every node of the network in the given list.
      The learn objects are pushed at the end of the list. All learn objects in
      the list use bayesian prior to compute the kernels.
     */
      void setBayesianLearnObjectList(
          std::vector<plLearnObject *> &learnObjectList,
          pmScoreValueType priorWeight);

      /**
      It destroys the given list of learn object.
     */
      static void freeLearnObjectList(
          std::vector<plLearnObject *> &learnObjectList);

      /**
      Creates a new computable object for a given node. The computable object is
      not attached to the node. The parameters of the newly created kernel can be
      uniform or randomly chosen.
     */
      plComputableObject *newComputableObject(const pmNode &node, bool random) const;

      /**
      It inits the list of the computable objects associated with every node. The
      existing computable objects can be replaced with new ones, and the ones that
      are newly created can have be uniform or random.
     */
      void initComputableObjectList(bool random, bool resetExistingObjects);

      /**
      It creates a new joint distribution from the current bayesian network. The user
      is responsable for its destruction.
     */
      plJointDistribution getJointDistribution() const;

      void infereParameters(pmBayesianNetwork *pFromBN);

      /**
    Method for incremental learning, only when the distributions are either
    plProbTable or plDistributionTable
    */
      /**
     * It takes a Datadescriptor, and apply parameter learning to the BN
     * Algorithm choice in the learn object generation is made by prior setters :
     * setLearnObject(.) , setLearnObjectML() , setLearnObjectEAP(.)
     */
      void learnParameters(pmCSVDataSet *DataDscptr);
      
      void update(plValues &old_line, plValues &new_line);
      
    

      /**
      * \brief Learns parameters from the data set DataDscptr.
      * By default the learning is non-incremental.
      * If you want an incremental learning call pmBayesianNetwork::setForgettingFactor(double forgFactor) before.
      * The incremental learning only works for simple distributions (plProbTable and plDistributionTable)
      * This function use a different method than learnParameters. The incrementation is done on data instead of probabilities.
      * \param DataDscptr pointer to the Data set use for learning
      */
      void learnParametersbis(pmCSVDataSet *DataDscptr);

      /**
      It returns a constant reference to the list of cardinalities of the variables
      which compose the network.
     */
      const std::vector<unsigned int> &getCardinalities();

      /**
      It returns the cardinality of the node with index number n.
     */
      unsigned int getCardinality(const unsigned int n) const;

      /**
      It returns the highest cardinality of the variables that compose the network.
     */
      unsigned int getMaxCardinality();

      /**
      It computes the dimension of the whole network. It is the sum of the dimension
      of every node.
     */
      unsigned int computeDim();

      /**
      It computes the dimension of the given node. The dimension of the node is the
      number of parameters which must be known in order to define the probability
      law of the node. For a discrete node Xi, its dimension is the cardinality of
      that node minus 1, multiplied with the cardinalities of all parents.
     */
      unsigned int computeNodeDim(const pmNode &node);

      /**
      It computes the dimension of the parents of a given node. It is the product of
      cardinalities of the parents. If the node has no parents, the dimension is 1.
     */
      unsigned int computeParentDim(const pmNode &node);

      /**
      It prints the bayesian network on the standard output. For every node, the list
      of parents is printed, along with its law of probability.
     */
      void printIt();

      /**
      Prints a summary of the bayesian network on stdout
     */
      void summary(bool displayLearnObject=false);

      void print_graph();

      bool printStructureDifferences(pmBayesianNetwork *pWithRespectToBN);

      const pmGraph &get_graph() const { return graph; }
      pmGraph &get_graph() { return graph; }

      /**
     * \brief Call after the constructor, it creates a naive bayesian structure.
     * \param[in] numVar : Define which node is the main node of the structure
     */
      void naiveBayes(int numVar);

      pmGraph graph;

      /**
      It prints the structure bayesian network on the stream : for every node, the list
      of parents is printed.
     */
      void printEdges(std::ostream &argstream);

      /**
      Methods used to load a bayesian network from a xml file
     */
      static pmBayesianNetwork load_from_xml(const std::string &file_name, std::string jd_name);

      /**
      Methods used to save a BN in a file
     */
      void save_as_genie(const std::string &file_name);
      void save_as_xml(const std::string &file_name, std::string jd_name = "jd");

      /**
      Method used to generate a visualization of the graph
     */
      void draw_graph_dot(const std::string &file_name);

      /**
      Method used to generate a CSV format file
     */
      void generate_csvData(const std::string &dataset_filename,
                            unsigned int nb_rows);

      /**
      Method used to generate a CSV format file with random incomplete data
     */
      void generate_csvIncompleteData(const std::string &dataset_filename,
                                      unsigned int nb_rows,
                                      std::vector<unsigned int> &indexes,
                                      std::vector<double> &probs_unk);

      //fonction qui permet de générer aléatoirement des valeurs pour chaque noeud du réseau
      std::vector<std::string> create_test_values();
      /***************************************************
     * Interface for boost graph library's functions
     ***************************************************/

      // Interface to boost library, added by F. Schnitzler on 23 september 2010
      // Number of vertices
      unsigned int num_vertices() const
      {
         return boost::num_vertices(graph);
      }

      // Number of edges
      // Number of vertices
      unsigned int num_edges() const
      {
         return boost::num_edges(graph);
      }

      // returns the source vertex of edge e
      pmNode source(const pmEdge &e) const
      {
         return boost::source(e, graph);
      }

      // returns the target vertex of edge e
      pmNode target(const pmEdge &e) const
      {
         return boost::target(e, graph);
      }

      // Edges
      // -----

      std::pair<pmEdgeIterator, pmEdgeIterator> edges() const
      {
         return boost::edges(graph);
      }

      // Check for the presence of an arrow
      bool is_arrow(pmNode u, pmNode v) const
      {
         return (boost::edge(u, v, graph).second && !boost::edge(v, u, graph).second);
      }

      // Check for the presence of a line
      bool is_line(pmNode u, pmNode v) const
      {
         return (boost::edge(u, v, graph).second && boost::edge(v, u, graph).second);
      }

      // Check whether two vertices are adjacent
      bool is_adjacent(pmNode u, pmNode v) const
      {
         return (boost::edge(u, v, graph).second ||
                 boost::edge(v, u, graph).second);
      }

      bool is_adjacent(unsigned int uInd, unsigned int vInd) const
      {
         return is_adjacent(boost::vertex(uInd, graph),
                            boost::vertex(vInd, graph));
      }

      // Parents of a vertex
      pmNode get_parent(pmNode v) const
      {
         // output the father of v
         pmInNodeIterator a1, a2;
         boost::tie(a1, a2) = inv_adjacent_vertices(v, graph);
         return *a1;
      }

      std::pair<pmInNodeIterator,
                pmInNodeIterator>
      parents(pmNode v) const
      {
         pmInNodeIterator a1, a2;
         boost::tie(a1, a2) = inv_adjacent_vertices(v, graph);
         return make_pair(a1, a2);
      }

      // Children of a vertex
      std::pair<pmOutNodeIterator,
                pmOutNodeIterator>
      children(pmNode v) const
      {
         return adjacent_vertices(v, graph);
      }

      // Structure modification
      // ----------------------

      /**
     * To be used only on a BayesianNetwork for which the plComputableObjectList is not yet calculated
     *
     * /todo Modify this function in order to update the plComputableObjectList if it exists
     */
      void add_arrow(plVariable u, plVariable v)
      {
         boost::add_edge(getNode(u), getNode(v), graph);
      }

      void add_arrow(pmNode u, pmNode v)
      {
         boost::add_edge(u, v, graph);
      }

      void remove_arrow(pmNode u, pmNode v)
      {
         boost::remove_edge(u, v, graph);
      }

      void add_line(pmNode u, pmNode v)
      {
         boost::add_edge(u, v, graph);
         boost::add_edge(v, u, graph);
      }

      void remove_line(pmNode u, pmNode v)
      {
         boost::remove_edge(u, v, graph);
         boost::remove_edge(v, u, graph);
      }

      void remove_line(unsigned int uInd, unsigned int vInd)
      {
         return remove_line(boost::vertex(uInd, graph),
                            boost::vertex(vInd, graph));
      }

      void clear_all_edges()
      {
         pmNodeIterator vi1, vi2;
         for (boost::tie(vi1, vi2) = vertices(); vi1 != vi2; ++vi1)
         {
            boost::clear_out_edges(*vi1, graph);
         }
      }

      plDistribution query(plVariable &target_variable, plValues &evidence);
      plDistribution query(plVariable &target_variable);

      /**
     * \brief
     * \details
     * \param variables :
     **/
      double computeKLDivergence(pmBayesianNetwork &BN_compared,
                                 bool usingMCMCEstimation);

      // begin Tedy Gherasim - reprises de la classe ResultAnalysis quand j'ai
      // recupéré la fonction compareBNs
      /**
     * \brief
     * \details
     * \param variables :
     */
      void compareBNs(pmGraph &origG,
                      pmGraph &computedG,
                      unsigned int &TP,
                      unsigned int &FP,
                      unsigned int &TN,
                      unsigned int &FN,
                      unsigned int &strHammingDistance);
      pmGraph convertDAGintoCPDAG(pmGraph graph);
      void print_CPDAG(pmGraph graph);
      
      /* Order the nodes in the topological order */
      void orderNodes(pmGraph &graph, std::vector<pmNode> &ndList, bool reverseOrder=false);
      /* Order the edges in the topological order */
      std::vector<pmEdge> orderEdges(pmGraph &graph, std::vector<pmNode> &ndList, bool reverseOrder=false);
      
      bool isParent(pmNode &w, pmNode &y, pmGraph &graph, pmEdge *wyEdge);
      void labelEdges(pmEdge *xyEdge,
                      pmNode &y,
                      pmGraph &graph,
                      std::string label,
                      bool onlyUnknownEdges,
                      bool incidentEdgesToo);
      bool isExistZYedgeAndZnotParentOfX(pmNode &x,
                                         pmNode &y,
                                         pmEdge *xyEdge,
                                         pmGraph &graph);
      // end //Tedy Gherasim - reprises de la classe ResultAnalysis quand j'ai
      // recupéré la fonction compareBNs
      //generation aléatoire de données dans un csv à partir d'un reseau bayésien

      void copy_structure(pmBayesianNetwork &bn);
      void generateRandomGraph(unsigned int nbNodes,
                               unsigned int maxInducedWidth,
                               unsigned int nbIterations,
                               unsigned int maxNodeDegree,
                               unsigned int maxNbEdges);

      /**
      * \brief PrivBayes application (differential privacy), add noise on computed probabilities
      * using Laplace distribution.
      * Use it after calling pmBayesianNetwork::learnParameters(pmCSVDataSet *DataDscptr) or
      * pmBayesianNetwork::learnParametersbis(pmCSVDataSet *DataDscptr)
      * \param DataDscptr pointer to the Data set used for learning, the Data set's
      * size will be use to calculate the scale of the Laplace distribution.
      * \param epsilon  A double, use to calculate the scale of the Laplace distribution.
      */
      void noisyConditionals(pmCSVDataSet *DataDscptr,double epsilon);

      //weight of BN in multinet algo
      double weight;

   protected:
      void resetComputableObject(const pmNode &node,
                                 plComputableObject *pCompObj);
      void computeCardinalities();
      void copyStructure(pmBayesianNetwork &bn);
      void copyComputableObjects(pmBayesianNetwork &bn);
      static plComputableObject *copyComputableObject(
          const plComputableObject &compObj);
      void copySingleComputableObject(const plVariable &var,
                                      plComputableObject *pCompObj);
      void copyMultiComputableObject(const plJointDistribution &jd,
                                     const plVariablesConjunction &vars,
                                     plComputableObject *pCompObj);

   private:
      /**
      It inits the curent bayesian network.
     */
      void initBayesianNetwork(const plVariablesConjunction &variables);

      void initBayesianNetworkFromJD(plJointDistribution &jd);

      // begin Tedy Gherasim - reprises de la classe ResultAnalysis quand j'ai
      // recupéré la fonction compareBNs
      std::vector<pmNode> nodeList;
      // end Tedy Gherasim - reprises de la classe ResultAnalysis quand j'ai
      // recupéré la fonction compareBNs

   protected:
      friend class pmUtility;
      template <typename rowDataType>
      friend class pmTwoNodepminkScore;
      template <typename rowDataType>
      friend class pmTwoNodepminkByDScore;
      template <typename rowDataType>
      friend class pmScoreAIC;
      template <typename rowDataType>
      friend class pmScoreBDeu;
      template <typename rowDataType>
      friend class pmScoreBIC;
      template <typename rowDataType>
      friend class pmScoreMDL;
      template <typename rowDataType>
      friend class pmMutualInformation;
      template <typename rowDataType>
      friend class pmDecomposableScore;
      template <typename rowDataType>
      friend class pmAlgorithm;
      template <typename rowDataType>
      friend class pmAlgoMWSTEM;
      template <typename rowDataType>
      friend class pmAlgoMWST;
      template <typename rowDataType>
      friend class pmAlgoK2;

      plVariablesConjunction variables;
      std::vector<unsigned int> cardinalities;
      unsigned int maxCardinality;
      boost::property_map<pmGraph, boost::vertex_index_t>::type vertex_id;
      boost::property_map<pmGraph, boost::computable_object_t>::type vertex_compObj;
      boost::property_map<pmGraph, boost::learn_object_t>::type vertex_learnObj;
      char *nameLearnAlgo;
      double forgettingFactor = 0;
      unsigned int PMMixed_nbNodes;
      unsigned int PMMixed_maxInducedWidth;
      unsigned int PMMixed_maxNodeDegree;
      unsigned int PMMixed_maxNbEdges;
   };
};     // namespace PILGRIM
#endif // pm_BAYESIAN_NETWORK_H
