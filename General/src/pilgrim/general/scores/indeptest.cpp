

#include <iostream>
#include <math.h>
#include <boost/math/distributions/chi_squared.hpp>

#include "pilgrim/general/scores/indepTest.h"


using namespace std;
using namespace boost;
using namespace PILGRIM;

chiSequare::chiSequare(pmDataSet * const _pCompDataSet, plVariablesConjunction *_variables,
             float *_alpha, double *_chi2, double *_p) {
  if (_variables != NULL) {
    variables = _variables;
  }

  if (_alpha != NULL) {
    alpha = *_alpha;
  } else{
    alpha = 0.05;
  }

  if (_chi2 != NULL) {
    chi2 = *_chi2;
  } else {
    chi2 = 0.05;
  }

  if (_p != NULL) {
    p = *_p;
  } else {
    p = 0.05;
  }

  if (_alpha != NULL) {
    alpha = *_alpha;
  } else {
    alpha = 0.05;
  }

  pCompDataSet = _pCompDataSet;
}

bool chiSequare::chi_square_test(int var_a, int var_b, std::vector <int> vars) {
  bool ind;
  df = 1;
  unsigned int counter = 0;


  ///****************input from user for indep test******************
  // vars contains indices of variables in the CSV file, but 1-based
  // cout<< "\n\n successfully called function 3" ;
  vars.push_back(var_b);
  vars.push_back(var_a);

  ///******************** loading variables in vectors****************************
  // a copy of data in the descriptor, including only data for interesting variables (those pointed to by vars)
  // first index: variables (ranges from 0 to vars.size() - 1)
  // second index: row index (from 0 to the number of rows minus 1)
  std::vector < std::vector <int> > test_var;
  for (unsigned int i = 0; i < vars.size(); ++i) {
    vector<int> vec;
    pCompDataSet->get_column_values(vars.at(i), vec, -INT_MAX); // TODO: The value to be used for undefined values is set to -INT_MAX. Check if it is applicable in all cases  
    test_var.push_back(vec);
  }
  std::vector< plFloat > min_val, max_val;
  pCompDataSet->get_data_range(min_val, max_val);

  for (int iter = vars.size()-1; iter >= 0; iter--) {
    counter++;
    if (counter <= 2 )
      df *= ((max_val.at(vars.at(iter)) - min_val.at(vars.at(iter)) + 1) - 1);  // r - 1
    else
      df *= (max_val.at(vars.at(iter)) - min_val.at(vars.at(iter)) + 1);  // |C|* |D|...........
  }
  //************** test  to proceed indeptest ***********
  // if N < 10 * df then dat is not engough to proceed the test

  if (vars.size() > 2 && test_var[1].size() < (10*df)) {
    ind = false;
    p = 0;
    chi2 = 0;
    return ind;
  }

  //****************************************************
  plVariablesConjunction all_variables;

  // Put all variables in plVariableconjunction one by one
  for (unsigned int i = 0; i < vars.size(); ++i) {
    std::ostringstream variable_name; variable_name << "v" << vars.at(i);  // generate dynamic variable names as a string v1,v2,v3
    plVariable variable(variable_name.str(), plIntegerType(min_val.at(vars.at(i)), max_val.at(vars.at(i))));
    all_variables = all_variables ^ variable;
  }

  plValues vals(all_variables);

  //*********************add points************************
  // load data in variables
  plLearnHistogram lh(all_variables);

  for (unsigned int r = 0; r < test_var[1].size(); ++r) {
    for (unsigned int c = 0; c < test_var.size(); ++c) {
      vals[c] = test_var[c][r];
    }
    // std::cout << "adding data point: " << vals << std::endl;
    lh.add_point(vals);
  }
  // **** plDistribution contingency_frequency_matrix = lh.get_distribution();
  plDistribution contingency_frequency_matrix = lh.get_distribution();

  ///*************************Finding all possible combinations***************************

  int cardA = max_val.at(test_var.size()-1) - min_val.at(test_var.size()-1)+1;
  int cardB = max_val.at(test_var.size()-2) - min_val.at(test_var.size()-2)+1;

  int num_table_values =  cardA * cardB;  // to declare table of r * c
  std::vector < vector < int > > contingencyTable (num_table_values + 1, vector< int > ( num_table_values + 1));  // index start from 1 rather than o
  int a = -1, b = -1, rowcounter = 0;
  std::vector <double> chi_sq_value;

  vals.reset();
  do {
    for (unsigned int j = 0; j < test_var.size(); j++) {
      vals[j] = (int)vals[j];

      if (j == (test_var.size()-2))  // variable B
        b = (int)vals[j];
      else if (j == (test_var.size()-1))  // variable A
        a = (int)vals[j];
    }
    contingencyTable[a][b] = (contingency_frequency_matrix.compute(vals)) * test_var[0].size();

    // If there is conditional variable then divide the contingency table into multi dimension.
    rowcounter++;
    if (rowcounter >= num_table_values || (rowcounter == num_table_values && test_var.size() == 2)) {
      // function call for chi2 calculations
      chi_sq_calculations(& contingencyTable, cardA, cardB, & chi_sq_value);
      rowcounter = 0;  // restart counter for next dimension
    }

  } while (vals.next());

  for (unsigned int i=0; i < chi_sq_value.size(); i++) {
    chi2 += chi_sq_value[i];
  }

  // function call for chi square distribution
  chi_squared_dist(chi2, vars, min_val, max_val, alpha, p, &ind, df);

  return ind;
}  // end of function

void chiSequare::chi_sq_calculations(std::vector < vector < int > >* observed_values_table, int cardA, int cardB, std::vector <double>* chi_sq_value) {
  vector <int> marginal_sum_A(cardA+1, 0), marginal_sum_B(cardB+1, 0);  // marginal total, index starting from "o" to maximum cardinality - 1.
  int num_table_values = cardA * cardB;  // to declare table of r * c
  vector < vector < double > > expected_values_Table (num_table_values + 1, vector< double > (num_table_values + 1));  // index start from 1 rather than o
  double sample_size = 0;

  //*********** marginal sum of variable A**************
  for (int i = 1; i <= cardA; i++) {
    for (int j = 1; j <= cardB; j++) {
      marginal_sum_A[i] = marginal_sum_A[i] + (*observed_values_table)[i][j];
    }
    sample_size += marginal_sum_A[i];
  }
  //*********** marginal sum of variable B **************
  for (int i = 1; i <= cardA; i++) {
    for (int j = 1; j <= cardB; j++) {
      marginal_sum_B[j] = marginal_sum_B[j] + (*observed_values_table)[i][j];
    }
  }

  //************ expected and chi2 values calculation *************
  double chi = 0 ;

  for (int i = 1; i <= cardA; i++) {
    for (int j = 1; j <= cardB; j++) {
      if (sample_size)
        expected_values_Table[i][j] = (double)( marginal_sum_A[i] * marginal_sum_B[j] ) / sample_size ;
      else
        expected_values_Table[i][j] = 0;

      if (expected_values_Table[i][j] > 5)
        chi += pow(((*observed_values_table)[i][j] - expected_values_Table[i][j]),2) / expected_values_Table[i][j];
      else if (expected_values_Table[i][j] == 0)
        chi += 0;
      else
        chi += pow( (abs((*observed_values_table)[i][j] - expected_values_Table[i][j]) - 0.5),2) / expected_values_Table[i][j];
    }
  }
  chi_sq_value->push_back(chi);
}


void chiSequare::chi_squared_dist(double chi2_value, std::vector <int> vars, std::vector< plFloat > min_val, std::vector< plFloat > max_val, double alpha, long double _p, bool *ind, unsigned int df) {
  using boost::math::chi_squared;
  using boost::math::cdf;

  chi_squared dist(df);
  double t_stat= chi2_value;
  _p = 1 - cdf(dist, t_stat);

  p = _p;

  // Determine if there is an association or not.
  if ( p > alpha)
    *ind = true;
  else
    *ind = false;

}  // end of chi sq dist function

long double chiSequare::getPValue() {
  return this->p;
}

double chiSequare::getChi2() {
  return this->chi2;
}

unsigned int chiSequare::getDF() {
  return this->df;
}
