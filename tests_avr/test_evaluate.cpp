//
// 数式評価のテスト
//

#include <string.h>

#include "botanist/analyser.hpp"
#include "botanist/evaluator/double.hpp"
#include "botanist/serializer.hpp"
#include "botanist/tokenizer.hpp"
#include "testcase.hpp"

using namespace botanist;

namespace botanisttests {

/// @brief 倍精度浮動小数評価器による演算
TEST(testAnalyseIntFormula) {
    BeginTestcase(result);

    Tokenizer tokenizer;
    EXPECT_EQ(tokenizer.tokenize("(((12+34) - 56*78)-78)/44+100"), 0, result);
    Analyser analyser;
    EXPECT_EQ(analyser.analyse(tokenizer.tokens()), 0, result);
    auto* rootNode = analyser.rootNode();
    EXPECT_NE(rootNode, nullptr, result);
    Serializer serializer;
    auto* serializedNode = serializer.serializeTree(rootNode);

    DoubleEvaluator evaluator;
    auto evalResult = static_cast<int>(evaluator.evaluate(serializedNode));
    EXPECT_EQ(evalResult, 0, result);  // (((12+34)-56*78)-78)/44+100=0

    EndTestcase(result);
}

const TestFunction tests[] = {
    testAnalyseIntFormula,
};

const size_t testCount = 1;

}  // namespace botanisttests
