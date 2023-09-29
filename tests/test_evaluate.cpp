//
// 数式評価のテスト
//

#include <gtest/gtest.h>

#include <collection2/list.hpp>
#include <collection2/stack.hpp>
#include <collection2/tree.hpp>
#include <cstring>

#include "botanist/analyser.hpp"
#include "botanist/evaluator/double.hpp"
#include "botanist/serializer.hpp"
#include "botanist/tokenizer.hpp"

using namespace botanist;
using namespace collection2;

using TokenList = List<Token>;
using SyntaxNodeTree = Tree<SyntaxNode>;

/// @brief 倍精度浮動小数評価器による演算
TEST(EvaluateTest, testAnalyseIntFormula) {
    Node<Token> tokenPool[32];
    TokenList tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);

    TreeNode<SyntaxNode> nodePool[16];
    SyntaxNodeTree syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);

    EXPECT_EQ(tokenizer.tokenize("(((12+34) - 56*78)-78)/44+100"), 0);
    EXPECT_EQ(analyser.analyse(), 0);
    auto* rootNode = analyser.rootNode();
    EXPECT_NE(rootNode, nullptr);
    Node<SyntaxNode> syntaxNodePool[16];
    List<SyntaxNode> syntaxNodeList(syntaxNodePool, sizeof(syntaxNodePool) / sizeof(syntaxNodePool[0]));
    Serializer serializer(syntaxNodeList);
    serializer.serializeTree(rootNode);

    double calcStackPool[16];
    Stack<double> calcStack(calcStackPool, sizeof(calcStackPool) / sizeof(calcStackPool[0]));
    DoubleEvaluator evaluator(calcStack);
    auto result = static_cast<int>(evaluator.evaluate(syntaxNodeList));
    EXPECT_EQ(result, 0);  // (((12+34)-56*78)-78)/44+100=0
}
