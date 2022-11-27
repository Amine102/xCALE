/*===========================================================================
 * Product        : ProBT
 * File           : plPredictionPerformanceReport.h
 * Author         : Kamel Mekhnacha <kamel.mekhnacha@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2016, ProBayes SAS  -  all rights reserved
 *===========================================================================*/


#ifndef PL_PREDICTION_PERFORMANCE_REPORT
#define PL_PREDICTION_PERFORMANCE_REPORT

#include "plConfig.h"

#include <plDLLExport.h>
#include <plObject.h>
#include <plVariablesConjunction.h>
#include <plVariableIndexer.h>

#include <vector>

/** Prediction Performance Report.

    It provides confusion matrix information (accuracy, precision, recall,...)
    in case of classification, and R2, RMSE, SSE, MBE, MAE, MRE in case of
    regression. It's used in model evaluation functions such as plBayesianNetwork::validate_prediction(),
    plCndDistribution::validate_prediction(), plConcurrentHmmSet::evaluate_online_classification(), and
    plConcurrentHmmSet::evaluate_offline_classification().

*/
class PL_DLL_API plPredictionPerformanceReport : public plObject
{
 public:
  /** Default constructor, for the benefit of Python wrappers. */
  plPredictionPerformanceReport();

  /** Construction for a given target */
  explicit plPredictionPerformanceReport(const plVariablesConjunction& target_vars);

  /** Construction using a vector of real values and a vector of predicted ones  */
  plPredictionPerformanceReport(const std::vector<plValues> &actual_values,
                                const std::vector<plValues> &predicted_values);

  /** Construction for classification using a vector of real values and a vector of predicted ones  */
  plPredictionPerformanceReport(const std::vector<unsigned int> &actual_values,
                                const std::vector<unsigned int> &predicted_values);

  /** Construction for regression using a vector of real values and a vector of predicted ones  */
  plPredictionPerformanceReport(const std::vector<plFloat> &actual_values,
                                const std::vector<plFloat> &predicted_values,
                                const std::string &variable_name="X");

  /** Construction for classification 

      To be used with function update_classification(unsigned int actual_index,
      unsigned int inferred_index,
      int data_row_number)
  */
  explicit plPredictionPerformanceReport(const std::vector<std::string> &class_names);
  
  /** Destructor */
  ~plPredictionPerformanceReport();
  
  /** Update prediction statistics.
      \param[in] actual_value The real target value
      \param[in] predicted_value The inferred target value
      \param[in] data_row_number The data row number to be used for keeping
      classification errors row lines when calling
      get_classification_error_row_numbers(). -1 is used when one does not need
      to keep error information
  */
  void update(const plValues &actual_value, const plValues &predicted_value, int data_row_number=-1);

  /** Update prediction statistics for classification.
      \param[in] actual_index The real target value index
      \param[in] predicted_index The inferred target value index
      \param[in] data_row_number The data row number to be used for keeping
      classification errors row lines when calling
      get_classification_error_row_numbers(). -1 is used when one does not need
      to keep error information
  */
  void update_classification(unsigned int actual_index,
                             unsigned int predicted_index,
                             int data_row_number);

  /** Get the number of validation examples */
  unsigned int get_n() const;

  /** Return True is classification results and False if regression */
  bool is_classification() const{return is_classif_;}

  /** Add another result to this */
  void add(const plPredictionPerformanceReport &other);

  /** Addition operator */
  plPredictionPerformanceReport operator+(const plPredictionPerformanceReport &other) const
  {
    plPredictionPerformanceReport ret(*this);
    ret.add(other);
    return ret;
  };
  
  /** Get a given entry of the confusion matrix.
      Applicable only for classification
  */
  unsigned int get_confusion_matrix_element(size_t real, size_t predicted) const;

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
  unsigned int get_n_classification_errors() const;

  /** Get the number of good classification responses.
      Applicable only for classification
  */
  unsigned int get_n_good_classifications() const;
 
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

  /** Return the row numbers corresponding to classification errors. 
      
      This corresponds to the wrong classified row numbers passed to the function
      update() as parameter, and the errornous predicted class index 
  */
  std::vector<std::pair<unsigned int, unsigned int> > get_classification_error_row_numbers() const
  {
    return classification_error_row_numbers_;
  }

  /** Get the set of values of the target 
     Applicable only for classification
  */
  std::vector<std::string> get_class_names() const;

  /** Get the names of the target variables
  */
  std::vector<std::string> get_target_names() const;

  /** Get the corresponding thml code */
  std::string to_html() const;
  
 protected:
  void update_regression(const plValues &actual_value, const plValues &inferred_value);
  void update_classification(const plValues &actual_value, const plValues &inferred_value,
                             int data_row_number);
  void print_regression_report(std::ostream &outs) const;
  void print_classification_report(std::ostream &outs) const;
  void increment_confusion_matrix_entry(size_t real_index, size_t predicted_index, unsigned int inc);
  void update_regression_elem(plFloat actual_value,
                              plFloat predicted_value,
                              unsigned int target_idx);
  void init();
  
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

  std::vector<std::pair<unsigned int, unsigned int> > classification_error_row_numbers_;

  std::vector<std::string> class_names_;
};

#endif // PL_PREDICTION_PERFORMANCE_REPORT

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:

