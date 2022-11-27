/*=============================================================================
 * Product        : ProBT 
 * File           : plPCA.h
 * Author         : Kamel Mekhnacha
 * Creation       : Feb 2015
 *
 *=============================================================================
 *        (c) Copyright 2015, ProBayes SAS  -  all rights reserved
 *=============================================================================
 */

#ifndef plPCA_H
#define plPCA_H

#include <vector>

#include "plConfig.h"

#include <plObject.h>
#include <plFloatMatrix.h>
#include <plFloatVector.h>
#include <plVariablesConjunction.h>

class plDataDescriptor;
class plLearnNdNormal;
class plValues;

/** Principal component analysis (PCA) 

    Linear dimensionality reduction using eigenvalue decomposition of the
    covariance matrix and keeping only the most significant eigen vectors to
    project the data to a lower dimensional space
*/
class PL_DLL_API plPCA : public plObject
{
 public:

  /** Default constructor */
  plPCA();

  /** Constructor using a set of variables */
  explicit plPCA(const plVariablesConjunction &variables);

  /** Fit the model with data */
  void fit(plDataDescriptor &data);

  /** Fit the model with data */
  inline void fit(const std::vector< std::vector<double> > &data)
  {
    fit_impl(data);
  }
  /** Fit the model with data */
  inline void fit(const std::vector< std::vector<float> > &data)
  {
    fit_impl(data);
  }
  /** Fit the model with data */
  inline void fit(const std::vector< std::vector<long double> > &data)
  {
    fit_impl(data);
  }
  /** Fit the model with data */
  inline void fit(const std::vector< plFloatVector > &data)
  {
    fit_impl(data);
  }
  /** Fit the model with data */
  inline void fit(const std::vector< plValues > &data)
  {
    fit_impl(data);
  }
  
  /** Components with maximum variance.
      If nc (number of components) is 0 then all components are returned 
  */
  plFloatMatrix components(size_t nc = 0) const;

  /** Percentage of variance explained by each of the selected components.

      If nc (number of components) is 0 then all components are returned and the sum of explained
      variances is equal to 1.0. 
  */
  plFloatVector explained_variance_ratio(size_t nc = 0) const;
  
  /** Explainded variance (eigenvalues) for the selected components
      If nc (number of components) is 0 then all eigenvalues are returned
  */
  plFloatVector explained_variance(size_t nc = 0) const;
  
  /** Return the number of components allowing to explain a given ratio (in [0, 1]) of the variance */
  size_t get_n_components_for_explained_variance(plFloat ratio);

  /** Return the number of components using Minka's MLE to guess the dimension.
   
      Based on 'Thomas P. Minka: Automatic Choice of Dimensionality for PCA. NIPS 2000: 598-604'
  */
  size_t get_n_components_mle() const;

  /** Per-feature empirical mean, estimated from the training set */
  plFloatVector data_mean() const;

  /** Empirical variance, estimated from the training set */
  plFloatMatrix data_variance() const;

  /** Compute data covariance with the generative model unsing 'nc' components

      variance = components(nc).T * S**2 * components(cn) + noise_variance(nc) * eye(n_features) where  S**2 contains the explained variances 
      explained_variance(nc) as diagonal
  */
  plFloatMatrix variance(size_t nc) const;

  /** Noise variance given 'nc' the number of components */
  plFloat noise_variance(size_t nc) const;
  
  /** Apply the dimensionality reduction on 'point' by keeping only the 'nc' principal components */
  inline std::vector<double> transform_point(const std::vector<double> &point, size_t nc = 0) const
  {
    return transform_point_impl_ret(point, nc);
  }
  /** Apply the dimensionality reduction on 'point' by keeping only the 'nc' principal components */
  inline std::vector<float> transform_point(const std::vector<float> &point, size_t nc = 0) const
  {
    return transform_point_impl_ret(point, nc);
  }
  /** Apply the dimensionality reduction on 'point' by keeping only the 'nc' principal components */
  inline std::vector<long double> transform_point(const std::vector<long double> &point, size_t nc = 0) const
  {
    return transform_point_impl_ret(point, nc);
  }
  /** Apply the dimensionality reduction on 'point' by keeping only the 'nc' principal components */
  inline plFloatVector transform_point(const plFloatVector &point, size_t nc = 0) const
  {
    return transform_point_impl_ret(point, nc);
  }

  /** Apply the dimensionality reduction on the set 'points' of points by keeping only the 'nc' principal components */
  inline std::vector< std::vector<double> > transform_points(const std::vector<std::vector<double> > &points, size_t nc = 0) const
  {
    return transform_points_impl(points, nc);
  }
  /** Apply the dimensionality reduction on the set 'points' of points by keeping only the 'nc' principal components */
  inline std::vector< std::vector<float> > transform_points(const std::vector<std::vector<float> > &points, size_t nc = 0) const
  {
    return transform_points_impl(points, nc);
  }
  /** Apply the dimensionality reduction on the set 'points' of points by keeping only the 'nc' principal components */
  inline std::vector< std::vector<long double> > transform_points(const std::vector<std::vector<long double> > &points, size_t nc = 0) const
  {
    return transform_points_impl(points, nc);
  }
  /** Apply the dimensionality reduction on the set 'points' of points by keeping only the 'nc' principal components */
  inline std::vector< plFloatVector > transform_points(const std::vector<plFloatVector> &points, size_t nc = 0) const
  {
    return transform_points_impl(points, nc);
  }

  /** Transform data back to its original space, i.e., return an input 'point_original' whose transform would be 'transformed_point' */
  inline std::vector<double> inverse_transform_point(const std::vector<double> &transformed_point) const
  {
    return inverse_transform_point_impl_ret(transformed_point);
  }
  /** Transform data back to its original space, i.e., return an input 'point_original' whose transform would be 'transformed_point' */
  inline std::vector<float> inverse_transform_point(const std::vector<float> &transformed_point) const
  {
    return inverse_transform_point_impl_ret(transformed_point);
  }
  /** Transform data back to its original space, i.e., return an input 'point_original' whose transform would be 'transformed_point' */
  inline std::vector<long double> inverse_transform_point(const std::vector<long double> &transformed_point) const
  {
    return inverse_transform_point_impl_ret(transformed_point);
  }
  /** Transform data back to its original space, i.e., return an input 'point_original' whose transform would be 'transformed_point' */
  inline plFloatVector inverse_transform_point(const plFloatVector &transformed_point) const
  {
    return inverse_transform_point_impl_ret(transformed_point);
  }

  /** Transform data back to its original space, i.e., return an input 'point_originals' whose transform would be 'transformed_points' */
  inline std::vector<std::vector<double> > inverse_transform_points(const std::vector<std::vector<double> > &transformed_points) const
  {
    return inverse_transform_points_impl(transformed_points);
  }
  /** Transform data back to its original space, i.e., return an input 'point_originals' whose transform would be 'transformed_points' */
  inline std::vector<std::vector<float> > inverse_transform_points(const std::vector<std::vector<float> > &transformed_points) const
  {
    return inverse_transform_points_impl(transformed_points);
  }
  /** Transform data back to its original space, i.e., return an input 'point_originals' whose transform would be 'transformed_points' */
  inline std::vector<std::vector<long double> > inverse_transform_points(const std::vector<std::vector<long double> > &transformed_points) const
  {
    return inverse_transform_points_impl(transformed_points);
  }
  /** Transform data back to its original space, i.e., return an input 'point_originals' whose transform would be 'transformed_points' */
  inline std::vector<plFloatVector> inverse_transform_points(const std::vector<plFloatVector> &transformed_points) const
  {
    return inverse_transform_points_impl(transformed_points);
  }
  
 private:
  template<typename T>
    void fit_impl(const std::vector<T > &points);

  template<typename T>
    void transform_point_impl(const T &point, size_t nc, T&transformed_point) const;
 
  template<typename T>
    T transform_point_impl_ret(const T &point, size_t nc) const;
  
  template<typename T>
    std::vector<T > transform_points_impl(const std::vector<T > &points, size_t) const;

  template<typename T>
    void inverse_transform_point_impl(const T &transformed_point, T&point) const;
  template<typename T>
    T inverse_transform_point_impl_ret(const T &transformed_point) const;

  template<typename T>
    std::vector<T> inverse_transform_points_impl(const std::vector<T > &transformed_points) const;

  void set_parameters(const plLearnNdNormal &LPX);

 protected:
  void Output(std::ostream &out) const;

  plFloat assess_dimension(size_t rank) const;
  
 protected:
  plFloatMatrix eigen_matrix_;
  plFloatVector eigen_values_;
  plFloatVector mean_;
  plFloatMatrix variance_;
  plVariablesConjunction variables_;
  bool is_initialized_;
  mutable plFloatVector tmp_vector_n_1_;
  mutable plFloatVector tmp_vector_n_2_;
  size_t n_samples_;
};

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
