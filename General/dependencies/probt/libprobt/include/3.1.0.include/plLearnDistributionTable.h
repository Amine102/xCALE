/*=============================================================================
 * Product        : ProBT 
 * File           : plLearnDistributionTable.h
 * Author         : Kamel Mekhnacha
 * Creation       : Thu Oct  2 13:45:57 2008
 *
 *=============================================================================
 *        (c) Copyright 2008, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plLearnDistributionTable_h
#define plLearnDistributionTable_h
 
#include "plConfig.h"

#include <plLearnCndObject.h>
#include <plVariablesConjunction.h>
#include <plDistributionTable.h>
#include <plVariableIndexer.h>

class plLearnObject;
template <typename T> class plCndLearnObject;

SPL_DECL1(plLearnDistributionTable);

/** This class implements conditional learning objects as a map. Learning a
    conditional distribution P(X | Y) consists in using a map of
    non-conditional learning objects. This map contains, for each possible
    value of Y a non-conditional learning object on X. These non-conditional
    learning objects are added using the plLearnDistributionTable::push() and
    plLearnDistributionTable::push_default() methods.

    \sa plCndLearnObject
*/
class PL_DLL_API plLearnDistributionTable : public plLearnCndObject
{
  template <typename T>
  friend plCndDistribution get_cnd_distribution_impl(const plCndLearnObject<T>&, const std::string&);
  
 protected:
  typedef std::map<unsigned int, plLearnObject*> LearnerMap_t;
 public:
  /** Constructor */
  plLearnDistributionTable(const plVariablesConjunction& left_vars,
                           const plVariablesConjunction& right_vars);

  /** Constructor when using the same type for non conditional learning
      objects for all the values of the right variables.  @em
      init_object will be used as initilization even after calling
      reset() function
  */
  plLearnDistributionTable(const plVariablesConjunction& left_vars,
                           const plVariablesConjunction& right_vars,
                           const plLearnObject &init_object);
  
  /** Default constructor to be used for serialisation */
  plLearnDistributionTable();

  /** Copy constructor */
  plLearnDistributionTable(const plLearnDistributionTable& other);

  /** Assignment */
  plLearnDistributionTable& operator=(const plLearnDistributionTable& other);
  
  /** Destructor */
  virtual ~plLearnDistributionTable();
  
  /** Inserts a new non-conditional learning object with a given right value.
      A clone of the passed @em learn_object is created and stored internally
      (i.e. with no side-effect on the passed @em learn_object).
  */
  void push(const plLearnObject& learn_object, const plValues &value);
  
  /** Inserts a new non-conditional learning object with a given right value.
      A clone of the passed @em learn_object is created and stored internally
      (i.e. with no side-effect on the passed @em learn_object).
  */
  void push(const plLearnObject& learn_object, int value);
  
  /** Inserts a new non-conditional learning object with a given right value.
      A clone of the passed @em learn_object is created and stored internally
      (i.e. with no side-effect on the passed @em learn_object).
  */
  void push(const plLearnObject& learn_object, plFloat value);
  
  /** Inserts a new non-conditional learning object as a default one for all
      the right values that have not been inserted using push(). A clone of the
      passed @em default_learn_object is created and stored internally
      (i.e. with no side-effect on the passed @em default_learn_object).
  */
  void push_default(const plLearnObject &default_learn_object);
  
  /** Resets learning to its initial state. */
  void reset();
  
  /** Creates the distribution table corresponding to the learnt conditional
      distribution. Consider calling the more efficient get_cnd_distribution()
      if you do not need to change the returned conditional distribution later.

      If no default learning object is provided using push_default(), then the
      returned distribution table does not contain a default function for the
      right values that have never been inserted. Consider calling
      push_default() on the returned distribution table in order to customize
      it.

      \sa get_cnd_distribution()
  */
  plDistributionTable get_distribution_table()const;

  /** Constructs the corresponding learnt conditional
      distribution.
      It's equivalent to calling get_distribution_table() but the returned
      distribution is read-only and thus is more efficient. Moreover, if no
      default learning object is inserted (no call to push_default()), then the
      returned conditional distribution will contain a uniform distribution for
      the right values that have never been inserted.
  */
  plCndDistribution get_cnd_distribution()const;

  /** Returns, for each right value, the total weight of the inserted points. */
  void get_total_weights(std::vector <plFloat> &w)const;

  virtual void rescale_total_weight(plFloat s);
  
  /** Returns, for each right value, the corresponding number of the inserted
      points.
  */
  void get_all_nsamples(std::vector <unsigned int> &n)const;

  /** Get the number of the inserted points for given right value */
  unsigned int get_nsamples_for_value(const plValues &right_value)const;

  
  /** Returns a pointer to the non conditional learning object corresponding to
      the @em right_value value of the right variables.  Returns 0 if @em
      right_value has never been seen.
  */
  const plLearnObject* get_learnt_object_for_value(const plValues &right_value)const;

  unsigned int get_n_parameters()const;

  virtual plLearnObject* clone()const
  {
    return new plLearnDistributionTable(*this);
  }


 protected:
  bool internal_addPoint(const plDataValues &point, plFloat weight = PL_ONE);
  bool internal_addPointWithDef(const plDataValues &point, 
				const std::vector <bool> &is_defined, 
				plFloat weight = PL_ONE);

  virtual void Output(std::ostream &out) const;

  virtual std::string get_name()const
  {
    return "plLearnDistributionTable";
  }

 private:
  void copy_data_internally(const plValues & values);
  
  template<typename T>
    void copy_data_internally(const T &val);
  
  template <typename T>
    void push_template(const plLearnObject& learn_object, T value);

  void clean();
  void finalize_copy();

  plDistribution create_uniform()const;

 protected:
  plLearnDistributionTable(const plVariablesConjunction& left_vars,
                           const plVariablesConjunction& right_vars,
                           const plLearnObject &init_object,
                           bool has_default_initializer);
  

 protected:
  SPL_ACCESS1(plLearnDistributionTable);

  LearnerMap_t NCndLObjects_map_;
  
  plLearnObject *default_learn_object_;
  
  mutable plDataValues left_value_;
  mutable plDataValues right_value_;


  plVariableIndexer right_vars_indexer_;
  plLearnObject *init_object_;
  bool has_default_initializer_;
};

#endif // plLearnDistributionTable_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
