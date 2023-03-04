/// @file
/// @brief 構文ツリー構成
///

#include "analyser.hpp"
#include "syntaxtree.hpp"

namespace botanist {

using SyntaxNodeKind = SyntaxNode::Kind;

void Analyser::analyse() {
    auto* rootNode = expression();
    dumpSyntaxNode(rootNode);
}

SyntaxNode* Analyser::expression() {
    auto* node = unary();

    if (consumeIf("+", 1)) {
        return createNewNode(SyntaxNodeKind::Add, node, expression(), &(currentToken->element));
    }

    if (consumeIf("-", 1)) {
        return createNewNode(SyntaxNodeKind::Subtract, node, expression(), &(currentToken->element));
    }

    return node;
}

SyntaxNode* Analyser::unary() {
    auto* node = factor();

    if (consumeIf("*", 1)) {
        return createNewNode(SyntaxNodeKind::Multiply, node, unary(), &(currentToken->element));
    }

    if (consumeIf("/", 1)) {
        return createNewNode(SyntaxNodeKind::Divide, node, unary(), &(currentToken->element));
    }

    return node;
}

SyntaxNode* Analyser::factor() {
    if (consumeIf("(", 1)) {
        auto* node = expression();
        currentToken = currentToken->next;  // 閉じ括弧の分
        return node;
    }

    auto* numberNode = createNewNode(SyntaxNodeKind::Number, nullptr, nullptr, &(currentToken->element));
    currentToken = currentToken->next;
    return numberNode;
}

}  // namespace botanist
