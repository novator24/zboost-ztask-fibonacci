cmake_minimum_required(VERSION 2.8)
project(rnd_module_magna_04)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif(NOT CMAKE_BUILD_TYPE)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wunused")

set(BOOST_COMPONENTS thread system)
find_package(BoostThread)
find_package(BoostSystem)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/zboost.ztask/)
include_directories(${BoostThread_INCLUDE_DIRS})
include_directories(${BoostSystem_INCLUDE_DIRS})

subdirs(libs)
subdirs(src)
