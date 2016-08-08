project(icfp2017)

# ----------------------------------------------------------------------
# cmake settings

cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

enable_testing()

# ----------------------------------------------------------------------
# Set include directories, c++ options, etc.

function(peria_add_cxx_flags flags)
  if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
  else()
    set(CMAKE_CXX_FLAGS "${flags} ${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
  endif()
endfunction()

peria_add_cxx_flags("-std=c++11 -Wall")
if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  peria_add_cxx_flags("-O0")
else()
  peria_add_cxx_flags("-O2")
endif()

include_directories(src)
include_directories(.)

# ----------------------------------------------------------------------
# Library settings
add_subdirectory(third_party/googletest/googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_subdirectory(third_party/gflags)
set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" third_party/gflags)

add_subdirectory(third_party/glog)
include_directories(${GLOG_INCLUDE_DIRS})

# ----------------------------------------------------------------------
# Functions
function(peria_add_library target)
  add_library(peria_${target} ${ARGN})
  target_link_libraries(peria_${target} ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(peria_${target} gflags glog)
endfunction()

function(peria_add_executable target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(${target} gflags glog)
endfunction()

# ----------------------------------------------------------------------
# Define test dependencies
function(peria_test target)
  peria_add_executable(${target}_test ${ARGN})
  target_link_libraries(${target}_test gtest gtest_main)
  add_test(${target}_test ${target}_test)
endfunction()

# ----------------------------------------------------------------------
# Display settings
message(STATUS "Build type:" ${CMAKE_BUILD_TYPE})

# ----------------------------------------------------------------------
# Add subdirectories
add_subdirectory(ai)
add_subdirectory(core)
add_subdirectory(server)



