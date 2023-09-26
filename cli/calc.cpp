//
// 単純な計算機
//

#include <iostream>

#include "botanist/analyser.hpp"
#include "botanist/evaluator/double.hpp"
#include "botanist/serializer.hpp"
#include "botanist/tokenizer.hpp"

using namespace botanist;

int main(int argc, char const* argv[]) {
    // 対象の式は実行引数経由で渡されることを想定
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " formula" << std::endl;
        return 1;
    }
    char const* formula = argv[1];

    // トークンリストを作成
    collection2::Node<botanist::Token> tokenPool[64];
    collection2::List<botanist::Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));

    Tokenizer tokenizer(tokenList);
    Analyser analyser(tokenList);
    bool isTokenized = tokenizer.tokenize(formula) == 0;
    if (!isTokenized) {
        std::cerr << "failed to tokenize" << std::endl;
        return 1;
    }
    bool isAnalysed = analyser.analyse() == 0;
    if (!isAnalysed) {
        std::cerr << "failed to construct syntax tree" << std::endl;
        return 1;
    }

    Serializer serializer;
    auto serializedRootNode = serializer.serializeTree(analyser.rootNode());
    if (serializedRootNode == nullptr) {
        std::cerr << "failed to serialize" << std::endl;
        return 1;
    }

    // 評価
    DoubleEvaluator evaluator;
    double result = evaluator.evaluate(serializedRootNode);
    std::cout << formula << " = " << result << std::endl;
    return 0;
}
