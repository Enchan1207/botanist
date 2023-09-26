/// @file
/// @brief 評価器 (倍精度浮動小数点数)
///

#include "botanist/evaluator/double.hpp"

namespace botanist {

bool DoubleEvaluator::getValueFromNode(const SyntaxNode* node, double& value) const {
    // 数値ノードを期待する
    if (node == nullptr || node->kind != SyntaxNode::Kind::Number) {
        return false;
    }

    // 文字列を取り出す
    char contentStr[node->length + 1];
    memset(contentStr, '\0', node->length + 1);
    memcpy(contentStr, node->content, node->length);

    // strtodに渡す
    const char* contentStrConst = contentStr;
    char** endPtr = nullptr;
    double result = strtod(contentStrConst, endPtr);
    if (endPtr != nullptr) {
        return false;
    }
    value = result;
    return true;
}

double DoubleEvaluator::evaluate(collection2::Node<SyntaxNode*>* node) {
    auto* currentNode = node;
    while (currentNode != nullptr) {
        const auto* syntaxNode = currentNode->element;

        // 数値ならスタックにpush
        double value = 0;
        if (getValueFromNode(syntaxNode, value)) {
            calcStack.push(value);
            currentNode = currentNode->next;
            continue;
        }

        // そうでなければ左辺値と右辺値を取り出す
        double rhs = 0;
        calcStack.pop(&rhs);
        double lhs = 0;
        calcStack.pop(&lhs);

        // 計算して積む
        switch (syntaxNode->kind) {
            case SyntaxNode::Kind::Add:
                calcStack.push(lhs + rhs);
                break;

            case SyntaxNode::Kind::Subtract:
                calcStack.push(lhs - rhs);
                break;

            case SyntaxNode::Kind::Multiply:
                calcStack.push(lhs * rhs);
                break;

            case SyntaxNode::Kind::Divide:
                calcStack.push(lhs / rhs);
                break;

            default:
                break;
        }

        // 次のノードへ
        currentNode = currentNode->next;
    }

    // 最後に残った値が計算結果
    double calculationResult = 0;
    calcStack.pop(&calculationResult);

    return calculationResult;
}

}  // namespace botanist
