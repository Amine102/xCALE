/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnObject.h
 * Author         : Kamel Mekhnacha
 * Creation       : Fri Feb 23 11:49:41 2007
 *
 *=============================================================================
 *        (c) Copyright 2006, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLearnObject_h
#define plLearnObject_h

#include "plConfig.h"

#include <limits>

#include <plDataValues.h>
#include <plValues.h>
#include <plVariablesConjunction.h>
#include <plComputableObject.h>
#include <plDLLExport.h>
#include <plDataDescriptor.h>
#include <plError.h>

class plJointDistribution;
class plLearnObject;

SPL_DECL1(plLearner);

//=============================================================================
/** This is the base class of all learning objects. */
class PL_DLL_API plLearner : public plObject
{};

//=============================================================================
SPL_DECL2(plLearnObject);

/** This is the base class of all incremental (adaptive) learning objects. */
class PL_DLL_API plLearnObject : public plLearner
{
 public:
  /** Constructor using a set of variables.*/
  explicit plLearnObject(const plVariablesConjunction &variables);
    
  /** Default constructor. */
  plLearnObject()
    : plLearner(), nsamples_(0), total_weight_(PL_ZERO), all_vars_(), values_() {}
	
  /** Copy constructor */
  plLearnObject(const plLearnObject &obj);
  
  /** Assignment operator */
  plLearnObject &operator=(const plLearnObject &obj);
    
  /** Destructor */
  virtual ~plLearnObject();
    
  /** Returns the number  of the points used 
      in learning (i.e. added using the \em add_point methods). */
  unsigned int get_nsamples() const {return nsamples_;}

  /** Returns the  sum of weights of the points used 
      in learning (i.e. added using add_point()). */
  plFloat get_total_weight() const {return total_weight_;}

  /** Multiply the weight of the past inserted points by a given factor
    
      This allows, for example, to decrease the importance of the old data points compared to the new ones 
  */
  virtual void rescale_total_weight(plFloat s) = 0;

  /** Resets learning (forgets all learned data). 
   */
  virtual void reset() 
  {
    nsamples_ = 0;
    total_weight_ = PL_ZERO;
  }
 
  /** Constructs the computable object corresponding to the learnt
      distribution */
  /** Constructs and returns the computable object (conditional or non
      conditional distribution) corresponding to the current state of
      this learner */
  virtual plComputableObject get_computable_object()const = 0;
    
        
  /** Adds a data point with a given weight.

      @param[in] point Data point to add.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const plValues &point, plFloat weight = PL_ONE)
  {
    // point.to_data_values(all_vars_.get_variable_list(), &values_);
    // return internal_addPoint(values_, weight);

    std::vector<bool> defs;
    point.to_data_values(all_vars_.get_variable_list(), &values_, defs);
    return internal_addPointWithDef(values_, defs, weight);
  }
    
 private:
  template <class arrayT>
    bool add_point_ptr(arrayT* point, plFloat weight = PL_ONE)
  {
    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPoint(values_, weight);
  }

 public:
  /** Adds a multidimensional data point with a given weight.

      @param[in] point Data point to add, as a C array.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const int* point, plFloat weight = PL_ONE)
  {
    return add_point_ptr(point, weight);
  }
  bool add_point(const unsigned int* point, plFloat weight = PL_ONE)
  {
    return add_point_ptr(point, weight);
  }
  bool add_point(const float* point, plFloat weight = PL_ONE)
  {
    return add_point_ptr(point, weight);
  }
  bool add_point(const double* point, plFloat weight = PL_ONE)
  {
    return add_point_ptr(point, weight);
  }
  bool add_point(const long double* point, plFloat weight = PL_ONE)
  {
    return add_point_ptr(point, weight);
  }

 private:
  template <class arrayT>
    bool add_point_vect(const std::vector<arrayT> & point, plFloat weight = PL_ONE)
  {
    assert(point.size() == values_.size());
    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPoint(values_, weight);
  }

 public:

  /** Adds a multidimensional data point with a given weight.

      @param[in] point Data point to add, as an STL vector.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const std::vector<int> & point, plFloat weight = PL_ONE)
  {
    return add_point_vect(point, weight);
  }
  bool add_point(const std::vector<unsigned int> & point, plFloat weight = PL_ONE)
  {
    return add_point_vect(point, weight);
  }
  bool add_point(const std::vector<double> & point, plFloat weight = PL_ONE)
  {
    return add_point_vect(point, weight);
  }
  bool add_point(const std::vector<float> & point, plFloat weight = PL_ONE)
  {
    return add_point_vect(point, weight);
  }
  bool add_point(const std::vector<long double> & point, plFloat weight = PL_ONE)
  {
    return add_point_vect(point, weight);
  }

  /** Add random unifom samples in  variable ranges */
  void add_random_samples(unsigned int n);
  
 private:
  template <class singleT>
    bool add_point_single(singleT point, plFloat weight = PL_ONE)
  {
    if(values_.size() != 1)
      THROW_PL_ERROR(8, "plLearnObject::add_point(T &, bool): variables dimension has to be 1, variables=" << this->get_variables());

    values_[0] = point;
    return internal_addPoint(values_, weight);
  }

 public:

  /** Adds a unidimensional data point with a given weight.

      @param[in] point Data point to add. It must be convertible to the type of
      data of the variable.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(int point, plFloat weight = PL_ONE)
  {
    return add_point_single(point, weight);
  }
  bool add_point(unsigned int point, plFloat weight = PL_ONE)
  {
    return add_point_single(point, weight);
  }
  bool add_point(double point, plFloat weight = PL_ONE)
  {
    return add_point_single(point, weight);
  }
  bool add_point(float point, plFloat weight = PL_ONE)
  {
    return add_point_single(point, weight);
  }
  bool add_point(long double point, plFloat weight = PL_ONE)
  {
    return add_point_single(point, weight);
  }

  /** Adds a multidimensional data point with a given weight; the components of
      the point are specified in non-contiguously.

      @param[in] point Data point to add, as a @em plValues.
      @param[in] is_defined Indicates which components should be used for
      learning.  is_defined[i] should be @em true if and only if point[i] is to
      be used.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times
      add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const plValues &point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    assert(is_defined.size() == values_.size());
    point.to_data_values(all_vars_.get_variable_list(), &values_);
    return internal_addPointWithDef(values_, is_defined, weight);
  }
  
 private:
  template <class arrayT>
    bool add_point_def_ptr(arrayT* point, 
                           const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    assert(values_.size() == is_defined.size());

    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPointWithDef(values_, is_defined, weight);
  }
 public:
  /** Adds a multidimensional data point with a given weight; the components of
      the point are specified in order but non-contiguously.

      @param[in] point Data point to add, as a C array.
      @param[in] is_defined Indicates which components should be used for
      learning.  is_defined[i] should be @em true if and only if point[i] is to
      be used.
      @param[in] weight Weight of the added point. If @em weight is an integer,
      add_point(x, weight) is equivalent to doing @em weight times
      add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const int* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_ptr(point, is_defined, weight);
  }
  bool add_point(const unsigned int* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_ptr(point, is_defined, weight);
  }
  bool add_point(const double* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_ptr(point, is_defined, weight);
  }
  bool add_point(const float* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_ptr(point, is_defined, weight);
  }
  bool add_point(const long double* point, 
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_ptr(point, is_defined, weight);
  }
  
 private:
  template <class arrayT>
    bool add_point_def_vect(const std::vector <arrayT> &point,
                            const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    assert(point.size() == values_.size());
    assert(point.size() == is_defined.size());

    for(size_t i = 0; i < values_.size(); ++i){
      values_[i] = point[i];
    }
    return internal_addPointWithDef(values_, is_defined, weight);
  }

 public:
  /** Adds a multidimensional data point with a given weight; the components of
      the point are specified in order but non-contiguously.

      @param[in] point Data point to add, as an STL vector.
      @param[in] is_defined Indicates which components should be used for
      learning.  is_defined[i] should be @em true if and only if point[i] is
      to be used.
      @param[in] weight Weight of the added point. If @em weight is an
      integer, add_point(x, weight) is equivalent to doing @em weight times
      add_point(x).

      @returns True if and only if the point could be added.
  */
  bool add_point(const std::vector <int> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_vect(point, is_defined, weight);
  }
  bool add_point(const std::vector <unsigned int> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_vect(point, is_defined, weight);
  }
  bool add_point(const std::vector <double> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_vect(point, is_defined, weight);
  }
  bool add_point(const std::vector <float> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_vect(point, is_defined, weight);
  }
  bool add_point(const std::vector <long double> &point,
                 const std::vector <bool> &is_defined, plFloat weight = PL_ONE)
  {
    return add_point_def_vect(point, is_defined, weight);
  }


  /** Learn using the data set defined by the container 'data_descriptor'.

      @param[in] data_descriptor the data set to be used.
      @param[in] n the number of data rows to be used.
      @param[in] weight the weight to be applyied to the passed data descriptor

      @return the number of the inserted points on success. Returns a negative value
      if the data descriptor is not OK (eg, CSV file does not exist, mysql
      server is not responding,...).

      It is the responsibility of the caller to setup the data descriptor properly
      before calling this method, for instance by calling rewind() on it if
      necessary. Also, this method does not attempt to rewind() the data descriptor
      before returning.
  */
  int learn(plDataDescriptor &data_descriptor, 
            unsigned int n = std::numeric_limits<unsigned int>::max(), plFloat weight=PL_ONE);

  int learn_dynamic(plDataDescriptor &data_descriptor, 
                    const std::vector< std::pair<plVariable, plVariable> > &ts_edges,
                    unsigned int n = std::numeric_limits<unsigned int>::max(),
                    plFloat weight=PL_ONE);

  int learn_dynamic(const std::vector<plValues> &data_set, 
                    const std::vector< std::pair<plVariable, plVariable> > &ts_edges,
                    unsigned int n = std::numeric_limits<unsigned int>::max(),
                    plFloat weight=PL_ONE);
    
 private:
  template <typename T>
    void learn_impl(const std::vector<T> &vals)
  {
    if(values_.size() != 1)
      THROW_PL_ERROR(8, "plLearnObject::learn(const std::vector<T> &): variables dimension has to be 1, variables=" << this->get_variables());

    for(size_t i = 0; i < vals.size(); ++i) {
      add_point(vals[i]);
    } 
  }
 public:
  /** Learn using a set of values */
  void learn(const std::vector<plValues> &vals)
  {
    for(size_t i = 0; i < vals.size(); ++i) {
      add_point(vals[i]);
    }
  }
  
  /** Learn using a set of one-dimensional values */
  void learn(const std::vector<int> &vals)
  {
    learn_impl(vals);
  }
  void learn(const std::vector<unsigned int> &vals)
  {
    learn_impl(vals);
  }
  void learn(const std::vector<double> &vals)
  {
    learn_impl(vals);
  }
  void learn(const std::vector<float> &vals)
  {
    learn_impl(vals);
  }
  void learn(const std::vector<long double> &vals)
  {
    learn_impl(vals);
  }
    
 private:
  template <typename T>
    void learn_def_impl(const std::vector<T> &vals, const std::vector<bool> &def)
  {
    if(values_.size() != 1)
      THROW_PL_ERROR(8,
                     "plLearnObject::learn(const std::vector<T> &, const std::vector<bool> &):"
                     << " variables dimension has to be 1, variables=" << this->get_variables());

    for(size_t i = 0; i < vals.size(); ++i) {
      if(def[i]) add_point(vals[i]);
    }
  }

 public:
  /** Learn using a set of one-dimensional values with their definition information */
  void learn(const std::vector<plValues> &vals, const std::vector<bool> &def)
  {
    learn_def_impl(vals, def);
  }
  
  /** Learn using a set of one-dimensional values with their definition information */
  void learn(const std::vector<int> &vals, const std::vector<bool> &def)
  {
    learn_def_impl(vals, def);
  }
  void learn(const std::vector<unsigned int> &vals, const std::vector<bool> &def)
  {
    learn_def_impl(vals, def);
  }
  void learn(const std::vector<double> &vals, const std::vector<bool> &def)
  {
    learn_def_impl(vals, def);
  }
  void learn(const std::vector<float> &vals, const std::vector<bool> &def)
  {
    learn_def_impl(vals, def);
  }
  void learn(const std::vector<long double> &vals, const std::vector<bool> &def)
  {
    learn_def_impl(vals, def);
  }


  /** Returns the left variables */
  virtual const plVariablesConjunction& get_left_variables()const = 0;

  /** Returns the right variables */
  virtual const plVariablesConjunction& get_right_variables()const = 0;

  /** Returns all variables */
  const plVariablesConjunction &get_variables()const
  {
    return all_vars_;
  }

  /** Returns the ACTUAL number of parameters. It could be different from the
      number of parameters returned by get_parameters.
      
      For example, the actual number of parameters of a N dimensional normal distribution 
      is N + N*(N+1)/2 because the covariance matrix is symmetric.
  */
  virtual unsigned int get_n_parameters()const = 0;


  /** Clone the learning object */
  virtual plLearnObject* clone()const = 0;


  /** Create the learnable decomposition 
      
      \param[in] variables observation variables
      
      \param[in] variable_group_indices A set of indices for variable
      groups.  For example, for variables = {X, A, Z},
      one can use variable_group_indices = [ [0, 2], [1] ] to declare two
      groups of variables [X, Z] and [A]. This leads to the decomposition P(X A
      Z) = P(X Z) P(A)
      
      \param[in] variable_group_parent_indices A set of indices for
      variable group parents.  For example, for variable =
      {X, A, Z}, one can use variable_group_indices = [ [0, 2], [1]
      ] to declare two groups of variables ([X, Z], [A]) and
      variable_group_parent_indices = [ [1], [] ] to declare [1] ([A]) as
      the parents for the first group [0, 2] ([X, Z]) and no parents for the
      second group [1] ([A]). This leads to the decomposition P(X A Z) = P(X Z
      | A) P(A)

      For each distribution learner, its parametric form (type) depends of the type of the variable conjunctions
      and their parents as returned by create_learner()

      \sa plLearnObject::create_learner()
  */
  static plLearnObject* create_learnable_decomposition(const plVariablesConjunction &variables,
                                                       const std::vector<std::vector<unsigned int> > &variable_group_indices=
                                                       std::vector<std::vector<unsigned int> >(),
                                                       const std::vector<std::vector<unsigned int> > &variable_group_parent_indices=
                                                       std::vector<std::vector<unsigned int> >());

  /** Same as above. However plVariablesConjunction are used instead of using indices */
  static plLearnObject* create_learnable_decomposition(const plVariablesConjunction &variables,
                                                       const std::vector<plVariablesConjunction> &variable_groups,
                                                       const std::vector<plVariablesConjunction> &variable_group_parents=
                                                       std::vector<plVariablesConjunction>());
  
  /** Creates a non-conditional distribution learner for a variable conjunction as follows:
      - if the 'variables' is discrete: plLearnLaplace(variables)
      - if the 'variables' is continuous and 1-dimentional: plLearn1dNormal(variables) 
      - if the 'variables' is continuous and N-dimentional: plLearnNdNormal(variables)

      Throws an error otherwise
  */
  static plLearnObject *create_learner(const plVariablesConjunction& variables);

  /** Creates a conditional distribution learner for a variable conjunction as follows:
      - if the 'variables' is discrete and all its parents are discrete: plCndLearnObject<plLearnLaplace>(variables, parents)
      - if the 'variables' is discrete and 1-dimentional, and all its parents are continuous: plLearnSoftmax(variables, parents)
      - if the 'variables' is discrete and 1-dimentional, and its parents are mixed: plLearnSoftmaxFamily(variables, continuous_parents, discrete_parents)
      - if the 'variables' is continuous and 1-dimentional, and all its parents are discrete: plCndLearnObject<plLearn1dNormal>(variables, parents)
      - if the 'variables' is continuous and N-dimentional, and all its parents are discrete: plCndLearnObject<plLearnNdNormal>(variables, parents)
      - if the 'variables' is continuous and 1-dimentional, and all its parents are continuous: plLearnLinearRegression(variables, parents)
      - if the 'variables' is continuous and 1-dimentional, and its parents are mixed: plLearnLinearRegressionFamily(variables, continuous_parents, discrete_parents)

      Throws an error otherwise
  */
  static plLearnObject *create_learner(const plVariablesConjunction& variables, const plVariablesConjunction& parents);
    
 protected:
  plLearnObject(const kplVariableList &variables);

   
  ///
  virtual bool internal_addPointWithDef(const plDataValues &point,
                                        const std::vector <bool> &is_defined,
                                        plFloat weight = PL_ONE);

  /* Adds a point @em point with a given weight @em weight and updates
     statistics.

     This virtual method has to be implemented by all derived classes that have
     to call this default implementation.
      
     The default implementation checks that @em point is in variable range and
     increments @em nsamples_ by 1 and increments @em total_weight_ by the
     value of @em weight.
  */
  virtual bool internal_addPoint(const plDataValues & point, plFloat weight = PL_ONE);
  
  int internal_learn(plDataDescriptor &data_descriptor, unsigned int n, plFloat weight);

  int internal_learn(plDataDescriptor &data_descriptor, 
                     const std::vector< std::pair<plVariable, plVariable> > &ts_edges,
                     unsigned int n, plFloat weight);

  void warn_out_of_range(const plDataValues & point) const;

 protected:
  SPL_ACCESS2(plLearnObject);

  void set_variables(const plVariablesConjunction& vars);
  
  unsigned int nsamples_;
  plFloat total_weight_;


  plVariablesConjunction all_vars_;
  plDataValues values_;

  friend class plLearnDistributions;
  friend class plParticleFilter;
  friend class plLearnDistributionTable;
};

//=============================================================================
SPL_DECL1(plLearnFrozenDistribution);

/** This class implements a learning object that learns nothing. It's
    just a helper to provide the possibility to freeze some
    distributions when using learning algorithms.  Actually, this
    learning object is initialized with a given distribution and simply
    returns the same distribution when the @em get_computable_object
    method is called.
*/
class PL_DLL_API plLearnFrozenDistribution : public plLearnObject
{
 public:
  
  /** Default constructor.
   */
  plLearnFrozenDistribution()
    : plLearnObject()
    , frozen_computable_object_()
    , left_vars_()
    , right_vars_()
    {}

  /** Constructor.
      @param[in] frozen_computable_object the computable object to freeze.
  */
  explicit plLearnFrozenDistribution(const plComputableObject &frozen_computable_object)
    : plLearnObject(frozen_computable_object.get_variables())
    , frozen_computable_object_(frozen_computable_object)
    , left_vars_(  frozen_computable_object_.get_left_variables() )
    , right_vars_( frozen_computable_object_.get_right_variables() )
    {}
  
  /** Returns the left variables */
  const plVariablesConjunction& get_left_variables()const
  {
    return left_vars_;
  }
  
  /** Returns the right variables */
  const plVariablesConjunction& get_right_variables()const
  {
    return right_vars_;
  }

  /** Returns the frozen computable object */
  plComputableObject get_computable_object()const
  {
    return frozen_computable_object_;
  }

  unsigned int get_n_parameters()const
  { 
    return 0;
  }

  virtual plLearnObject* clone()const
  {
    return new plLearnFrozenDistribution(*this);
  }

  virtual void rescale_total_weight(plFloat /* s */){}

 protected:
  virtual void Output(std::ostream &out) const
  {
    if(left_vars_.is_empty())
      out << "plLearnFrozenDistribution: Empty";
    else
      out << "plLearnFrozenDistribution: " << get_computable_object();
  }

 protected:  
  SPL_ACCESS1(plLearnFrozenDistribution);

  plComputableObject frozen_computable_object_;
  plVariablesConjunction left_vars_;
  plVariablesConjunction right_vars_;
};




#endif //plLearnObject_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
