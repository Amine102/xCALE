/*=============================================================================
 * Product        : PILGRIM
 * File           : pm_15DBN.h
 * Author         : ED DAHMOUNI Bouthayna
 * Creation       : JUIN 30 2016

  *=============================================================================
 *        (c) Copyright 2013, PILGRIM- all rights reserved
 *=============================================================================
 */
#ifndef pm_15dBN_H
#define pm_15dBN_H
#pragma once

#include <pilgrim/general/pmDynamicBayesianNetwork.h>
#include <plSerializer.h>

#include "pilgrim/general/pmBayesianNetwork.h"
namespace PILGRIM {
    class pmDynamicBayesianNetwork;
}


//=============================================================================

/*!
   *  \brief pm15dBN class
   *
   *  \details This class header represents a 1.5 dynamic bayesian network. 
   *  \author  ED-DAHMOUNI Bouthayna
   *  \date      Juin 30, 2016
   *  \bug       The function "static pm15dBN unserialize(const std::string &file_name, std::string jd_id,std::string OI_id)" returns a warning that needs to be handled, it's due to the conjuction of a set of variables into the for loop.
   *  \todo      Source code can be optimized
   */

class pm15dBN : public PILGRIM::pmBayesianNetwork {
public:
    /*!
     *  \brief Default constructor.
     */
    pm15dBN();

    /**
     * \brief Constructor from joint distribution
     * \details It constructs a new 1.5 dynamic Bayesian network, giving a joint distribution.
	 * the values of the attributes present and futur will be 0 and 1  respectively. 
     * \param[in] jd plJointDistribution
     */
    pm15dBN(const plJointDistribution& jd);


    /**
     * \brief Constructor from joint distribution
     * \details It constructs a new 1.5 dynamic Bayesian network, giving a joint distribution, the outgoingInterface and the values of the attributes present and futur.
     * \param[in] jd plJointDistribution
	 * \author T GHERASIM
     */
    pm15dBN(const plJointDistribution& jd, int present, int futur, std::vector<plVariable> outgoingInterface);

    /**
     * \brief Constructor from a Bayesian nework
     * \details It constructs a new 1.5 dynamic Bayesian network, giving a Bayesian network.
	 * It copies all the computable objects and initializes the present and the futur values to 0 and 1 respectively.
     * \param[in] bn pmByesianNetwork
     */
    pm15dBN(pm15dBN* bn);

    /**
     * \brief Constructor from a Dynamic Bayesian nework
     * \details It constructs a 1.5 dynamic Bayesian network, giving a dynamic Bayesian network.
	 * It defines the Outgoing Interface and initializes the present and the futur values to 0 and 1 respectively.
     * \param[in] dBN pmDynamicBayesianNetwork 
     */

    pm15dBN(PILGRIM::pmDynamicBayesianNetwork* dBN, plValues evidences = plValues());
    //pm15dBN(pmDynamicBayesianNetwork dBN);
    /**
      * \brief Copy constructor. 
      * \details It copies a 1.5 Dynamic Bayesian network into another one.
     */

    pm15dBN(const pm15dBN& dBN_15);


    /**
      * \brief Destructor. 
      * \details It destroys the 1.5 bayesian network.
     */
    //pm15dBN( pm15dBN* dBN_15);
    ~pm15dBN();


    int                     getPresent() const;
    int                     getFutur() const;
    std::vector<plVariable> getOutgoingInterface() const;
    void                    setOutgoingInterface(const std::vector<plVariable>& OI);


    /** 
	 * \brief Summary the 1.5 dBN and his Outgoing Interface
     */
    void summary_1_5();

    /**
     * \brief Serialize a 1.5 dBN and save it as a file under a customized extension.
	 * \param[in] file_name - the target file name (full uri : extension included).
	 * \param[in] jd_id - the joint distribution identifier - it's used to save and get back the jd from the file.
	 * \param[in] OI_id - the outgoing interface identifier - it's used to save and get back the outgoing interface from the file.
	 */
    void serialize(const std::string& file_name, const std::string& jd_id, const std::string& OI_id);
    /**
	* \brief Serialize a 1.5 dBN and save it as a plSerializer object.
	* \param[in] serial_save - the target plSerializer object.
	* \param[in] jd_id - the joint distribution identifier - it's used to save and get back the jd from the file.
	* \param[in] OI_id - the outgoing interface identifier - it's used to save and get back the outgoing interface from the file.
	*/
    void serialize(plSerializer& serial_save, const std::string& jd_id, const std::string& OI_id,
                   const std::string& time_id = "present_futur"); /**
     * \brief Unserialize a 1.5 dBN and return the equivalent 1.5 dBN.
	 * \param[in] file_name - the serialized file name.
	 * \param[in] jd_id - the joint distribution identifier - it's used to get back the jd from the file.
	 * \param[in] OI_id - the outgoing interface identifier - it's used to get back the outgoing interface from the file.
	 */
    static pm15dBN unserialize(const std::string& file_name, std::string jd_id, std::string OI_id);

    /**
	* \brief Unserialize a 1.5 dBN from a plSerializer object and return the equivalent 1.5 dBN.
	* \param[in] serial_load - the plSerializer object.
	* \param[in] jd_id - the joint distribution identifier - it's used to get back the jd from the file.
	* \param[in] OI_id - the outgoing interface identifier - it's used to get back the outgoing interface from the file.
	*/
    static pm15dBN unserialize(plSerializer serial_load, std::string jd_id, std::string OI_id, std::string time_id = "present_futur");

    /**
		*\brief inc
	*/
    plComputableObjectList query_JT(plVariablesConjunction target_variable, plValues evidence, bool recupEv = false);
    /**
		*\brief inc
	*/
    plComputableObjectList query_JT(plVariablesConjunction target_variable);

    plComputableObjectList query_JD(plVariablesConjunction target_variable, plValues evidence, bool recupEv = false);
    /**
	*\brief inc
	*/
    plComputableObjectList query_JD(plVariablesConjunction target_variable);
    /**
	 * \brief Query the 1.5 dynamic bayesian network about P(target_variable, instant_t | evidences). This method does not modify (or updates) the current pm15dBN object.
	 * \param target_variable 
	 * \param instant - for which time / step we calculate P(target_variable, instant_t | evidences); it accepts values from 0 to n;
	 * \param evidences - a vector<plValues> containing on each position i the evidence for the step i; if, for a step k there is no evidence, the vector must contain a void plValues(); the size of evidenceVector is not related to the value of the parameter instant (it can be inferior, equal or superior)
	*/

    /**
	 * \brief Predict the states of a set of nodes in different instants and returns a vector(multidimensional) of probabilities : vector of probabilities for all the nodes for each timestep.
	 * \param[in] nodes - a vector<plVariable> containing the nodes we need to predict.
	 * \param[in] instants - a vector<int> containing instants on which we want to predict the probabilities. Instants can't be inferior to t+2. 
	*/
    std::vector<std::vector<plDistribution>> predict(std::vector<plVariable> nodes, std::vector<int> instants);
    /**
	* \brief Predict the states of a set of nodes in different instants and returns a vector(multidimensional) of probabilities : vector of probabilities for all the nodes for each timestep.
	* \param[in] nodes - a vector<plVariable> containing the nodes we need to predict.
	* \param[in] instants - a vector<int> containing instants on which we want to predict the probabilities. Instants can't be inferior to t+2.
	* \param[in] evidences - a vector<plValues> containing a plValues for each update of the predict (max(instants)+1)
	*/
    std::vector<plComputableObjectList> predict(std::vector<plVariablesConjunction> nodes, std::vector<int> instants, std::vector<plValues> evidences);

    std::vector<std::vector<int>> sample(int t_max, plDistribution G0);
    /**
	* \brief Create a csv file with random data from the 1.5 DBN.
	* \param[in] dataset_filename - Name of the futur csv file.
	* \param[in] t_max - size of each sample.
	* \param[in] nbrOfSample - number of samples.
	* \param[in] G0 - Initial distribution.
	* \param[in] num_sequence - display or not the number of the sequence
	* \param[in] num_time_slice - number of time slice represented on each line
	*/
    void generate_csv_samples_Data(const std::string& dataset_filename, unsigned int t_max, int nbrOfSample, plDistribution G0, bool num_sequence = true, int num_time_slice = 1);
    void set_inference_JT(bool model);

    /**
	 * @brief Calls the @sa update function to update the interface from the current transition model of the 1.5dBN.
	 * 		  The distribution is the distribution of 1.5dBN and the suffix is "_t".
	 *
	 * @param evidences The list of evidences for the current slice that will be passed to @sa update.
     */
    void updateGT(plValues evidences);

private:
    int                     present;
    int                     futur;
    std::vector<plVariable> outgoingInterface;
    bool                    use_JT = true;

    /** 
	int present = the current timestep of the 1.5 dBN
	int futur = the next timestep of the 1.5 dBN
	vector<plVariable> outgoingInterface = all the vertices from G1 (transition_G) whose names dont end with "_t"

	The 1.5 DBN contains :
	- all the vertices (variables) of G1 (transition_G)
	- all the edges of G1
	- new edges (u,v) where u,v belong to G1_(t-1) and G1 contains and edge (u_t, v_t)
	- the correponding computable objects
     */
    void setPresent(int t);
    void setFutur(int t);
    void majGT(std::vector<plValues>* evidences);
    /**
	 * \brief Update the 1.5 dynamic bayesian network without evidences
	 */
    void increment();

    /**
	 * @brief Calls the @sa update function to update the interface from G0.
	 * 		  The distribution is the distribution of G0 and the suffix is "_t0".
	 * 
	 * @param evidences The list of evidences for the current slice that will be passed to @sa update.
	 * @param dBN 		The DBN from which we will extract the initial BN (G0).
	 */
    void updateG0(plValues evidences, pmDynamicBayesianNetwork* dBN);

    /**
	 * \brief Update the 1.5 dynamic bayesian network using evidences based on another network's distribution and variables.
	 * 		  And since it's a 1.5dBN, it will only update the outgoing interface at t+1.
	 * 
	 * @param jointDistribution The joint distribution of the bayesian network from which we update the 1.5dBN.
	 * @param vars 				The variables of the bayesian network from which we update the 1.5dBN.
	 * @param evidences 		The list of evidences for the current slice.
	 * @param suffix 			The suffix of the variables of the network from which we update 1.5dBN (ex: "_t", "_t0").
	 */
    void update(pmBayesianNetwork* bn, plVariablesConjunction vars, plValues evidences, std::string suffix);
};
#endif // pm_15dBN_H
