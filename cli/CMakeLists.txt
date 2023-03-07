#
# cli
#

cmake_minimum_required(VERSION 3.0)

## usageを含めたサンプル
add_executable(main)
target_sources(main PRIVATE
    main.cpp
)
target_include_directories(main PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(main PRIVATE
    -Wall
)
target_link_libraries(main PRIVATE
    botanist
    collection2
)

## 単純な計算機
add_executable(calc)
target_sources(calc PRIVATE
    calc.cpp
)
target_include_directories(calc PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(calc PRIVATE
    -Wall
)
target_link_libraries(calc PRIVATE
    botanist
    collection2
)
