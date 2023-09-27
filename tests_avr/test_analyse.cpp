//
// 数式解析のテスト
//
#include <avr/pgmspace.h>
#include <string.h>

#include <collection2/list.hpp>

#include "botanist/analyser.hpp"
#include "botanist/tokenizer.hpp"
#include "testcase.hpp"

using namespace botanist;
using namespace collection2;

namespace botanisttests {

/// @brief さまざまな式の解析
TEST(testAnalyseFormulas) {
    BeginTestcase(result);

    struct pattern_t {
        char source[48];
        size_t expected;
    };
    const static struct pattern_t patterns[] PROGMEM = {
        {"1.23-45.6", 0},
        {"12*", 2},
        {"1++1", 2},
        {"", 1},
        {"(12+(34*56)", 8},
        {"12)", 1},
    };
    const size_t patternCount = sizeof(patterns) / sizeof(pattern_t);

    Node<Token> tokenPool[16];
    List<Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);

    Analyser analyser(tokenList);
    struct pattern_t pattern;
    for (size_t i = 0; i < patternCount; i++) {
        memcpy_P(&pattern, &patterns[i], sizeof(struct pattern_t));
        tokenizer.tokenize(pattern.source);
        EXPECT_EQ(analyser.analyse(), pattern.expected, result);
    }
    EndTestcase(result);
}

/// @brief プールが埋まるほど複雑な数式
TEST(testTooLongFormula) {
    BeginTestcase(result);

    // 1+1+...+1+1 と続くトークンリストを生成する
    constexpr size_t tokenLength = 65;  // must be odd number
    collection2::Node<Token> internalTokensData[tokenLength];
    collection2::List<Token> tokenList(internalTokensData, tokenLength);
    for (size_t i = 0; i < tokenLength; i++) {
        if (i % 2 == 0) {
            tokenList.append(Token(Token::Kind::Number, "1", 1));
        } else {
            tokenList.append(Token(Token::Kind::Operator, "+", 1));
        }
    }

    Analyser analyser(tokenList);
    auto analyseResult = analyser.analyse();
    EXPECT_NE(analyseResult, 0, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EndTestcase(result);
}

const TestFunction tests[] = {
    testAnalyseFormulas,
    testTooLongFormula,
};

const size_t testCount = sizeof(tests) / sizeof(TestFunction);

}  // namespace botanisttests
