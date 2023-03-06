//
// トークナイズのテスト
//

#include <gtest/gtest.h>

#include "tokenizer.hpp"

using namespace botanist;

/// @brief 単項式
TEST(TokenizeTest, testTokenizeUnary) {
    EXPECT_EQ(Tokenizer("1").tokenize(), 0);
    EXPECT_EQ(Tokenizer("123").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23").tokenize(), 0);

    EXPECT_NE(Tokenizer("1.2.3").tokenize(), 0);
}

/// @brief 多項式 (加減算)
TEST(TokenizeTest, testTokenizeAddSub) {
    // 単純な評価
    EXPECT_EQ(Tokenizer("1+1").tokenize(), 0);
    EXPECT_EQ(Tokenizer("123+456").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23-4.56").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23 + 4.56").tokenize(), 0);
    EXPECT_EQ(Tokenizer("1.23 - 4.56 + 7.89").tokenize(), 0);

    // 数式としては正しくないが、トークナイズに成功する
    EXPECT_EQ(Tokenizer("1.23+").tokenize(), 0);
    EXPECT_EQ(Tokenizer("4.56 -").tokenize(), 0);
    EXPECT_EQ(Tokenizer("7.89 + 0.12 -").tokenize(), 0);
}

/// @brief 多項式 (加減算) : トークン数確認
TEST(TokenizeTest, testTokenizeAddSubAndCount) {
    // 整数値
    Tokenizer intPolynomialTokenizer("1 + 2 - 3 + 4 - 5");
    EXPECT_EQ(intPolynomialTokenizer.tokenize(), 0);
    auto* intTokenizedNode = intPolynomialTokenizer.tokens();
    int intNodeCount = 1;
    while (intTokenizedNode->next != nullptr) {
        intTokenizedNode = intTokenizedNode->next;
        intNodeCount++;
    }
    EXPECT_EQ(intNodeCount, 9);

    // 実数値
    Tokenizer realPolynomialTokenizer("1.2+3.4-5.6+7.8-9.0");
    EXPECT_EQ(realPolynomialTokenizer.tokenize(), 0);
    auto* realTokenizedNode = realPolynomialTokenizer.tokens();
    int realNodeCount = 1;
    while (realTokenizedNode->next != nullptr) {
        realTokenizedNode = realTokenizedNode->next;
        realNodeCount++;
    }
    EXPECT_EQ(realNodeCount, 9);
}
