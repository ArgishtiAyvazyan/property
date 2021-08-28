cmake_minimum_required(VERSION 3.16)
project(property_lib)

set(CMAKE_CXX_STANDARD 20)

########################################################################################################################
# Unit tests.
########################################################################################################################
IF (BUILD_UNIT_TESTS)
    add_subdirectory("tests")
ENDIF ()

add_subdirectory("include")
