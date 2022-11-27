/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnDistributions.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Feb 23 11:59:30 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLearnDistributions_h
#define plLearnDistributions_h

#include "plConfig.h"

#include <plLearnObject.h>
#include <cassert>

SPL_DECL2(plLearnDistributions);

/** This class allows to learn a set of conditional and non-conditional distributions
    in the same time. */
class PL_DLL_API plLearnDistributions :public plLearnObject
{
 private:
  
  template <class arrayT>
    bool add_point_if_not_frozen_impl_ptr(arrayT* point, const std::vector<bool> &is_not_frozen_learner, plFloat weight)
  {
    for(unsigned int i = 0; i < values_.size(); i++){
      values_[i] = point[i];
    }
    return addFilterPoint(values_, is_not_frozen_learner, weight);
  }
     
  template <class arrayT>
    bool add_point_if_not_frozen_impl_vect(const std::vector <arrayT> &point, const std::vector<bool> &is_not_frozen_learner, plFloat weight)
  {
    assert(values_.size() == point.size());
    assert(values_.size() > 0);
    return add_point_if_not_frozen_impl_ptr(&(point[0]), is_not_frozen_learner, weight);
  }
    
  template <class singleT>
    bool add_point_if_not_frozen_impl_single(singleT point, const std::vector<bool> &is_not_frozen_learner, plFloat weight)
  {
    assert(values_.size() == 1);
    return add_point_if_not_frozen_impl_ptr(&point, is_not_frozen_learner, weight);
  }
  
 public:

  /** Default constructor 
      
      To be used especially with add_object() to insert objects
  */
  plLearnDistributions();

  /** Insert an object to be be learnt 
      This function may be used with a default constructed plLearnDistributions
  */
  void add_object(const plLearnObject &object);

  /** Construct an object allowing to jointly learn a set of distributions
      corresponding to the learning object vector @em learn_objects.  
      @em vars represents the order of data values when inserted without
      using a plValues container (i.e., using stl vectors and pointers).

     The constructor clones the passed @em learn_objects and stores them
     internally (i.e. with no side-effect on the passed @em learn_objects).

     ATTENTION: when using this function in Python, pass the parameter
      'learn_objects' as a Python list
  */
  plLearnDistributions(const std::vector <plLearnObject*> &learn_objects,                          
		       const plVariablesConjunction &vars);


  /** Construct an object allowing to jointly learn a set of distributions
      corresponding to the learning object vector @em learn_objects. 

      The constructor clones the passed @em learn_objects and stores them 
      internally (i.e. with no side-effect on the passed @em learn_objects)
      unless @em do_not_clone_learners is set to @em true

      ATTENTION: using this constructor, you have to:
      - use @em plValues when using @em add_point, or
      - ensure that the order of the learning data corresponds to
      the order defined by variables apparition in the
      concatenation of the variables of all elements of @em
      learn_objects.

      ATTENTION: when using this function in Python, pass the parameter
      'learn_objects' as a Python list
  */
  explicit plLearnDistributions(const std::vector <plLearnObject*> &learn_objects,
                                bool do_not_clone_learners=false);

  /** Copy constructor */
  plLearnDistributions(const plLearnDistributions& other);

  /** Assignment */
  plLearnDistributions &operator=(const plLearnDistributions& other);

  /** Destructor. */
  virtual ~plLearnDistributions();
                    
  /** Resets all learning objects. */
  void reset();

  /** Constructs the computable object corresponding to the learnt
      distribution.  Actually, the returned object is a product
      plComputableObject (that can be conditional or not).

      When no variable order is given to the constructor (ie, using
      plLearnDistributions(const std::vector <plLearnObject*> &)), the left
      variables of the constructed computable object are the concatenation, in
      the  apparition order, of the left variables of all elements of the vector @em
      learn_objects passed to the constructor.  Its right variables are the
      concatenation of the right variables of all elements of @em learn_objects
      and that are not in left one above.

      When an order is provided (ie, using plLearnDistributions(const
      std::vector <plLearnObject*> &learn_objects, const plVariablesConjunction
      &vars)), the left variables of the constructed computable object are the
      concatenation, respecting the order 'vars', of the left variables of all
      elements of the vector @em learn_objects passed to the constructor.  Its
      right variables are the concatenation, respecting the order 'vars', of
      the right variables of all elements of @em learn_objects and that are not
      in left one above.

      If the learnt set of distributions contains a given variable on the left
      side several times (ie, not a valid decomposition), calling this method
      will throw an error when creating the plComputableObject instance.
  */
  plComputableObject get_computable_object()const;

  /** Returns the list of all learnt computable objects. */
  plComputableObjectList get_computable_object_list()const;
  
  /** Adds a point @em point (represented as a @em plValues) with a
      given weight @em weight and updates the statistics. 
      @em is_not_frozen_learner is an array of boolean values to say,
      for each learning object, if it has to be updated or not.*/
  bool add_point_if_not_frozen(const plValues &point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    point.to_data_values(all_vars_.get_variable_list(), &values_);
    return addFilterPoint(values_, is_not_frozen_learner, weight);
  }

  /** Same as above, but @em point is a C array. */
  bool add_point_if_not_frozen(const float* point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_ptr(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const double* point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_ptr(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const long double* point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_ptr(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const int* point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_ptr(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const size_t* point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_ptr(point, is_not_frozen_learner, weight);
  }

  /** Same as above, but @em point is an STL array. */
  bool add_point_if_not_frozen(const std::vector<float> &point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_vect(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const std::vector<double> &point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_vect(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const std::vector<long double> &point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_vect(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const std::vector<int> &point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_vect(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(const std::vector<unsigned int> &point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_vect(point, is_not_frozen_learner, weight);
  }

  
  /** Same as above, but @em point is an unidimensional value. 
      ATTENTION: this method can only be used for one-dimensional cases. */
  bool add_point_if_not_frozen(float point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_single(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(double point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_single(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(long double point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_single(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(int point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_single(point, is_not_frozen_learner, weight);
  }
  bool add_point_if_not_frozen(size_t point, const std::vector<bool> &is_not_frozen_learner, plFloat weight = PL_ONE)
  {
    return add_point_if_not_frozen_impl_single(point, is_not_frozen_learner, weight);
  }
  
  /** Returns the left variables.  

      ATTENTION: when using the constructor plLearnDistributions(const
      std::vector <plLearnObject*> &learn_objects, const plVariablesConjunction
      &vars), get_variables() is not necessary equivalent to the concatenation
      of get_left_variables() and get_right_variables().
  */
  const plVariablesConjunction& get_left_variables() const
  {
    if(to_be_initialized_) {
      init(learn_objects_);
      to_be_initialized_ = false;
    }
    return left_variables_;
  }
  
  /** Returns the right variables.
  */
  const plVariablesConjunction& get_right_variables() const
  {
    if(to_be_initialized_) {
      init(learn_objects_);
      to_be_initialized_ = false;
    }
    return right_variables_;
  }

   /** Returns the ACTUAL number of parameters. It could be different from the
      number of parameters returned by get_parameters. For example, the actual
      number of parameters of a N dimensional normal distribution is N +
      N*(N+1)/2 because the covariance matrix is symmetric.

      The number of parameters of \em plLearnDistributions instance is the sum
      of the number of parameters of each containd \em plLearnObject.
  */
  unsigned int get_n_parameters()const;

  /** Return \em nsamples for each learned distribution */
  void get_all_nsamples(std::vector <unsigned int > &n)const;

  /** Return  \em total_weight for each learned distribution */
  void get_total_weights(std::vector <plFloat> &w)const;

  virtual void rescale_total_weight(plFloat s);

  virtual plLearnObject* clone()const
  {
    return new plLearnDistributions(*this);
  }

  /** Return the vector of the stored learn objects */
  const std::vector <plLearnObject*> &get_learn_objects()const
  {
    return learn_objects_;
  }

  /** Get the number of learn objects */
  size_t get_learn_objects_number() const
  {
    return learn_objects_.size();
  }
  
  /** Get the learn object at a quiven position. Return 0 if the position is out-of-range */
  plLearnObject *get_learn_object(size_t n) const
  {
    if(n < learn_objects_.size()) return learn_objects_[n];
    return 0;
  }
  
 protected:
  SPL_ACCESS2(plLearnDistributions);

  /// This is const because it needs to be called from get_{left,right}_variables() const.
  void init(const std::vector <plLearnObject*> &learn_objects) const;
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
 

  bool addFilterPoint(const plDataValues &point, const std::vector<bool> &distrib_to_update, plFloat weight = PL_ONE)
  {
    unsigned int i, j;
    bool res = true;

    nsamples_++;
    total_weight_ += weight;
      
    for( i = 0; i < learn_objects_.size(); i++){
	
      if(distrib_to_update[i]){
        for(j = 0; j < point_to_children_points_[i].size(); j++){
          children_points_[i][j] = point[  point_to_children_points_[i][j] ];
        }
        res = res && learn_objects_[i]->internal_addPoint(children_points_[i], weight);
      }
    }
	
    return res;
  }

  bool internal_addPointWithDef(const plDataValues &point, 
                                const std::vector <bool> &is_defined, 
                                plFloat weight = PL_ONE);

  void clean();

  void finalize_copy();

  // private constructor used for deserialization
  plLearnDistributions(const std::vector <plLearnObject*> &learn_objects,                          
		       const plVariablesConjunction &vars,
                       bool clone_passed_learn_objects);

  virtual void Output(std::ostream &out) const;

 protected:
  mutable std::vector <plLearnObject*> learn_objects_; // mutable needed for get_left_variables() -> init()
  mutable std::vector <plDataValues> children_points_; // mutable needed for get_left_variables() -> init()
  mutable std::vector < std::vector < unsigned int> > point_to_children_points_; // mutable needed for get_left_variables() -> init()
  mutable plVariablesConjunction left_variables_; // mutable needed for get_left_variables() -> init()
  mutable plVariablesConjunction right_variables_; // mutable needed for get_left_variables() -> init()
  bool delete_learn_objects_;
  mutable bool to_be_initialized_; // mutable needed for get_left_variables() -> init()
    
  template <class T> friend class plCndLearnObject;
};


#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
