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
        if (forward("+", 1) && currentTokenNode != nullptr) {
            // 加算ノードを構成
            // TODO: 解析途中でプールがいっぱいになったらどうしよう?
            auto newNode = createNewNode();
            newNode->kind = SyntaxNodeKind::Add;
            newNode->content = currentTokenNode->element.content;
            newNode->length = currentTokenNode->element.length;
            newNode->lhs = node;

            // 右辺値は後で計算する
            newNode->rhs = unary();
            node = newNode;
            continue;
        }

        if (forward("-", 1) && currentTokenNode != nullptr) {
            // 減算ノードを構成
            auto newNode = createNewNode();
            newNode->kind = SyntaxNodeKind::Subtract;
            newNode->content = currentTokenNode->element.content;
            newNode->length = currentTokenNode->element.length;
            newNode->lhs = node;

            // 右辺値は後で計算する
            newNode->rhs = unary();
            node = newNode;
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
            auto newNode = createNewNode();
            newNode->kind = SyntaxNodeKind::Multiply;
            newNode->content = currentTokenNode->element.content;
            newNode->length = currentTokenNode->element.length;
            newNode->lhs = node;

            // 右辺値は後で計算する
            newNode->rhs = factor();
            node = newNode;
            continue;
        }

        if (forward("/", 1)) {
            // 除算ノードを構成
            auto newNode = createNewNode();
            newNode->kind = SyntaxNodeKind::Divide;
            newNode->content = currentTokenNode->element.content;
            newNode->length = currentTokenNode->element.length;
            newNode->lhs = node;

            // 右辺値は後で計算する
            newNode->rhs = factor();
            node = newNode;
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
        auto newNode = createNewNode();
        newNode->kind = SyntaxNodeKind::Number;
        newNode->content = currentTokenNode->element.content;
        newNode->length = currentTokenNode->element.length;
        newNode->lhs = nullptr;
        newNode->rhs = nullptr;
        currentTokenNode = currentTokenNode->next;
        return newNode;
    }

    return nullptr;
}

}  // namespace botanist
