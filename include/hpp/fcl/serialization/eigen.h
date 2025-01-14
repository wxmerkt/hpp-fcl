//
// Copyright (c) 2017-2021 CNRS INRIA
//

/*
 Code adapted from Pinocchio and https://gist.githubusercontent.com/mtao/5798888/raw/5be9fa9b66336c166dba3a92c0e5b69ffdb81501/eigen_boost_serialization.hpp
 Copyright (c) 2015 Michael Tao
*/

#ifndef HPP_FCL_SERIALIZATION_EIGEN_H
#define HPP_FCL_SERIALIZATION_EIGEN_H

#include <Eigen/Dense>

#include <boost/serialization/split_free.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/array.hpp>


namespace boost
{
  namespace serialization
  {
    
#ifndef HPP_FCL_SKIP_EIGEN_BOOST_SERIALIZATION
  
    template <class Archive, typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    void save(Archive & ar, const Eigen::Matrix<Scalar,Rows,Cols,Options,MaxRows,MaxCols> & m, const unsigned int /*version*/)
    {
      Eigen::DenseIndex rows(m.rows()), cols(m.cols());
      if (Rows == Eigen::Dynamic)
        ar & BOOST_SERIALIZATION_NVP(rows);
      if (Cols == Eigen::Dynamic)
        ar & BOOST_SERIALIZATION_NVP(cols);
      ar & make_nvp("data",make_array(m.data(), (size_t)m.size()));
    }
    
    template <class Archive, typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    void load(Archive & ar, Eigen::Matrix<Scalar,Rows,Cols,Options,MaxRows,MaxCols> & m, const unsigned int /*version*/)
    {
      Eigen::DenseIndex rows = Rows, cols = Cols;
      if (Rows == Eigen::Dynamic)
        ar >> BOOST_SERIALIZATION_NVP(rows);
      if (Cols == Eigen::Dynamic)
        ar >> BOOST_SERIALIZATION_NVP(cols);
      m.resize(rows,cols);
      ar >> make_nvp("data",make_array(m.data(), (size_t)m.size()));
    }
    
    template <class Archive, typename Scalar, int Rows, int Cols, int Options, int MaxRows, int MaxCols>
    void serialize(Archive & ar, Eigen::Matrix<Scalar,Rows,Cols,Options,MaxRows,MaxCols> & m, const unsigned int version)
    {
      split_free(ar,m,version);
    }
  
    template <class Archive, typename PlainObjectBase, int MapOptions, typename StrideType>
    void save(Archive & ar, const Eigen::Map<PlainObjectBase,MapOptions,StrideType> & m, const unsigned int /*version*/)
    {
      Eigen::DenseIndex rows(m.rows()), cols(m.cols());
      if (PlainObjectBase::RowsAtCompileTime == Eigen::Dynamic)
        ar & BOOST_SERIALIZATION_NVP(rows);
      if (PlainObjectBase::ColsAtCompileTime == Eigen::Dynamic)
        ar & BOOST_SERIALIZATION_NVP(cols);
      ar & make_nvp("data",make_array(m.data(), (size_t)m.size()));
    }
  
    template <class Archive, typename PlainObjectBase, int MapOptions, typename StrideType>
    void load(Archive & ar, Eigen::Map<PlainObjectBase,MapOptions,StrideType> & m, const unsigned int /*version*/)
    {
      Eigen::DenseIndex rows = PlainObjectBase::RowsAtCompileTime, cols = PlainObjectBase::ColsAtCompileTime;
      if (PlainObjectBase::RowsAtCompileTime == Eigen::Dynamic)
        ar >> BOOST_SERIALIZATION_NVP(rows);
      if (PlainObjectBase::ColsAtCompileTime == Eigen::Dynamic)
        ar >> BOOST_SERIALIZATION_NVP(cols);
      m.resize(rows,cols);
      ar >> make_nvp("data",make_array(m.data(), (size_t)m.size()));
    }
  
    template <class Archive, typename PlainObjectBase, int MapOptions, typename StrideType>
    void serialize(Archive & ar, Eigen::Map<PlainObjectBase,MapOptions,StrideType> & m, const unsigned int version)
    {
      split_free(ar,m,version);
    }
  
#endif // ifned HPP_FCL_SKIP_EIGEN_BOOST_SERIALIZATION
  }
}

#endif // ifndef HPP_FCL_SERIALIZATION_EIGEN_H
