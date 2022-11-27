
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
//#include <crtdbg.h>

#include <pl.h>
#include <iostream>
#include <math.h>

#include <boost/math/distributions/chi_squared.hpp>
#include <boost/math/distributions/normal.hpp>

#include "pilgrim/general/scores/pmMI.h"
#include "pilgrim/general/pmSlidingWindow.h"

using namespace std;
using namespace boost;

using namespace PILGRIM;

mutualInformation::mutualInformation(pmCSVDataSet *const _pCompDataSet, plVariablesConjunction *_variables, bool _usingSlidingWindow,
                                     float *_alpha, float *_p)
{
  if (_alpha != NULL)
  {
    alpha = *_alpha;
  }
  else
  {
    alpha = 0.05;
  }

  if (_p != NULL)
  {
    p = *_p;
  }
  else
  {
    p = 0.05;
  }

  variables = *_variables;
  pCompDataSet = _pCompDataSet;
  numOfRecords = pCompDataSet->get_n_records();
  usingSlidingWindow = _usingSlidingWindow;
  if (usingSlidingWindow)
  {
    numOfRecords = pmSlidingWindow::N + numOfRecords; //Total_records = Numberofrecord seen so far + records in new window
  }
}

bool mutualInformation::computeMI(const unsigned int &var_a, const unsigned int &var_b)
{
  std::vector<unsigned int> vars;
  return computeMI_SW(var_a, var_b, vars); // we want to use compute_SW() for both cases
}

bool mutualInformation::computeMI(const unsigned int &var_a, const unsigned int &var_b, std::vector<unsigned int> vars)
{
  return computeMI_SW(var_a, var_b, vars); // we want to use compute_SW() for both cases

  // bool ind;
  // df = 1;

  // ///****************input from user for indep test******************
  // // vars contains indices of variables in the CSV file, but 1-based
  // vars.push_back(var_b);
  // vars.push_back(var_a);

  // // check if degree of freedom is already calculated
  // if (this->df < 2)
  //   this->computeDegreeOfFreedom(vars);

  // if (this->df == 0)
  // {
  //   ind = true;
  //   p = 0;
  //   MI = 0;
  //   return ind;
  // }

  // //==================ProBT MI function ===================
  // try
  // {
  //   plEdgeScoreMI /*<rowDataType>*/ mutInfo(*(pCompDataSet->get_data_descriptor()));
  //   MI = 0;

  //   unsigned int v1, v2;

  //   if (vars.size() > 2)
  //   { // to check if there are conditional variables
  //     unsigned int varSize = vars.size();
  //     v2 = vars.at(varSize - 1); // to extract last two variables A and B
  //     v1 = vars.at(varSize - 2);
  //     plVariablesConjunction cond_variables;
  //     // Put cond variables in plVariableconjunction one by one
  //     for (size_t i = 0; i < vars.size() - 2; ++i)
  //     {
  //       cond_variables = cond_variables ^ variables[vars[i]];
  //     }
  //     MI = mutInfo(variables[v1], variables[v2], cond_variables);
  //   }
  //   else
  //   { // if there is no conditional variable
  //     v2 = vars.at(0);
  //     v1 = vars.at(1);
  //     MI = mutInfo(variables[v1], variables[v2]);
  //   }
  // }
  // catch (std::exception &e)
  // {
  //   cout << "\n error in calling ProBT MI function ";
  //   std::cerr << e.what() << endl;
  //   std::system("pause");
  //   cout << "stop!! " << endl;
  // }

  // //==========================================================
  // // function call for chi square distribution
  // this->p = this->chi_square_dist(ind, df);

  // return ind;
} // end of function

float mutualInformation::chi_square_dist(bool &ind, unsigned int &df)
{
  if (df < 1)
    cout << "Error! please check degree of freedom, it will generate boost error:  df = " << df;

  boost::math::chi_squared dist(df);
  using boost::math::cdf;

  if (MI < 0) // to avoid negative values due to computation precision
    MI = 0;

  float MI_value = MI * 2 * numOfRecords;

  float pV = 0;

  try
  {
    pV = 1 - cdf(dist, MI_value);
  }
  catch (std::exception &e)
  {
    cout << "\n Error in calling Boost chi2 distribution function ";
    std::cerr << e.what() << endl;
    std::system("pause");
    cout << "stop!! " << endl;
  }

  Quantile_value = quantile(dist, 1 - alpha);

  // Determine if there is an association or not.
  if (pV > alpha)
    ind = true;
  else
    ind = false;

  return pV;
}

float mutualInformation::getMI()
{
  return this->MI;
}

float mutualInformation::getPValue()
{
  return this->p;
}

unsigned int mutualInformation::getDF()
{
  return this->df;
}

void mutualInformation::computeDegreeOfFreedom(std::vector<unsigned int> &vars)
{
  this->df = 1;
  unsigned int counter = 0;
  std::vector<unsigned int>::reverse_iterator revItr;

  for (revItr = vars.rbegin(); revItr != vars.rend(); ++revItr)
  {
    counter++;
    if (counter <= 2)
    {
      this->df *= variables[*revItr].cardinality() - 1;
    }
    else
    {
      this->df *= variables[*revItr].cardinality();
    }
  }
}

bool mutualInformation::isDataSufficientToProceedIndTest(const unsigned int &var_a, const unsigned int &var_b, std::vector<unsigned int> vars)
{
  vars.push_back(var_b);
  vars.push_back(var_a);
  this->computeDegreeOfFreedom(vars);

  if (numOfRecords >= (10 * this->df))
    return true;
  else
    return false;
}

float mutualInformation::getQuantileValue()
{
  return this->Quantile_value;
}

bool mutualInformation::computeMI_SW(const unsigned int &var_a, const unsigned int &var_b)
{
  std::vector<unsigned int> vars;
  return computeMI_SW(var_a, var_b, vars);
}

bool mutualInformation::computeMI_SW(const unsigned int &var_a, const unsigned int &var_b, std::vector<unsigned int> vars)
{
  std::ofstream log_file(
      "log_file.txt", std::ios_base::out | std::ios_base::app);
  log_file<<"ComputeMI_SW() called"<<endl;
  bool ind;
  df = 1;
  MI = 0;
  std::vector<plFloat> Nab;

  unsigned int cardA = variables[var_a].cardinality();
  unsigned int cardB = variables[var_b].cardinality();

  unsigned int N;

  N = numOfRecords;

  plVariablesConjunction cond_variables;
  // Put cond variables in plVariableconjunction one by one
  for (size_t i = 0; i < vars.size(); ++i)
  {
    cond_variables = cond_variables ^ variables[vars[i]];
  }
  unsigned int cardC = cond_variables.cardinality();

  // vars contains indices of variables
  vars.push_back(var_b);
  vars.push_back(var_a);

  // check if degree of freedom is already calculated
  if (this->df < 2)
    this->computeDegreeOfFreedom(vars);

  if (this->df == 0)
  {
    ind = true;
    p = 0;
    MI = 0;
    return ind;
  }

  reverse(vars.begin(), vars.end());
  // To compare MI value with probt MI()
  log_file<<"FrequencyCount() called"<<endl;
  this->frequencyCount(vars, Nab);
  if (vars.size() > 2)
  { // to check if there are conditional variables
    MI = this->compute_cnd_mi(Nab, cardA, cardB, cardC, N);

    // To compare MI value with probt MI()
  }
  else
  { // if there is no conditional variable
    MI = this->compute_mi(Nab, cardA, cardB, N);
    // To compare MI value with probt MI()
  }

  // function call for chi square distribution
  this->p = this->chi_square_dist(ind, df);

  return ind;
}

plFloat mutualInformation::compute_mi(const std::vector<plFloat> &Nab,
                                      unsigned int cardA, unsigned int cardB,
                                      unsigned int N)
{
  // MI(A,B) = log(N) + (term1 - term2 - term3) / N
  // term1 = Sum_ab Nab log(Nab)
  // term2 = Sum_ab Nab log(Na) = Sum_a Na log(Na)
  // term3 = Sum_ab Nab log(Nb) = Sum_b Nb log(Nb)

  plFloat term1 = PL_ZERO, term2 = PL_ZERO, term3 = PL_ZERO;
  for (unsigned int i = 0; i < cardA; ++i)
  {
    plFloat Na = PL_ZERO;
    for (unsigned int j = 0; j < cardB; ++j)
    {
      const plFloat &cnt = Nab[i * cardB + j];
      Na += cnt;
      if (cnt != PL_ZERO)
        term1 += cnt * std::log(cnt);
    }
    if (Na != PL_ZERO)
      term2 += Na * std::log(Na);
  } // end for

  for (unsigned int j = 0; j < cardB; ++j)
  {
    plFloat Nb = PL_ZERO;
    for (unsigned int i = 0; i < cardA; ++i)
    {
      Nb += Nab[i * cardB + j];
    }
    if (Nb != PL_ZERO)
      term3 += Nb * std::log(Nb);
  }

  const plFloat score = std::log((float)N) + (term1 - term2 - term3) / N;

  return std::max(score, PL_ZERO);
}

//===============================================================================================
plFloat mutualInformation::compute_cnd_mi(const std::vector<plFloat> &Nabc,
                                          unsigned int cardA, unsigned int cardB, unsigned int cardC,
                                          unsigned int N)
{
  // CMI = (term1 + term2 - term3 - term4) / N
  // term1 = Sum_abc Nabc log(Nc) = Sum_c Nc log(Nc)
  // term2 = Sum_abc Nabc log(Nabc)
  // term3 = Sum_abc Nabc log(Nac) = Sum_ac Nac log(Nac)
  // term4 = Sum_abc Nabc log(Nbc) = Sum_bc Nbc log(Nbc)

  plFloat term1 = PL_ZERO, term2 = PL_ZERO, term3 = PL_ZERO, term4 = PL_ZERO;

  for (unsigned int k = 0; k < cardC; ++k)
  {
    plFloat Nc = PL_ZERO;
    for (unsigned int j = 0; j < cardB; ++j)
    {
      plFloat Nbc = PL_ZERO;
      for (unsigned int i = 0; i < cardA; ++i)
      {
        const plFloat &cnt = Nabc[(i * cardB + j) * cardC + k];
        if (cnt != PL_ZERO)
          term2 += cnt * std::log(cnt);
        Nbc += cnt;
      }
      if (Nbc != PL_ZERO)
        term4 += Nbc * std::log(Nbc);
      Nc += Nbc;
    }
    if (Nc != PL_ZERO)
      term1 += Nc * std::log(Nc);

    for (unsigned int i = 0; i < cardA; ++i)
    {
      plFloat Nac = PL_ZERO;
      for (unsigned int j = 0; j < cardB; ++j)
      {
        const plFloat &cnt = Nabc[(i * cardB + j) * cardC + k];
        Nac += cnt;
      }
      if (Nac != PL_ZERO)
        term3 += Nac * std::log(Nac);
    }
  }
  const plFloat score = (term1 + term2 - term3 - term4) / N;
  return std::max(score, PL_ZERO);
}

void mutualInformation::frequencyCount(const std::vector<unsigned int> &SearchedVariablesIndices, vector<plFloat> &freq)
{
  pmFrequencyCounter *fq = new pmFrequencyCounter(pCompDataSet->get_data_descriptor(), variables);
  fq->frequencyCount(SearchedVariablesIndices, freq);
  delete fq;
}
