/*=============================================================================
 * Product        : PILGRIM
 * File           : DynamicBayesianNetwork.h
 * Author         : GHADA TRABELSI
 * Creation       : Fev 01 2013

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */

  /**
   *  \brief pmDynamicBayesianNetwork class - a specific implementation of a Two-Timeslice Bayesian Network model (2TBN)
   *
   *  \details This class header represents a Dynamic Bayesian Network model and more precisely a Two-Timeslice Bayesian Network model (2TBN).
   *  \author  GHADA TRABELSI
   *  \author  Toader GHERASIM
   *  \author  Romain RINCE
   *  \date      March 11, 2013
   *  \pre       If there are initialization conditions or configuration parameters then write here.
   *  \bug       If there is known bug, please put here
   *  \warning   There is no warning.
   *  \todo      There is no todo.
   */


#ifndef pm_DYNAMIC_BAYESIAN_NETWORK_H
#define pm_DYNAMIC_BAYESIAN_NETWORK_H

#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <pl.h>
#include <plSerializer.h>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

#include "pilgrim/general/pm15dBN.h"
#include "pilgrim/general/pmUtility.h"
#include "pilgrim/general/pmBayesianNetwork.h"

namespace PILGRIM {
class pmDynamicBayesianNetwork {
 public:

	enum InferenceAlgorithms { Unroll_Static, Interface };

    //! Default constructor.
    pmDynamicBayesianNetwork();

    /**
     * \brief Main constructor. It constructs a new dynamic bayesian network (a 2TBN - Two-Timeslice Bayesian Network model), giving the two bayesian network (G0 - initial, G1 - transition) Models. 
	 * \param[in] G0 - the initial network - a pmBayesianNetwork corresponding to the instant 0 of the 2TBN. The names of all the variables (or vertices) of G0 must respect the pattern "namevariable_t0". 
	 * \param[in] G1 - the transition network  - a pmBayesianNetwork corresponding to a given instant "t". 
	 It contains 2 types of variables (vertices) : 
	 (1) variables corresponding to the step "t". The names of these variables must respect the pattern "namevariable_t";  
	 (2) variables corresponding to the step "t-1". The names of these variables must respect the pattern "namevariable". 
	 For each variable of type (2) named "myvariable" G1 must contain another variable named "myvariable_t" and G0 must contain a variable named "myvariable_t0".
    */
    pmDynamicBayesianNetwork(pmBayesianNetwork* G0, pmBayesianNetwork* G1, InferenceAlgorithms algorithm = Interface);

	/**
	* \brief Main constructor. It constructs a new dynamic bayesian network (a 2TBN - Two-Timeslice Bayesian Network model), giving the two bayesian network (G0 - initial, G1 - transition) Models.
	* \param[in] filenameDBN_T0 - the initial network file - a csv file corresponding to the data for instant 0 of the 2TBN".
	* \param[in] filenameDBN_T0 - the transition network file - a csv file corresponding to the data for mutiple instant. The data must be order like this A,B,A_t,B_t.
	* \param[in] entete_T0 - number of line that must be ignore when the data are reading for T0.
	* \param[in] entete_T0 - number of line that must be ignore when the data are reading for G_T.
	* \param[in] score_name_T0 - Score used to calculate the structure of T0.
	* \param[in] score_name - Score used to calculate the structure of G_T.
	* \param[in] algo - Determine witch algorithm will be used to make prediction.
	*/

	//COPY CONSTRUCTOR FOR DEBUG
	/*pmDynamicBayesianNetwork(const pmDynamicBayesianNetwork& other);*/

	//pmDynamicBayesianNetwork(char* filenameDBN_T0, char* filenameDBN, size_t entete_T0 = 1, size_t entete = 1, std::string score_name_T0 = "BDeu", std::string score_name ="BIC" , int algo = 0);
    /**
	 * TODO: doc 
	 */
	pmDynamicBayesianNetwork(const std::string& file_name, const std::string& jd0_label, const std::string& jdt_label, int algorithm=0);
	
	/**
      * \brief Destructor. 
      * \details It destroys the Dynamic bayesian network and the associated computable objects.
     */
    virtual ~pmDynamicBayesianNetwork();
    
	/**
      \brief It loads a DBN(2-TBN) from 2 genie/smile files.
	 * \param[in] fileNameG0 - the file correponding to the initial network (see the main construtor documentation for more details)
	 * \param[in] fileNameG1 - the file correponding to the transition network (see the main construtor documentation for more details)
     */
    pmDynamicBayesianNetwork * load_from_genie(const std::string &fileNameG0,
                                               const std::string &fileNameG1);
    /**
     * \brief It loads a DBN(2-TBN) from 2 Netica files
	 * \param[in] fileNameG0 - the file correponding to the initial network (see the main construtor documentation for more details)
	 * \param[in] fileNameG1 - the file correponding to the transition network (see the main construtor documentation for more details)
     */
	pmDynamicBayesianNetwork * load_from_netica(const std::string &fileNameG0,
                                                const std::string &fileNameG1);
    /**
     * \brief It loads a DBN(2-TBN) from 2 hugin files.
	 * \param[in] fileNameG0 - the file correponding to the initial network (see the main construtor documentation for more details)
	 * \param[in] fileNameG1 - the file correponding to the transition network (see the main construtor documentation for more details)
     */
	pmDynamicBayesianNetwork * load_from_hugin(const std::string &fileNameG0,
                                               const std::string &fileNameG1);
    /** 
	 * \brief Save the initial and the transition network the G0 and GT of the DBN (2-TBN) as 2 genie/smile files
     */
    void save_as_genie(const std::string &fileNameG0,
                       const std::string &fileNameG1);
	/** 
	 * \brief Save the initial and the transition network the G0 and GT of the DBN (2-TBN) as 2 Netica files
     */
    void save_as_netica(const std::string &fileNameG0,
                        const std::string &fileNameG1);
	/** 
	 * \brief Save the initial and the transition network the G0 and GT of the DBN (2-TBN) as 2 hugin files
     */
    void save_as_hugin(const std::string &fileNameG0,
                       const std::string &fileNameG1);
	
	void save_as_xml(const std::string& file,
					 std::string jd0_name="jd0",
					 std::string jdT_name="jdT");
	/**
	*\brief Set 0 to use the unroll method and 1 to use the interface algorithm
	*/
	void setAlgorithm(InferenceAlgorithms algo);

	pmBayesianNetwork* unroll(int t);
	pmBayesianNetwork* unroll2(int t);

	/**
	 * \brief Queries the dynamic bayesian network about P(target_variable, instant | evidenceVector)
	 * \param target_variable - a variable from the transition graph(G_t); the name of the variable can end with "_t" (but is not mandatory); examples : "myvariable", "myvariable_t"
	 * \param instant - for which time / step we calculate P(target_variable, instant | evidenceVector); it accepts values from 0 to n;
	 * \param evidenceVector - a vector<plValues> containing on each position i the evidence for the step i; if, for a step k there is no evidence, the vector must contain a void plValues(); the size of evidenceVector is not related to the value of the parameter instant (it can be inferior, equal or superior)
	 * \author  GHERASIM Toader
	*/
    plDistribution query(plVariable& target_variable, int instant, std::vector<plValues> * evidenceVector);

	plDistribution query2(plVariable& target_variable, int instant, std::vector<plValues> * evidenceVector);

	/**
	 * @brief 					This method allows to browse the network and ask multiple questions at once,
	 * 							the inference algorithm used is the one of the `pmDynamicBayesianNetwork` (`this->algorithm`). \n 
	 * 							If the algorithm has not been impelmented i this function yet, it will throw a `std::logic_error`. \n 
	 * 							The usable algorithms are :
	 * 							- `InferenceAlgorithms::Interface`
	 * 							- `InferenceAlgorithms::Unroll_Static`
	 * 
	 * > Example : \n 
	 * > >   **prediction**( \n 
	 * > > >	**targets_lists**=[{NodeA_t, NodeB_t}, {NodeA_t}, {NodeB_t}, {NodeB_t}], \n 
	 * > > > 	**instants_to_ask**=[0, 2, 3, 7], \n 
	 * > > > 	**all_evidences**=[{NodeB_t0=1, NodeC_t0=0}, {NodeB=1, NodeC_t=0}, {NodeB=0}] \n 
	 * > >   ) \n 
	 * > >   will return something like :
	 * @code {.json}
	 * [ { "NodeA_t":{"0": 0.57452 ; "1": 0.43658} }, { "NodeA_t":{"0": 0.3261 ; "1": 0.6849} }, { "NodeB_t": {"0": 0.4189 ; "1": 0.5921} } ]
	 * @endcode
	 * 
	 * @param targets_lists		A vector of plVariablesConjunction (list of targets) you want to ask the model for.
	 * 							The targets must be variables of the transition model (ex: _NodeA, NodeB_t_).
	 * @param instants_to_ask 	A vector of int that lists the instants in time at which we wanna ask the models (ex: 0, 4, 5, 8)
	 * 							(it must match the size of the `targets` as `targets_lists[n]` will be used at instant `instants_to_ask[n]`).
	 * @param all_evidences 	The list of evidences for each instant for _T0_ to _Tk_ with _k_ the last instant to question, evidences at _T0_ must end with **_t0**.
	 * 							(`instants_to_ask[size_of_instants - 1]`) (ex: _NodeA = value1, NodeC_t = value0_).
	 * @return std::vector<plComputableObjectList> The return value is the vector containing the **lists of targets** and their **marginalized probability table**.
	 */
	std::vector<plComputableObjectList> prediction(std::vector<plVariablesConjunction> targets_lists, std::vector<int> instants_to_ask, std::vector<plValues> all_evidences);
    
	/**
      It returns a constant reference to the variables that compose the network in G0.
     */
    const plVariablesConjunction& getVariablesforG0();
    /**
      It returns a constant reference to the variables that compose the network in G-> (or G1).
     */
    const plVariablesConjunction& getVariablesforGtransition();

    /** It creates a new joint distribution from the current initial network. The user
      is responsable for its destruction.
     */
    plJointDistribution getJointDistributionforG0();

    /** It creates a new joint distribution from the current transition network. The user
        is responsable for its destruction.
     */
    plJointDistribution getJointDistributionforGtransition();

	/** It creates a new transition from the current the current initial network. The user
        is responsable for its destruction.
     */
	pmBayesianNetwork& getTransition_G();
	pmBayesianNetwork& getInitial_G();
    void summary(bool displayLearnObject=false);



	/** 
	* \brief  Queries the dynamic bayesian network about P(target_variable, instant | evidenceVector)
	* \param target_variable - the target variable with a simple name (without "_t" or "_t0" at the end of the name); example : "myvariable"
	* \param instant - for which timestep we calculate P(target_variable, instant | evidenceVector); it accepts values from 0 to n;
	* \param evidenceVector - a vector<plValues> containing, on each position i, the evidence for the step i; if, for a step k there is no evidence, the vector must contain a void plValues(); the size of evidenceVector should be less than or equal to the value of the parameter instant 
    */
	
	plDistribution query_optimized(plVariable& target_variable,
                         int instant,
                         std::vector<plValues> * evidenceVector);

	pmBayesianNetwork& simpleGT();

	plDistribution query_15dbn(plVariable& target_variable,int instant, std::vector<plValues> evidenceVector);
	//sauvegarde les donn�es du DBN dans un fichier xml

	void serializeDBN(const std::string& file, std::string jd0_name="jd0", std::string jdT_name="jdT");

    void serialize(plSerializer& serializer, const std::string& jd0_name="jd0", const std::string& jdT_name="jdT");
	//importe les donn�es d'un DBN depuis un fichier xml

	//modifie l'algorithme qui sera utilis� pour faire l'inf�rence. 0 pour le DBN classique, 1 pour l'algorithme interface.
	void setInferenceMethod(int method);

	pmDynamicBayesianNetwork* load_from_xml(const std::string &file_name, std::string jd0 = "jd0", std::string jdT = "jdT");

	/**
	* \brief Create a csv file with random data from the DBN.
	* \param[in] dataset_filename - Name of the futur csv file.
	* \param[in] t_max - size of each sample.
	* \param[in] nbrOfSample - number of samples.
	* \param[in] num_sequence - display or not the number of the sequence
	* \param[in] num_time_slice - number of time slice represented on each line
	*/
	void generate_csv_data(const std::string &dataset_filename, unsigned int t_max, int nbrOfSample, bool num_sequence = true, int num_time_slice = 1);

	void clique_to_petri(std::vector<std::vector<std::vector<plVariable>>> all_cliquesParents);
	std::vector<std::vector<std::vector<plVariable>>> get_Linked_Vars();
	std::vector<std::vector<std::vector<plVariable>>> get_max_clique(std::vector<std::vector<std::vector<plVariable>>>finalLinkedVars);

 private:
    pmBayesianNetwork* initial_G;
    pmBayesianNetwork* transition_G;


	/**
	* \brief Create a csv file with random data from the DBN.
	* \param[in] dataset_filename - Name of the futur csv file.
	* \param[in] t_max - size of each sample.
	* \param[in] nbrOfSample - number of samples.
	* \param[in] num_sequence - display or not the number of the sequence
	* \param[in] num_time_slice - number of time slice represented on each line
	*/
	

	InferenceAlgorithms algorithm = Interface;
	
	/** 
	Temporal edge = an edge that links in G1 (transition_G) two vertices correponding to two different timesteps ("t-1" and "t"); "aVariable" <--> "anotherVariable_t"
	Etmp = The set of temporal edges of G1 (transition_G)
	Outgoing-Interface (OI) = all the vertices from G1 (transition_G) whose names dont end with "_t"

	The 1.5 DBN contains :
	- all the vertices (variables) of G1 (transition_G)
	- all the edges of G1
	- new edges (u,v) where u,v belong to G1_(t-1) and G1 contains and edge (u_t, v_t)
	- the correponding computable objects
     */
	pmBayesianNetwork* get_1_5_dbn();

	std::vector<plVariable> getOutgoingInterface();
	
	bool check_1_5_dbn(pmBayesianNetwork* the_1_5_dbn);

};
}  // namespace PILGRIM

#endif  // pm_DYNAMICBAYESIAN_NETWORK_H
