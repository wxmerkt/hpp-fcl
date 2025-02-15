#
# Software License Agreement (BSD License)
#
#  Copyright (c) 2014-2020 CNRS-LAAS, INRIA
#  Author: Florent Lamiraux, Joseph Mirabel
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#   * Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#   * Redistributions in binary form must reproduce the above
#     copyright notice, this list of conditions and the following
#     disclaimer in the documentation and/or other materials provided
#     with the distribution.
#   * Neither the name of CNRS-LAAS. nor the names of its
#     contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.1)
set(CXX_DISABLE_WERROR TRUE)

set(PROJECT_NAME hpp-fcl)
set(PROJECT_DESCRIPTION
  "HPP fork of FCL -- The Flexible Collision Library"
  )
SET(PROJECT_USE_CMAKE_EXPORT TRUE)

# Do not support CMake older than 2.8.12
CMAKE_POLICY(SET CMP0022 NEW)
SET(PROJECT_USE_KEYWORD_LINK_LIBRARIES TRUE)
SET(DOXYGEN_USE_TEMPLATE_CSS TRUE)

# Check if the submodule cmake have been initialized
IF(NOT EXISTS "${CMAKE_SOURCE_DIR}/cmake/base.cmake")
  MESSAGE(FATAL_ERROR "\nPlease run the following command first:\ngit submodule update --init\n")
ENDIF()

include(cmake/boost.cmake)
include(cmake/python.cmake)
include(cmake/hpp.cmake)
include(cmake/apple.cmake)
include(cmake/ide.cmake)

SET(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake-modules/
  ${CMAKE_MODULE_PATH})

# If needed, fix CMake policy for APPLE systems
APPLY_DEFAULT_APPLE_CONFIGURATION()

OPTION(BUILD_PYTHON_INTERFACE "Build the python bindings" ON)

COMPUTE_PROJECT_ARGS(PROJECT_ARGS LANGUAGES CXX)
PROJECT(${PROJECT_NAME} ${PROJECT_ARGS})

ADD_PROJECT_DEPENDENCY(Eigen3 REQUIRED PKG_CONFIG_REQUIRES "eigen3 >= 3.0.0")

if(BUILD_PYTHON_INTERFACE)
  FIND_PACKAGE(eigenpy 2.2 REQUIRED)
endif()

# Required dependencies
SET_BOOST_DEFAULT_OPTIONS()
EXPORT_BOOST_DEFAULT_OPTIONS()
IF(WIN32)
  ADD_PROJECT_DEPENDENCY(Boost REQUIRED COMPONENTS chrono thread date_time serialization)
ELSE(WIN32)
  ADD_PROJECT_DEPENDENCY(Boost REQUIRED chrono serialization)
ENDIF(WIN32)
if (BUILD_PYTHON_INTERFACE)
  FINDPYTHON()
  search_for_boost_python(REQUIRED)
  find_package(Boost REQUIRED COMPONENTS system)
endif ()

# Optional dependencies
ADD_PROJECT_DEPENDENCY(octomap PKG_CONFIG_REQUIRES "octomap >= 1.6")
if(octomap_FOUND)
  SET(HPP_FCL_HAS_OCTOMAP TRUE)
  string(REPLACE "." ";" VERSION_LIST ${octomap_VERSION})
  list(GET VERSION_LIST 0 OCTOMAP_MAJOR_VERSION)
  list(GET VERSION_LIST 1 OCTOMAP_MINOR_VERSION)
  list(GET VERSION_LIST 2 OCTOMAP_PATCH_VERSION)
  message(STATUS "FCL uses Octomap")
else()
  SET(HPP_FCL_HAS_OCTOMAP FALSE)
  message(STATUS "FCL does not use Octomap")
endif()

option(HPP_FCL_HAS_QHULL "use qhull library to compute convex hulls." FALSE)
if(HPP_FCL_HAS_QHULL)
  if(DEFINED CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD EQUAL 98)
    message(FATAL_ERROR "Cannot use qhull library with C++ < 11.\nYou may add -DCMAKE_CXX_STANDARD=11")
  endif()
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/third-parties)
  execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_SOURCE_DIR}/third-parties/qhull/src/libqhullcpp
    ${CMAKE_CURRENT_BINARY_DIR}/third-parties/libqhullcpp
    )
  set(Qhullcpp_PREFIX ${CMAKE_BINARY_DIR}/third-parties)
  find_path(Qhull_r_INCLUDE_DIR
    NAMES libqhull_r/libqhull_r.h
    PATHS ${Qhull_PREFIX}
    )
  find_library(Qhull_r_LIBRARY
    NAMES libqhull_r.so
    PATHS ${Qhull_PREFIX}
    )
endif()

FIND_PACKAGE(assimp REQUIRED)

SET(${PROJECT_NAME}_HEADERS
  include/hpp/fcl/collision_data.h
  include/hpp/fcl/BV/kIOS.h
  include/hpp/fcl/BV/BV.h
  include/hpp/fcl/BV/RSS.h
  include/hpp/fcl/BV/OBBRSS.h
  include/hpp/fcl/BV/BV_node.h
  include/hpp/fcl/BV/AABB.h
  include/hpp/fcl/BV/OBB.h
  include/hpp/fcl/BV/kDOP.h
  include/hpp/fcl/narrowphase/narrowphase.h
  include/hpp/fcl/narrowphase/gjk.h
  include/hpp/fcl/shape/convex.h
  include/hpp/fcl/shape/details/convex.hxx
  include/hpp/fcl/shape/geometric_shape_to_BVH_model.h
  include/hpp/fcl/shape/geometric_shapes.h
  include/hpp/fcl/shape/geometric_shapes_utility.h
  include/hpp/fcl/distance_func_matrix.h
  include/hpp/fcl/collision.h
  include/hpp/fcl/collision_func_matrix.h
  include/hpp/fcl/distance.h
  include/hpp/fcl/math/matrix_3f.h
  include/hpp/fcl/math/vec_3f.h
  include/hpp/fcl/math/types.h
  include/hpp/fcl/math/transform.h
  include/hpp/fcl/data_types.h
  include/hpp/fcl/BVH/BVH_internal.h
  include/hpp/fcl/BVH/BVH_model.h
  include/hpp/fcl/BVH/BVH_front.h
  include/hpp/fcl/BVH/BVH_utility.h
  include/hpp/fcl/collision_object.h
  include/hpp/fcl/collision_utility.h
  include/hpp/fcl/octree.h
  include/hpp/fcl/fwd.hh
  include/hpp/fcl/mesh_loader/assimp.h
  include/hpp/fcl/mesh_loader/loader.h
  include/hpp/fcl/internal/BV_fitter.h
  include/hpp/fcl/internal/BV_splitter.h
  include/hpp/fcl/internal/intersect.h
  include/hpp/fcl/internal/tools.h
  include/hpp/fcl/internal/traversal_node_base.h
  include/hpp/fcl/internal/traversal_node_bvh_shape.h
  include/hpp/fcl/internal/traversal_node_bvhs.h
  include/hpp/fcl/internal/traversal_node_octree.h
  include/hpp/fcl/internal/traversal_node_setup.h
  include/hpp/fcl/internal/traversal_node_shapes.h
  include/hpp/fcl/internal/traversal_recurse.h
  include/hpp/fcl/internal/traversal.h
  include/hpp/fcl/serialization/fwd.h
  include/hpp/fcl/serialization/AABB.h
  include/hpp/fcl/serialization/BV_node.h
  include/hpp/fcl/serialization/BV_splitter.h
  include/hpp/fcl/serialization/BVH_model.h
  include/hpp/fcl/serialization/collision_data.h
  include/hpp/fcl/serialization/collision_object.h
  include/hpp/fcl/serialization/eigen.h
  include/hpp/fcl/serialization/OBB.h
  include/hpp/fcl/serialization/RSS.h
  include/hpp/fcl/serialization/OBBRSS.h
  include/hpp/fcl/timings.h
  )

add_subdirectory(doc)
add_subdirectory(src)
if (BUILD_PYTHON_INTERFACE)
  add_subdirectory(python)
endif ()
add_subdirectory(test)

pkg_config_append_libs("hpp-fcl")
IF(HPP_FCL_HAS_OCTOMAP)
  # FCL_HAVE_OCTOMAP kept for backward compatibility reasons.
  PKG_CONFIG_APPEND_CFLAGS(
    "-DHPP_FCL_HAS_OCTOMAP -DHPP_FCL_HAVE_OCTOMAP -DFCL_HAVE_OCTOMAP -DOCTOMAP_MAJOR_VERSION=${OCTOMAP_MAJOR_VERSION} -DOCTOMAP_MINOR_VERSION=${OCTOMAP_MINOR_VERSION} -DOCTOMAP_PATCH_VERSION=${OCTOMAP_PATCH_VERSION}")
ENDIF(HPP_FCL_HAS_OCTOMAP)

# Install catkin package.xml
INSTALL(FILES package.xml DESTINATION share/${PROJECT_NAME})
