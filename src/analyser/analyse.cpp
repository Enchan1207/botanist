/// @file
/// @brief 構文ツリー構成
///

#include "analyser.hpp"
#include "syntaxtree.hpp"

namespace botanist {

void Analyser::analyse() {
    auto* rootNode = expression();
    dumpSyntaxNode(rootNode);
}

SyntaxNode* Analyser::expression() {
    auto* node = unary();

    if (consumeIf("+", 1)) {
        auto* addNode = getNewNode();
        addNode->kind = SyntaxNode::Kind::Add;
        addNode->applyDataFromToken(&(cursor->element));
        addNode->lhs = node;
        addNode->rhs = expression();
        return node;
    }

    if (consumeIf("-", 1)) {
        auto* subNode = getNewNode();
        subNode->kind = SyntaxNode::Kind::Subtract;
        subNode->applyDataFromToken(&(cursor->element));
        subNode->lhs = node;
        subNode->rhs = expression();
        return node;
    }

    return node;
}

SyntaxNode* Analyser::unary() {
    auto* node = factor();

    if (consumeIf("*", 1)) {
        auto* mulNode = getNewNode();
        mulNode->kind = SyntaxNode::Kind::Multiply;
        mulNode->applyDataFromToken(&(cursor->element));
        mulNode->lhs = node;
        mulNode->rhs = unary();
        return node;
    }

    if (consumeIf("/", 1)) {
        auto* divNode = getNewNode();
        divNode->kind = SyntaxNode::Kind::Divide;
        divNode->applyDataFromToken(&(cursor->element));
        divNode->lhs = node;
        divNode->rhs = unary();
        return node;
    }

    return node;
}

SyntaxNode* Analyser::factor() {
    if (consumeIf("(", 1)) {
        auto* node = expression();
        cursor++;  // 閉じ括弧の分
        return node;
    }

    auto numberNode = getNewNode();
    numberNode->kind = SyntaxNode::Kind::Number;
    numberNode->applyDataFromToken(&(cursor->element));
    cursor++;
    return numberNode;
}

}  // namespace botanist
