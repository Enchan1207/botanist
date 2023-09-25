#
# botanist
#
cmake_minimum_required(VERSION 3.0)

#
# project configuration
#
project(botanist
    VERSION 0.1.0
    DESCRIPTION "parse and evaluate formula"
    LANGUAGES C CXX
)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# global dependencies
#
include(FetchContent)
FetchContent_Declare(
    collection2
    GIT_REPOSITORY https://github.com/Enchan1207/collection2
    GIT_TAG v0.4.0
)
FetchContent_MakeAvailable(collection2)

#
# subdirectories
#
add_subdirectory(src)

option(FORMULA_PARSER_BUILD_CLI "Enable testing of botanist" OFF)
if(FORMULA_PARSER_BUILD_CLI)
    message(NOTICE "CLI application source included")
    add_subdirectory(cli)
endif()

#
# unit testing configuration
#
option(FORMULA_PARSER_BUILD_TESTS "Enable testing of botanist" OFF)
if(FORMULA_PARSER_BUILD_TESTS)
    set(INSTALL_GTEST OFF)
    enable_testing()
    message(NOTICE "botanist: unit testing is enabled")
    
    add_subdirectory(tests)
endif()
