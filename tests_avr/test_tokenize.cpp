//
// トークナイズのテスト
//

#include <string.h>

#include "testcase.hpp"
#include "tokenizer.hpp"

using namespace botanist;

namespace botanisttests {

/// @brief 正当な単項式
TEST(testTokenizeValidUnary) {
    BeginTestcase(result);
    EXPECT_EQ(Tokenizer().tokenize(""), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("1"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("123"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("1.23"), 0, result);
    EndTestcase(result);
}

/// @brief 不正な単項式
TEST(testTokenizeInvalidUnary) {
    BeginTestcase(result);
    // 変数には対応していない
    EXPECT_EQ(Tokenizer().tokenize("a"), 1, result);
    EXPECT_EQ(Tokenizer().tokenize("bc"), 1, result);

    // 関数は使えない
    EXPECT_EQ(Tokenizer().tokenize("sin(45)"), 1, result);

    // 小数点で始まってはいけない
    EXPECT_EQ(Tokenizer().tokenize(".1"), 1, result);

    // "1" までパースし、2文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer().tokenize("1."), 2, result);

    // "1.2" までパースし、4文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer().tokenize("1.2.3"), 4, result);

    // "1.2" までパースし、4文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer().tokenize("1.2.3.4"), 4, result);
    EndTestcase(result);
}

/// @brief 多項式
TEST(testTokenizePolynomial) {
    BeginTestcase(result);
    // 四則演算
    EXPECT_EQ(Tokenizer().tokenize("1+1"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("123-456"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("1.23*4.56"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("1.23 / 4.56"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("1.23 - 4.56 + 7.89"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("1.23 + 4.56 - 7.89 * 0.12 / 3.45"), 0, result);

    // 数式としては正しくないが、トークナイズには成功する
    // (トークナイザは数式の意味を理解していないため)
    EXPECT_EQ(Tokenizer().tokenize("1.23+"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("4.56 -"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("7.89 * 0.12 /"), 0, result);
    EXPECT_EQ(Tokenizer().tokenize("1/0"), 0, result);
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

/// @brief さまざまな長さの数式を連続でトークナイズする
TEST(testContinuousTokenization) {
    BeginTestcase(result);
    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize("1.23"), 0, result);
    EXPECT_NE(tokenizer.tokens(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("1.2.3.4"), 4, result);
    EXPECT_EQ(tokenizer.tokens(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("1+1"), 0, result);
    EXPECT_NE(tokenizer.tokens(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("sin(45)"), 1, result);
    EXPECT_EQ(tokenizer.tokens(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("1+1*2"), 0, result);
    EXPECT_NE(tokenizer.tokens(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("( 434*26222)+79999 / 0"), 0, result);
    EXPECT_NE(tokenizer.tokens(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("1."), 2, result);
    EXPECT_EQ(tokenizer.tokens(), nullptr, result);
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
    testTokenizeValidUnary,
    testTokenizeInvalidUnary,
    testTokenizePolynomial,
    testTokenizeAddSubAndCount,
    testTokenizeComplexPolynomial,
    testContinuousTokenization,
    testTooLongToken,
};

const size_t testCount = 7;

}  // namespace botanisttests
