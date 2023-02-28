#
# formula_parser
#
cmake_minimum_required(VERSION 3.0)
project(formula_parser
    VERSION 0.1.0
    DESCRIPTION "parse and evaluate formula"
    LANGUAGES C CXX
)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(src)
