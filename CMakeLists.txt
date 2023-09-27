#
# botanist
#
cmake_minimum_required(VERSION 3.10)

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
    GIT_TAG v0.5.0
)
FetchContent_MakeAvailable(collection2)

#
# build option
#
option(FORMULA_PARSER_HEADLESS "Headless build (avoid stdio operation)" OFF)
if(FORMULA_PARSER_HEADLESS)
    message(NOTICE "Headless build enabled")
endif()

#
# subdirectories
#
add_subdirectory(src)
if(NOT FORMULA_PARSER_HEADLESS)
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
    
    # If you use botanist on AVR microcontroller
    # using Enchan1207/cmake-avr, tests_avr will be added instead of tests.
    if(DEFINED BUILD_FOR_AVR AND BUILD_FOR_AVR)
        add_subdirectory(tests_avr)
    else()
        add_subdirectory(tests)
    endif()
endif()
