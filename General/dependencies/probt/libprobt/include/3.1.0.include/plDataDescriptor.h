/*=============================================================================
 * Product        : ProBT 
 * File           : plDataDescriptor.h
 * Author         : Kamel Mekhnacha
 * Creation       : Tue Feb 20 14:07:49 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plDataDescriptor_h
#define plDataDescriptor_h

#include "plConfig.h"

#include <plObject.h>

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>

#include <plVariablesConjunction.h>
#include <plValues.h>
#include <plVariable.h>

#include <plPlotData.h>
#include <plRandom.h>

// Work around a compilation problem under MSVC10
#undef min
#undef max 

class plVariableDiscretizer;

/** This abstract class defines a sequential data container.

    This data descriptor is intended to be used with learning functions such as
    plLearnObject::learn(), plBayesianNetwork::learn_parameters(), plBayesianNetwork::learn_structure(), 
    and plEMLearner::run().
*/
class PL_DLL_API plDataDescriptor :public plObject
{
 public:
  /** Constructor */
  plDataDescriptor()
    : plObject()
    , is_ok_(true)
    , n_(-1)
    {}
  
  /** Returns @em true if the initialization (construction) is OK. 

      \return @em true if the initialization (construction) is OK
  */
  bool is_ok()const{return is_ok_;}

  /** Destructor. */
  virtual ~plDataDescriptor(){}

  /** Returns the number of fields (columns).
      
      \return the number of fields (columns/variables).
  */
  virtual unsigned int get_num_fields()const = 0;

  /** Rewinds the container to go back to its beginning */
  virtual void rewind() = 0;

  /** Get the next data record (row) and advances by one record in the data
      
      \param[out] data_values an array of values for all variables described in the record.
      
      \param[out] data_definition a vector stating, for each value in \em
      data_values, whether it's defined or not.
      
      \return \em false iif the end of the container is reached.
      
      The returned pointers are managed internally: you should not attempt to
      delete them, or use them after another call to get_data_record(). Also,
      you should not use them when get_data_record() returns \em false.

      For Python, the signature of this function is changed to return its results.
      It can be used as follows:
      @code
      data_decriptor = ...
      not_done, data_values, data_definition = data_decriptor.get_data_record()
      @endcode
  */
  virtual bool get_data_record(const plValues*& data_values, 
                               const std::vector<bool>*& data_definition) = 0;

  /** Get the defined values from the next data record (row) and advances by one record in the data
      
      \param[out] data_values an array of values for all variables defined in the record.
      
      \return \em false iif the end of the container is reached.
  */
  bool get_next_defined_values(plValues& data_values);

  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] undef_value The value to be used for undefined values (missing)
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  template <typename T>
    void get_data_matrix_template(std::vector < std::vector<T> > &data_matrix,
                                  const T &undef_value,
                                  unsigned int n = std::numeric_limits<unsigned int>::max(),
                                  plFloat sampling_rate = PL_ONE)
  {
    data_matrix.clear();

    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    unsigned int i = 0;
    std::vector<T> row(get_num_fields());

    const bool use_sampling_rate = (sampling_rate < PL_ONE);
    if(use_sampling_rate) n = std::numeric_limits<unsigned int>::max();

    while( i < n && get_data_record(data_values, data_definitions) ) {
      if(!use_sampling_rate || plRandomFloat(PL_ONE) <= sampling_rate) {
        for(size_t j = 0; j < data_values->size(); ++j) {
          row[j] = ( (*data_definitions)[j] ? (*data_values)[j] : undef_value );
        }
        data_matrix.push_back(row);
        ++i;
      }
    }
  }

  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] undef_value The value to be used for undefined values (missing)
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector < std::vector<double> > &data_matrix,
                       double undef_value=NAN,
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, undef_value, n, sampling_rate);
  }
  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] undef_value The value to be used for undefined values (missing)
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector < std::vector<float> > &data_matrix,
                       float undef_value=NAN,
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, undef_value, n, sampling_rate);
  }
  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] undef_value The value to be used for undefined values (missing)
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector < std::vector<long double> > &data_matrix,
                       long double undef_value=NAN,
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, undef_value, n, sampling_rate);
  }
  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] undef_value The value to be used for undefined values (missing)
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector < std::vector<int> > &data_matrix,
                       int undef_value,
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, undef_value, n, sampling_rate);
  }
  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] undef_value The value to be used for undefined values (missing)
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector < std::vector<unsigned int> > &data_matrix,
                       unsigned int undef_value,
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, undef_value, n, sampling_rate);
  }
  /** Fills the data matrix corresponding to the data container. 
      \param[out] data_matrix the returned data matrix.
      \param[in] undef_value The value to be used for undefined values (missing)
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector < std::vector<long unsigned int> > &data_matrix,
                       long unsigned int undef_value,
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, undef_value, n, sampling_rate);
  }

  /** Fills the data matrix corresponding to the data container. It assumes no missing data
      \param[out] data_matrix the returned data matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector < plValues > &data_matrix,
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE);
  
  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  template <typename T>
    void get_data_matrix_template(std::vector <std::vector<T> > &data_matrix,
                                  std::vector <std::vector<bool> > &def_matrix, 
                                  unsigned int n = std::numeric_limits<unsigned int>::max(),
                                  plFloat sampling_rate = PL_ONE)
  {
    data_matrix.clear();
    def_matrix.clear();

    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    unsigned int i = 0;
    std::vector<T> row(get_num_fields());

    const bool use_sampling_rate = (sampling_rate < PL_ONE);
    if(use_sampling_rate) n = std::numeric_limits<unsigned int>::max();
    
    while( i < n && get_data_record(data_values, data_definitions) ) {
      if(!use_sampling_rate || plRandomFloat(PL_ONE) <= sampling_rate) {
        for(size_t j = 0; j < data_values->size(); ++j) {
          if( (*data_definitions)[j]) row[j] = (*data_values)[j];
        }
        data_matrix.push_back(row);
        def_matrix.push_back(*data_definitions);
        ++i;
      }
    }
  }

  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector <std::vector<double> > &data_matrix,
                       std::vector <std::vector<bool> > &def_matrix, 
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, def_matrix, n, sampling_rate);
  }
  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector <std::vector<float> > &data_matrix,
                       std::vector <std::vector<bool> > &def_matrix, 
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, def_matrix, n, sampling_rate);
  }
  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored
      
      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector <std::vector<long double> > &data_matrix,
                       std::vector <std::vector<bool> > &def_matrix, 
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, def_matrix, n, sampling_rate);
  }
  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector <std::vector<int> > &data_matrix,
                       std::vector <std::vector<bool> > &def_matrix, 
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, def_matrix, n, sampling_rate);
  }
  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector <std::vector<unsigned int> > &data_matrix,
                       std::vector <std::vector<bool> > &def_matrix, 
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, def_matrix, n, sampling_rate);
  }
  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector <std::vector<long unsigned int> > &data_matrix,
                       std::vector <std::vector<bool> > &def_matrix, 
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE)
  {
    get_data_matrix_template(data_matrix, def_matrix, n, sampling_rate);
  }

  /** Fills the data matrix (and the corresponding definition matrix) 
      corresponding to the data container. 
      
      \param[out] data_matrix the returned data matrix.
      \param[out] def_matrix the corresponding definition matrix.
      \param[in] n the number of rows (data records) to be copied
      \param[in] sampling_rate when sampling_rate < 1., the data descriptor is sampled and the parameter n above is ignored

      This function does not rewind this data descriptor
  */
  void get_data_matrix(std::vector <plValues > &data_matrix,
                       std::vector <std::vector<bool> > &def_matrix, 
                       unsigned int n = std::numeric_limits<unsigned int>::max(),
                       plFloat sampling_rate = PL_ONE);

    
  /** Returns the range of the data.

      \param[out] min_vals the minimal value found in the data container for each data field (column).
      \param[out] max_vals the maximal value found in the data container for each data field (column).

      This function has as side effect the rewinding of this data descriptor
  */
  virtual void get_data_range(std::vector<plFloat> &min_vals, std::vector<plFloat> &max_vals);

  /** Computes some simple stats on the data.
      \param[out] min_vals the minimal value for each data field (column).
      \param[out] max_vals the maximal value for each data field (column).
      \param[out] mean_vals the mean value for each data field (column).
      \param[out] stdev_vals the standard deviation for each data field (column).
      \param[out] n the number of defined records (rows) for each data field (column).
      \param[out] ncomplete_rows the number of complete records (rows) (with no missing values)

      \returns the number of data records (rows)
  */
  virtual unsigned int get_simple_stats(std::vector<plFloat> &min_vals, std::vector<plFloat> &max_vals,
                                        std::vector<plFloat> &mean_vals, std::vector<plFloat> &stdev_vals,
                                        std::vector<unsigned int> &n,
                                        unsigned int &ncomplete_rows);


  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  template <typename T>
    void get_column_values_template(unsigned int n, std::vector<T> &column_vals, const T& undef_value)
  {
    column_vals.clear();

    if(n >= get_num_fields() ) return;
        
    rewind();
    
    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    while( get_data_record(data_values, data_definitions) ) {
      const T val = ( (*data_definitions)[n] ? (*data_values)[n] : undef_value );
      column_vals.push_back(val);
    }
    rewind();
  }

  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, std::vector<double> &column_vals, double undef_value=NAN)
  {
    get_column_values_template(n, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, std::vector<float> &column_vals, float undef_value=NAN)
  {
    get_column_values_template(n, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, std::vector<long double> &column_vals, long double undef_value=NAN)
  {
    get_column_values_template(n, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, std::vector<int> &column_vals, int undef_value)
  {
    get_column_values_template(n, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, std::vector<unsigned int> &column_vals, unsigned int undef_value)
  {
    get_column_values_template(n, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, std::vector<long unsigned int> &column_vals, long unsigned int undef_value)
  {
    get_column_values_template(n, column_vals, undef_value);
  }

  /** Get a column vector.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, std::vector<std::string> &column_vals, const std::string &undef_value="")
  {
    get_column_values_template(n, column_vals, undef_value);
  }
  
  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  template <typename T>
    void get_column_values_template(unsigned int n, 
                                    std::vector<T> &column_vals,
                                    std::vector<bool> &column_defs)
  {
    column_vals.clear();
    column_defs.clear();

    if(n >= get_num_fields() ) return;
    
    rewind();
    
    const plValues *data_values;
    const std::vector<bool> *data_definitions;
    while( get_data_record(data_values, data_definitions) ) {
      column_vals.push_back( (*data_values)[n] );
      column_defs.push_back( (*data_definitions)[n] );
    }
    rewind();
  }

  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, 
                         std::vector<double> &column_vals,
                         std::vector<bool> &column_defs)
  {
    get_column_values_template(n, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.
  */
  void get_column_values(unsigned int n, 
                         std::vector<float> &column_vals,
                         std::vector<bool> &column_defs)
  {
    get_column_values_template(n, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, 
                         std::vector<long double> &column_vals,
                         std::vector<bool> &column_defs)
  {
    get_column_values_template(n, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, 
                         std::vector<int> &column_vals,
                         std::vector<bool> &column_defs)
  {
    get_column_values_template(n, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, 
                         std::vector<unsigned int> &column_vals,
                         std::vector<bool> &column_defs)
  {
    get_column_values_template(n, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, 
                         std::vector<long unsigned int> &column_vals,
                         std::vector<bool> &column_defs)
  {
    get_column_values_template(n, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] n column number.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_column_values(unsigned int n, 
                         std::vector<std::string> &column_vals,
                         std::vector<bool> &column_defs)
  {
    get_column_values_template(n, column_vals, column_defs);
  }

  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  template <typename T>
    void get_variable_values_template(const plVariable& variable, 
                                      std::vector<T> &column_vals,
                                      const T& undef_value)
  {
    int n = observed_variables().get_variable_position(variable);

    if(n >= 0) {
      get_column_values((unsigned int)n, column_vals, undef_value);
    }
  }

  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<double> &column_vals,
                           double undef_value=NAN)
  {
    get_variable_values_template(variable, column_vals, undef_value);
  }

  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<float> &column_vals,
                           float undef_value=NAN)
  {
    get_variable_values_template(variable, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<long double> &column_vals,
                           long double undef_value=NAN)
  {
    get_variable_values_template(variable, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<int> &column_vals,
                           int undef_value)
  {
    get_variable_values_template(variable, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<unsigned int> &column_vals,
                           unsigned int undef_value)
  {
    get_variable_values_template(variable, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<long unsigned int> &column_vals,
                           long unsigned int undef_value)
  {
    get_variable_values_template(variable, column_vals, undef_value);
  }
  /** Get a column vector.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[in] undef_value The value to be used for undefined values (missing)

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<std::string> &column_vals,
                           const std::string& undef_value)
  {
    get_variable_values_template(variable, column_vals, undef_value);
  }
    
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  template <typename T>
    void get_variable_values_template(const plVariable& variable, 
                                      std::vector<T> &column_vals,
                                      std::vector<bool> &column_defs)
  {
    int n = observed_variables().get_variable_position(variable);

    if(n >= 0) {
      get_column_values((unsigned int)n, column_vals, column_defs);
    }
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<double> &column_vals,
                           std::vector<bool> &column_defs)
  {
    get_variable_values_template(variable, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<float> &column_vals,
                           std::vector<bool> &column_defs)
  {
    get_variable_values_template(variable, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<long double> &column_vals,
                           std::vector<bool> &column_defs)
  {
    get_variable_values_template(variable, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<int> &column_vals,
                           std::vector<bool> &column_defs)
  {
    get_variable_values_template(variable, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<unsigned int> &column_vals,
                           std::vector<bool> &column_defs)
  {
    get_variable_values_template(variable, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<long unsigned int> &column_vals,
                           std::vector<bool> &column_defs)
  {
    get_variable_values_template(variable, column_vals, column_defs);
  }
  /** Get a column vector and the corresponding definition values.
      \param[in] variable the variable of interest.
      \param[out] column_vals the returned column vector.
      \param[out] column_defs the corresponding definition values.

      This function has as side effect the rewinding of this data descriptor
  */
  void get_variable_values(const plVariable& variable, 
                           std::vector<std::string> &column_vals,
                           std::vector<bool> &column_defs)
  {
    get_variable_values_template(variable, column_vals, column_defs);
  }


  /** Get the defined values */
  std::vector < plValues >
    get_defined_value_array(unsigned int n = std::numeric_limits<unsigned int>::max(),
                            plFloat sampling_rate = PL_ONE);
  
  /** Validate the data set by checking that the provided
      values are compatible with the types of the given variable
      conjunction associaded to the data descriptor.

      This function has as side effect the rewinding of this data descriptor

      \param[out] erroneous_lines line numbers (starting from 1) in which
      incompatible values have been found.
      
      @return true iff no incompatible values have been found.
  */
  virtual bool validate(std::vector<unsigned int> &erroneous_lines);

  /** Get the number of records (rows) in the data set.  

      ATTENTION: For the first call, the default implementation scans the data
      set and thus has a complexity of O(n) where \em n is the size.
  
      This function has as side effect the rewinding of this data descriptor

      @return the size of the data set.
  */
  virtual unsigned int get_n_records();

  /** Get the variables associated to the descriptor */
  virtual plVariablesConjunction observed_variables()=0;

#ifndef SWIG
  /** Prints the header and the 'n' first rows in the output stream 'sout'.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual std::ostream& print_head(unsigned int n = 20, std::ostream& sout = std::cout, char sep = '\t');

#endif

  /** Prints the header and the 'n' first rows in the file 'file'.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual void print_head(const std::string &file, unsigned int n = 20, char sep = '\t');

  virtual std::string get_head(unsigned int n = 20, char sep = '\t');

#ifndef SWIG
  /** Prints some simple statistic of the data.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual std::ostream& print_simple_stats(std::ostream& sout = std::cout, char sep = '\t');
#endif


  /** Prints some simple statistic of the data.
      This function has as side effect the rewinding of this data descriptor
  */
  virtual void print_simple_stats(const std::string &file, char sep = '\t');

  virtual std::string get_simple_stats(char sep = '\t');
  
  /** Return the discretization intervals for the variables corresponding to
      columns @em variable_columns according to this data set

      ATTENTION: when using this function in Python, pass the parameters
      'discretizers' and 'variable_columns' as Python lists
  */
  std::vector<std::vector<plFloat> >
    use_for_discretization(const std::vector<plVariableDiscretizer*> &discretizers,
                           const std::vector<unsigned int>& variable_columns);
  
  /** Return the discretization intervals for the variable corresponding to
      column @em variable_column according to this data set
  */
  std::vector<plFloat> use_for_discretization(plVariableDiscretizer* discretizer,
                                              unsigned int variable_column);

  /** Return true iif the data descriptor has been associated to a set of variables */
  virtual bool has_observed_variables() const
  {
    return true;
  }

  /** Skip the next n data lines */
  virtual void skip_next_lines(size_t n);

#ifndef SWIG
  /** Save the data descriptor as a CSV.
      This function has as side effect the rewinding of this data descriptor
  */
  void save_as_csv(std::ostream &csv_stream,
                   char separator, 
                   const std::string &undef_string);
#endif
  /** Save the data descriptor as a CSV.
      This function has as side effect the rewinding of this data descriptor
  */
  void save_as_csv(const std::string &csv_file,
                   char separator, 
                   const std::string &undef_string);

  /** Generate gnuplot instruction for plotting the data as a time-serie.
      'time_stamp_column' is the number, starting from 1, of the date-houre colum
      This function has as side effect the rewinding of this data descriptor
  */
  virtual void generate_gnuplot(const std::string &gnuplot_file, 
                                const std::string &title, 
                                unsigned int time_stamp_column = 0,
                                const std::string time_stamp_format = "%m/%d/%Y:%H:%M:%S",
                                plPlotType plottype = PL_DEFAULT_PLOT,
                                bool plot_points = false);

  static bool generate_gnuplot_for_csv_data(const std::string &csv_file, 
                                            char separator,
                                            const std::vector<std::string>  &used_columns_names,
                                            const std::vector<bool> &column_is_used,
                                            unsigned int n_ignored_lines,
                                            const std::string &gnuplot_file,
                                            const std::string &title, 
                                            unsigned int time_stamp_column,
                                            const std::string time_stamp_format,
                                            plPlotType plottype,
                                            bool plot_points);

  /** Get the n head rows as an html table */
  std::string head_html(unsigned int n= 20);

  /** Only for Python: Plot the data */
  void python_plot()
  {
    THROW_PL_ERROR(89, "plDataDescriptor.python_plot() is only available for Python");
  }

  /** Only for Python: Return the data as a Python list */
  void python_as_list()
  {
    THROW_PL_ERROR(89, "plDataDescriptor.python_as_list() is only available for Python");
  }

  /** Only for Python: Return the data as a Numpy array */
  void python_as_numpy_array()
  {
    THROW_PL_ERROR(89, "plDataDescriptor.python_as_numpy_array() is only available for Python");
  }

  /** Only for Python: Return the data as a Numpy recarray */
  void python_as_numpy_recarray()
  {
    THROW_PL_ERROR(89, "plDataDescriptor.python_as_numpy_recarray() is only available for Python");
  }

  /** Only for Python: Return the data as a Pandas dataframe */
  void python_as_pandas_dataframe()
  {
    THROW_PL_ERROR(89, "plDataDescriptor.python_as_pandas_dataframe() is only available for Python");
  }
  
  /** Create a plDataDescriptor from a Python data array (Numpy, Pandas)
      Only for Python: Return the data as a plNumPyDataDescriptor 
  */
  static void create_from_python_data()
  {
    THROW_PL_ERROR(89, "plDataDescriptor.create_from_python_data() is only available for Python");
  }
  
 protected:
  virtual void Output(std::ostream &os) const;

  virtual void print_header(std::ostream& sout, const std::string &add_text, char sep) const;

 protected:
  bool is_ok_;
  int n_;
};

/** Interface for a data descriptor able to return lines of data as uninterpreted raw string vectors.
 */
class PL_DLL_API plStringDataDescriptor {
 public:
  virtual ~plStringDataDescriptor() {}

  /** Returns the raw, uninterpreted data record as a vector of strings. This is a virtual
      method that should be filled in by implementations.

      \param[out] data_values Line of data split in elements, resized as needed and filled in by this method.

      @return false iff the end of the container is reached.

      This method consumes one line of data descriptor input.
  */
  virtual bool get_data_record(std::vector<std::string> & data_values) = 0;

  /** Tries to guess the variables described by a string data descriptor, by examining its data.
      In the current implementation, one pass is made over all data of the descriptor.

      \param[in] header Whether the first line of data is a header describing the variable names.
      \param[in] n_discretization_bins Discretization to apply to real variables.

      - A null value prescribes no discretization: plRealType is used.
      - A positive value prescribes a plDiscreteIntervalType with n_discretization_bins equal length bins
      - A negative value prescribes a plDiscreteIntervalType with -n_discretization_bins equal frequency bins
      
      @return A conjunction of variables suitable to describe the variables in the data descriptor.
  */
  plVariablesConjunction guess_variables(bool header, int n_discretization_bins = 0);
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
