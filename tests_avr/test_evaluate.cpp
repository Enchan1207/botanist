//
// 数式評価のテスト
//

#include <string.h>

#include <collection2/list.hpp>
#include <collection2/stack.hpp>
#include <collection2/tree.hpp>

#include "botanist/analyser.hpp"
#include "botanist/evaluator/double.hpp"
#include "botanist/serializer.hpp"
#include "botanist/tokenizer.hpp"
#include "testcase.hpp"

#define arraySize(array) sizeof(array) / sizeof(array[0])

using namespace botanist;
using namespace collection2;

namespace botanisttests {

/// @brief 倍精度浮動小数評価器による演算
TEST(testAnalyseIntFormula) {
    BeginTestcase(result);

    const uint8_t poolCapacity = 32;

    // トークナイザを構成
    Node<Token> tokenPool[poolCapacity];
    List<Token> tokenList(tokenPool, arraySize(tokenPool));
    Tokenizer tokenizer(tokenList);

    // 数式をトークナイズ
    EXPECT_EQ(tokenizer.tokenize("(44/22*3+(59-100))/7+10"), 0, result);
    EXPECT_EQ(tokenList.amount(), 17, result);

    // アナライザを構成
    TreeNode<SyntaxNode> treeNodePool[poolCapacity];
    Tree<SyntaxNode> syntaxTree(treeNodePool, arraySize(treeNodePool));
    Analyser analyser(tokenList, syntaxTree);

    // トークンリストから構文木を生成
    EXPECT_EQ(analyser.analyse(), 0, result);
    const auto* rootNode = analyser.rootNode();
    EXPECT_NE(rootNode, nullptr, result);

    // シリアライザを構成
    Node<SyntaxNode> serializedNodePool[poolCapacity];
    List<SyntaxNode> syntaxNodeList(serializedNodePool, arraySize(serializedNodePool));
    Serializer serializer(syntaxNodeList);

    // ノードを直列化
    serializer.serializeTree(rootNode);

    // 評価器を構成
    double calcStackPool[poolCapacity];
    Stack<double> calcStack(calcStackPool, arraySize(calcStackPool));
    DoubleEvaluator evaluator(calcStack);

    // 直列化したノードを投入して計算 回答を確認
    auto evalResult = static_cast<int>(evaluator.evaluate(syntaxNodeList));
    EXPECT_EQ(evalResult, 5, result);

    EndTestcase(result);
}

const TestFunction tests[] = {
    testAnalyseIntFormula,
};

const size_t testCount = 1;

}  // namespace botanisttests
