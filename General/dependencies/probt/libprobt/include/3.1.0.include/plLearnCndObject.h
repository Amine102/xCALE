/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnCndObject.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  12 13:45:57 2010
 *
 *=============================================================================
 *        (c) Copyright 2010, ProBayes SAS  -  all rights reserved
 *=============================================================================
*/

#ifndef plLearnCndObject_h
#define plLearnCndObject_h

#include "plConfig.h"

#include <plLearnObject.h>
#include <plVariablesConjunction.h>
#include <plCndDistribution.h>

SPL_DECL1(plLearnCndObject);

/** This is an abstract class for all conditional learning objects.
 */
class PL_DLL_API plLearnCndObject : public plLearnObject
{
 public:
  /** Default constructor */
  plLearnCndObject()
    : plLearnObject()
    , left_vars_()
    , right_vars_()
    {}

  /** Constructor */
  plLearnCndObject(const plVariablesConjunction &left_vars,
		   const plVariablesConjunction &right_vars)
    : plLearnObject(left_vars^right_vars)
    , left_vars_(left_vars)
    , right_vars_(right_vars)
    {}
  
  /** Returns the left variables of this conditional learning object.
  */
  const plVariablesConjunction& get_left_variables()const
  {
    return left_vars_;
  }

  /** Returns the right variables of this conditional learning object.
  */
  const plVariablesConjunction& get_right_variables()const
  {
    return right_vars_;
  }

  /** Constructs and returns the conditional ditribution corresponding to the current state of this learner */
  virtual plCndDistribution get_cnd_distribution() const = 0;

  virtual plComputableObject get_computable_object()const
  {
    return get_cnd_distribution();
  }

  using plLearnObject::add_point;
  
 private:
  template <class arrayT>
    bool add_point_ptr_cnd(arrayT* left_point, arrayT* right_point, plFloat weight = PL_ONE)
  {
    for(size_t i = 0; i < left_vars_.size(); ++i){
      values_[i] = left_point[i];
    }
    size_t j = left_vars_.size();
    for(size_t i = 0; i < right_vars_.size(); ++i){
      values_[j++] = right_point[i];
    }
    
    return internal_addPoint(values_, weight);
  }
  
 public:
  
  /** Adds a multidimensional data point with a given weight to learn (left | righ).
      
      @param[in] left_point Data point for left variables to add, as a C array.
      @param[in] right_point Data point for right variables to add, as a C array.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, y, weight) is equivalent to doing @em weight times add_point(x, y).
      
      @returns True if and only if the point could be added (i.e. is valid).
  */
  bool add_point(const int* left_point, const int* right_point, plFloat weight = PL_ONE)
  {
    return add_point_ptr_cnd(left_point, right_point, weight);
  }
  bool add_point(const unsigned int* left_point, const unsigned int* right_point, plFloat weight = PL_ONE)
  {
    return add_point_ptr_cnd(left_point, right_point, weight);
  }
  bool add_point(const float* left_point, const float* right_point, plFloat weight = PL_ONE)
  {
    return add_point_ptr_cnd(left_point, right_point, weight);
  }
  bool add_point(const double* left_point, const double* right_point, plFloat weight = PL_ONE)
  {
    return add_point_ptr_cnd(left_point, right_point, weight);
  }
  bool add_point(const long double* left_point, const long double* right_point, plFloat weight = PL_ONE)
  {
    return add_point_ptr_cnd(left_point, right_point, weight);
  }
  
 protected:    
  SPL_ACCESS1(plLearnCndObject);
  plVariablesConjunction left_vars_;
  plVariablesConjunction right_vars_;
};


#endif // plLearnCndObject_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
