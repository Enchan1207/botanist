//
// botanist
//

#include <collection2/list.hpp>
#include <collection2/tree.hpp>
#include <iostream>

#include "botanist/analyser.hpp"
#include "botanist/evaluator/double.hpp"
#include "botanist/serializer.hpp"
#include "botanist/tokenizer.hpp"

using namespace collection2;
using namespace botanist;
using TokenKind = Token::Kind;

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
    collection2::Node<Token> tokenPool[64];
    collection2::List<Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);
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
    collection2::TreeNode<SyntaxNode> nodePool[16];
    collection2::Tree<SyntaxNode> syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);
    auto analysisResult = analyser.analyse();
    if (analysisResult != 0) {
        std::cerr << "Analyse failed: " << analysisResult << std::endl;
        return 1;
    }
    const auto* rootNode = analyser.rootNode();
    // analyser.dumpSyntaxTree(rootNode);

    // 構文木を直列化し、スタックマシンで動かせるレベルまで持っていく
    std::cout << "Serialize..." << std::endl;
    Node<SyntaxNode> syntaxNodePool[64];
    List<SyntaxNode> syntaxNodeList(syntaxNodePool, sizeof(syntaxNodePool) / sizeof(syntaxNodePool[0]));
    Serializer serializer(syntaxNodeList);
    serializer.serializeTree(rootNode);
    // serializer.dumpSeralizedNodeList();

    // 評価器に渡す
    std::cout << "Evaluate..." << std::endl;
    double calcStackPool[64];
    Stack<double> calcStack(calcStackPool, sizeof(calcStackPool) / sizeof(calcStackPool[0]));
    DoubleEvaluator evaluator(calcStack);
    double calculationResult = evaluator.evaluate(syntaxNodeList);
    std::cout << "ans = " << calculationResult << std::endl;

    return 0;
}
