#
# formula_parser sources
#

cmake_minimum_required(VERSION 3.0)

## add external dependencies here

## configure executable
add_executable(formula_parser)
target_sources(formula_parser PRIVATE
    main.cpp
    tokenize.cpp
)
target_include_directories(formula_parser PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(formula_parser PRIVATE
    -Wall
)
