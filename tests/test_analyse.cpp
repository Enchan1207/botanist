//
// 数式解析のテスト
//

#include <gtest/gtest.h>

#include <cstring>

#include "analyser.hpp"
#include "tokenizer.hpp"

using namespace botanist;

/// @brief 正当な単項式
TEST(AnalyseTest, testAnalyseValidFormula) {
    // 先にトークナイザ側でトークン列を生成しておく必要がある
    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize("1.23-45.6"), 0);

    Analyser analyser(tokenizer.tokens());
    const auto* rootNode = analyser.analyse();

    // 各ノードが想定通りの構造になっているか
    EXPECT_EQ(rootNode->kind, SyntaxNode::Kind::Subtract);
    EXPECT_EQ(rootNode->lhs->kind, SyntaxNode::Kind::Number);
    EXPECT_EQ(rootNode->rhs->kind, SyntaxNode::Kind::Number);

    // 内容は正当か
    char buf[5] = {0};
    memcpy(buf, rootNode->lhs->content, rootNode->lhs->length);
    EXPECT_EQ(strcmp(buf, "1.23"), 0);
    memcpy(buf, rootNode->rhs->content, rootNode->rhs->length);
    EXPECT_EQ(strcmp(buf, "45.6"), 0);
}

/// @brief 不正な単項式
TEST(AnalyseTest, testAnalyseInvalidFormula) {
    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize("12*"), 0);

    Analyser analyser(tokenizer.tokens());
    const auto* rootNode = analyser.analyse();

    EXPECT_EQ(rootNode->kind, SyntaxNode::Kind::Multiply);
    EXPECT_EQ(rootNode->lhs->kind, SyntaxNode::Kind::Number);
    EXPECT_EQ(rootNode->rhs, nullptr);
}

/// @brief 無を解析
TEST(AnalyseTest, testAnalyseEmptyTokens) {
    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize(""), 0);

    Analyser analyser(tokenizer.tokens());
    const auto* rootNode = analyser.analyse();
    EXPECT_EQ(rootNode, nullptr);
}
