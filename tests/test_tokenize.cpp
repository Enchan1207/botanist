//
// トークナイズのテスト
//

#include <gtest/gtest.h>

#include "tokenizer.hpp"

using namespace botanist;

/// @brief 正当な単項式
TEST(TokenizeTest, testTokenizeValidUnary) {
    EXPECT_EQ(Tokenizer("1").tokenize(), 0);
    EXPECT_EQ(Tokenizer("123").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23").tokenize(), 0);
}

/// @brief 不正な単項式
TEST(TokenizeTest, testTokenizeInvalidUnary) {
    // 小数点で始まってはいけない
    EXPECT_EQ(Tokenizer(".1").tokenize(), 1);

    // "1" までパースし、2文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer("1.").tokenize(), 2);

    // "1.2" までパースし、4文字目の '.' でエラーとなる
    EXPECT_EQ(Tokenizer("1.2.3").tokenize(), 4);
}

/// @brief 多項式 (加減算)
TEST(TokenizeTest, testTokenizeAddSub) {
    // 単純な評価
    EXPECT_EQ(Tokenizer("1+1").tokenize(), 0);
    EXPECT_EQ(Tokenizer("123+456").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23-4.56").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23 + 4.56").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23 - 4.56 + 7.89").tokenize(), 0);

    // 数式としては正しくないが、トークナイズには成功する
    // (トークナイザは数式の意味を理解していないため)
    EXPECT_EQ(Tokenizer("1.23+").tokenize(), 0);
    EXPECT_EQ(Tokenizer("4.56 -").tokenize(), 0);
    EXPECT_EQ(Tokenizer("7.89 + 0.12 -").tokenize(), 0);
}

/// @brief 多項式 (加減算) : トークン数確認
TEST(TokenizeTest, testTokenizeAddSubAndCount) {
    // 整数値のみ
    Tokenizer intPolynomialTokenizer("123 + 234-345 + 456-567");
    EXPECT_EQ(intPolynomialTokenizer.tokenize(), 0);
    auto* intTokenizedNode = intPolynomialTokenizer.tokens();
    int intNodeCount = 1;
    while (intTokenizedNode->next != nullptr) {
        intTokenizedNode = intTokenizedNode->next;
        intNodeCount++;
    }
    EXPECT_EQ(intNodeCount, 9);

    // 混在
    Tokenizer realPolynomialTokenizer("12+3.4-56+7.8-90");
    EXPECT_EQ(realPolynomialTokenizer.tokenize(), 0);
    auto* realTokenizedNode = realPolynomialTokenizer.tokens();
    int realNodeCount = 1;
    while (realTokenizedNode->next != nullptr) {
        realTokenizedNode = realTokenizedNode->next;
        realNodeCount++;
    }
    EXPECT_EQ(realNodeCount, 9);
}
