#include <ostream>
#include <vector>

#include "pilgrim/general/pmCSVDataSet.h"
#include "pilgrim/general/pmUtility.h"

#include <plStructureLearner.h>

using namespace PILGRIM;

using namespace std;
/*
 =====CONSTS=====
 */

const int CARDINALITY = 10;

/*
 =====CONSTRUCTORS=====
 */
pmCSVDataSet::pmCSVDataSet(const std::string& _file_name,

                           size_t &winSize,

                           size_t &nmbFields,

                           char separator,
                           int start_num_gaussians,
                           bool force_num_gaussians) : file_name(_file_name)
{
  loadDataSet(winSize, nmbFields, separator);
  std::pair<bool,int> force_gaussians = std::make_pair(force_num_gaussians, start_num_gaussians);
  create_and_guess_variables(force_gaussians);
}

pmCSVDataSet::pmCSVDataSet(const std::string& _file_name,

                           size_t &winSize,

                           size_t &nmbFields,

                           char separator,

                           const plVariablesConjunction &variables) : file_name(_file_name)
{
  loadDataSet(winSize, nmbFields, separator);
  data_descriptor->set_variables(variables);
}

pmCSVDataSet::pmCSVDataSet(const std::string& _file_name) : file_name(_file_name)
{
  size_t winSize = 1;
  size_t &pwinSize = winSize;
  char separator = guess_delimiter();
  size_t nmbFields = guess_nb_variables(separator);
  loadDataSet(pwinSize, nmbFields, separator);
  create_and_guess_variables();
}
pmCSVDataSet::pmCSVDataSet(const std::string& _file_name, vector<string> vars_undiscretized, int vars_cardinality) : file_name(_file_name)
{
  size_t winSize = 1;
  size_t &pwinSize = winSize;
  char separator = guess_delimiter();
  size_t nmbFields = guess_nb_variables(separator);
  loadDataSet(pwinSize, nmbFields, separator);
  create_and_guess_variables(vars_undiscretized, vars_cardinality);
}

pmCSVDataSet::pmCSVDataSet(const std::string& _file_name, int n_discretization_bins) : file_name(_file_name)
{
  size_t winSize = 1;
  size_t &pwinSize = winSize;
  char separator = guess_delimiter();
  size_t nmbFields = guess_nb_variables(separator);
  loadDataSet(pwinSize, nmbFields, separator);
  create_and_guess_variables(n_discretization_bins);
}


pmCSVDataSet::pmCSVDataSet(const std::string& _file_name, const plVariablesConjunction &variables) : file_name(_file_name)
{
  char separator = guess_delimiter();
  bool store_data = true;
  char line_end = '\n';
  static string undef_str ="";
  static vector<unsigned int> columns_to_ignore;
  char quote = '\"';
  bool flag = false;

  do {
    FILE* fp = fopen(file_name.c_str(), "r");
    if (fp) {
      flag=true;
      fclose(fp);
    } else {
      cout << "Enter  a correct path to the data file: ...\n";
      cin >> file_name;
    }

  } while (!flag);
  data_descriptor = new plCSVFileDataDescriptor(
                          file_name,
                          variables,
                          store_data,
                          separator,
                          line_end,
                          undef_str,
                          quote);
}

/*
 =====DESTRUCTOR=====
 */
pmCSVDataSet::~pmCSVDataSet(void)
{

  delete data_descriptor;
}

plCSVFileDataDescriptor *pmCSVDataSet::get_data_descriptor()
{
  return data_descriptor;
}

unsigned int pmCSVDataSet::get_num_fields()
{
  return data_descriptor->get_num_fields();
}

std::string& pmCSVDataSet::get_filename()
{
  return file_name;
}

size_t pmCSVDataSet::get_n_records()
{
  return data_descriptor->get_n_records();
}

plVariablesConjunction pmCSVDataSet::observed_variables()
{
  return data_descriptor->observed_variables();
}

bool pmCSVDataSet::get_data_record(vector<string> &records)
{
  return data_descriptor->get_data_record(records);
}

/*
=====INIT=====
*/
void pmCSVDataSet::create_and_set_variables(size_t &nmbFields)
{

  // assign variables names

  plVariablesConjunction all_variables;

  std::vector<plFloat> min_val, max_val;

  data_descriptor->get_data_range(min_val, max_val);

  // Put all variables in plVariableconjunction one by one

  for (size_t i = 0; i < nmbFields; ++i)
  {

    std::ostringstream variable_name;

    variable_name << "v" << i; // generate dynamic variable names as a string v1,v2,v3

    plVariable variable(variable_name.str(), plIntegerType(min_val.at(i), max_val.at(i)));

    all_variables = all_variables ^ variable;
  }

  data_descriptor->set_variables(all_variables);
}

/*
 =====CREATION OF THE VARIABLES : here the discretization happens =====
 */

void pmCSVDataSet::create_and_guess_variables(std::pair<bool,int> force_num_gaussians)
{
  // empty variable conjunction
  plVariablesConjunction all_variables;

  // empty datadescripor for static method
  plCSVDataDescriptor dataDescriptor(file_name);

  // do not change this or it crashes
  bool is_header = true;
  int n_discretization_bins = 0;
  string str_file = file_name;
  const string delimiter = ";";
  // add a log folder
  string file_result = "log_discretisation";
  vector<double> cutpoints;
  plJointDistribution model;
  plDiscreteIntervalType type_discret;
  plVariablesConjunction result;
  bool modeVerbose = false;

  // using probt3.1 guess_variable static method from external library include headers
  all_variables = dataDescriptor.guess_variables(str_file,
                                                 is_header,
                                                 n_discretization_bins,
                                                 delimiter);
  std::string str(file_name);
  // setting the variables guessed
  for (unsigned int i = 0; i < all_variables.dim(); ++i)
  {
    // if the variable is a plRealType or the cardinality is bigger than the one fixed
    if (all_variables[i].get_var_type() == 1 || all_variables[i].cardinality() > CARDINALITY)
    {
      pmUtility pm;
      cutpoints = pm.discretisation(i, str, file_result, model, all_variables[i], modeVerbose, force_num_gaussians);
      type_discret = plDiscreteIntervalType(cutpoints);
      result = all_variables.replace_type(all_variables[i].name(), type_discret);
      all_variables = result;
    }
  }
  data_descriptor->set_variables(all_variables);
}

//====  This method discretizes variables using classic discretization methods already implemented in probT ==/

void pmCSVDataSet::create_and_guess_variables(int n_discretization_bins)
{
  // empty variable conjunction
  plVariablesConjunction all_variables;

  // empty datadescripor for static method
  plCSVDataDescriptor dataDescriptor(file_name);

  // do not change this or it crashes
  bool is_header = true;
  string str_file = file_name;
  const string delimiter = ";";

  // using probt3.1 guess_variable static method from external library include headers
  all_variables = dataDescriptor.guess_variables(str_file,
                                                 is_header,
                                                 n_discretization_bins,
                                                 delimiter);

  // setting the variables guessed
  data_descriptor->set_variables(all_variables);
}

// Note : Do not fill the indiscretized vectors with variables that would be guessed as plRealType : these must be discretized

void pmCSVDataSet::create_and_guess_variables(vector<string> vars_undiscretized, int vars_cardinality)
{
  // empty variable conjunction
  plVariablesConjunction all_variables;

  // empty datadescripor for static method
  plCSVDataDescriptor dataDescriptor(file_name);

  // do not change this or it crashes
  bool is_header = true;
  int n_discretization_bins = 0;
  string str_file = file_name;
  const string delimiter = ";";
  string file_result = "log_discretisation";
  vector<double> cutpoints;
  plJointDistribution model;
  plDiscreteIntervalType type_discret;
  plVariablesConjunction result;
  bool condition;
  vector<int> vars_test;
  bool modeVerbose = false;
  //

  // using probt3.1 guess_variable static method from external library include headers
  all_variables = dataDescriptor.guess_variables(str_file,
                                                 is_header,
                                                 n_discretization_bins,
                                                 delimiter);
  std::string str(file_name);
  // setting the variables guessed
  for (unsigned int i = 0; i < all_variables.dim(); ++i)
  {
    if (count(vars_undiscretized.begin(), vars_undiscretized.end(), all_variables[i].name()))
    {
      condition = true;
    }
    if ((all_variables[i].get_var_type() == 1 || all_variables[i].cardinality() > vars_cardinality) && !condition)
    {
      pmUtility pm;
      cutpoints = pm.discretisation(i, str, file_result, model, all_variables[i], modeVerbose);
      type_discret = plDiscreteIntervalType(cutpoints);
      result = all_variables.replace_type(all_variables[i].name(), type_discret);
      all_variables = result;
    }
  }
  data_descriptor->set_variables(all_variables);
}

char pmCSVDataSet::guess_delimiter()
{
  // Opening just first line (header most likely)
  fstream myFile(file_name, ios::in);
  string first_line;
  getline(myFile, first_line);
  myFile.close();
  string csv_separators = ",;";
  size_t i(0);
  bool not_found = true;
  char sep;
  while ((not_found) && (i < csv_separators.size()))
  {
    size_t pos(0);
    pos = first_line.find_first_of(csv_separators[i]);
    if (pos != string::npos && pos != 0)
    {
      sep = first_line[pos];
      not_found = false;
    }
    else
      i++;
  }
  return sep;
}

size_t pmCSVDataSet::guess_nb_variables(char delim)
{
  // Opening just first line (header most likely)
  fstream myFile(file_name, ios::in);
  string first_line;
  getline(myFile, first_line);
  myFile.close();

  // Needed variables
  string element;
  size_t count(0);

  // My new stream to delimit
  istringstream myLine(first_line);
  while (getline(myLine, element, delim))
  {
    count++;
  }
  return count;
}

/*
=====PRINTERS=====
*/
void pmCSVDataSet::summary()
{
  cout << "###########\n Dataset summary\n###########\n";
  // WARNING: Don't use print_simple_stats() after print_head()
  cout << "--- Stats ---\n";
  data_descriptor->print_simple_stats();
  cout << "--- Head ---\n";
  data_descriptor->print_head();
}

void pmCSVDataSet::print_variables()
{
  for (size_t i; i < observed_variables().get_names().size(); i++)
  {
    cout << observed_variables().get_names()[i] << endl;
  }
}

/*
=====PROTECTED INIT METHOD====
*/
void pmCSVDataSet::loadDataSet(size_t &winSize, size_t &nmbFields, char separator)
{
  bool store_data = true;
  unsigned int ignore_lines = 0 + winSize;
  char line_end = '\n';
  static string undef_str = "";
  static vector<unsigned int> columns_to_ignore;
  char quote = '\"';
  bool flag = false;

  do
  {
    FILE *fp = fopen(file_name.c_str(), "r");
    if (fp)
    {
      flag = true;
      fclose(fp);
    }
    else
    {
      cout << "Enter  a correct path to the data file: ...\n";
      cin >> file_name;
    }

  } while (!flag);
  data_descriptor = new plCSVFileDataDescriptor(
      file_name,
      nmbFields,
      store_data,
      ignore_lines,
      separator,
      line_end,
      undef_str,
      columns_to_ignore,
      quote);
}
