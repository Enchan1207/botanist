//
// 数式解析のテスト
//

#include <gtest/gtest.h>

#include <collection2/list.hpp>
#include <collection2/tree.hpp>
#include <cstring>

#include "botanist/analyser.hpp"
#include "botanist/tokenizer.hpp"

using namespace botanist;
using namespace collection2;

using TokenList = List<Token>;
using SyntaxNodeTree = Tree<SyntaxNode>;

/// @brief 正当な単項式
TEST(AnalyseTest, testAnalyseValidFormula) {
    Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    EXPECT_EQ(tokenizer.tokenize("1.23-45.6"), 0);

    TreeNode<SyntaxNode> nodePool[16];
    SyntaxNodeTree syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);
    EXPECT_EQ(analyser.analyse(), 0);
    const auto* rootNode = analyser.rootNode();

    // 各ノードが想定通りの構造になっているか
    EXPECT_EQ(rootNode->element.kind, SyntaxNode::Kind::Subtract);
    EXPECT_EQ(rootNode->lhs->element.kind, SyntaxNode::Kind::Number);
    EXPECT_EQ(rootNode->rhs->element.kind, SyntaxNode::Kind::Number);

    // 内容は正当か
    char buf[5] = {0};
    memcpy(buf, rootNode->lhs->element.content, rootNode->lhs->element.length);
    EXPECT_EQ(strcmp(buf, "1.23"), 0);
    memcpy(buf, rootNode->rhs->element.content, rootNode->rhs->element.length);
    EXPECT_EQ(strcmp(buf, "45.6"), 0);
}

/// @brief 不正な単項式
TEST(AnalyseTest, testAnalyseInvalidUnary) {
    Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);

    TreeNode<SyntaxNode> nodePool[16];
    SyntaxNodeTree syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);
    EXPECT_EQ(tokenizer.tokenize("12*"), 0);
    EXPECT_EQ(analyser.analyse(), 2);
    EXPECT_EQ(analyser.rootNode(), nullptr);
}

/// @brief 不正な数式
TEST(AnalyseTest, testAnalyseInvalidFormula) {
    Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);

    TreeNode<SyntaxNode> nodePool[16];
    SyntaxNodeTree syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);
    EXPECT_EQ(tokenizer.tokenize("1++1"), 0);
    EXPECT_EQ(analyser.analyse(), 2);
    EXPECT_EQ(analyser.rootNode(), nullptr);
}

/// @brief 無を解析
TEST(AnalyseTest, testAnalyseEmptyTokens) {
    Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);

    TreeNode<SyntaxNode> nodePool[16];
    SyntaxNodeTree syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);
    EXPECT_EQ(tokenizer.tokenize(""), 0);
    EXPECT_EQ(analyser.analyse(), 1);
    const auto* rootNode = analyser.rootNode();
    EXPECT_EQ(rootNode, nullptr);
}

/// @brief さまざまな長さの数式を連続で構文木に変換する
TEST(AnalyseTest, testContinuousAnalysis) {
    Node<Token> tokenPool[16];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);

    TreeNode<SyntaxNode> nodePool[16];
    SyntaxNodeTree syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);

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
    Node<Token> internalTokensData[tokenLength];
    List<Token> tokenList(internalTokensData, tokenLength);
    for (size_t i = 0; i < tokenLength; i++) {
        if (i % 2 == 0) {
            tokenList.append(Token(Token::Kind::Number, "1", 1));
        } else {
            tokenList.append(Token(Token::Kind::Operator, "+", 1));
        }
    }

    TreeNode<SyntaxNode> nodePool[16];
    SyntaxNodeTree syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);
    auto result = analyser.analyse();
    EXPECT_NE(result, 0);
    EXPECT_EQ(analyser.rootNode(), nullptr);
    std::cerr << "analyse failed: at token # " << result << std::endl;
}
