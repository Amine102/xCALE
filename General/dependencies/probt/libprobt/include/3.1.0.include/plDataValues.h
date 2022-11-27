/*=============================================================================
 * Product        : ProBT 
 * File           : plDataValues.h
 * Author         : Juan-Manuel Ahuactzin
 * Creation       : Tue Jan 29 10:40:44 2002
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

#ifndef plDataValues_h
#define plDataValues_h

#include "plConfig.h"


#include <plData.h>

#include <list>
#include <cassert>
#include <algorithm>




/*****************************************************************************
 *                           External Classes
 *****************************************************************************/
class kplVariable;
typedef std::list<kplVariable*> kplVariableList;


/*****************************************************************************
 *                           Classes Definitions
 *****************************************************************************/

SPL_DECL2(plDataValues);

// A \em plDataValues is an array of \em plData used to store the values of
// a set of \em plVariablesConjunction.
class PL_DLL_API plDataValues
{
  SPL_ACCESS2(plDataValues);
  friend class plValues;

 protected:
  std::vector<plData> v_;
 protected:
 
  void Output(std::ostream &out) const; 

 public:

  //===========================================================================
  explicit inline plDataValues(size_t size=0)
    : v_(size)
  {}
  
  //===========================================================================
  inline plDataValues(size_t size, plData::plData_type type)
    : v_(size)
  {
    for(size_t i = 0; i < size; ++i){
      v_[i].data_type_ = type;
    }
  }

  //===========================================================================  
  inline plDataValues(const plDataValues &other)
    : v_(other.v_)
  {}

  //===========================================================================  
  explicit plDataValues(const kplVariableList &variables_list);

  //===========================================================================
  inline ~plDataValues()
  {}
   
 //===========================================================================  
  inline plDataValues& operator=(const plDataValues &other)
  {
    v_ = other.v_;

    return *this;
  }
  
  //===========================================================================
  inline void resize(size_t ns)
  {
    v_.resize(ns);
  }

  //===========================================================================
  inline size_t size() const 
  {
    return v_.size();
  }
    
  //===========================================================================
  inline void deliver_values(const std::vector <std::pair <int,int> > &key_box, 
                             plDataValues &addresses_value) const
  {
    for(size_t i=0;i<key_box.size();++i){
      const std::pair<int,int> &p = key_box[i];
      /* the key box contains in "first" the addresse position and in 
         "second" the source position */
      addresses_value.v_[ size_t(p.first) ] = v_[ size_t(p.second) ];
    }
    
  }

  //===========================================================================
  inline void deliver_values_inverse(const std::vector <std::pair <int,int> > &key_box, 
                                     plDataValues &addresses_value) const
  {
    for(size_t i=0;i<key_box.size();++i){
      const std::pair<int,int> &p = key_box[i];
      /* the key box contains in "first" the addresse position and in 
         "second" the source position */
      addresses_value.v_[ size_t(p.second) ] = v_[ size_t(p.first) ];
    }
    
  }
  
  //===========================================================================
  inline bool operator==(const plDataValues &value) const
  {
    if (size() != value.size())
      return false;
      
    const size_t values_size = size();
      
    for(size_t i=0;i<values_size;++i){
      if (! (v_[i] == value.v_[i])){
        return false;
      }
    }
    return true;
  }
  
  //===========================================================================
  inline bool operator!=(const plDataValues &value) const
  {
    return !(*this == value);
  }

  //===========================================================================
  inline plData& operator[](size_t i)
  {
    assert(i < size());
    return v_[i];
  }

  //===========================================================================
  inline const plData& operator[](size_t i) const 
  {
    assert(i < size());
    return v_[i];
  }

  //===========================================================================
  inline bool operator < (const plDataValues &b ) const
  {
    size_t i = 0;
    while(i < size()){
      if (v_[i] < b.v_[i]) {return true;}
      if (v_[i] > b.v_[i]) {return false;}
      ++i ;
    }
    return false;
  }
  
  //===========================================================================
  inline bool operator>=(const plDataValues &b ) const {return !(*this < b);}

  //===========================================================================
  inline bool operator>(const plDataValues &b ) const
  {
    size_t i = 0;
    while(i < size()){
      if (v_[i] > b.v_[i]) {return true;}
      if (v_[i] < b.v_[i]) {return false;}
      ++i;
    }
    return false;
  }

  //===========================================================================
  inline bool operator<=(const plDataValues &b ) const {return !(*this > b);}
  


  //===========================================================================
  inline void extract(plDataValues &result, 
                      size_t start) const
  {
    assert( start < size() );
    assert( (start + result.size()) <= size() );
    const size_t res_size = result.size();
    for(size_t i = 0; i < res_size; ++i){
      result.v_[i] = v_[i+start];
    }
  }

  //===========================================================================
  inline void copy_into(plDataValues &result, 
                        size_t start_in_result) const
  {
    assert( start_in_result < result.size() );
    assert( (start_in_result + size()) <= result.size() );
    const size_t s = size();
    for(size_t i = 0; i < s; ++i){
      result.v_[i+start_in_result] = v_[i];
    }
  }


 public:
  //===========================================================================
  void set(const kplVariableList &variables_list);
  
  //===========================================================================
  friend inline std::ostream &operator<<(std::ostream &out, const plDataValues &data)
  {
    data.Output(out);
    return out;
  }

  //===========================================================================
  friend inline void concatenate(plDataValues &result, 
                                 const plDataValues &v1, 
                                 const plDataValues &v2)
  {
    const size_t s1 = v1.v_.size();
    const size_t s2 = v2.v_.size();
    
    assert( result.v_.size() == s1 + s2 );
    
    for(size_t i = 0; i < s1; ++i) {
      result.v_[i] = v1.v_[i];
    }
    
    for(size_t i = 0; i < s2; ++i) {
      result.v_[i+s1] = v2.v_[i];
    }
    
  }
  
};




/** Checks that a plDataValues contains valid values corresponding to a given
    list of variables. Raises a plError if not.
*/
void kpl_validate_data_values(plDataValues const & values,
			      kplVariableList const & variables);


#endif //plDataValues_h

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
