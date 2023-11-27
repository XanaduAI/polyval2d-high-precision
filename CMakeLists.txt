project(polyval2d-high-precision LANGUAGES CXX)
cmake_minimum_required(VERSION 3.17)

set(CMAKE_EXPORT_COMPILE_COMMANDS "ON")
set(CMAKE_CXX_STANDARD 20)

option(ENABLE_PYTHON "Enable python binding" ON)
option(ENABLE_WARNINGS "Enable all warnings" ON)
option(BUILD_TESTS OFF)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

add_library(polyval2d_high_precision_compile_options INTERFACE)
if(ENABLE_WARNINGS)
    target_compile_options(polyval2d_high_precision_compile_options INTERFACE -Wall -Wextra)
endif()


################################################################################
#                                Set dependencies
################################################################################
include(FetchContent)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_library(polyval2d_high_precision_external_libs INTERFACE)

# Add MPFR
find_package(MPFR REQUIRED)

target_include_directories(polyval2d_high_precision_external_libs INTERFACE ${MPFR_INCLUDE_DIRS})
target_link_libraries(polyval2d_high_precision_external_libs INTERFACE ${MPFR_LIBRARIES})

# Add GMP
find_package(GMP REQUIRED)
message(STATUS "Found GMP; GMP_INCLUDES = ${GMP_INCLUDES}, GMP_LIBRARIES=${GMP_LIBRARIES}")
target_link_libraries(polyval2d_high_precision_external_libs INTERFACE ${GMP_LIBRARIES})
target_include_directories(polyval2d_high_precision_external_libs INTERFACE ${GMP_INCLUDES})

set(BOOST_MP_STANDALONE ON)
set(BOOST_MATH_STANDALONE ON)

FetchContent_Declare(
    boost_multiprecision
    GIT_REPOSITORY https://github.com/boostorg/multiprecision.git
    GIT_TAG        boost-1.79.0
)
FetchContent_MakeAvailable(boost_multiprecision)

FetchContent_Declare(
    boost_math
    GIT_REPOSITORY https://github.com/boostorg/math.git
    GIT_TAG        boost-1.79.0
)
FetchContent_MakeAvailable(boost_math)

FetchContent_Declare(
    boost_config
    GIT_REPOSITORY https://github.com/boostorg/config.git
    GIT_TAG        boost-1.79.0
)
FetchContent_MakeAvailable(boost_config)
target_link_libraries(polyval2d_high_precision_external_libs INTERFACE boost_config boost_math boost_multiprecision)


if(ENABLE_PYTHON)
    find_package(Python COMPONENTS Interpreter Development)
    FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG        v2.11.1
    )
    FetchContent_MakeAvailable(pybind11)
endif()

if(ENABLE_PYTHON)
    pybind11_add_module(_polyval2d_binding src/binding.cpp)
    target_link_libraries(_polyval2d_binding PUBLIC polyval2d_high_precision_compile_options polyval2d_high_precision_external_libs)
    set_target_properties(_polyval2d_binding PROPERTIES CXX_VISIBILITY_PRESET hidden)
endif()

add_executable(test "src/test.cpp")
target_link_libraries(test PUBLIC polyval2d_high_precision_external_libs)
#target_include_directories(test  PUBLIC "src")
