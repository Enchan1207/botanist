/// @file
/// @brief 解析補助関数
///

#include "analyser.hpp"
#include "syntaxtree.hpp"

namespace botanist {

SyntaxNode* Analyser::createNewNode(const SyntaxNode::Kind kind, SyntaxNode* lhs, SyntaxNode* rhs, const Token* token) {
    // TODO: 要素数ハードコードの削除
    for (size_t i = 0; i < 64; i++) {
        // フリーなノードを探し、値を入れる
        auto* targetNode = &(syntaxNodePool[i]);
        if (targetNode->kind != SyntaxNode::Kind::Invalid) {
            continue;
        }
        targetNode->kind = kind;
        targetNode->lhs = lhs;
        targetNode->rhs = rhs;
        targetNode->applyDataFromToken(token);
        return targetNode;
    }
    return nullptr;
}

bool Analyser::consumeIf(const Token::Kind kind) {
    if (currentToken->element.kind == kind) {
        currentToken = currentToken->next;
        return true;
    }
    return false;
}

bool Analyser::consumeIf(const char* content, const size_t length) {
    // 比較対象の方が短いことはありえない
    if (length > currentToken->element.length) {
        return false;
    }

    // 一致しなくなったらfalseを返す
    for (size_t i = 0; i < length; i++) {
        if (*(currentToken->element.content + i) != *(content + i)) {
            return false;
        }
    }

    currentToken = currentToken->next;
    return true;
}

}  // namespace botanist
