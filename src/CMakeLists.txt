#
# botanist sources
#

cmake_minimum_required(VERSION 3.10)

## configure library
add_library(botanist)
target_sources(botanist PRIVATE
    tokenizer/tokenize.cpp
    tokenizer/func.cpp
    analyser/analyse.cpp
    analyser/func.cpp
    serializer/serialize.cpp
    evaluator/double.cpp
)

if(!FORMULA_PARSER_HEADLESS)
    target_sources(botanist PRIVATE
        tokenizer/dump.cpp
        serializer/dump.cpp
        analyser/dump.cpp
    )
endif()

target_include_directories(botanist PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(botanist PRIVATE
    -Wall
)
if(FORMULA_PARSER_HEADLESS)
    target_compile_definitions(botanist PRIVATE
        HEADLESS=1
    )
endif()
target_link_libraries(botanist PUBLIC
    collection2 
)
