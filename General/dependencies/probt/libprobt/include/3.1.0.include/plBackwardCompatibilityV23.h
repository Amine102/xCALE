/*=============================================================================
 * Product        : ProBT 
 * File           : plBackwardCompatibilityV23.h
 * Author         : Kamel Mekhnacha
 * Creation       : June 2015
 *
 *=============================================================================
 *        (c) Copyright 2015, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plBackwardCompatibilityV23_h
#define plBackwardCompatibilityV23_h

typedef plDeterministic plDirac;
typedef plCndDeterministic plFunctionalDirac;
typedef plContinuousIntervalType plIntervalType; 

#define inverse inverse_using_greville
#define diagonalize compute_eigen_decomposition
#define set_verbose_sl set_verbose_structure_learning
#define add_point_filter add_point_if_not_frozen

typedef plVariable plSymbol;
#define has_same_symbols_as has_same_variables_as
#define get_symbol_with_name get_variable_with_name
#define get_symbol_position get_variable_position
#define get_discrete_symbols get_discrete_variables

typedef plVariableCollection plArray;

typedef plLearnDistributions plLearnDistribVector;

#endif // plBackwardCompatibilityV23_h


// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
