#
# formula_parser sources
#

cmake_minimum_required(VERSION 3.0)

# dependency
include(FetchContent)
FetchContent_Declare(
    collection2
    GIT_REPOSITORY https://github.com/Enchan1207/collection2
    GIT_TAG v0.1.0
)
FetchContent_MakeAvailable(collection2)

## configure executable
add_executable(formula_parser)
target_sources(formula_parser PRIVATE
    main.cpp
    parse_elements.cpp
    tokenize.cpp
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
