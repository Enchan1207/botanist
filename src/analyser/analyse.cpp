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
        return createNewNode(SyntaxNodeKind::Add, node, expression(), &(cursor->element));
    }

    if (consumeIf("-", 1)) {
        return createNewNode(SyntaxNodeKind::Subtract, node, expression(), &(cursor->element));
    }

    return node;
}

SyntaxNode* Analyser::unary() {
    auto* node = factor();

    if (consumeIf("*", 1)) {
        return createNewNode(SyntaxNodeKind::Multiply, node, unary(), &(cursor->element));
    }

    if (consumeIf("/", 1)) {
        return createNewNode(SyntaxNodeKind::Divide, node, unary(), &(cursor->element));
    }

    return node;
}

SyntaxNode* Analyser::factor() {
    if (consumeIf("(", 1)) {
        auto* node = expression();
        cursor++;  // 閉じ括弧の分
        return node;
    }

    auto* numberNode = createNewNode(SyntaxNodeKind::Number, nullptr, nullptr, &(cursor->element));
    cursor++;
    return numberNode;
}

}  // namespace botanist
