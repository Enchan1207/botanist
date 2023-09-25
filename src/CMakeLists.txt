#
# botanist sources
#

cmake_minimum_required(VERSION 3.10)

## configure library
add_library(botanist)
target_sources(botanist PRIVATE
    tokenizer/tokenize.cpp
    tokenizer/func.cpp
    tokenizer/dump.cpp
    analyser/analyse.cpp
    analyser/func.cpp
    analyser/dump.cpp
    serializer/serialize.cpp
    serializer/dump.cpp
    evaluator/double.cpp
)
target_include_directories(botanist PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(botanist PRIVATE
    -Wall
)
target_link_libraries(botanist PUBLIC
    collection2 
)
