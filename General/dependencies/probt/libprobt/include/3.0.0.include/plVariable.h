/*=============================================================================
 * Product        : ProBT 
 * File           : plVariable.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 11:49:21 2001
 *
 *=============================================================================
 *     (c) Copyright 2000, Centre National de la Recherche Scientifique,
 *                              all rights reserved
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plVariable_h
#define plVariable_h

#include "plConfig.h"

#include <plVariablesConjunction.h>
#include <plType.h>


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//============================================================================
/** 
    A \em plVariable is a set containing one and only one unidimensional
    variable.
*/
class PL_DLL_API plVariable : public plVariablesConjunction
{
 public:
  
  /** Constructor from a \em print_name as a \em string, selected by the
      user, and a previously defined \em variable_type. */
  plVariable(const std::string &print_name, 
             const plType &variable_type);
  
  /** Constructor from a \em print_name as a \em string, selected by the
      user, and a previously defined \em variable. */
  plVariable(const std::string &print_name, 
             const plVariable & variable);

  /** Default empty plVariable constructor. */
  plVariable();
  

  /** Destructor, erase the variable. */
  virtual ~plVariable();

  /** Promote from a plVariablesConjunction. If the object being copied
      contains more than one variable, an exception is raised. */
  explicit plVariable(const plVariablesConjunction&);


#ifdef PL_GLOBAL_VARIABLE_TABLE
  /** Implicit conversion of a name string to a plVariable. Returns an existing
      plVariable (the last created one) having \em name as name if any and throws
      an exception if no plVariable with this name exists.
  */
  plVariable(const std::string &name);

  plVariable(const char *name);

  /** Returns an existing plVariable (the last created one) having \em name as
      name if any and throws an exception if no plVariable with this name exists.
  */
  static plVariable get_variable_with_name(const std::string &name);
  
#endif // PL_GLOBAL_VARIABLE_TABLE

 protected:

  explicit plVariable(kplVariable* kpl_variable);

#ifdef PL_GLOBAL_VARIABLE_TABLE
  void construct_from_name(const char *name);
#endif // PL_GLOBAL_VARIABLE_TABLE

  friend class plVariableCollection;
  friend class plVariablesConjunction;
  friend class plVariablesConjunction_const_iterator;
  friend class plCndLogNormal;
  friend class plCndUniform;
  friend class plCndBeta;
  friend class plCndExponential;
  friend class plCndGamma;
  friend class plCndWeibull;
  friend class plCndVonMises;
  friend bool PL_DLL_API operator<(plVariable const & a, plVariable const & b);
};

/** Defines a total, stable order over plVariables, suitable for putting
    plVariables into a std::map or std::set. The order is arbitrary, and is
    not guaranteed to be related to cardinalities or variable names.
*/
bool PL_DLL_API operator<(plVariable const & a, plVariable const & b);

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
