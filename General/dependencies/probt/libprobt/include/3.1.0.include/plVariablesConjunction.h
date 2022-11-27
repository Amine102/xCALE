/*=============================================================================
 * Product        : ProBT 
 * File           : plVariablesConjunction.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Apr 10 10:59:32 2002
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


#ifndef plVariablesConjunction_h
#define plVariablesConjunction_h

#include "plConfig.h"

#include <list>
#include <vector>
#include <memory>
#include <set>
#include <cstddef>

#include <plObject.h>
#include <plMath.h>
#include <plVariableType.h>
#include <plType.h>

/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplVariable;
typedef std::list<kplVariable*> kplVariableList;
class plVariable;
class plDataDescriptor;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

class plVariablesConjunction;

#if !defined(SWIG) || !defined(SWIGPYTHON)
/*
	\em plVariablesConjunction const iterator.
 */
class PL_DLL_API plVariablesConjunction_const_iterator
{
public:
    typedef plVariablesConjunction_const_iterator  Self_;
    typedef kplVariableList::const_iterator node_type;

    typedef ptrdiff_t                     difference_type;
    typedef std::bidirectional_iterator_tag    iterator_category;
    typedef plVariable                      value_type;

    plVariablesConjunction_const_iterator()
	: node_() { };

    plVariablesConjunction_const_iterator(node_type node)
	: node_(node) { };

    virtual ~plVariablesConjunction_const_iterator();

    value_type operator*() const;

    PL_UNIQUE_PTR<value_type> operator->() const;

    /// x.deref() === *x
    value_type deref() const;

    Self_& operator++();

    /// x.incr() === ++x
    Self_& incr();

    Self_ operator++(int);

    Self_& operator--();

    Self_ operator--(int);

    bool operator==(const Self_& other) const;

    bool operator!=(const Self_& other) const;

    Self_ operator+(int n);

private:
    node_type node_;
};
#endif //SWIGPYTHON

//=============================================================================
SPL_DECL2(plVariablesConjunction);

typedef std::set<kplVariable const *> kplVariableSet;
typedef std::vector<kplVariable *> kplModVariableVector;

/** The \em plVariablesConjunction class implements the conjunction of a
    set of variables. */
class PL_DLL_API plVariablesConjunction : public plSampleSpaceObject
{
protected:
  /** Size of the variable list (variable_list), faster that the acces 
      to size() function */
  size_t Dsize_;

  /** List of the function arguments variables */
  kplVariableList variable_list_;

  /** Mirrors variable_list_. variable_set_ may be empty, use variable_set()
      to make sure it is valid and get it. Used to speed up operator^=().
  */
  mutable kplVariableSet variable_set_;
  kplVariableSet & variable_set() const;

  /** Mirrors variable_list_. variable_vector_ may be empty, use variable_vector()
      to make sure it is valid and get it. Used to speed up operator[](size_t).
   */
  mutable kplModVariableVector variable_vector_;
  kplModVariableVector & variable_vector() const;

  /** Constructor using a  list of kplVariables */
  explicit plVariablesConjunction(const kplVariableList &kernel_variable_list);

  /** Constructor using one kplVariable */
  explicit plVariablesConjunction(kplVariable* kernel_variable);

  /** clean up the variable list */
  void cleanup();

  void set(const kplVariableList &kernel_variable_list);

  /** Writes the variable at the output given by "out" */
  virtual void Output(std::ostream &out) const;  

  static const plVariablesConjunction empty_;
  
public:
  /** Associated const_iterator. **/
  typedef plVariablesConjunction_const_iterator const_iterator;
  
  /* Needed by the Python bindings, but not part of the user API.  Do not
     use. 
  */
  inline const kplVariableList& get_variable_list() const
  { return variable_list_; }

  /** Default constructor, creates an empty variable conjuction */
  plVariablesConjunction();

  /** Constructor using a set of plVariables 
      
      Assumes that each plVariable is found only once.
  */
  explicit plVariablesConjunction(const std::vector<plVariable> &variables);

  /** Constructor using begin and end iterators */
  plVariablesConjunction(plVariablesConjunction::const_iterator b, plVariablesConjunction::const_iterator e);

  /** Destructor, if the variable represents a list erase the list but
      not the variables on the list. If empty variable, erase the
      empty variable */
  virtual ~plVariablesConjunction();

#ifdef PL_HAVE_CPP11
  /** Move constructor */
  plVariablesConjunction(plVariablesConjunction&& tmp);
    
  /** Move assignment */
  plVariablesConjunction& operator=(plVariablesConjunction&& tmp);
#endif
    
  /** Return the concatenation of the variable \em var (at right side of the
      operator) with the variable at left side. If a variable is found twice,
      only the first occurrence will be kept and a warning 62 will be raised.
  */
  plVariablesConjunction operator^(const plVariablesConjunction &variable) const;

  /** Return the concatenation of the variable \em var (at right side of the
      operator) with the variable at left side. If a variable is found twice,
      only the first occurrence will be kept and a warning 62 will be raised.
  */
  plVariablesConjunction concatenate(const plVariablesConjunction &variable,
                                     bool warn_on_duplicated=true) const;
  
  /** Returns the number of values taken by the variable
      conjunction. For example, if a variable 'x' takes 10 possible
      values and another variable 'y' takes 3 possible values, then
      the conjunction x^y takes 30 (10 x 3) possible values.  It
      returns zero if the variable is not discretized (ie,
      non-discretized plRealType)
  */
  long double cardinality()const;

  /** Returns true if the conjunction contains only discretizable variables
      (ie, all types excluding non-discretized plRealType), false
      otherwise. */
  bool is_discretized() const;

  /** Returns true if the conjunction contains only discrete variables, false otherwise. */
  bool is_discrete() const;

  /** Returns true if the conjunction contains only continuous variables
      (ie, plRealType or plContinuousIntervalType), false otherwise. */
  bool is_continuous() const;

  /** Returns the number of variables in the conjunction */
  inline size_t dim() const { return Dsize_; }

  /** Returns the number of variables in the conjunction */
  inline size_t size() const { return Dsize_; }

  /** Returns true if the conjunction is empty. */
  inline bool is_empty() const { return (Dsize_==0); }
  
  /** Returns the variable's type as a 'plVariableType' value, where
      plVariableType is defined as an enum:   
      enum {PL_INTEGER, PL_REAL, PL_CONINUOUS_INTERVAL, 
      PL_DISCRETE_INTERVAL, PL_LABEL, PL_UNKNOWN_TYPE}
      
      In the case of a multi-dimensional conjunction, the type of the
      first variable is returned.
  */
  plVariableType get_var_type() const;

  /** Returns the variable's type as a 'plType' object. In the case of a
      conjunction, the type of the first variable is returned. The returned
      object can in turn be converted into the appropriate derived class
      (plIntegerType, plRealType, plContinuousIntervalType, ...) so you can
      use its specialized methods and query its characteristics. In order to
      know into which class the object has to be converted, use the
      get_var_type() method.
  */
  plType get_type() const;

  /** Gets the variable name, given by the user.
      In the case of a conjunction, the name of the first variable is returned. 
  */
  virtual std::string name() const;

  /** Get the names of all the variables in the conjunction. */
  std::vector<std::string> get_names() const;

  /** 
      Returns a const_iterator to the first variable in this conjunction.
  */
  const_iterator begin() const
  { return const_iterator(variable_list_.begin()); }

  /** 
      Returns a const_iterator pointing one item after the last variable
      in this conjunction.
   */
  const_iterator end() const
  { return const_iterator(variable_list_.end()); }

  /** 
      Tests if this conjunction is a superset of 'other'.
      (a ^ b).contains(a) == true
      (a ^ b).contains(b) == true
      (a ^ b).contains(c) == false
      (a ^ b ^ c).contains(a ^ b) == true
      (a ^ b ^ c).contains(a ^ b ^ d) == false
   */
  bool contains(const plVariablesConjunction& other) const;
  
  /**
     Tests if this conjunction has the same variables as 'other'.
     (a ^ b).has_same_variables_as(b ^ a) == true;
     (a ^ b).has_same_variables_as(b ^ a ^ c) == false;
     (a ^ b).has_same_variables_as(b) == false;
  */
  bool has_same_variables_as(const plVariablesConjunction& other)const;

  /** Test intersection with another conjunction */
  bool intersects(const plVariablesConjunction& other)const;

  /** Get the intersection with another conjunction */
  plVariablesConjunction get_intersection(const plVariablesConjunction& other)const;

  /** Assignment operator */
  plVariablesConjunction& operator=(const plVariablesConjunction& other);

  /** Same as operator=() */
  plVariablesConjunction& assign_from(const plVariablesConjunction& other)
  {
    return operator=(other);
  }

  /** Copy constructor*/
  plVariablesConjunction(const plVariablesConjunction& other);

  /** Convert the plVariablesConjunction into a vector of plVariable objects. */
  void get_all_variables(std::vector<plVariable> &variables) const;
  /** Same as get_all_variables() but returns the vector by value */
  std::vector<plVariable> as_variable_vector() const;

  /** Get the n^th variable of the conjunction. */
  plVariable operator[](size_t n) const;

  /** Get the n^th variable of the conjunction. */
  plVariable at(size_t n) const;

  /** Return true if this and the other object are the same. */
  bool operator==(const plVariablesConjunction& other) const;

  /** Return true if this and the other object are the same. */
  bool equal(const plVariablesConjunction& other) const
  {
    return operator==(other);
  }


  /** Return true if this and the other object are not the same. */
  inline bool operator!=(const plVariablesConjunction& other) const
  { return ! this->operator==(other); }

  /** Return true if this and the other object are not the same. */
  bool not_equal(const plVariablesConjunction& other) const
  {
    return operator!=(other);
  }

  /** Return the empty variable conjunction. */
  static const plVariablesConjunction& empty();

  /** Return the position of a given variable in the conjunction.
      Return -1 if the variable is not found.
  */
  int get_variable_position(const plVariable& variable)const;

  /** Return the position of a given variable in the conjunction.
      Return -1 if the variable is not found.
  */
  int get_variable_position(const std::string& name)const;

  /** Replace the variable with name 'name' with a new variable 
      having the same name and the new provided type 
   */
  plVariablesConjunction replace_type(const std::string &name, const plType &new_type);
  
  /**  In place concatenation of the variable \em var (at right side of the
       operator) with the variable at left side.  If a variable is found twice,
      only the first occurrence will be kept and a warning 62 will be raised.

      WARNING: This operator has effect only on base plVariablesConjunction
      objects. For all subclasses this operator does not have any effect
      (e.g. You can't use a plVariable at the left side of the operator).
  */
  plVariablesConjunction &operator^=(const plVariablesConjunction &var);

  /**  In place concatenation of the variable \em var (at right side of the
       operator) with the variable at left side.  If a variable is found twice,
       only the first occurrence will be kept and a warning 62 will be raised.
       
      WARNING: This operator has effect only on base plVariablesConjunction
      objects. For all subclasses this operator does not have any effect
      (e.g. You can't use a plVariable at the left side of the operator).
  */
  plVariablesConjunction &push_back(const plVariablesConjunction &var,
                                    bool warn_duplicated=true);

  /** Remove one variable from the variable conjunction and return a reference to the conjunction */
  plVariablesConjunction& remove(const plVariable& variable);

  /** Remove one variable from the variable conjunction and return a reference to the conjunction 
   */
  plVariablesConjunction& remove(const std::string& variable_name);

  /** Clear the conjunction */
  void clear()
  {
    cleanup();
  }

  /** Returns the first variable having @em name as print name. 
      Raises an exception if no variable with this print name is found.
  */
  plVariable get_variable_with_name(const std::string &name)const;


  /** Returns the first variable having @em names.

      The returned conjunction keeps the order of this conjunction
      Example: {Z, Y, X}.get_variables_with_name( ["X", "Y"] ) will return
      the {Y, X} conjunction
  */
  plVariablesConjunction
    get_variables_with_names(const std::vector<std::string> &names)const;
  
  /** Difference operator */
  plVariablesConjunction operator-(const plVariablesConjunction& other)const;

  /** Difference */
  plVariablesConjunction subtract(const plVariablesConjunction& other)const
  {
    return operator-(other);
  }

  /** Get a sorted-by-name copy of the conjunction */
  plVariablesConjunction sort_by_name() const;

  /** Get the discrete variables of the conjunction */
  plVariablesConjunction  get_discrete_variables() const;

  /** "Less than" operator */
  bool operator<(const plVariablesConjunction& other) const;

  /** "Less than"  */
  bool less(const plVariablesConjunction& other) const
  {
    return operator<(other);
  }

  /** Discretize a subset of variables using plEqualFrequenciesVariableDiscretizer and replace them in
      the returned new conjunction. The other variables are returned unchanged 
  */
  plVariablesConjunction discretize_and_replace(const std::vector<std::string> &variables_to_replace,
                                                const std::vector<unsigned int> &discretization_bins,
                                                plDataDescriptor &data) const;

  /** Create a variable conjunction from variable names, types (continuous or integer), and min/max values.

      \param[in] variable_names The names of the variables

      \param[in] variable_is_continuous For each variable in the
      list above, True if the variable is continuous, False otherwise

      \param[in] variable_min For each variable in the list above,
      the min value for the range (inclusive)

      \param[in] variable_max For each variable in the list above,
      the max value for the range (inclusive for discrete variables and
      exclusive for continuous ones)
  */
  static plVariablesConjunction create_variables(const std::vector<std::string> &variable_names,
                                                 const std::vector<bool> &variable_is_continuous,
                                                 const std::vector<plFloat> &variable_min,
                                                 const std::vector<plFloat> &variable_max);
    
  
  friend class plData;
  friend class plVariableCollection;
  friend class plCndDistribution;
  friend class plCndNormal;
  friend class plCndLogNormal;
  friend class plCndUnknown;
  friend class plComputableObject;
  friend class plDeterministic;
  friend class plCndDeterministic;
  friend class plGamma; 
  friend class plJointDistribution;
  friend class plKalmanFilter;
  friend class plDistribution;
  friend class plDistributionTable;
  friend class plNormal;
  friend class plPoisson;
  friend class plProbTable;
  friend class plVariable;
  friend class plUniform;
  friend class plUnknown;
  friend class plValues;
  friend class plLogNormal;
  friend class plVariablesConjunction_const_iterator;
  friend class plExponential;
  friend class plBeta;
  friend class plBinomial;
  friend class plExternalFunction;
  friend class plExternalProbFunction;
  friend class plVariableIndexer;
  friend class plLearnSparseHistogram;
  friend class plLearnObject;
  friend class plLearnDistributions;
  friend class plDirichlet;
  friend class plBayesLearnProbTable;
  friend class plLearnNdNormal;
  friend class plCndAnonymousDistribution;
  friend class plBayesianFilter;
  friend class plAnonymousDistribution;
  friend class plIneqConstraint;
  friend class plExternalFunctionFromC;
  friend class plExternalProbFunctionFromC;
  friend class plLearnHistogram;
  friend class plHMM;
  friend class plCndUniform; 
  friend class plCndPoisson;
  friend class plCndBinomial;
  friend class plJtDistribution;
  friend class plCndJtDistribution;
  friend class plJunctionTree_impl;
  friend class plParticleFilter;
  friend class plLearnDistributionTable;
  friend class plEMLearner;
  friend class plWeibull;
  friend class plComputableObjectList;
  friend class plCndBeta;
  friend class plCndExponential;
  friend class plCndGamma;
  friend class plCndWeibull;
  friend class plVonMises;
  friend class plCndVonMises;
  friend class kplDescription;
  template <class T> friend class plCndLearnObject;

  SPL_ACCESS2(plVariablesConjunction);
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
