//
// 単純な計算機
//

#include <iostream>

#include "evaluator/double.hpp"
#include "parser.hpp"

using namespace botanist;

int main(int argc, char const* argv[]) {
    // 対象の式は実行引数経由で渡されることを想定
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " formula" << std::endl;
        return 1;
    }
    char const* formula = argv[1];

    // パース
    Parser parser;
    auto* serializedNode = parser.parse(formula);
    if (serializedNode == nullptr) {
        std::cerr << "invalid formula" << std::endl;
        return 1;
    }

    // 評価
    DoubleEvaluator evaluator;
    double result = evaluator.evaluate(serializedNode);
    std::cout << formula << " = " << result << std::endl;
    return 0;
}
