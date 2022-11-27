/*=============================================================================
 * Product        : ProBT 
 * File           : plKMeans.h
 * Author         : Kamel Mekhnacha
 * Creation       : February 2015
 *
 *=============================================================================
 *     (c) Copyright 2015, Probayes SAS
 *=============================================================================
 *
 *------------------------- Description ---------------------------------------
 *   
 *   
 *-----------------------------------------------------------------------------
 */

#ifndef plKMeans_h
#define plKMeans_h

#include "plConfig.h"

#include <plMath.h>
#include <plRandom.h>

/** Template class implementing k-means clustering
    
    \param PointT: The type for data vector (e.g., std::vector<double> for nD dimensional case 
    or 'double' for one-dimensional case)
    
    \param PointTFuncs: a structure implementing  functions:
    - plFloat dist2(const PointT &p1, const PointT &p2) : the square of the distance to be used (e.g., cartesian)
    - void inplace_add(PointT &output, const PointT &inc)
    - void inplace_div(PointT &output, size_t n)
    - void set_to_zero(PointT &output)

    \sa plNdCartesian
    \sa plOneDCartesian
    \sa plKMeansNdCartesian 
    \sa plKMeans1dCartesian
    
    plKMeansNdCartesian and plKMeans1dCartesian are typedefs using this template.
*/
template <typename PointT, typename PointTFuncs>
class plKMeans
{
public:

#ifndef SWIG
  /** Constructor with no copy of the data
      \param[in] k the number of clusters
      \param[in] data_points a pointer to  a vector of data samples (size n)
      \param[in] forgy_initialization true if one wants a Forgy initialization, false
      if one wants a random repartition initialization
  */
  explicit plKMeans(size_t k, const std::vector<PointT> *data_points=0, bool forgy_initialization=true)
    : k_(k)
    , points_(0)
    , assignment_()
    , means_(k)
    , forgy_initialization_(forgy_initialization)
    , counts_(k)
    , data_vector_()
  {
    if(data_points) set_data(data_points);
  }
#endif

  /** Constructor copying the data
      \param[in] k the number of clusters
      \param[in] data_points a vector of data samples (size n)
      \param[in] forgy_initialization true if one wants a Forgy initialization, false
      if one wants a random repartition initialization
  */
  plKMeans(size_t k, const std::vector<PointT> &data_points, bool forgy_initialization=true)
    : k_(k)
    , points_(0)
    , assignment_()
    , means_(k)
    , forgy_initialization_(forgy_initialization)
    , counts_(k)
    , data_vector_(data_points)
  {
    set_data(&data_vector_);
  }

  /** Set/change the sample data points to be used (size n) */
  void set_data(const std::vector<PointT> *data_points)
  {
    assert(data_points);
    
    points_ = data_points;
    assignment_.resize(points_->size());

    initialize();
  }

  /** Run the algorithm until convergence
      
      The results can be retrieved using:
      - get_means(), get_counts(), and get_assignments() or
      - get_mean(), get_count(), and get_assignment()
      
      \return the number of iterations 
  */ 
  size_t run()
  {
    assert(points_);
    assert(assignment_.size() == points_->size());
 
    size_t n = 0;
    bool converged = false;
    while(!converged) {
      update_means();
      converged = assign();
      ++n;
    }

    return n;
  }

  /** Get the resulting k means (cluster centers) */
  inline const std::vector<PointT> &get_means() const
  {
    return means_;
  }

  /** Get the count corresponding to each cluster */
  inline const std::vector<unsigned int> &get_counts() const
  {
    return counts_;
  }
  
  /** Get, for each data point, the id (in {0, .., k-1}) of the cluster to which it has been assigned */
  inline const std::vector<unsigned int> &get_assignments() const
  {
    return assignment_;
  }
  
  /** Run the algorithm until convergence and return the results
      \param[out] means the returned k centers of clusters (size k)
      \param[out] counts the count for each cluster (size k)
      \param[out] assignments the index of the assigned cluster for each data point (size n)

      \return the number of iterations
  */ 
  size_t run(std::vector<PointT> &means, std::vector<unsigned int> &counts, std::vector<unsigned int> &assignments)
  {
    const size_t n = run();

    means = means_;
    assignments = assignment_;
    counts = counts_;

    return n;
  }

  /** Return the number of clusters k */
  inline size_t get_k() const {return k_;}

  /** Get the ith (i in {0, .., k-1}) cluster (mean) */
  inline const PointT &get_mean(size_t i) const
  {
    assert(i < k_);
    return means_[i];
  }

  /** Get the count (number of data points) assigned to the ith (i in {0, .., k-1}) cluster */
  inline unsigned int get_count(size_t i) const
  {
    assert(i < k_);
    return counts_[i];
  }

  /** Get the index (in {0, .., k-1}) of the cluster to which the nth data point has been assigned */
  inline unsigned int get_assignment(size_t n) const
  {
    assert(points_);
    assert(n < points_->size());
    return assignment_[n];
  }

  /** Get a pointer to the data points */
  inline const std::vector<PointT> *get_data() const
  {
    return points_;
  }
  
  /** Get the number of data samples */
  inline size_t get_n_data() const
  {
    if(!points_) return 0;
    return points_->size();
  }
  
  /** Get the nth data point */
  inline const PointT &get_data(size_t n) const
  {
    assert(points_);
    assert(n < points_->size());
    return (*points_)[n];
  }
  
  /** Only for Python: Plot the clustering results */
  void python_plot_results()
  {
    THROW_PL_ERROR(89, "plKMeans.python_plot_results() is only available for Python");
  }
  
private:
  void initialize()
  {
    assert(points_);
    assert(!points_->empty());
    
    if(forgy_initialization_) {// Forgy method
      for(size_t i = 0; i < k_; ++i) {
        means_[i] = (*points_)[size_t(plRandomInt( (unsigned int)points_->size()) ) ];
      }

      assign();
    }
    else {// Random Partition method
      for(size_t i = 0; i < k_; ++i) {
        // just get the right size
        means_[i] = (*points_)[0];
      }
      
      for(size_t p = 0; p < points_->size(); ++p) {
        assignment_[p] = plRandomInt((unsigned int)k_);
      }
    }
  }

  bool assign()
  {
    bool converged = true;
    for(size_t p = 0; p < points_->size(); ++p) {
      plFloat min_dist = std::numeric_limits<plFloat>::infinity();
      size_t best_mean = 0;
      for(size_t m = 0; m < k_; ++m) {
        const plFloat d = PointTFuncs::dist2(means_[m], (*points_)[p]);
        if(d <= min_dist) {
          min_dist = d;
          best_mean = m;
        }
      }
      if(assignment_[p] != best_mean) {
        converged = false;
        assignment_[p] = best_mean;
      }
    }

    return converged;
  }

  void update_means()
  {
    counts_.assign(k_, 0);
    
    for(size_t m = 0; m < k_; ++m) {
      PointTFuncs::set_to_zero(means_[m]);
    }
    
    for(size_t p = 0; p < points_->size(); ++p) {
      const size_t assigned = assignment_[p];
      PointTFuncs::inplace_add(means_[assigned], (*points_)[p]);
      ++counts_[ assigned ];
    }
    
    for(size_t m = 0; m < k_; ++m) {
      if(counts_[m] > 0) {
        PointTFuncs::inplace_div(means_[m], counts_[m]);
      }
    }
     
  }

  
private:
  size_t k_;
  const std::vector<PointT> *points_;
  std::vector<unsigned int> assignment_;
  std::vector<PointT> means_;
  bool forgy_initialization_;
  std::vector<unsigned int> counts_;
  std::vector<PointT> data_vector_;
};

/** Structure implementing the k-means utility functions for the 1D case using cartesian distance
    \sa plKMeans
    \sa plNdCartesian
*/
struct plOneDCartesian
{
  /** Cartesian distance */
  static inline plFloat dist2(plFloat p1, plFloat p2) 
  {
    return (p1-p2)*(p1-p2);
  }

  /** Add a point to another */
  static inline void inplace_add(plFloat &output, plFloat inc)
  {
    output += inc;
  }

  /** Divide a point by n */
  static inline void inplace_div(plFloat &output, size_t n)
  {
    assert(n > 0);
    output /= plFloat(n);
  }

  /** Set the point to zero */
  static inline void set_to_zero(plFloat &output)
  {
    output = PL_ZERO;
  }

};

/** Structure implementing the k-means utility functions for the nD case using cartesian distance
    \sa plKMeans
    \sa plOneDCartesian
*/
template <typename vecT>
struct plNdCartesian
{

  /** Cartesian distance */
  static plFloat dist2(const vecT &p1, const vecT &p2) 
  {
    assert(p1.size() == p2.size());
    
    plFloat dist = PL_ZERO;
    for(size_t i = 0; i < p1.size(); ++i) {
      const plFloat diff = p1[i] - p2[i];
      dist += (diff*diff);
    }
    
    return dist;
  }

  /** Add a vector point to another */
  static void inplace_add(vecT &output, const vecT &inc)
  {
    assert(output.size() == inc.size());
    
    for(size_t i = 0; i < output.size(); ++i) {
      output[i] += inc[i];
    }
  }

  /** Divide a vector point by n */
  static void inplace_div(vecT &output, size_t n)
  {
    assert(n > 0);
    
    for(size_t i = 0; i < output.size(); ++i) {
      output[i] /= plFloat(n);
    }
  }

  /** Set the vector point to zero */
  static void set_to_zero(vecT &output)
  {
    for(size_t i = 0; i < output.size(); ++i) {
      output[i] = PL_ZERO;
    }
  }

};

/** k-means for Nd case using cartesian distance */
typedef plKMeans<std::vector<plFloat>, plNdCartesian<std::vector<plFloat> > > plKMeansNdCartesian;

/** k-means for 1d case using cartesian distance */
typedef plKMeans<plFloat, plOneDCartesian> plKMeans1dCartesian;

#endif

// Local Variables:
// mode: c++
// indent-tabs-mode: nil
// c-basic-offset: 2
// fill-column: 79
// End:
