/*=============================================================================
 * Product        : ProBT 
 * File           : plConcurrentHmmSet.h
 * Author         : Kamel Mekhnacha
 * Creation       : Mon Jun 18 15:57:58 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plConcurrentHmmSet_h
#define plConcurrentHmmSet_h

#include "plConfig.h"

#include <vector>
#include <string>


#include <plDLLExport.h>
#include <plMath.h>
#include <plHMM.h>
#include <plFloatMatrix.h>

SPL_DECL2(plConcurrentHmmSet);

/** @class plConcurrentHmmSet
    This class implements a model for recognition and filtering using
    a set of HMMs.
*/
class PL_DLL_API plConcurrentHmmSet  :public plBuiltinModel
{
 public:
  /** @brief Default constructor */
  plConcurrentHmmSet();
    
  /** @brief Destructor */
  ~plConcurrentHmmSet(){}
  

  /** @brief Insert a new HMM model.
      @param[in] model the HMM model to be inserted.
  */
  void add_model(const plHMM &model);

  /** @brief Removes the model at a given index.
      @param[in] index the index of the model to be removed.
  */
  void remove_model(unsigned int index);

  /** @brief Removes the model with a given name.
      @param[in] name the name of the model to be removed.
  */
  void remove_model(const std::string &name);

  /** @brief Clear all HMM models.
  */
  void clear_models();


  /** @brief Set/change transition matrices between models (HMMs) i.e. to pass from a class value (model) Cpred to a class value (model) Cnew.

      @param[in] transition_matrix_final_states Transition matrix for final states

      transition_matrix_final_states[p][q] should hold P([Cnew=q] | [Cpred =
      q] and Cpred was in a final state) i.e the probability, during a time step, to pass from
      the class p to the class q when assuming that the current internal state for classe p is a final one.
      transition_matrix_final_states[p] must be a vector of probabilities whose elements sum up to one.

      @param[in] transition_matrix_non_final_states Transition matrix for non final states

      transition_matrix_non_final_states[p][q] should hold P([Cnew=q] | [Cpred =
      q] and Cpred was in a NON final state) i.e the probability, during a time step, to pass from
      the class p to the class q when assuming that the current internal state for classe p is a final one. 
      transition_matrix_non_final_states[p] must be a vector of probabilities whose elements sum up to one.
  */
  void set_transition_matrices(const std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
			       const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states);
  
  /** @brief Set/change an element of the transition matrices between models (HMMs).
  */
  void set_transition_matrices(unsigned int mp, unsigned int m,
			       plProbValue final,
			       plProbValue non_final);

  /** @brief Return the number of the contained HMMs.
      @return the number of the contained HMMs.
  */
  inline size_t get_nmodels()const{return models_.size();}
  
  /** @brief Get, for a given observation sequence, the most probable model
      and the corresponding most probable state sequence.
      
      @param[out] state_sequence the most probable state sequence corresponding to the most probable model (HMM)
      @param[in] obs_seq observation sequence
      @param[in] only_final_states a flag to say whether the retained model must be in a final state
      
      @return the index of the most probable model (HMM)
  */
  unsigned int get_best_model_most_probable_state_sequence(plHMM::StateSequence_t &state_sequence, 
                                                           const plHMM::ObservationSequence_t &obs_seq,
                                                           bool only_final_states = false)const;

  /** @brief Get, for a given observation sequence, the most probable model
      and the corresponding most probable state sequence.
      
      @param[out] state_sequence the most probable state sequence corresponding to the most probable model (HMM)
      @param[in] obs_seq observation sequence
      @param[in] only_final_states a flag to say whether the retained model must be in a final state
      
      @return the name of the most probable model (HMM)
  */
  const std::string &get_best_model_most_probable_state_sequence_by_name(plHMM::StateSequence_t &state_sequence, 
                                                                         const plHMM::ObservationSequence_t &obs_seq,
                                                                         bool only_final_states = false)const;
  
  
  /** @brief Get, for a given observation sequence, the most probable model.
      
      @param[in] obs_seq observation sequence
      
      @return the index of the most probable model (HMM)
  */
  unsigned int get_best_model(const plHMM::ObservationSequence_t &obs_seq)const;

  /** @brief Get, for a given observation sequence, the most probable model.
      
      @param[in] obs_seq observation sequence
      
      @param[out] log_lk_per_obs the minimal log-likelihood normalized by the length of each sequence

      @return the index of the most probable model (HMM)
  */
  unsigned int get_best_model(const plHMM::ObservationSequence_t &obs_seq, plFloat &log_lk_per_obs)const;
  
  /** @brief Get, for a given observation sequence, the most probable model.
      
      @param[in] obs_seq observation sequence
      
      @return the name of the most probable model (HMM)
  */
  const std::string &get_best_model_name(const plHMM::ObservationSequence_t &obs_seq)const;


  /**  @brief Given an observation sequence, get  for each model (HMM), the corresponding log-probability value.
       
       @param[out] models_log_probabilities log probability for each model (HMM) 
       @param[in] observation_sequence observation sequence

       @return the index of the most probable model (HMM)
  */
  unsigned int get_all_models_log_probabilities(std::vector <plFloat> &models_log_probabilities,
                                                const plHMM::ObservationSequence_t &observation_sequence)const;

  /**  @brief Given an observation sequence, get  for each model (HMM), the corresponding log-probability value.
       
       @param[out] models_log_probabilities log probability for each model (HMM) 
       @param[in] observation_sequence observation sequence

       @return the name of the most probable model (HMM)
  */
  const std::string &get_all_models_log_probabilities_name(std::vector <plFloat> &models_log_probabilities,
                                                           const plHMM::ObservationSequence_t &observation_sequence)const;
  
  /**  @brief Given an observation sequence, get  for each model (HMM), the  most probable state sequence
       and the corresponding log-likelihood value.
       
       @param[out] observation_log_likelihoods observation log-likelihood value for each model (HMM) 
       (i.e., P(observation_sequence | HMM_i) )
       @param[out] state_sequences most probable sequence for each model (HMM)
       @param[in] observation_sequence observation sequence
  */
  void get_all_models_most_probable_state_sequences(std::vector <plFloat> &observation_log_likelihoods,
                                                    std::vector < plHMM::StateSequence_t > &state_sequences, 
                                                    const plHMM::ObservationSequence_t &observation_sequence)const;  
  
  /** @brief Use an observation to update  the probability table over the contained models (HMMs)
      and return the likelihood of the observation.
      
      @param[in] observation the observation vector
      @return the log-likelihood of the observation
  */
  plFloat update_models_states_probabilities(const plHMM::Observation_t &observation);


  /** @brief Use an observation to update  the probability table over the contained models (HMMs)
      and return the likelihood of the observation.
      
      @param[in] observation the observation values
      @return the log-likelihood of the observation
  */
  plFloat update_models_states_probabilities(const plValues &observation);
  
  /** @brief Get the probability (belief) for a given model (HMM)
      @param[in] n the index of the model
      @return the probability (belief) for the model with index @em n
  */
  plProbValue get_model_probability(unsigned int n)const;

  /** @brief Get the probability (belief) for a given model (HMM)
      @param[in] name the name of the model
      @return the probability (belief) for the model with name @em name
  */
  plProbValue get_model_probability(const std::string &name)const;


  /** @brief Reset the state distribution to the initial one for each model (HMM). 
      ATTENTION: this function does not reset models probabilities to the initial ones

      \sa reset()
   */
  void reset_models_state_distributions();


  /** @brief Reset the HMM set:
      - reset the state distribution to the initial one for each model (HMM)
      - reset models probabilities to the initial ones
   */
  void reset();


  /** @brief Get the logarithm of the probability (belief) table over all models (HMMs)
      @return the logarithm of the probability (belief) table over all models (HMMs)
      \sa get_models_probabilities()
  */
  inline const std::vector <plFloat> &get_models_probabilities_log()const
  {return models_probabilities_log_;}


  /** @brief Get the probability (belief) table over all models (HMMs)
      @param[out] models_probabilities the probability (belief) table over all models (HMMs)
      \sa get_models_probabilities_log()
  */
  void get_models_probabilities(std::vector <plProbValue> &models_probabilities)const;

  /** @brief Get the probability (belief) table over all models (HMMs)
      @return the probability (belief) table over all models (HMMs)
      \sa get_models_probabilities_log()
  */
  std::vector <plProbValue> get_models_probabilities()const
  {
    std::vector <plProbValue> pr;
    get_models_probabilities(pr);
    
    return pr;
  }

  /** @brief Set the current probability (belief) table over all models (HMMs)
      @param[in] models_probabilities the probability (belief) table over all models (HMMs)
  */
  void set_models_probabilities(const std::vector <plProbValue> &models_probabilities);


  /** @brief Set the initial and the current probability (belief) table over all models (HMMs)
      @param[in] init_models_probabilities the initial probability (belief) table over all models (HMMs)
  */
  void set_init_models_probabilities(const std::vector<plProbValue> &init_models_probabilities);

  /** @brief Same as set_init_models_probabilities() above. 
      Kept only for backward compatibility 
  */
  inline void set_models_prior(const std::vector <plProbValue> &models_probabilities)
  {
    set_init_models_probabilities(models_probabilities);
  }
 
  /** @brief Get transition matrices between models (HMMs).

      @param[out] transition_matrix_final_states transition matrix for final states
      @param[out] transition_matrix_non_final_states transition matrix for non final states
  */
  void get_transition_matrices(std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
			       std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states)const;


  /** @brief Get a reference to the nth model (HMM).

      @param[in] n the number of the model (HMM)  
      @return a reference to the nth model (HMM)
  */
  plHMM &get_model(unsigned int n);

  /** @brief Get a reference to the nth model (HMM).

      @param[in] name the name of the model (HMM)  
      @return a reference to the nth model (HMM)
  */
  plHMM &get_model(const std::string &name);
	
  /** @brief Get a const reference to the nth model (HMM) if any.

      @param[in] n the number of the model (HMM)  
      @return a const reference to the nth model (HMM)
  */
  const plHMM &get_model(unsigned int n) const;

  /** @brief Get a const reference to the model (HMM) with a given name if any.

      @param[in] name the name of the model (HMM)  
      @return a const reference to the nth model (HMM)
  */
  const plHMM &get_model(const std::string &name) const;
	
  /**
     \brief Make a pure prediction step.
  */
  void predict_models_states_probabilities();

   /**
     Compute the dissimilarity [Rabiner 89] of HMMi to HMMj.
     \sa compute_symmetric_dissimilarity_matrix
  */
  plFloatMatrix compute_dissimilarity_matrix(const std::vector< plHMM::ObservationSequenceArray_t > &obs_sequences)const;

  /**
     Compute the dissimilarity [Rabiner 89] of HMMi to HMMj.

     High dissimilarity values correspond to good offline classification performances

     \sa compute_symmetric_dissimilarity_matrix()
  */
  plFloatMatrix compute_dissimilarity_matrix(const std::vector<std::vector<std::string> >  &csv_observation_files,
                                             bool csv_file_has_header,
                                             unsigned int csv_file_nrows_to_ignore,
                                             const std::vector<unsigned int>& csv_file_columns_to_ignore)const;

  /**
     Compute the symmetric dissimilarity [Rabiner 89] between each pair (HMMi, HMMj).

     High dissimilarity values correspond to good offline classification performances
     
     \sa compute_dissimilarity_matrix()
  */
  plFloatMatrix compute_symmetric_dissimilarity_matrix(const std::vector< plHMM::ObservationSequenceArray_t > &obs_sequences)const;

  /**
     Compute the symmetric dissimilarity [Rabiner 89] between each pair (HMMi, HMMj).

     High dissimilarity values correspond to good offline classification performances
     
     \sa compute_dissimilarity_matrix()
  */
  plFloatMatrix compute_symmetric_dissimilarity_matrix(const std::vector<std::vector<std::string> >  &csv_observation_files,
                                                       bool csv_file_has_header,
                                                       unsigned int csv_file_nrows_to_ignore,
                                                       const std::vector<unsigned int>& csv_file_columns_to_ignore)const;
  

  /**
     \brief Returns @em true iff the nth HMM's probability to be in a final state
     is greater or equal to @em prob.
  */
  bool is_in_final_state(unsigned int n, plProbValue prob) const;
  
  /** @brief Get the initial probability (belief) table over all models (HMMs)
      @return the initial probability (belief) table over all models (HMMs)
  */
  const std::vector<plProbValue> &get_init_models_probabilities() const
  {
    return init_models_probabilities_;
  }

  /** Saves the HMM set in a file */
  void save(const std::string& file_name)const;

  /** Loads the HMM set from a file */
  void load(const std::string& file_name);


  /** Evaluate the model regarding the offline classification (whole observation sequence) 
      
      \param[in] test_obs_sequences A vector of observation sequence arrays (an observation sequence array for each class (HMM)). 
      An observation sequence is a vector of float vectors
      (observations). In each observation, order of elements matter. This order
      must correspond to the variables as in
      `observation_variables()` or the order defined
      by the parameter observation_variable_names.  In case the orders do not match, you will
      get incorrect results, and possibly occurrence of plWarning 17 (value out
      of range).

      \param[in] class_names The names of the classes (HMMs)
      \param[out] min_log_likelihood_per_obs The minimal log-likelihood obtenaind on the test data for each class (HMM)

      \return a plPredictionPerformanceReport instance summarizing the offline classification performance

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  plPredictionPerformanceReport evaluate_offline_classification(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences,
                                                                const std::vector<std::string> &class_names,
                                                                std::vector<plFloat> &min_log_likelihood_per_obs) const;

  plPredictionPerformanceReport evaluate_offline_classification(const std::vector<std::vector<std::string> >  &csv_testing_files,
                                                                bool csv_file_has_header,
                                                                unsigned int csv_file_nrows_to_ignore,
                                                                const std::vector<unsigned int> &csv_file_columns_to_ignore,
                                                                const std::vector<std::string> &class_names,
                                                                std::vector<plFloat> &min_log_likelihood_per_obs) const;

  /** Evaluate the model regarding the online classification (per observation) 
      
      \param[in] test_obs_sequences A vector of observation sequence arrays (an observation sequence array for each class (HMM)). 
      An observation sequence is a vector of float vectors
      (observations). In each observation, order of elements matter. This order
      must correspond to the variables as in
      `observation_variables()` or the order defined
      by the parameter observation_variable_names.  In case the orders do not match, you will
      get incorrect results, and possibly occurrence of plWarning 17 (value out
      of range).

      \param[in] class_names The names of the classes (HMMs)

      \param[in] transition_matrix_final_states  Transition matrix for final states (see definition in set_transition_matrices())

      \param[in] transition_matrix_non_final_states Transition matrix for non final states  (see definition in set_transition_matrices())

      \param[in] obs_llk_threshold The minimal log-likelihood of the observations to be accepted. If the log-likelihood of the observation is less than this threshold, then the
      classification  will return UNKNOWN (last column in the confusion matrix)

      \return a plPredictionPerformanceReport instance summarizing the online classification performance

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
   */
  plPredictionPerformanceReport evaluate_online_classification(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences,
                                                               const std::vector<std::string> &class_names,
                                                               const std::vector <std::vector < plProbValue > > &transition_matrix_final_states=
                                                               std::vector <std::vector < plProbValue > >(),
                                                               const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states=
                                                               std::vector <std::vector < plProbValue > >(),
                                                               plFloat obs_llk_threshold=-std::numeric_limits<plFloat>::infinity());

  /** Evaluate the model regarding the online classification (per observation)

      \param[in] test_obs_sequences A vector of observation sequence arrays (an
      observation sequence array for each class (HMM)). An observation
      sequence is a vector of float vectors (observations). In each
      observation, order of elements matter. This order must correspond to the
      variables as in
      `observation_variables()` or the order defined by
      the parameter observation_variable_names.  In case the orders do not
      match, you will get incorrect results, and possibly occurrence of
      plWarning 17 (value out of range).

      \param[in] class_values The class values (starting from 0) corresponding to each each time-step (observation) in the test_obs_sequences sequences

      \param[in] class_names The names of the classes (HMMs) 

      \param[in] transition_matrix_final_states Transition matrix for final states (see definition in set_transition_matrices())

      \param[in] transition_matrix_non_final_states Transition matrix for non final states  (see definition in set_transition_matrices())

      \param[in] obs_llk_threshold The minimal log-likelihood of the observations to be accepted. If the log-likelihood of the observation is less than this threshold, then the
      classification  will return UNKNOWN (last column in the confusion matrix)

      \return a plPredictionPerformanceReport instance summarizing the online classification performance

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  plPredictionPerformanceReport evaluate_online_classification(const plHMM::ObservationSequenceArray_t &test_obs_sequences,
                                                               const std::vector<std::vector<unsigned int> > &class_values,
                                                               const std::vector<std::string> &class_names,
                                                               const std::vector <std::vector < plProbValue > > &transition_matrix_final_states=
                                                               std::vector <std::vector < plProbValue > >(),
                                                               const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states=
                                                               std::vector <std::vector < plProbValue > >(),
                                                               plFloat obs_llk_threshold=-std::numeric_limits<plFloat>::infinity());


  /** Evaluate the model regarding the online classification (per observation) 

      \param[in] test_obs_sequence An observation sequence array. An
      observation sequence is a vector of float vectors (observations). In each
      observation, order of elements matter. This order must correspond to the
      variables as in
      `observation_variable()` or the order defined by
      the parameter observation_variable_names.  In case the orders do not
      match, you will get incorrect results, and possibly occurrence of
      plWarning 17 (value out of range).

      \param[in] class_values The class values (starting from 0) corresponding to each each time-step (observation) in the test_obs_sequences sequences

      \param[in] class_names  The names of the classes (HMMs) 

      \param[in] transition_matrix_final_states Transition matrix for final states (see definition in set_transition_matrices())

      \param[in] transition_matrix_non_final_states Transition matrix for non final states  (see definition in set_transition_matrices())

      \param[in] obs_llk_threshold The minimal log-likelihood of the observations to be accepted. If the log-likelihood of the observation is less than this threshold, then the
      classification  will return UNKNOWN (last column in the confusion matrix)

      \return a plPredictionPerformanceReport instance summarizing the online classification performance

      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  plPredictionPerformanceReport evaluate_online_classification(const plHMM::ObservationSequence_t &test_obs_sequence,
                                                               const std::vector<unsigned int> &class_values,
                                                               const std::vector<std::string> &class_names,
                                                               const std::vector <std::vector < plProbValue > > &transition_matrix_final_states=
                                                               std::vector <std::vector < plProbValue > >(),
                                                               const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states=
                                                               std::vector <std::vector < plProbValue > >(),
                                                               plFloat obs_llk_threshold=-std::numeric_limits<plFloat>::infinity());


  /** Set/unset verbose mode */
  void set_verbose(bool verbose)
  {
    verbose_ = verbose;
  }

  /** Get the observation variables */
  plVariablesConjunction observation_variables() const
  {
    if(models_.empty()) return plVariablesConjunction();
    return models_[0].observation_variables();
  }
  
 private:

  template<typename TRAIT>
    plPredictionPerformanceReport
    evaluate_offline_classification_impl(const typename TRAIT::data_params_vector_t &test_obs_sequences,
                                         const std::vector<std::string> &class_names,
                                         std::vector<plFloat> &min_log_likelihood_per_obs) const;
    
 protected:
  SPL_ACCESS2(plConcurrentHmmSet);
  
  plFloat compute_prediction_prob_log(unsigned int m)const;
  plFloat compute_transition_prob_log(unsigned int mp, unsigned int m)const;
  void update_size();
  
  void Output(std::ostream& out)const;

  template <typename T>
    plFloat update_models_states_probabilities_impl(const T &observation);

 protected:
  std::vector <plHMM> models_;
  std::vector <plFloat> models_probabilities_log_;
  
  std::vector <std::vector<plFloat> > transition_log_final_;
  std::vector <std::vector<plFloat> > transition_log_non_final_;
  
  bool has_default_transition_matrices_;

 protected:
  mutable std::vector <plFloat> log_vals_;
  std::vector <plFloat> new_models_probabilities_log_;

  mutable std::vector< std::vector<plFloat> > state_distribution_log_;
  std::vector<plProbValue> init_models_probabilities_;
  bool verbose_;
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
