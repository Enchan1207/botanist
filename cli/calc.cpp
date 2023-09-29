//
// 単純な計算機
//

#include <collection2/list.hpp>
#include <collection2/tree.hpp>
#include <iostream>

#include "botanist/analyser.hpp"
#include "botanist/evaluator/double.hpp"
#include "botanist/serializer.hpp"
#include "botanist/tokenizer.hpp"

using namespace botanist;
using namespace collection2;

int main(int argc, char const* argv[]) {
    // 対象の式は実行引数経由で渡されることを想定
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " formula" << std::endl;
        return 1;
    }
    char const* formula = argv[1];

    // トークンリストを作成
    Node<Token> tokenPool[64];
    List<Token> tokenList(tokenPool, sizeof(tokenPool) / sizeof(tokenPool[0]));
    Tokenizer tokenizer(tokenList);

    // 構文ツリーを作成
    TreeNode<SyntaxNode> nodePool[16];
    Tree<SyntaxNode> syntaxTree(nodePool, sizeof(nodePool) / sizeof(nodePool[0]));
    Analyser analyser(tokenList, syntaxTree);
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

    // 構文ツリーを直列化
    Node<SyntaxNode> syntaxNodePool[64];
    List<SyntaxNode> syntaxNodeList(syntaxNodePool, sizeof(syntaxNodePool) / sizeof(syntaxNodePool[0]));
    Serializer serializer(syntaxNodeList);
    serializer.serializeTree(analyser.rootNode());

    // 評価
    double calcStackPool[64];
    Stack<double> calcStack(calcStackPool, sizeof(calcStackPool) / sizeof(calcStackPool[0]));
    DoubleEvaluator evaluator(calcStack);
    double result = evaluator.evaluate(syntaxNodeList);
    std::cout << formula << " = " << result << std::endl;
    return 0;
}
