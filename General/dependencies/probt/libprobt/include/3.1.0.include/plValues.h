/*=============================================================================
 * Product        : ProBT 
 * File           : plValues.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Wed Mar 21 12:30:16 2001
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

#ifndef plValues_h
#define plValues_h

#include "plConfig.h"

#include <map>

#include <plData.h>
#include <plDataValues.h>
#include <plObject.h>
#include <plVariablesConjunction.h>
#include <plDLLExport.h>

#include <cassert>

SPL_DECL2(plValues);



/*****************************************************************************
 *                           External Classes
 *****************************************************************************/

class kplVariable;

/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/


//=======================================================================
/** A plValues is an object storing the values of a set of variables. */
class PL_DLL_API plValues : public plSampleSpaceObject
{
  friend class plValuesAccess;
 protected:
  SPL_ACCESS2(plValues);

  /* The list of variables for which the values are stored */
  kplVariableList variable_list_;

  typedef std::map <kplVariable const *, int> t_map_vector;
  /* A map, mapping each variable into a position for recovering its value */
  t_map_vector map_vector_;

  /* An array of values assosiated to each of the variables */
  plDataValues *values_;

  /* A boolean stating whether the container is responsible for \em value_ or not.
     This is needed because \em values_ may be set by pointer.
     \sa set_values_address
  */
  bool values_to_be_deleted_;

 
 private:
  const plData& value_for_variable(kplVariable const * variable) const;
  plData& value_for_variable(kplVariable const * variable);
  void destroy();
  void finalise_construction();
  
 public:

  /** Default constructor */
  plValues();
  
  /** Copy constructor */
  plValues(const plValues& val);
  
  /** Create a plValues allowing to store the values of the variable
      conjunction \em variable and initialize its value to the first value of
      the conjunction
  */
  explicit plValues (const plVariablesConjunction &variables);

  /* Internal method: Create a plValues allowing to store the value of the
     variable conjunction \em vars and initialize its value to the first value
     of the conjunction
  */
  explicit plValues (const kplVariableList &vars);

  /* Internal method: Create a plValues allowing to store the values of the
     variable conjunction \em vars and initialize its value to \em val
  */
  plValues (const kplVariableList &vars, const plDataValues &val);

#ifdef PL_HAVE_CPP11
  /** Move constructor */
  plValues(plValues&& tmp);
  
  /** Move assignment */
  plValues& operator=(plValues&& tmp);
#endif

  /** Destructor */
  ~plValues();

  /** Return true if default-constructed. */
  inline bool is_empty() const { return (0 == values_); }

  /** Return \em true iff the object is fully initialized (non-empty) or the
      current values are compatible with the types of the variable
      conjunction. */
  bool is_valid() const;

  /** Return \em true iff the object is fully initialized (non-empty) or the
      current values are compatible with the types of the variable conjunction
      for which \em check_variable is \em true.
  */
  bool is_valid(const std::vector<bool> &check_variable) const;

  /** Get a const reference to the value of a given variable */
  const plData& operator[](const plVariable &variable) const;

  /** Get a reference to the value of a given variable */
  plData& operator[](const plVariable &variable);

  /** Get a const reference to the value of the variable at position \em
      position
  */
  plData& operator[](size_t position);

  /** Get a reference to the value of the variable at position \em
      position
  */
  const plData& operator[](size_t position) const;

  /** Get a const reference to the value of the variable with the name \em
      name
  */
  plData& operator[](const std::string &name);

  /** Get a reference to the value of the variable with the name \em
      name
  */
  const plData& operator[](const std::string &name) const;


  /** Returns the number of the stored variables  */
  inline size_t size() const { return ( values_ ? values_->size() : 0 ); }

  /** Resets all variable values to their initial values */
  void reset();

  /** Resets particular variables values to their initial values */
  void reset(const plVariablesConjunction &variables);

  /** Makes an increment iteration of all variables  */
  bool next();
  
  /** Makes an increment iteration of a set of particular variables */
  bool next(const plVariablesConjunction &variable);

  /** Returns the corresponding variable conjunction. */
  inline plVariablesConjunction get_variables() const 
  {return plVariablesConjunction(variable_list_);}
  
  /** Assignment operator */
  plValues& operator=(const plValues &v);

  /** Same as operator=() */
  plValues& assign_from(const plValues& other)
  {
    return operator=(other);
  }
 
  /** Equality operator */
  bool operator==(const plValues &v) const;

  /** Equality  */
  bool equal(const plValues &v) const
  {
    return operator==(v);
  }
  
  /** Non equality operator */
  bool operator!=(const plValues &v) const {return !(*this == v);}

  /** Non equality  */
  bool not_equal(const plValues &v) const
  {
    return operator!=(v);
  }
  
  /** "Less than" operator */
  bool operator<(const plValues &v) const;

  /** "Less than"  */
  bool less(const plValues &v) const
  {
    return operator<(v);
  }
  
  /** "Greater than or equal" operator */
  bool operator>=(const plValues &v) const{return !(*this < v);}

  /** "Greater than or equal" */
  bool greater_or_equal(const plValues &v) const
  {
    return operator>=(v);
  }
  
  /** "Greater than" operator */
  bool operator>(const plValues &v) const;

  /** "Greater than" */
  bool greater(const plValues &v) const
  {
    return operator>(v);
  }

  
  /** "Less than or equal" operator */
  bool operator<=(const plValues &v) const{return !(*this > v);}

  /** "Less than or equal" */
  bool less_or_equal(const plValues &v) const
  {
    return operator<=(v);
  }

  /** Creates a new plValues object identical to this one, except that
      it binds a variable to a value. The variable does not need
      to be already bound in the original object.

      This is useful to build plValues objects inline. For example:

      @code
      std::cout << question.instantiate(plValues().add(a, 42).add(b, 66)).compile() << std::endl;
      @endcode
  */
  plValues add(const plVariable & variable, const plData &value) const;
  plValues add(const plVariable & variable, const int &value) const;
  plValues add(const plVariable & variable, const unsigned int &value) const;
#ifndef SWIG // Python just needs double
  plValues add(const plVariable & variable, const float &value) const;
#endif
  plValues add(const plVariable & variable, const double &value) const;
  plValues add(const plVariable & variable, const std::string &value) const;

  /** Creates a new plValues object identical to this one, except that
      the values corresponding to @em variables are removed
  */
  plValues remove(const plVariablesConjunction & variables) const;

  /** Creates a new plValues object identical to this one, except that
      the values corresponding to the variable with name @em name is removed
  */
  plValues remove(const std::string & variable_name) const;
  
  /** Renames the variables assuming that the old and the new variables have
      the same number and same types 
  */
  void rename(const plVariablesConjunction &new_variable);

  /** Set partially the value.
      @em partial_value variables are assumed to be a subset of this value
      variables
  */
  void set_partially(const plValues& partial_value);
  
  /** Set the value from another value Same as set_partially(), however the
      variables of @em set_partially are not assued to be a subset of this
      value variables
  */
  void set_from(const plValues& other_value);
                
  /** "a ^ b" returns a new plValues object binding the union of variables of
      plValues a and b, with values as specified by a and b. If a variable is bound
      in both a and b, the value in b is kept.
  */
  plValues operator^(plValues const & other)const;

  /** Same as operator^() above
   */
  plValues concatenate(plValues const & other)const
  {
    return operator^(other);
  }

  /** "a ^= b" sets "a" to the binding of the union of variables of
      plValues a and b, with values as specified by a and b. If a variable is bound
      in both a and b, the value in b is kept.
  */
  plValues& operator^=(plValues const & other);

  /** Same as operator^=() above
   */
  plValues& push_back(plValues const & other)
  {
    return operator^=(other);
  }

  /** Returns the support area of this value.
      The support area for an interval variable is the size of the current interval.
      For any value of any other variable, the support area is always 1.
      For a conjunction of variables, the support area is the product
      of support areas for each variable in the conjunction.
  */
  
  plFloat support_area() const;

  /* Internal method: Uses a given address as the current value array */
  inline void set_values_address(plDataValues *val)
  {
    if(values_to_be_deleted_) delete values_;
    values_ = val;
    values_to_be_deleted_ = false;
  }
  
  /* Internal method: Get a reference the value array */
  inline plDataValues &values()
  {
    assert(values_);
    //assert(values_to_be_deleted_);
    return *values_;
  }

  /* Internal method: Get a const reference the value array */
  inline const plDataValues &values()const
  {
    assert(values_);
    return *values_;
  }

  /* Internal method: Return the values of a list of variables */
  void to_data_values(const kplVariableList &variable, plDataValues *ptr,
                      std::vector<bool> &definition) const;

  /* Internal method: Return the values of a list of variables */
  void to_data_values(const kplVariableList &variable, plDataValues *ptr) const;

  /* Internal method: Set the value of a given variable if the variable is in
     the container variables and returns true.  Returns false otherwise 
  */
  bool set_variable_value(const kplVariable *variable, const plData &value);


  /** Get value for a given variable */
  const plData &get_value(const plVariable&s) const
  {
    return operator[](s);
  }
  /** Get value at a given position */
  const plData &get_value(size_t i) const
  {
    return operator[](i);
  }

  /** Get value from a variable name */
  const plData &get_value(const std::string &name) const
  {
    return operator[](name);
  }
  
  /** Get value for a given variable as int */
  int get_value_as_int(const plVariable&s) const
  {
    return int(operator[](s));
  }

  /** Get value at a given position as int */
  int get_value_as_int(size_t i) const
  {
    return int(operator[](i));
  }

  /** Get value for a given variable as double */
  double get_value_as_double(const plVariable&s) const
  {
    return double(operator[](s));
  }

  /** Get value at a given position as double */
  double get_value_as_double(size_t i) const
  {
    return double(operator[](i));
  }

  /** Get value for a given variable as string label */
  std::string get_value_as_label(const plVariable&s) const;

  /** Get value at a given position as string label */
  std::string get_value_as_label(size_t i) const;

  /** Set value for a given variable */
  void set_value(const plVariable&s, const plData &data)
  {
    operator[](s) = data;
  }

  /** Set value for a given position */
  void set_value(size_t i, const plData &data)
  {
    operator[](i) = data;
  }

  /** Set value for a given variable name */
  void set_value(const std::string &name, const plData &data)
  {
    operator[](name) = data;
  }
  
  /** Set value for a given variable */
  void set_value(const plVariable&s, int data)
  {
    operator[](s) = data;
  }

  
  /** Set value for a given position */
  void set_value(size_t i, int data)
  {
    operator[](i) = data;
  }

  /** Set value for a given variable name */
  void set_value(const std::string &name, int data)
  {
    operator[](name) = data;
  }

  /** Set value  for a given variable */
  void set_value(const plVariable&s, unsigned int data)
  {
    operator[](s) = data;
  }

  /** Set value  for a given position */
  void set_value(size_t i, unsigned int data)
  {
    operator[](i) = data;
  }

  /** Set value for a given variable name */
  void set_value(const std::string &name, unsigned int data)
  {
    operator[](name) = data;
  }

  /** Set value for a given variable */
  void set_value(const plVariable&s, double data)
  {
    operator[](s) = data;
  }

  /** Set value for a given position */
  void set_value(size_t i, double data)
  {
    operator[](i) = data;
  }

  /** Set value for a given variable name */
  void set_value(const std::string &name, double data)
  {
    operator[](name) = data;
  }

#ifndef SWIG
  /** Set value for a given variable */
  void set_value(const plVariable&s, float data)
  {
    operator[](s) = data;
  }
  
  /** Set value for a given position */
  void set_value(size_t i, float data)
  {
    operator[](i) = data;
  }

  /** Set value for a given variable name */
  void set_value(const std::string &name, float data)
  {
    operator[](name) = data;
  }
#endif
  
  /** Set value for a given variable */
  void set_value(const plVariable&s, const std::string &data)
  {
    operator[](s) = data;
  }

  /** Set value for a given position */
  void set_value(size_t i, const std::string &data)
  {
    operator[](i) = data;
  }

  /** Set value for a given variable name */
  void set_value(const std::string &name, const std::string &data)
  {
    operator[](name) = data;
  }
  
  /** Returns the values for which 'defined' is true */
  plValues get_defined(const std::vector<bool> & defined) const;

  /** Set at a random value in variable ranges */
  void set_to_random();
  
 protected:
  void Output(std::ostream &out) const;

 private:
  void set_internal(const plValues& partial_value,
                    bool error_if_notfound,
                    const std::string &caller);
  
  template <typename T>
    plValues add_impl(const plVariable & variable, const T &value) const;
};



#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
