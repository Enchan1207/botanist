//
// トークナイズのテスト
//

#include <avr/pgmspace.h>
#include <string.h>

#include "botanist/tokenizer.hpp"
#include "testcase.hpp"

using namespace botanist;
using namespace collection2;

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

    Node<Token> tokenPool[16];
    List<Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
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
    Node<Token> tokenPool[16];
    List<Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer intPolynomialTokenizer(tokenList);
    EXPECT_EQ(intPolynomialTokenizer.tokenize("123 + 234-345 * 456/567"), 0, result);
    auto* intTokenizedNode = tokenList.head();
    int intNodeCount = 1;
    while (intTokenizedNode->next != nullptr) {
        intTokenizedNode = intTokenizedNode->next;
        intNodeCount++;
    }
    EXPECT_EQ(intNodeCount, 9, result);

    // 混在
    Tokenizer realPolynomialTokenizer(tokenList);
    EXPECT_EQ(realPolynomialTokenizer.tokenize("12+3.4-56*7.8/90"), 0, result);
    auto* realTokenizedNode = tokenList.head();
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
    Node<Token> tokenPool[16];
    List<Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer complexTokenizer(tokenList);
    EXPECT_EQ(complexTokenizer.tokenize("(12+3.4)-(56*7.8)/90"), 0, result);
    auto* tokenizedNode = tokenList.head();
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
    Node<Token> tokenPool[16];
    List<Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
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
