//
// 数式評価のテスト
//

#include <gtest/gtest.h>

#include <cstring>

#include "botanist/analyser.hpp"
#include "botanist/evaluator/double.hpp"
#include "botanist/serializer.hpp"
#include "botanist/tokenizer.hpp"

using namespace botanist;

/// @brief 倍精度浮動小数評価器による演算
TEST(EvaluateTest, testAnalyseIntFormula) {
    collection2::Node<Token> tokenPool[32];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
    EXPECT_EQ(tokenizer.tokenize("(((12+34) - 56*78)-78)/44+100"), 0);
    Analyser analyser;
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 0);
    auto* rootNode = analyser.rootNode();
    EXPECT_NE(rootNode, nullptr);
    Serializer serializer;
    auto* serializedNode = serializer.serializeTree(rootNode);

    DoubleEvaluator evaluator;
    auto result = static_cast<int>(evaluator.evaluate(serializedNode));
    EXPECT_EQ(result, 0);  // (((12+34)-56*78)-78)/44+100=0
}
