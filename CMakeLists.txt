cmake_minimum_required(VERSION 2.9)
project(gsl1 C CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GSL REQUIRED)

set(GSL_FIT_DIR ./include/gsl-curve-fit/)
include_directories(${GSL_FIT_DIR})

set(LIBRARIES "GSL::gsl;GSL::gslcblas")

set(GSL_DIR "C:/soft/gsl-inst/include/")

file(GLOB sources *.cpp)
file(GLOB headers *.h)

add_executable(gsl1 main.cpp ${GSL_FIT_DIR}/curve_fit.cpp)
target_link_libraries(gsl1 ${LIBRARIES} )
target_include_directories(gsl1 PUBLIC ${GSL_DIR})
