//
// 数式解析のテスト
//

#include <gtest/gtest.h>

#include <collection2/list.hpp>
#include <cstring>

#include "botanist/analyser.hpp"
#include "botanist/tokenizer.hpp"

using namespace botanist;

using TokenList = collection2::List<Token>;

/// @brief 正当な単項式
TEST(AnalyseTest, testAnalyseValidFormula) {
    // 先にトークナイザ側でトークン列を生成しておく必要がある
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    EXPECT_EQ(tokenizer.tokenize("1.23-45.6"), 0);

    Analyser analyser(tokenList);
    EXPECT_EQ(analyser.analyse(), 0);
    const auto* rootNode = analyser.rootNode();

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
TEST(AnalyseTest, testAnalyseInvalidUnary) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    Analyser analyser(tokenList);
    EXPECT_EQ(tokenizer.tokenize("12*"), 0);
    EXPECT_EQ(analyser.analyse(), 2);
    EXPECT_EQ(analyser.rootNode(), nullptr);
}

/// @brief 不正な数式
TEST(AnalyseTest, testAnalyseInvalidFormula) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    Analyser analyser(tokenList);
    EXPECT_EQ(tokenizer.tokenize("1++1"), 0);
    EXPECT_EQ(analyser.analyse(), 2);
    EXPECT_EQ(analyser.rootNode(), nullptr);
}

/// @brief 無を解析
TEST(AnalyseTest, testAnalyseEmptyTokens) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    Analyser analyser(tokenList);
    EXPECT_EQ(tokenizer.tokenize(""), 0);
    EXPECT_EQ(analyser.analyse(), 1);
    const auto* rootNode = analyser.rootNode();
    EXPECT_EQ(rootNode, nullptr);
}

/// @brief さまざまな長さの数式を連続で構文木に変換する
TEST(AnalyseTest, testContinuousAnalysis) {
    collection2::Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Analyser analyser(tokenList);
    Tokenizer tokenizer(tokenList);

    EXPECT_EQ(tokenizer.tokenize("1.23"), 0);
    EXPECT_EQ(analyser.analyse(), 0);
    EXPECT_NE(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("12+"), 0);
    EXPECT_EQ(analyser.analyse(), 2);
    EXPECT_EQ(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("1234+5678"), 0);
    EXPECT_EQ(analyser.analyse(), 0);
    EXPECT_NE(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("(12+(34*56)"), 0);
    EXPECT_EQ(analyser.analyse(), 8);
    EXPECT_EQ(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("(12+(34/56)+78"), 0);
    EXPECT_EQ(analyser.analyse(), 10);
    EXPECT_EQ(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("12)"), 0);
    EXPECT_EQ(analyser.analyse(), 1);
    EXPECT_EQ(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("123++456"), 0);
    EXPECT_EQ(analyser.analyse(), 2);
    EXPECT_EQ(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("1+1*2-4"), 0);
    EXPECT_EQ(analyser.analyse(), 0);
    EXPECT_NE(analyser.rootNode(), nullptr);

    EXPECT_EQ(tokenizer.tokenize("(812*46999)+17777 / 0"), 0);
    EXPECT_EQ(analyser.analyse(), 0);
    EXPECT_NE(analyser.rootNode(), nullptr);
}

/// @brief プールが埋まるほど複雑な数式
TEST(AnalyseTest, testTooLongFormula) {
    // 1+1+...+1+1 と続くトークンリストを生成する
    constexpr size_t tokenLength = 129;  // must be odd number
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
    auto result = analyser.analyse();
    EXPECT_NE(result, 0);
    EXPECT_EQ(analyser.rootNode(), nullptr);
    std::cerr << "analyse failed: at token # " << result << std::endl;
}
