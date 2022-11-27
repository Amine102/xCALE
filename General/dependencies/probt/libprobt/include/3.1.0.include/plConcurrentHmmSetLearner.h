/*=============================================================================
 * Product        : ProBT 
 * File           : plConcurrentHmmSetLearner.h
 * Author         : Kamel Mekhnacha
 * Creation       : February 2016
 *
 *=============================================================================
 *     (c) Copyright 2016, Probayes SAS
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plConcurrentHmmSetLearner_h
#define plConcurrentHmmSetLearner_h


#include "plConfig.h"

#include <plMath.h>
#include <plDLLExport.h>
#include <plHMM.h>
#include <plConcurrentHmmSet.h>
#include <plPredictionPerformanceReport.h>
#include <plFloatMatrix.h>

#include <vector>
#include <string>
#include <map>

class plLearnObject;

#define PL_DEFAULT_N_RANDOM_INIT 3
#define PL_DEFAULT_PRIOR_WEIGHT 0.01
#define PL_DEFAULT_CONV_THRESHOLD PL_ZERO

/** This class implements HMM set learning and evaluation

    \sa plHMM
    \sa plConcurrentHmmSet
*/
class PL_DLL_API plConcurrentHmmSetLearner 
{
 private:
  plConcurrentHmmSetLearner(const plConcurrentHmmSetLearner&); // NI
  plConcurrentHmmSetLearner& operator=(const plConcurrentHmmSetLearner&); // NI
  
 public:

  /** Constructor 
      
      \param[in] model_name The name of the model to be used for naming the
      learnt models and the corresponding ProBT xml files when saved

      \param[in] class_names The name of the classes of the model

      \param[in] observation_decomposition The observation (emission)
      distribution learner. The passed pointer is cloned and stored internally.
      The observation (emission) variables can be
      discrete and/or continuous. For example, when assuming a joint normal
      distribution on a set of continuous observation variables, one can use a
      plLearnNdNormal. One can use a plLearnHistogram in case of discrete
      variables. Moreover, the observation (emission) distribution can be
      decomposed. For example, a 2D observation distribution can be decomposed
      as:
      - \f$ P(O_x O_y) = P(O_x) P(O_y) \f$, 
      - \f$ P(O_x O_y) = P(O_x) P(O_y | O_x) \f$.\n
      Using ProBT, a such decomposed emission distribution can be expressed
      using the @em plLearnDistributions class to define the learner of the
      emission distribution. If observation_decomposition is not set, then a
      multidimensional Normal is assumed.

      \param[in] transition_matrix_final_states Transition matrix for final
      states.  transition_matrix_final_states[p][q] should hold P([Cnew=q] |
      [Cpred = q] and Cpred was in a final state) i.e the probability, during a
      time step, to pass from the class p to the class q when assuming that the
      current internal state for class p is a final one.
      transition_matrix_final_states[p] must be a vector of probabilities whose
      elements sum up to one.

      \param[in] transition_matrix_non_final_states Transition matrix for non
      final states.  transition_matrix_non_final_states[p][q] should hold
      P([Cnew=q] | [Cpred = q] and Cpred was in a NON final state) i.e the
      probability, during a time step, to pass from the class p to the class q
      when assuming that the current internal state for class p is a final one.
      transition_matrix_non_final_states[p] must be a vector of probabilities
      whose elements sum up to one.
      
      If transition_matrix_non_final_states or transition_matrix_final_states
      are empty, then identity matrices are assumed
  */
  plConcurrentHmmSetLearner
    (const std::string &model_name,
     const std::vector<std::string> &class_names,
     const plLearnObject* observation_decomposition=0,
     const std::vector <std::vector < plProbValue > > &transition_matrix_final_states=
     std::vector <std::vector < plProbValue > >(),
     const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states=
     std::vector <std::vector < plProbValue > >());

  /** Destructor */
  ~plConcurrentHmmSetLearner();

  /** Set/unset verbose mode */
  void set_verbose(bool verbose)
  {
    verbose_ = verbose;
  }

  /** Set the output stream to be used when verbose mode is enabled.

      The default is  std::cout 
  */
  void set_output_stream(std::ostream &out)
  {
    output_stream_ = &out;
  }

  /** Enable/disable multithreading */
  void enable_multi_threading(bool enable_it)
  {
    enable_multi_threading_ = enable_it;
  }

  /** Set/change the observation model
      
      \param[in] observation_decomposition The observation (emission)
      distribution learner.  The observation (emission) variables can be
      discrete and/or continuous. For example, when assuming a joint normal
      distribution on a set of continuous observation variables, one can use a
      plLearnNdNormal.  One can use a plLearnHistogram in case of discrete
      variables. Moreover, the observation (emission) distribution can be
      decomposed. For example, a 2D observation distribution can be decomposed
      as:
      - \f$ P(O_x O_y) = P(O_x) P(O_y) \f$, 
      - \f$ P(O_x O_y) = P(O_x) P(O_y | O_x) \f$.\n
      Using ProBT, a such decomposed emission distribution can be expressed
      using the @em plLearnDistributions class to define the learner of the
      emission distribution. If observation_decomposition is not set, then a
      multidimensional Normal is assumed.
  */
  void set_observation_model(const plLearnObject& observation_decomposition);

  /** Set/change the observation model 

      \param[in] observation_variable_names The names of the observation
      variables

      \param[in] observation_variable_is_continuous For each variable in the
      list above, True if the variable is continuous, False otherwise

      \param[in] observation_variable_min For each variable in the list above,
      the min value for the range (inclusive)

      \param[in] observation_variable_max For each variable in the list above,
      the max value for the range (inclusive for discrete variables and
      exclusive for continuous ones)

      \param[in] observation_variable_groups A set of indices for variable
      groups.  For example, for observation_variable_names = ["X", "A", "Z"],
      one can use observation_variable_groups = [ [0, 2], [1] ] to declare two
      groups of variables [X, Z] and [A]. This leads to the decomposition P(X A
      Z) = P(X Z) P(A)
      
      \param[in] observation_variable_group_parents A set of indices for
      variable group parents. For example, for observation_variable_names =
      ["X", "A", "Z"], one can use observation_variable_groups = [ [0, 2], [1]
      ] to declare two groups of variables ([X, Z], [A]) and
      observation_variable_group_parents = [ [1], [] ] to declare [1] ([A]) as
      the parents for the first group [0, 2] ([X, Z]) and no parents for the
      second group [1] ([A]). This leads to the decomposition P(X A Z) = P(X Z | A) P(A)

      For each distribution learner, its parametric form (type) depends of the type of the variable conjunctions
      and their parents as returned by plLearnObject::create_learner()

      \sa plLearnObject::create_learnable_decomposition(), plLearnObject::create_learner()
      \sa plVariablesConjunction::create_variables()
  */
  void set_observation_model
    (const std::vector<std::string> &observation_variable_names,
     const std::vector<bool> &observation_variable_is_continuous=std::vector<bool>(),
     const std::vector<plFloat> &observation_variable_min=std::vector<plFloat>(),
     const std::vector<plFloat> &observation_variable_max=std::vector<plFloat>(),
     const std::vector<std::vector<unsigned int> > &observation_variable_groups=
     std::vector<std::vector<unsigned int> >(),
     const std::vector<std::vector<unsigned int> > &observation_variable_group_parents=
     std::vector<std::vector<unsigned int> >());

  /** Same as above. However plVariablesConjunction are used instead of using indices */
  void set_observation_model(const plVariablesConjunction &observation_variables,
                             const std::vector<plVariablesConjunction> &observation_variable_groups=
                             std::vector<plVariablesConjunction>(),
                             const std::vector<plVariablesConjunction> &observation_variable_group_parents=
                             std::vector<plVariablesConjunction>());
  
  /** Set/change the transition matrices
      
      \param[in] transition_matrix_final_states Transition matrix for final
      states transition_matrix_final_states[p][q] should hold P([Cnew=q] |
      [Cpred = q] and Cpred was in a final state) i.e the probability, during a
      time step, to pass from the class p to the class q when assuming that the
      current internal state for class p is a final one.
      transition_matrix_final_states[p] must be a vector of probabilities whose
      elements sum up to one.
      
      \param[in] transition_matrix_non_final_states Transition matrix for non
      final states transition_matrix_non_final_states[p][q] should hold
      P([Cnew=q] | [Cpred = q] and Cpred was in a NON final state) i.e the
      probability, during a time step, to pass from the class p to the class q
      when assuming that the current internal state for class p is a non final
      one.  transition_matrix_non_final_states[p] must be a vector of
      probabilities whose elements sum up to one.

      If these matrices are not set, then identity ones are assumed
  */
  void set_transition_matrices
    (const std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
     const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states);

  
  /** Run learning assuming a given list of numbers for the internal HMM
      states.
      
      The learning will generate a map (key: name, value: model) with the
      learnt models (plConcurrentHmmSet) (see get_model_map()).

      For example, for model_name="SequenceClassifier" and candidate_nstates =
      [5, 10, 50], run_learning() will generate the plConcurrentHmmSet
      instances with the following names:
      - "SequenceClassifier05": A plConcurrentHmmSet instance with HMMs having
      5 internal states each
      - "SequenceClassifier10": A plConcurrentHmmSet instance with HMMs having
      10 internal states each
      - "SequenceClassifier50": A plConcurrentHmmSet instance with HMMs having
      50 internal states each
      - "SequenceClassifierBestLK": A plConcurrentHmmSet instance with HMMs
      having each the number of internal states allowing to maximize the
      learning log-likelihood
      - "SequenceClassifierBestBIC": A plConcurrentHmmSet instance with HMMs
      having each the number of internal states allowing to maximize the
      learning BIC score
      - "SequenceClassifierBestAIC": A plConcurrentHmmSet instance with HMMs
      having each the number of internal states allowing to maximize the
      learning AIC score

      The values of the log-likelihood, BIC, and AIC score can be retrieved using
      get_model_log_likelihood(), get_model_bic(), and get_model_aic() respectively.

      \param[in] learn_obs_sequences A vector of observation sequence arrays
      (an observation sequence array for each class (HMM)).  An observation
      sequence is a vector of float vectors (observations). In each
      observation, order of elements matter. This order must correspond to the
      observation_decomposition learner's variables as in
      `observation_decomposition->get_left_variables()` or the order defined by
      the parameter observation_variable_names.  In case the orders do not
      match, you will get incorrect results, and possibly occurrence of
      plWarning 17 (value out of range).

      \param[in] candidate_nstates The candidate numbers of HMM internal
      states.

      \param[in] n_initialisation_random_trials The number of random
      initialisations. Providing a large number maximizes the chance of having
      a well-fitted model.  In addition to the random initialisation trials, a
      deterministic initialisation is also performed (ie. this leads to
      (n_initialisation_random_trials+1) initialisations)

      \param[in] learning_prior_weight The weight to be used for initialising
      the emission distribution from the whole data set. A small value will
      lead to a more accurate classification while increasing the risk af
      having degenerated statistics (especially when using a large number of
      internal states).

      \param[in] convergence_threshold convergence threshold of
      relative log-likelihood change between two successive iterations
      (smaller value means more iterations). The default value is set to 0.0
      The convergence criterion is computed as:\n
      | log-likelihood(t) - log-likelihood(t-1) | / | (log-likelihood(t) + log-likelihood(t-1)/2.0 | < convergence_threshold.

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  void run_learning(const std::vector<plHMM::ObservationSequenceArray_t> &learn_obs_sequences,
                    const std::vector <unsigned int> &candidate_nstates,
                    unsigned int n_initialisation_random_trials=PL_DEFAULT_N_RANDOM_INIT,
                    plFloat learning_prior_weight=PL_DEFAULT_PRIOR_WEIGHT,
                    plFloat convergence_threshold=PL_DEFAULT_CONV_THRESHOLD);

  /** Run learning assuming a given list of numbers for the internal HMM
      states. The learning data is provided as a list of csv files (sequences) and 
      each file is loaded when needed (not preloaded in memory) 
      
      The learning will generate a map (key: name, value: model) with the
      learnt models (plConcurrentHmmSet) (see get_model_map()).

      For example, for model_name="SequenceClassifier" and candidate_nstates =
      [5, 10, 50], run_learning() will generate the plConcurrentHmmSet
      instances with the following names:
      - "SequenceClassifier05": A plConcurrentHmmSet instance with HMMs having
      5 internal states each
      - "SequenceClassifier10": A plConcurrentHmmSet instance with HMMs having
      10 internal states each
      - "SequenceClassifier50": A plConcurrentHmmSet instance with HMMs having
      50 internal states each
      - "SequenceClassifierBestLK": A plConcurrentHmmSet instance with HMMs
      having each the number of internal states allowing to maximize the
      learning log-likelihood
      - "SequenceClassifierBestBIC": A plConcurrentHmmSet instance with HMMs
      having each the number of internal states allowing to maximize the
      learning BIC score
      - "SequenceClassifierBestAIC": A plConcurrentHmmSet instance with HMMs
      having each the number of internal states allowing to maximize the
      learning AIC score

      The values of the log-likelihood, BIC, and AIC score can be retrieved using
      get_model_log_likelihood(), get_model_bic(), and get_model_aic() respectively.
        
      \param[in] csv_learning_files A vector of string vectors (a string vector
      for each class (HMM)).
      
      \param[in] candidate_nstates The candidate numbers of HMM internal
      states.
      
      \param[in] csv_file_has_header True if the csv files do have a header
      with the names of observation variables (the names of
      observation_decomposition->get_left_variables() of those provided in
      observation_variable_names
      
      \param[in] csv_file_nrows_to_ignore The number of rows to be ignored in
      the csv files. Only used when csv_file_has_header=False
      
      \param[in] csv_file_columns_to_ignore The indices (starting from 0) of
      the csv columns to be ignored. Only used when csv_file_has_header=False
      
      \param[in] n_initialisation_random_trials The number of random
      initialisations. Providing a large number maximise to chance for having a
      well-fitted model. In addition to the random initialisation trials, a
      deterministic initialisation is also performed (ie. this leads to
      (n_initialisation_random_trials+1) initialisations)
      
      \param[in] learning_prior_weight The weight to be used for initialising
      the emission distribution from the whole data set. A small value will
      lead to a more accurate classification while increasing the risk af
      having degenerated statistics during the learning process especially when
      using a large number of internal states.

      \param[in] convergence_threshold convergence threshold of
      relative log-likelihood change between two successive iterations
      (smaller value means more iterations). The default value is set to 0.0
      The convergence criterion is computed as:\n
      | log-likelihood(t) - log-likelihood(t-1) | / | (log-likelihood(t) + log-likelihood(t-1)/2.0 | < convergence_threshold.
  */
  void run_learning(const std::vector<std::vector<std::string> > &csv_learning_files,
                    const std::vector<unsigned int> &candidate_nstates,
                    bool csv_file_has_header=false,
                    unsigned int csv_file_nrows_to_ignore=0,
                    const std::vector<unsigned int> &csv_file_columns_to_ignore=
                    std::vector<unsigned int>(),
                    unsigned int n_initialisation_random_trials=PL_DEFAULT_N_RANDOM_INIT,
                    plFloat learning_prior_weight=PL_DEFAULT_PRIOR_WEIGHT,
                    plFloat convergence_threshold=PL_DEFAULT_CONV_THRESHOLD);
    
  /** Get a map (key: name, value: model) with the learnt models
      (plConcurrentHmmSet).

      This function must be called after calling run_learning()
  */
  const std::map<std::string, plConcurrentHmmSet> &get_model_map() const {return model_map_;}
  
  /** Get the learning log-likelihood score per class and per number of states

      This function must be called after calling run_learning()
  */
  std::vector<std::vector<plFloat> > get_model_log_likelihood() const {return model_log_likelihood_;}

  /** Get the learning bic score per class and per number of states
      
      This function must be called after calling run_learning()
  */
  std::vector<std::vector<plFloat> > get_model_bic() const {return model_bic_;}

  /** Get the learning aic score per class and per number of states
      
      This function must be called after calling run_learning()
  */
  std::vector<std::vector<plFloat> > get_model_aic() const {return model_aic_;}

  /** Save the learnt models (plConcurrentHmmSet) as ProBT xml files in a given
      output directory.
    
      This function must be called after calling run_learning()

      \param[in] model_output_dir The output directory where the files will be
      saved. The directory must exist and have writing permissions
      
      For example, for model_name="SequenceClassifier" and candidate_nstates =
      [5, 10, 50], run_learning() and save_models() will generate the following
      files:
      - "model_output_dir/SequenceClassifier05.xml": A plConcurrentHmmSet
      instance with HMMs having 5 internal states each
      - "model_output_dir/SequenceClassifier10.xml": A plConcurrentHmmSet
      instance with HMMs having 10 internal states each
      - "model_output_dir/SequenceClassifier50.xml": A plConcurrentHmmSet
      instance with HMMs having 50 internal states each
      - "model_output_dir/SequenceClassifierBestLK.xml": A plConcurrentHmmSet
      instance with HMMs having each the number of internal states allowing
      to maximize the learning log-likelihood
      - "model_output_dir/SequenceClassifierBestBIC.xml": A plConcurrentHmmSet
      instance with HMMs having each the number of internal states allowing
      to maximize the learning BIC score
      - "model_output_dir/SequenceClassifierBestAIC.xml": A plConcurrentHmmSet
      instance with HMMs having each the number of internal states allowing
      to maximize the learning AIC score
  */
  void save_models(const std::string &model_output_dir) const;


  /** Load the data matrix for model learning/testing 
      
      \param[in] csv_files A vector of file names
      
      \param[in] csv_file_nrows_to_ignore The number of rows to be ignored in
      the csv files
      
      \param[in] csv_file_columns_to_ignore The indices (starting from 0) of
      the csv columns to be ignored

      \param[in] ncols The number of fields to be loaded. If 0 (the default value), 
      the number will be guessed from the csv files
      
      \return obs_sequences The loaded observation matrix: A vector of
      observation sequence arrays (an observation sequence array for each class
      (HMM)). An observation sequence is a vector of float vectors
      (observations)
      
      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  static plHMM::ObservationSequenceArray_t
    load_csv_files(const std::vector<std::string>  &csv_files,
                   unsigned int csv_file_nrows_to_ignore,
                   const std::vector<unsigned int> &csv_file_columns_to_ignore,
                   unsigned int ncols=0);

  /** Load the data matrix for model learning/testing 
      
      \param[in] csv_files A vector of string vectors (a string vector for each
      class (HMM))
      
      \param[in] csv_file_nrows_to_ignore The number of rows to be ignored in
      the csv files
      
      \param[in] csv_file_columns_to_ignore The indices (starting from 0) of
      the csv columns to be ignored
      
      \return obs_sequences The loaded observation matrix: A vector of
      observation sequence arrays (an observation sequence array for each class
      (HMM)). An observation sequence is a vector of float vectors
      (observations)

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  static std::vector<plHMM::ObservationSequenceArray_t>
    load_csv_files(const std::vector<std::vector<std::string> >  &csv_files,
                   unsigned int csv_file_nrows_to_ignore,
                   const std::vector<unsigned int>& csv_file_columns_to_ignore);
    
  /** Load the data matrix for model learning/testing 
      
      \param[in] csv_files A vector of string vectors (a string vector for each
      class (HMM))
      
      \param[in] csv_file_has_header True if the csv files do have header with
      the names of observation variables (the names of
      observation_decomposition->get_left_variables() of those provided in
      observation_variable_names
      
      \param[in] csv_file_nrows_to_ignore The number of rows to be ignored in
      the csv files. Only used when csv_file_has_header=False
      
      \param[in] csv_file_columns_to_ignore The indices (starting from 0) of
      the csv columns to be ignored. Only used when csv_file_has_header=False

      \return obs_sequences The loaded observation matrix: A vector of
      observation sequence arrays (an observation sequence array for each class
      (HMM)). An observation sequence is a vector of float vectors
      (observations)

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  std::vector<plHMM::ObservationSequenceArray_t>
    load_csv_data(const std::vector<std::vector<std::string> >  &csv_files,
                  bool csv_file_has_header=false,
                  unsigned int csv_file_nrows_to_ignore=0,
                  const std::vector<unsigned int>& csv_file_columns_to_ignore=
                  std::vector<unsigned int>()) const;

  /** Get a list of files from a file system hierarchy. 
      
      For example, in a gesture recognition example with 3 classes (gesture1,
      gesture2, gesture3) with sets of sequence files recorded for 2 persons
      (person1, person2). If we have the following file system hierarchy:

      \verbatim
      | gesture_data
      | |- gesture1
      | | | - person1
      | | | | - example1.txt
      | | | | - example2.txt
      | | | - person2
      | | | | - example1.txt
      | | | | - example2.txt
      | | | | - example3.txt
      | | | - person3
      | | | | - example1.txt
      | | | | - example2.txt
      | | | | - example3.txt
      | |- gesture2
      | | | - person1
      | | | | - example1.txt
      | | | - person2
      | | | | - example1.txt
      | | | | - example2.txt
      | | | | - example3.txt
      | | | - person3
      | | | | - example1.txt
      | | | | - example2.txt
      | | | | - example3.txt
      | |- gesture3
      | | | - person1
      | | | | - example1.txt
      | | | | - example2.txt
      | | | | - example3.txt
      | | | - person2
      | | | | - example1.txt
      | | | | - example2.txt
      | | | | - example3.txt
      | | | | - example4.txt
      | | | - person3
      | | | | - example1.txt
      | | | | - example2.txt
      | | | | - example3.txt
      \endverbatim

      Calling file_system_to_file_names() with data_path="gesture_data",
      class_names=["gesture1", "gesture2", "gesture3"] and
      profile_names=["person1", "person2"] will return:
      
      \verbatim
      [
      [
      "gesture_data/gesture1/person1/example1.txt", 
      "gesture_data/gesture1/person1/example2.txt", 
      "gesture_data/gesture1/person2/example1.txt", 
      "gesture_data/gesture1/person2/example2.txt", 
      "gesture_data/gesture1/person2/example3.txt"
      ]
      [
      "gesture_data/gesture2/person1/example1.txt", 
      "gesture_data/gesture2/person2/example1.txt", 
      "gesture_data/gesture2/person2/example2.txt", 
      "gesture_data/gesture2/person2/example3.txt"
      ]
      [
      "gesture_data/gesture3/person1/example1.txt", 
      "gesture_data/gesture3/person1/example2.txt",
      "gesture_data/gesture3/person1/example3.txt",   
      "gesture_data/gesture3/person2/example1.txt", 
      "gesture_data/gesture3/person2/example2.txt", 
      "gesture_data/gesture3/person2/example3.txt",
      "gesture_data/gesture3/person2/example4.txt"
      ]
      ]
      \endverbatim

      \param[in] data_path The root file hierarchy to scan
      \param[in] class_names The class names corresponding to the
      subdirectories of data_path to scan
      \param[in] profile_names The profile names corresponding to the
      subdirectories of data_path/class_name to scan
      \param[in] file_prefix Prefix of the files to be loaded from the
      directories data_path/class_name/profile_name
      \param[in] file_suffix Suffix of the files to be loaded from the
      directories data_path/class_name/profile_name

      \return A vector of string vectors (a string vector for each class
      (HMM)).
  */
  static std::vector<std::vector<std::string> > file_system_to_file_names( const std::string &data_path,
                                                                           const std::vector<std::string> &class_names,
                                                                           const std::vector<std::string> &profile_names,
                                                                           const std::string &file_prefix="",
                                                                           const std::string &file_suffix="");

  /** Get a list of files from a file system hierarchy. 
      
      For example, in a gesture recognition example with 3 classes (gesture1, gesture2, gesture3). 
      If we have the following file system hierarchy:
      \verbatim
      | gesture_data
      | |- gesture1
      | | | - example1.txt
      | | | - example2.txt
      | |- gesture2
      | | | - example1.txt
      | |- gesture3
      | | | - example1.txt
      | | | - example2.txt
      | | | - example3.txt
      \endverbatim

      Calling file_system_to_file_names() with data_path="gesture_data",
      class_names=["gesture1", "gesture2", "gesture3"] and
      profile_names=["person1", "person2"] will return:

      \verbatim
      [
      ["gesture_data/gesture1/example1.txt", "gesture_data/gesture1/example2.txt"]
      ["gesture_data/gesture2/example1.txt"]
      ["gesture_data/gesture3/example1.txt", "gesture_data/gesture3/example2.txt", "gesture_data/gesture3/example3.txt"]
      ]
      \endverbatim

      \param[in] data_path The root file hierarchy to scan
      \param[in] class_names The class names corresponding to the
      subdirectories of data_path to scan
      \param[in] file_prefix Prefix of the files to be loaded from the
      directories data_path/class_name
      \param[in] file_suffix Suffix of the files to be loaded from the
      directories data_path/class_name

      \return A vector of string vectors (a string vector for each class (HMM)).
  */
  static std::vector<std::vector<std::string> > file_system_to_file_names( const std::string &data_path,
                                                                           const std::vector<std::string> &class_names,
                                                                           const std::string &file_prefix="",
                                                                           const std::string &file_suffix="");

  /** Get the list of files of a given directory */
  static bool get_sorted_directory_files(const std::string &dir_name,
                                         std::vector <std::string> &file_names,
                                         const std::string &file_prefix="",
                                         const std::string &file_suffix="");
  
  /** Run offline evaluation of all the learnt models and put the results in a
      map (see model_offline_performance_map())

      \param[in] test_obs_sequences A vector of observation sequence arrays (an
      observation sequence array for each class (HMM)).  An observation
      sequence is a vector of float vectors (observations). In each
      observation, order of elements matter. This order must correspond to the
      observation_decomposition learner's variables as in
      `observation_decomposition->get_left_variables()` or the order defined by
      the parameter observation_variable_names.  In case the orders do not
      match, you will get incorrect results, and possibly occurrence of
      plWarning 17 (value out of range).

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  void run_offline_evaluation(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences);

  /** Same as above however each observation sequence is loaded when needed (not preloaded in memory) */
  void run_offline_evaluation(const std::vector<std::vector<std::string> >  &csv_testing_files,
                              bool csv_file_has_header=false,
                              unsigned int csv_file_nrows_to_ignore=0,
                              const std::vector<unsigned int> &csv_file_columns_to_ignore=
                              std::vector<unsigned int>());
  
  /** Get a map (key: model name, value: model offline perfs as a plPredictionPerformanceReport) for each  model */
  const std::map<std::string, plPredictionPerformanceReport> &model_offline_performance_map() const {return model_offline_perf_map_;}

  /** Run online evaluation of all the learnt models and put the results in a
      map (see model_online_performance_map())

      \param[in] test_obs_sequences A vector of observation sequence arrays (an
      observation sequence array for each class (HMM)).  An observation
      sequence is a vector of float vectors (observations). In each
      observation, order of elements matter. This order must correspond to the
      observation_decomposition learner's variables as in
      `observation_decomposition->get_left_variables()` or the order defined by
      the parameter observation_variable_names.  In case the orders do not
      match, you will get incorrect results, and possibly occurrence of
      plWarning 17 (value out of range).
      
      \param[in] transition_matrix_final_states Transition matrix for final
      states (see definition in set_transition_matrices())

      \param[in] transition_matrix_non_final_states Transition matrix for non
      final states (see definition in set_transition_matrices())

      \param[in] obs_llk_threshold The minimal log-likelihood of the
      observations to be accepted. If the log-likelihood of the observation is
      less than this threshold, then the classification will return UNKNOWN
      (last column in the confusion matrix)

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  void run_online_evaluation(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences,
                             const std::vector <std::vector < plProbValue > > &transition_matrix_final_states=
                             std::vector <std::vector < plProbValue > >(),
                             const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states=
                             std::vector <std::vector < plProbValue > >(),
                             plFloat obs_llk_threshold=-std::numeric_limits<plFloat>::infinity());

  /** Run online evaluation of all the learnt models and put the results in a
      map (see model_online_performance_map())

      \param[in] test_obs_sequences An observation sequence array. An
      observation sequence is a vector of float vectors (observations). In each
      observation, order of elements matter. This order must correspond to the
      observation_decomposition learner's variables as in
      `observation_decomposition->get_left_variables()` or the order defined by
      the parameter observation_variable_names.  In case the orders do not
      match, you will get incorrect results, and possibly occurrence of
      plWarning 17 (value out of range).

      \param[in] class_values The class values (starting from 0) corresponding
      to each each time-step (observation) in the test_obs_sequences sequences

      \param[in] transition_matrix_final_states Transition matrix for final
      states (see definition in set_transition_matrices())

      \param[in] transition_matrix_non_final_states Transition matrix for non
      final states (see definition in set_transition_matrices())

      \param[in] obs_llk_threshold The minimal log-likelihood of the
      observations to be accepted. If the log-likelihood of the observation is
      less than this threshold, then the classification will return UNKNOWN
      (last column in the confusion matrix)

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  void run_online_evaluation(const plHMM::ObservationSequenceArray_t &test_obs_sequences,
                             const std::vector<std::vector<unsigned int> > &class_values,
                             const std::vector <std::vector < plProbValue > > &transition_matrix_final_states=
                             std::vector <std::vector < plProbValue > >(),
                             const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states=
                             std::vector <std::vector < plProbValue > >(),
                             plFloat obs_llk_threshold=-std::numeric_limits<plFloat>::infinity());

  /** Get a map (key: model name, value: model offline perfs as a plPredictionPerformanceReport) for each learnt model */
  const std::map<std::string, plPredictionPerformanceReport> &model_online_performance_map() const {return model_online_perf_map_;}

  /** Compute the dissimilarity matrices of the learnt models and put the results in a
      map.

      High dissimilarity values correspond to good offline classification performances

      \param[in] obs_sequences An observation sequence array. An observation
      sequence is a vector of float vectors (observations). In each
      observation, order of elements matter. This order must correspond to the
      observation_decomposition learner's variables as in
      `observation_decomposition->get_left_variables()` or the order defined by
      the parameter observation_variable_names. In case the orders do not
      match, you will get incorrect results, and possibly occurrence of
      plWarning 17 (value out of range).

      \param[in] symmetric If True, symmetric dissimilarity matrices are computed. 

      \sa plConcurrentHmmSet::compute_dissimilarity_matrix()

      \sa plHMM::ObservationSequenceArray_t
      \sa plHMM::ObservationSequence_t
      \sa plHMM::Observation_t
  */
  void compute_dissimilarity_matrices(const std::vector<plHMM::ObservationSequenceArray_t> &obs_sequences,
                                      bool symmetric=false);

  /** Same as above however each observation sequence is loaded when needed (not preloaded in memory) */
  void compute_dissimilarity_matrices(const std::vector<std::vector<std::string> > &csv_learning_files,
                                      bool csv_file_has_header=false,
                                      unsigned int csv_file_nrows_to_ignore=0,
                                      const std::vector<unsigned int> &csv_file_columns_to_ignore=
                                      std::vector<unsigned int>(),
                                      bool symmetric=false);

  /** Get a map (key: model name, value: dissimilarity matrix) for each learnt model */
  const std::map<std::string, plFloatMatrix > &model_dissimilarity_matrix_map() const {return model_dissimilarity_matrix_map_;}

  /** Get class names */
  const std::vector<std::string>& get_class_names() const {return class_names_;}

  /** Only for Python: Plot the log-likelihood, BIC, and AIC scores */
  void python_plot_learning_scores()
  {
    THROW_PL_ERROR(89, "plConcurrentHmmSetLearner.python_plot_learning_scores() is only available for Python");
  }

  /** Get the observation variables */
  plVariablesConjunction observation_variables() const;

  /** Get the file examples corresponding to classification errors.      
      It returns for, each error, a pair <file, errornous predicted class index>.

      \param[in] csv_files The files corresponding to the sequences passed to run_offline_evaluation()
      \param[in] errors Classification error information as returned by plPredictionPerformanceReport::get_classification_error_row_numbers()

      To be used in conjunction with plPredictionPerformanceReport::get_classification_error_row_numbers() as follows:
      \code
      plConcurrentHmmSetLearner my_learner(...);
      .
      .
      .
      my_learner.run_learning(...)
      .
      .
      .
      my_learner.run_offline_evaluation(csv_test_files, ...)
      plPredictionPerformanceReport my_perf_report = my_learner.model_offline_performance_map()["GestureAcceleroGyroBestBIC"];
      
      std::vector<std::pair<std::string, unsigned int> > 
      error_files = plConcurrentHmmSetLearner::get_error_files(csv_test_files, my_perf_report.get_classification_error_row_numbers());
      
      \endcode
  */
  static std::vector<std::pair<std::string, unsigned int> >
    get_error_files(const std::vector<std::vector<std::string> > &csv_files,
                    const std::vector<std::pair<unsigned int, unsigned int> > &errors);


  /** Get the observation model 
      
      Return 0 if not set
  */
  const plLearnObject *get_observation_model() const {return observation_decomposition_;}



   /** Writes the graphviz dot drawing instructions to represent the graph
       corresponding to the observation model in @em file_name.

      The parameters dot_node_shape, dot_node_color, dot_node_fill_color,
      dot_background_color, and dot_rankdir correspond to graphviz dot ones (see
      http://www.graphviz.org/pdf/dotguide.pdf). There values will be inserted
      in the generated dot file without checking their validity.

      To generate an image file from the generated graphvis dot @em file_name,
      you can use "dot -T'img_format' 'file_name' -o file_name.'img_format' "
      in which 'img_format' could be "pdf", "png",...  
      The supported image formats could be found in
      http://www.graphviz.org/pdf/dotguide.pdf
  */
  void observation_model_draw_graph_dot(const std::string& file_name, 
                                        const std::string& dot_node_shape="", 
                                        const std::string& dot_node_color="",
                                        const std::string& dot_node_fill_color="",
                                        const std::string& dot_background_color="",
                                        const std::string& dot_edge_color="",
                                        const std::string& dot_rankdir="")const;
    

  /** Only for Python: Same as observation_model_draw_graph_dot() above */
  void python_observation_model_draw_graph_dot()
  {
    THROW_PL_ERROR(89, "plConcurrentHmmSetLearner.python_observation_model_draw_graph_dot() is only available for Python");
  }
  
 protected:
  void run_offline_evaluation_parallel(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences);
  void run_offline_evaluation_sequential(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences);

  void run_offline_evaluation_parallel(const std::vector<std::vector<std::string> >  &csv_testing_files,
                                       bool csv_file_has_header,
                                       unsigned int csv_file_nrows_to_ignore,
                                       const std::vector<unsigned int> &csv_file_columns_to_ignore);
  void run_offline_evaluation_sequential(const std::vector<std::vector<std::string> >  &csv_testing_files,
                                         bool csv_file_has_header,
                                         unsigned int csv_file_nrows_to_ignore,
                                         const std::vector<unsigned int> &csv_file_columns_to_ignore);

  

  void run_online_evaluation_parallel(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences,
                                      const std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
                                      const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states,
                                      plFloat obs_llk_threshold);
  void run_online_evaluation_sequential(const std::vector<plHMM::ObservationSequenceArray_t> &test_obs_sequences,
                                        const std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
                                        const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states,
                                        plFloat obs_llk_threshold);

  void run_online_evaluation_parallel(const plHMM::ObservationSequenceArray_t &test_obs_sequences,
                                      const std::vector <std::vector<unsigned int> > &class_values,
                                      const std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
                                      const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states,
                                      plFloat obs_llk_threshold);
  void run_online_evaluation_sequential(const plHMM::ObservationSequenceArray_t &test_obs_sequences,
                                        const std::vector <std::vector<unsigned int> > &class_values,
                                        const std::vector <std::vector < plProbValue > > &transition_matrix_final_states,
                                        const std::vector <std::vector < plProbValue > > &transition_matrix_non_final_states,
                                        plFloat obs_llk_threshold);

  void compute_dissimilarity_matrices_parallel(const std::vector<plHMM::ObservationSequenceArray_t> &obs_sequences, bool symmetric);
  void compute_dissimilarity_matrices_sequential(const std::vector<plHMM::ObservationSequenceArray_t> &obs_sequences, bool symmetric);

  void compute_dissimilarity_matrices_parallel(const std::vector<std::vector<std::string> > &csv_learning_files,
                                               bool csv_file_has_header,
                                               unsigned int csv_file_nrows_to_ignore,
                                               const std::vector<unsigned int> &csv_file_columns_to_ignore,
                                               bool symmetric);
  void compute_dissimilarity_matrices_sequential(const std::vector<std::vector<std::string> > &csv_learning_files,
                                                 bool csv_file_has_header,
                                                 unsigned int csv_file_nrows_to_ignore,
                                                 const std::vector<unsigned int> &csv_file_columns_to_ignore,
                                                 bool symmetric);
  
  

  template <typename TRAIT>
    void run_learning_impl( const typename TRAIT::data_params_vector_t &learn_obs_sequences,
                            const std::vector <unsigned int> &nstates,
                            unsigned int em_trials,
                            plFloat learning_prior_weight,
                            plFloat convergence_threshold);
  
  plBayesianNetwork get_observation_model_bn() const;
  
 private:
  std::string model_name_;
  std::vector<std::string> class_names_;
  plLearnObject *observation_decomposition_;
  std::vector <std::vector < plProbValue > > transition_matrix_final_states_;
  std::vector <std::vector < plProbValue > > transition_matrix_non_final_states_;
 
  std::vector<std::vector<plFloat> >  model_log_likelihood_;
  std::vector<std::vector<plFloat> >  model_bic_;
  std::vector<std::vector<plFloat> >  model_aic_; 
  std::map<std::string, plConcurrentHmmSet> model_map_;
  std::map<std::string, plPredictionPerformanceReport> model_offline_perf_map_;
  std::map<std::string, plPredictionPerformanceReport> model_online_perf_map_;
  std::map<std::string, plFloatMatrix > model_dissimilarity_matrix_map_;
  
  bool enable_multi_threading_;
  bool verbose_;
  std::ostream *output_stream_;
};


#endif // plConcurrentHmmSetLearner_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
