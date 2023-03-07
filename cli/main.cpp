//
// formula_parser
//

#include <iostream>

#include "evaluator.hpp"
#include "parser.hpp"

using namespace collection2;
using TokenKind = botanist::Token::Kind;

int main(int argc, char const* argv[]) {
    // 対象の式は実行引数経由で渡されることを想定
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " formula" << std::endl;
        return 1;
    }
    char const* formula = argv[1];
    std::cout << formula << std::endl;

    // トークナイザに渡す
    std::cout << "Tokenize..." << std::endl;
    botanist::Tokenizer tokenizer;
    auto tokenizationResult = tokenizer.tokenize(formula);
    if (tokenizationResult != 0) {
        for (size_t i = 0; i < tokenizationResult - 1; i++) {
            std::cerr << ' ';
        }
        std::cerr << "\033[31;1m^\033[0m" << std::endl;
        std::cerr << "Tokenize failed: unexpected character found at " << tokenizationResult << std::endl;
        return 1;
    }
    tokenizer.dumpTokenList();

    // トークンリストを構文木に変換
    std::cout << "Analyse..." << std::endl;
    botanist::Analyser analyser(tokenizer.tokens());
    botanist::SyntaxNode* rootNode = analyser.analyse();
    if (rootNode == nullptr) {
        std::cerr << "Analyse failed" << std::endl;
        return 1;
    }
    analyser.dumpSyntaxTree(rootNode);

    // 構文木を直列化し、スタックマシンで動かせるレベルまで持っていく
    std::cout << "Serialize..." << std::endl;
    botanist::Serializer serializer;
    auto* serializedNode = serializer.serializeTree(rootNode);
    serializer.dumpSeralizedNodeList();

    // 評価器に渡す
    std::cout << "Evaluate..." << std::endl;
    botanist::FPEvaluator evaluator;
    double calculationResult = evaluator.evaluate(serializedNode);
    std::cout << "ans = " << calculationResult << std::endl;

    return 0;
}
