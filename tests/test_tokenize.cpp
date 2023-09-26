//
// トークナイズのテスト
//

#include <gtest/gtest.h>

#include <cstring>
#include <iostream>

#include "botanist/tokenizer.hpp"

using namespace botanist;

using TokenList = collection2::List<Token>;

/// @brief 正当な単項式
TEST(TokenizeTest, testTokenizeValidUnary) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    EXPECT_EQ(Tokenizer(tokenList).tokenize(""), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("123"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.23"), 0);
}

/// @brief 不正な単項式
TEST(TokenizeTest, testTokenizeInvalidUnary) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));

    // 変数には対応していない
    EXPECT_EQ(Tokenizer(tokenList).tokenize("a"), 1);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("bc"), 1);

    // 関数は使えない
    EXPECT_EQ(Tokenizer(tokenList).tokenize("sin(45)"), 1);

    // 小数点で始まってはいけない
    EXPECT_EQ(Tokenizer(tokenList).tokenize(".1"), 1);

    // "1" までパースし、2文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1."), 2);

    // "1.2" までパースし、4文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.2.3"), 4);

    // "1.2" までパースし、4文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.2.3.4"), 4);
}

/// @brief 多項式
TEST(TokenizeTest, testTokenizePolynomial) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));

    // 四則演算
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1+1"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("123-456"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.23*4.56"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.23 / 4.56"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.23 - 4.56 + 7.89"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.23 + 4.56 - 7.89 * 0.12 / 3.45"), 0);

    // 数式としては正しくないが、トークナイズには成功する
    // (トークナイザは数式の意味を理解していないため)
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1.23+"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("4.56 -"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("7.89 * 0.12 /"), 0);
    EXPECT_EQ(Tokenizer(tokenList).tokenize("1/0"), 0);
}

/// @brief トークン数の確認
TEST(TokenizeTest, testTokenizeAddSubAndCount) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));

    // 整数値のみ
    Tokenizer intPolynomialTokenizer(tokenList);
    EXPECT_EQ(intPolynomialTokenizer.tokenize("123 + 234-345 * 456/567"), 0);
    auto* intTokenizedNode = tokenList.head();
    int intNodeCount = 1;
    while (intTokenizedNode->next != nullptr) {
        intTokenizedNode = intTokenizedNode->next;
        intNodeCount++;
    }
    EXPECT_EQ(intNodeCount, 9);

    // 混在
    Tokenizer realPolynomialTokenizer(tokenList);
    EXPECT_EQ(realPolynomialTokenizer.tokenize("12+3.4-56*7.8/90"), 0);
    auto* realTokenizedNode = tokenList.head();
    int realNodeCount = 1;
    while (realTokenizedNode->next != nullptr) {
        realTokenizedNode = realTokenizedNode->next;
        realNodeCount++;
    }
    EXPECT_EQ(realNodeCount, 9);
}

/// @brief より複雑な多項式
TEST(TokenizeTest, testTokenizeComplexPolynomial) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer complexTokenizer(tokenList);
    EXPECT_EQ(complexTokenizer.tokenize("(12+3.4)-(56*7.8)/90"), 0);
    auto* tokenizedNode = tokenList.head();
    int nodeCount = 1;
    while (tokenizedNode->next != nullptr) {
        tokenizedNode = tokenizedNode->next;
        nodeCount++;
    }
    EXPECT_EQ(nodeCount, 13);
}

/// @brief さまざまな長さの数式を連続でトークナイズする
TEST(TokenizeTest, testContinuousTokenization) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    EXPECT_EQ(tokenizer.tokenize("1.23"), 0);
    EXPECT_EQ(tokenizer.tokenize("1.2.3.4"), 4);
    EXPECT_EQ(tokenizer.tokenize("1+1"), 0);
    EXPECT_EQ(tokenizer.tokenize("sin(45)"), 1);
    EXPECT_EQ(tokenizer.tokenize("1+1*2"), 0);
    EXPECT_EQ(tokenizer.tokenize("( 434*26222)+79999 / 0"), 0);
    EXPECT_EQ(tokenizer.tokenize("1."), 2);
}

/// @brief トークナイザが扱えない長さのトークン
TEST(TokenizeTest, testTooLongToken) {
    // 1+1+1+1+1+... と続く数式を生成する
    constexpr size_t tokenLength = 256;
    char tooLongToken[tokenLength + 1] = {0};
    for (size_t i = 0; i < tokenLength; i += 2) {
        const char* sample = "1+";
        memcpy(tooLongToken + i, sample, 2);
    }
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    auto result = tokenizer.tokenize(tooLongToken);
    EXPECT_NE(result, 0);
}
