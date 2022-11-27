/*=============================================================================
 * Product        : 
 * File           : indepTest.h
 * Author         : 
 * Creation       : 
 
  *=============================================================================
 *        (c) Copyright 2013, PILGRIM - all rights reserved
 *=============================================================================
 */


#ifndef _INDEPTEST_H
#define _INDEPTEST_H

#include <pl.h>
#include <math.h>
#include <vector>

#include <boost/math/distributions/chi_squared.hpp>
#include "pilgrim/general/pmCommonTypeDefinitions.h"

typedef plDataDescriptor/*<pmMyRowDataType>*/ pmDataSet;

using namespace std;

namespace PILGRIM{

//chi2 class
class chiSequare
{
public:
	/**! Constructor. **/
	chiSequare(pmDataSet * const _pCompDataSet, plVariablesConjunction *_variables = 0,
				float *_alpha = 0, double *_chi2 = 0, double *_p = 0);

	/**! Run the chi2 test. **/
	bool chi_square_test(int, int, std::vector <int>);

	/**! Compute the observed Chi2 value. **/
	void chi_sq_calculations( std::vector < vector < int > >*, int , int , std::vector <double>*);

	/**! Compute the p-value and tell if H0 is accepted of rejected. **/
	void chi_squared_dist(double , std::vector <int> , std::vector< plFloat > , std::vector< plFloat >, double , long double, bool *, unsigned int );

	/**! Get the p-value. **/
	long double getPValue();

	/**! Get the observed Chi2 value. **/
	double getChi2();

	/**! Get the degree of freedom. **/
	unsigned int getDF();

protected:
	float alpha;
	double chi2;
	long double p;
	unsigned int  df;
	pmDataSet *pCompDataSet;
	plVariablesConjunction* variables;
};

	};
#endif //
