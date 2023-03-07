/// @file
/// @brief 構文木構成
///

#include "analyser.hpp"

namespace botanist {

using SyntaxNodeKind = SyntaxNode::Kind;

SyntaxNode* Analyser::analyse() {
    return expression();
}

SyntaxNode* Analyser::expression() {
    auto* node = unary();

    while (true) {
        if (forward("+", 1)) {
            // 加算ノードを構成
            // TODO: 解析途中でプールがいっぱいになったらどうしよう?
            node = createNewNode(SyntaxNodeKind::Add, node, unary());
            continue;
        }

        if (forward("-", 1)) {
            // 減算ノードを構成
            node = createNewNode(SyntaxNodeKind::Subtract, node, unary());
            continue;
        }

        break;
    }

    return node;
}

SyntaxNode* Analyser::unary() {
    auto* node = factor();

    while (true) {
        if (forward("*", 1)) {
            // 乗算ノードを構成
            node = createNewNode(SyntaxNodeKind::Multiply, node, factor());
            continue;
        }

        if (forward("/", 1)) {
            // 除算ノードを構成
            node = createNewNode(SyntaxNodeKind::Divide, node, factor());
            continue;
        }

        break;
    }

    return node;
}

SyntaxNode* Analyser::factor() {
    if (forward("(", 1)) {
        auto* node = expression();

        // 閉じ括弧があってほしい
        if (!forward(")", 1)) {
            return nullptr;
        }
        return node;
    }

    if (currentTokenNode != nullptr) {
        auto newNode = createNewNode(SyntaxNodeKind::Number);
        currentTokenNode = currentTokenNode->next;
        return newNode;
    }

    return nullptr;
}

}  // namespace botanist
