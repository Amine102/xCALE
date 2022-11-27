/*===========================================================================
 * Product        : ProBT
 * File           : plVariableGuesser.h
 * Author         : Ronan Le Hy <ronan.lehy@probayes.com>
 *
 *===========================================================================
 *        (c) Copyright 2010, Probayes SAS  -  all rights reserved
 *===========================================================================*/

#ifndef PLVARIABLEGUESSER_H
#define PLVARIABLEGUESSER_H

#include "plConfig.h"
#include <set>
#include <string>
#include "plVariable.h"

/** Heuristic to create a plVariable from its observed values (as strings).
    Add values using add(). Then get back a plVariable using variable().
 */
class PL_DLL_API plVariableGuesser {
 protected:
  enum type_t {
    INT, FLOAT, LABEL, UNKNOWN
  };

 private:
  type_t type_;
  plFloat min_;
  plFloat max_;
  std::set<std::string> string_values_;
  std::string name_;

 protected:
  static bool value_is_null(std::string const & v);
  static int int_of_string(std::string const & value, bool & success);
  static plFloat float_of_string(std::string const & value, bool & success);

  void update_min_max(plFloat v);

  void add_int(std::string const & value);
  void add_float(std::string const & value);
  void add_label(std::string const & value);
  void add_unknown(std::string const & value);

 public:
  plVariableGuesser(std::string const & name);

  // default copy ctor and assignment op are OK

  void add(std::string const & value);

  plVariable as_int() const;
  plVariable as_real() const;
  plVariable as_interval(int discretization = 100) const;
  plVariable as_label() const;

  /* Return a dummy integer (plIntegerType(0,0)) if the variable has 0 or 1 observed values */
  plVariable variable(int real_discretization = -1) const;
};


#endif /* PLVARIABLEGUESSER_H */
