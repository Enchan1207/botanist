//
// トークナイズのテスト
//

#include <avr/pgmspace.h>
#include <string.h>

#include "testcase.hpp"
#include "tokenizer.hpp"

using namespace botanist;

namespace botanisttests {

/// @brief さまざまな式のトークナイズ
TEST(testTokenize) {
    BeginTestcase(result);

    struct pattern_t {
        char source[48];
        size_t expected;
    };
    const static struct pattern_t patterns[] PROGMEM = {
        // 成功
        {"", 0},
        {"1", 0},
        {"123", 0},
        {"1.23", 0},

        // 変数は使えない
        {"a", 1},
        {"bc", 1},

        // 関数は使えない
        {"sin(45)", 1},

        // 不正な式
        {".1", 1},
        {"1.", 2},
        {"1.2.3", 4},
        {"1.2.3.4", 4},

        // 多項式
        {"1+1", 0},
        {"123-456", 0},
        {"1.23*4.56", 0},
        {"1.23 / 4.56", 0},
        {"1.23 - 4.56 + 7.89", 0},
        {"1.23 + 4.56 - 7.89 * 0.12 / 3.45", 0},
        {"( 434*26222)+ 79999 / 0", 0},

        // トークナイザは式の意味を理解しないので、このような不正な式も通る
        {"1.23+", 0},
        {"4.56 -", 0},
        {"7.89 * 0.12 /", 0},
        {"1/0", 0},
    };
    const size_t patternCount = sizeof(patterns) / sizeof(struct pattern_t);

    Tokenizer tokenizer;
    struct pattern_t pattern;
    for (size_t i = 0; i < patternCount; i++) {
        memcpy_P(&pattern, &patterns[i], sizeof(struct pattern_t));
        EXPECT_EQ(tokenizer.tokenize(pattern.source), pattern.expected, result);
    }
    EndTestcase(result);
}

/// @brief トークン数の確認
TEST(testTokenizeAddSubAndCount) {
    BeginTestcase(result);
    // 整数値のみ
    Tokenizer intPolynomialTokenizer;
    EXPECT_EQ(intPolynomialTokenizer.tokenize("123 + 234-345 * 456/567"), 0, result);
    auto* intTokenizedNode = intPolynomialTokenizer.tokens();
    int intNodeCount = 1;
    while (intTokenizedNode->next != nullptr) {
        intTokenizedNode = intTokenizedNode->next;
        intNodeCount++;
    }
    EXPECT_EQ(intNodeCount, 9, result);

    // 混在
    Tokenizer realPolynomialTokenizer;
    EXPECT_EQ(realPolynomialTokenizer.tokenize("12+3.4-56*7.8/90"), 0, result);
    auto* realTokenizedNode = realPolynomialTokenizer.tokens();
    int realNodeCount = 1;
    while (realTokenizedNode->next != nullptr) {
        realTokenizedNode = realTokenizedNode->next;
        realNodeCount++;
    }
    EXPECT_EQ(realNodeCount, 9, result);
    EndTestcase(result);
}

/// @brief より複雑な多項式
TEST(testTokenizeComplexPolynomial) {
    BeginTestcase(result);
    Tokenizer complexTokenizer;
    EXPECT_EQ(complexTokenizer.tokenize("(12+3.4)-(56*7.8)/90"), 0, result);
    auto* tokenizedNode = complexTokenizer.tokens();
    int nodeCount = 1;
    while (tokenizedNode->next != nullptr) {
        tokenizedNode = tokenizedNode->next;
        nodeCount++;
    }
    EXPECT_EQ(nodeCount, 13, result);
    EndTestcase(result);
}

/// @brief トークナイザが扱えない長さのトークン
TEST(testTooLongToken) {
    BeginTestcase(result);
    // 1+1+1+1+1+... と続く数式を生成する
    constexpr size_t tokenLength = 256;
    char tooLongToken[tokenLength + 1] = {0};
    for (size_t i = 0; i < tokenLength; i += 2) {
        const char* sample = "1+";
        memcpy(tooLongToken + i, sample, 2);
    }
    Tokenizer tokenizer;
    EXPECT_NE(tokenizer.tokenize(tooLongToken), 0, result);
    EndTestcase(result);
}

const TestFunction tests[] = {
    testTokenize,
    testTokenizeAddSubAndCount,
    testTokenizeComplexPolynomial,
    testTooLongToken,
};

const size_t testCount = sizeof(tests) / sizeof(TestFunction);

}  // namespace botanisttests
