#
# formula_parser
#

#
# project configuration
#
cmake_minimum_required(VERSION 3.0)
project(formula_parser
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
    GIT_TAG v0.3.0
)
FetchContent_MakeAvailable(collection2)

#
# subdirectories
#
add_subdirectory(src)
add_subdirectory(cli)
