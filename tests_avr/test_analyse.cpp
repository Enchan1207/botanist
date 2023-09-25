//
// 数式解析のテスト
//
#include <string.h>

#include <collection2/list.hpp>

#include "analyser.hpp"
#include "testcase.hpp"
#include "tokenizer.hpp"

using namespace botanist;

namespace botanisttests {

/// @brief 正当な単項式
TEST(testAnalyseValidFormula) {
    BeginTestcase(result);

    // 先にトークナイザ側でトークン列を生成しておく必要がある
    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize("1.23-45.6"), 0, result);

    Analyser analyser;
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 0, result);
    const auto* rootNode = analyser.rootNode();

    // 各ノードが想定通りの構造になっているか
    EXPECT_EQ(rootNode->kind, SyntaxNode::Kind::Subtract, result);
    EXPECT_EQ(rootNode->lhs->kind, SyntaxNode::Kind::Number, result);
    EXPECT_EQ(rootNode->rhs->kind, SyntaxNode::Kind::Number, result);

    // 内容は正当か
    char buf[5] = {0};
    memcpy(buf, rootNode->lhs->content, rootNode->lhs->length);
    EXPECT_EQ(strcmp(buf, "1.23"), 0, result);
    memcpy(buf, rootNode->rhs->content, rootNode->rhs->length);
    EXPECT_EQ(strcmp(buf, "45.6"), 0, result);

    EndTestcase(result);
}

/// @brief 不正な単項式
TEST(testAnalyseInvalidUnary) {
    BeginTestcase(result);

    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize("12*"), 0, result);

    Analyser analyser;
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 2, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EndTestcase(result);
}

/// @brief 不正な数式
TEST(testAnalyseInvalidFormula) {
    BeginTestcase(result);

    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize("1++1"), 0, result);

    Analyser analyser;
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 2, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EndTestcase(result);
}

/// @brief 無を解析
TEST(testAnalyseEmptyTokens) {
    BeginTestcase(result);

    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize(""), 0, result);

    Analyser analyser;
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 1, result);
    const auto* rootNode = analyser.rootNode();
    EXPECT_EQ(rootNode, nullptr, result);

    EndTestcase(result);
}

/// @brief さまざまな長さの数式を連続で構文木に変換する
TEST(testContinuousAnalysis) {
    BeginTestcase(result);

    Analyser analyser;
    Tokenizer tokenizer;

    EXPECT_EQ(tokenizer.tokenize("1.23"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 0, result);
    EXPECT_NE(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("12+"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 2, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("1234+5678"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 0, result);
    EXPECT_NE(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("(12+(34*56)"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 8, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("(12+(34/56)+78"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 10, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("12)"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 1, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("123++456"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 2, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("1+1*2-4"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 0, result);
    EXPECT_NE(analyser.rootNode(), nullptr, result);

    EXPECT_EQ(tokenizer.tokenize("(812*46999)+17777 / 0"), 0, result);
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 0, result);
    EXPECT_NE(analyser.rootNode(), nullptr, result);

    EndTestcase(result);
}

/// @brief プールが埋まるほど複雑な数式
TEST(testTooLongFormula) {
    BeginTestcase(result);

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

    Analyser analyser;
    auto analyseResult = analyser.analyse(tokenList.head());
    EXPECT_NE(analyseResult, 0, result);
    EXPECT_EQ(analyser.rootNode(), nullptr, result);

    EndTestcase(result);
}

const TestFunction tests[] = {
    testAnalyseValidFormula,
    testAnalyseInvalidUnary,
    testAnalyseInvalidFormula,
    testAnalyseEmptyTokens,
    testContinuousAnalysis,
    testTooLongFormula,
};

const size_t testCount = 6;

}  // namespace botanisttests
