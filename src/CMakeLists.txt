#
# formula_parser sources
#

cmake_minimum_required(VERSION 3.0)

## configure library
add_library(formula_parser)
target_sources(formula_parser PRIVATE
    tokenizer/parse_elements.cpp
    tokenizer/tokenize.cpp
    parser/parser.cpp
    analyser/analyse.cpp
    analyser/func.cpp
    analyser/dump.cpp
    serializer/serialize.cpp
    serializer/dump.cpp
    evaluator/operate.cpp
)
target_include_directories(formula_parser PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(formula_parser PRIVATE
    -Wall
)
target_link_libraries(formula_parser PRIVATE
    collection2 
)
