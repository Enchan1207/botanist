/// @file
/// @brief 解析補助関数
///

#include "botanist/analyser.hpp"

namespace botanist {

SyntaxNode* Analyser::createNewNode(const SyntaxNode::Kind kind, SyntaxNode* lhs, SyntaxNode* rhs) {
    // フリーなノードを探す
    SyntaxNode* freeNode = nullptr;
    // TODO: 要素数ハードコードの削除
    for (size_t i = 0; i < 64; i++) {
        auto* node = &(syntaxNodePool[i]);
        if (node->kind != SyntaxNode::Kind::Empty) {
            continue;
        }
        freeNode = node;
        break;
    }

    if (freeNode == nullptr) {
        // 空きがなかった
        return nullptr;
    }

    freeNode->kind = kind;
    freeNode->lhs = lhs;
    freeNode->rhs = rhs;
    if (currentTokenNode != nullptr) {
        auto token = currentTokenNode->element;
        freeNode->content = token.content;
        freeNode->length = token.length;
    } else {
        freeNode->content = nullptr;
        freeNode->length = 0;
    }
    return freeNode;
}

bool Analyser::expect(const Token::Kind kind) const {
    // 終端チェック
    if (currentTokenNode == nullptr) {
        return false;
    }
    return currentTokenNode->element.kind == kind;
}

bool Analyser::expect(const char* content, const size_t length) const {
    // 終端チェック
    if (currentTokenNode == nullptr) {
        return false;
    }

    // 比較対象の方が短いことはありえない
    if (length > currentTokenNode->element.length) {
        return false;
    }

    // 一致しなくなったらfalseを返す
    for (size_t i = 0; i < length; i++) {
        if (*(currentTokenNode->element.content + i) != *(content + i)) {
            return false;
        }
    }
    return true;
}

bool Analyser::forward(const Token::Kind kind) {
    auto result = expect(kind);
    if (result) {
        advanceTokenList();
    }
    return result;
}

bool Analyser::forward(const char* content, const size_t length) {
    auto result = expect(content, length);
    if (result) {
        advanceTokenList();
    }
    return result;
}

}  // namespace botanist
