#
# botanist tests
#
cmake_minimum_required(VERSION 3.0)

# GoogleTestのフェッチと有効化
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

## 外部依存関係を追加...

# テストターゲットの構成
add_executable(botanist_test)
target_sources(botanist_test PRIVATE
    test_tokenize.cpp
    test_analyse.cpp
    test_evaluate.cpp
)
target_link_libraries(botanist_test PRIVATE
    GTest::gtest_main
    collection2
    botanist
)
target_include_directories(botanist_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIRS}
)

# テストの実行
gtest_discover_tests(botanist_test)