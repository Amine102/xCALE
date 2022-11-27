/*=============================================================================
 * Product        : ProBT 
 * File           : plCSVFileDataDescriptor.h
 * Author         : Kamel Mekhnacha
 * Creation       : Wed Feb 21 14:55:39 2007
 *
 *=============================================================================
 *        (c) Copyright 2007, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plCSVFileDataDescriptor_h
#define plCSVFileDataDescriptor_h

#include "plConfig.h"

#include <plDataDescriptor.h>
#include <plCSVLineParser.h>
#include <plLabelType.h>
#include <plVariablesConjunction.h>
#include <plVariable.h>
#include <plPlotData.h>

#include <fstream>
#include <algorithm>


// Note: do not write "file:", some versions of Doxygen (1.7.1) interpret it as an URI.
/** A data descriptor for data in CSV (comma-separated values) format. It can parse
    the data from a file :
    - it allows to specify the variables and order of the columns inside the file
    - it allows parsing numeric fields (for non-label variable types) and textual fields 
    (for label variable types)
    - it allows caching the data if several passes are needed and if the memory allows it
    - empty lines are skipped
    - leading and trailing spaces and tabulations around a quoted field are ignored
    - each field can be quoted. The used quote (" by default) is customisable in the constructor or 
    using set_quote())
    - records (lines) are ended/separated by a line delimiter 
    (its default value is '\\n' and is customisable in the constructor or using set_line_delimiter())
    - fields are separated with a field delimiter  (its default value is set automatically among ';' ',' , ' ', '\\t'
    and is customisable in the constructor or using set_field_delimiter())
    - an empty field is considered undefined additionally, a field equal to undefined_field 
    (its default value is 'empty string' and is customisable in the constructor or using 
    set_undefined_field()).
*/
class PL_DLL_API plCSVFileDataDescriptor  :public plDataDescriptor
{ 
 private:
  // disable copy and assignment
  plCSVFileDataDescriptor(const plCSVFileDataDescriptor&);
  plCSVFileDataDescriptor & operator=(const plCSVFileDataDescriptor &);
  
 public:
 
  /** Constructs a CSV data descriptor over a file containing CSV data.
      
      @param[in] file_name The name of the file containing the CSV data.  The
      file must exist and be readable.
      
      @param[in] variables A conjunction describing the variables present in
      the file. This should be a superset of the variables announced in the
      file header if any. The other variables announced in the file header and
      that are not present is this parameter are simply ignored. This means
      that observed_variables() (and also the values returned by
      get_data_record() ) will return variables that are both in this parameter
      and in the file header (that is to say, the intersection of these two
      sets) but in the order specified by the variables announced in the file
      header. The special case corresponding to a file with no header is
      assumed if the intersection is empty. This means that
      observed_variables() (and also the values returned by get_data_record() )
      will return all the variables in this parameter at the same order (see
      the example at the end of this documentation).
    
      @param[in] cache_data Whether this descriptor should cache the data, in
      order to avoid parsing after the first rewind(). Its default values is
      'true'. If memory allows it, this provides a significant speed boost when
      several passes are made over the data. ATTENTION: If cache_data is set to
      'true', all the following parsing parameters are ignored if they are set
      after the first utilization of the descriptor.

      @param[in] field_delimiter the field separator used in the CSV data. It's
      default value is set automatically among ';' ',' , ' ', '\\t' and is customisable
      in the constructor or using set_field_delimiter().

      @param[in] line_delimiter The line separator used in the CSV data. It's
      default value is '\\n' and it can be set later using set_line_delimiter().

      @param[in] undefined_field The value that will correspond to an undefined
      field in the CSV data.  Its default value is "" (empty string) and it can be set later using
      set_undefined_field(). In addition to this value, an empty field always
      corresponds to an undefined field.

      @param[in] quote The character to be used for quoting text fields. The
      default is " and it can be set later using set_quote().

      Example: Let's consider the following file content:
      @code
      "COLOR";"X";"A"
      BLUE;1.3;0
      RED;3.1;1
      @endcode
      and the 'variables' parameter having 4 variables with names 'A',
      'COLOR', 'B', 'C'.  In this case, observed_variables() will return {COLOR,
      A} and the first data record will be {COLOR=BLUE, A=0}.
  */
  plCSVFileDataDescriptor(const std::string &file_name,
                          const plVariablesConjunction &variables,
                          bool cache_data = true,
			  char field_delimiter = 0, 
			  char line_delimiter = '\n', 
			  const std::string &undefined_field ="",
                          char quote='"');


  /** Constructs a CSV data descriptor over a file containing CSV data without
      associating it to a set of variables.  The actual association to a set of
      variables could be done later by calling set_variables().  If the
      association is not done (no call to set_variables()) then only numeric
      fields are allowed (no textual fields) and textual fields will be
      considered as undefined ones.
      
      @param[in] file_name The name of the file containing the CSV data.  The
      file must exist and be readable.

      @param[in] num_fields The number of columns (fields) to be read. If 0 (the default value),
      the file will be parsed in order to guess its number of columns.
      
      @param[in] cache_data Whether this descriptor should cache the data, in
      order to avoid parsing after the first rewind(). Its default values is
      'true'. If memory allows it, this provides a significant speed boost when
      several passes are made over the data. ATTENTION: If cache_data is set to
      'true', all the following parsing parameters are ignored if they are set
      after the first utilization of the descriptor.
      
      @param[in] ignore_lines The number of header lines to be ignored. Its
      default value is 0 and it can be set later using set_ignore_lines().

      @param[in] field_delimiter the field separator used in the CSV data. It's
      default value is set automatically among ';' ',' , ' ', '\\t' and is customisable
      in the constructor or using set_field_delimiter().
      
      @param[in] line_delimiter The line separator used in the CSV data. It's
      default value is '\\n' and it can be set later using set_line_delimiter().
      
      @param[in] undefined_field The value that will correspond to an undefined
      field in the CSV data.  Its default value is "" (empty string) and it can be set later using
      set_undefined_field(). In addition to this value, an empty field always
      corresponds to an undefined field.
      
      @param[in] columns_to_ignore The indices (starting from 0) of the columns
      (fields) to be ignored in the file. The default value is 'empty' (no
      columns to ignore) and it can be set later using set_columns_to_ignore().

      @param[in] quote The character to be used for quoting text fields. The default is
      " and it can be set later using set_quote().
  */
  explicit plCSVFileDataDescriptor(const std::string &file_name, 
                                   unsigned int num_fields = 0,
                                   bool cache_data = true,
                                   unsigned int ignore_lines = 0,
                                   char field_delimiter = 0, 
                                   char line_delimiter = '\n', 
                                   const std::string &undefined_field ="",
                                   const std::vector<unsigned int> &columns_to_ignore = std::vector<unsigned int>(),
                                   char quote='"');


  /** Destructor */
  ~plCSVFileDataDescriptor();

  /** Guess the field delimiter among the passed ones based on statistics on the 'nrows' first lines of the file.
      Return 0 if no delimiter is found (one-column file) 
  */
  static char guess_field_delimiter(const std::string &file_name,
                                    const std::string &delimiters = ";, \t",
                                    size_t nrows=10);

  /** Guess the field of fields among of the file. */
  static unsigned int guess_number_of_fields(const std::string &file_name);
  
  void rewind();
  
  /** Get the next data record (row) as a vector of strings and advances by one
      record in the data Returns false if eof.
      ATTENTON: This should not be called using cached CSV descriptors
  */
  bool get_data_record(std::vector<std::string> &records);

  /** Load a csv file into a data matrix */
  static void load_data_matrix(const std::string &csv_file_name,
                               std::vector < std::vector<double> > &data_matrix,
                               double undef_value=NAN,
                               const std::vector<unsigned int> &columns_to_ignore = std::vector<unsigned int>(),
                               unsigned int ignore_lines = 0,
                               unsigned int n = std::numeric_limits<unsigned int>::max(),
                               plFloat sampling_rate = PL_ONE)
  {
    load_data_matrix_template(csv_file_name, data_matrix, undef_value, columns_to_ignore, ignore_lines, n, sampling_rate);
  }
  
  /** Load a csv file into a data matrix */
  static void load_data_matrix(const std::string &csv_file_name,
                               std::vector < std::vector<float> > &data_matrix,
                               float undef_value=NAN,
                               const std::vector<unsigned int> &columns_to_ignore = std::vector<unsigned int>(),
                               unsigned int ignore_lines = 0,
                               unsigned int n = std::numeric_limits<unsigned int>::max(),
                               plFloat sampling_rate = PL_ONE)
  {
    load_data_matrix_template(csv_file_name, data_matrix, undef_value, columns_to_ignore, ignore_lines, n, sampling_rate);
  }
  
  /** Load a csv file into a data matrix */
  static void load_data_matrix(const std::string &csv_file_name,
                               std::vector < std::vector<long double> > &data_matrix,
                               long double undef_value=NAN,
                               const std::vector<unsigned int> &columns_to_ignore = std::vector<unsigned int>(),
                               unsigned int ignore_lines = 0,
                               unsigned int n = std::numeric_limits<unsigned int>::max(),
                               plFloat sampling_rate = PL_ONE)
  {
    load_data_matrix_template(csv_file_name, data_matrix, undef_value, columns_to_ignore, ignore_lines, n, sampling_rate);
  }

  /** Load a csv file into a data matrix */
  static void load_data_matrix(const std::string &csv_file_name,
                               std::vector < std::vector<int> > &data_matrix,
                               int undef_value,
                               const std::vector<unsigned int> &columns_to_ignore = std::vector<unsigned int>(),
                               unsigned int ignore_lines = 0,
                               unsigned int n = std::numeric_limits<unsigned int>::max(),
                               plFloat sampling_rate = PL_ONE)
  {
    load_data_matrix_template(csv_file_name, data_matrix, undef_value, columns_to_ignore, ignore_lines, n, sampling_rate);
  }
  
 private:

  template<typename T>
    bool read_variable_from_current_line_impl(plData &val);
  
  template<typename T>
    static void load_data_matrix_template(const std::string &csv_file_name,
                                          std::vector < std::vector<T> > &data_matrix,
                                          const T &undef_value,
                                          const std::vector<unsigned int> &columns_to_ignore,
                                          unsigned int ignore_lines,
                                          unsigned int n,
                                          plFloat sampling_rate)
  {
    plCSVFileDataDescriptor desc(csv_file_name);
    desc.set_columns_to_ignore(columns_to_ignore);
    desc.set_ignore_lines(ignore_lines);
    
    desc.get_data_matrix_template(data_matrix, undef_value, n, sampling_rate);
  }
  
 public:
  /** Get the next data record (row) and advances by one record in the data.
      
      \param[out] data_values an array of values for all variables described in the record.
      
      \param[out] data_definition a vector stating, for each value in \em
      data_values, whether it's defined or not.
      
      \return \em false iif the end of the container is reached.
      
      The returned pointers are managed internally: you should not attempt to
      delete them, or use them after another call to get_data_record(). Also,
      you should not use them when get_data_record() returns \em false.
  */
  bool get_data_record(const plValues *& data_values,
                       const std::vector<bool>*& data_definition);
    
  unsigned int get_num_fields()const;
	

  /** Get the number of records (rows) in the data set.
   
      ATTENTION: if not cached, the first call scans the data
      set and thus has a complexity of O(n) where \em n is the size.
  
      @return the size of the data set.
  */
  virtual unsigned int get_n_records();


  /** Set the variables to be associated to the data descriptor.
      This method is to be called when the descriptor is constructed without providing
      the variables list. After this call, observed_variables() will return 
      the passed 'variables' parameter.
      
      @param[in] variables The variables to be associated to the data descriptor
  */
  void set_variables(const plVariablesConjunction &variables);

  virtual plVariablesConjunction observed_variables();

  /** Set the field separator to be used in the CSV data */
  void set_field_delimiter(char field_delimiter);

  /** Set the line separator to be used in the CSV data */
  void set_line_delimiter(char line_delimiter);
  
  /** Set the 'undefined_field' string to be used in the CSV data */
  void set_undefined_field(const std::string &undefined_field);

  /** Set the quote chracter to be used in the CSV data */
  void set_quote(char quote);

  /** Set the columns (fields) to be ignored in the CSV file.

      The passed columns are ignored only if the descriptor is constructed using
      plCSVFileDataDescriptor(const std::string &file_name, unsigned int num_fields,...).
      Otherwise, this call has no effect.
   */
  void set_columns_to_ignore(const std::vector<unsigned int> &columns_to_ignore);

  /** Add a column (fields) to be ignored in the CSV file.
      
      The passed columns are ignored only if the descriptor is constructed using
      plCSVFileDataDescriptor(const std::string &file_name, unsigned int num_fields,...).
      Otherwise, this call has no effect.
  */
  void add_column_to_ignore(unsigned int column_to_ignore);

  /** Set the number of lines to be ignored in the CSV file.
      
      This number excludes the first header line if the descriptor is constructed using
      plCSVFileDataDescriptor(const std::string &file_name, const plVariablesConjunction &variables, ...)
      and the CSV source contains a header line.
  */
  void set_ignore_lines(unsigned int n);

  /** Set the caching flag 

      If memory allows it, this provides a significant speed boost when
      several passes are made over the data.
  */
  void set_cache_data(bool cache_it);

  /** Set the number of columns (fields) to be read. 

      If 0 (the default value), the file will be parsed in order to guess its
      number of columns.
  */
  void set_num_fields(unsigned int num_fields);

  
  void generate_gnuplot(const std::string &gnuplot_file, 
                        const std::string &title, 
                        unsigned int time_stamp_column = 0,
                        const std::string time_stamp_format = "%m/%d/%Y:%H:%M:%S",
                        plPlotType plottype = PL_DEFAULT_PLOT,
                        bool plot_points = false);
  
  virtual bool has_observed_variables() const;
   
 protected:

  bool open_file(const std::string &csv_data_file);

  plValues create_row_values() const;
    
  void init();
 
  // fill obs_vars, num_fields_, columns_to_ignore_
  bool get_header_variables(const plVariablesConjunction &among_variables,
                            plVariablesConjunction &observed_vars);

  void parse_header();
    
  void skip_header_lines();

  virtual void Output(std::ostream &os)const;
 
  void read_line(plValues &current_row, std::vector<bool> &current_row_def);

  static unsigned int guess_number_of_fields(plCSVLineParser &parser, std::istream &the_stream);

  virtual void print_header(std::ostream& sout, const std::string &add_text, char sep) const;

 protected:
  unsigned int num_fields_;
 
  plCSVLineParser parser_;
  bool cache_data_;
  unsigned int ignore_lines_;
  plValues current_row_;
  std::vector<bool> current_row_def_;

  std::vector<plValues> all_data_;
  
  std::vector<std::vector<bool> > all_data_def_;


  unsigned int current_row_number_;
  std::ifstream the_stream_;

  bool is_rewinded_;
  std::vector<unsigned int> columns_to_ignore_;

  plVariablesConjunction variables_;
  std::vector<bool> is_label_;
  std::vector< plLabelType > label_;

  bool init_done_;
  bool need_header_parsing_;
  plVariablesConjunction passed_variables_;
  std::string file_name_;
  std::vector<bool> not_in_ignored_columns_;
  std::string old_locale_;
  bool in_cache_;
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
