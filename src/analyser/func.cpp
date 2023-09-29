/// @file
/// @brief 解析補助関数
///

#include "botanist/analyser.hpp"

namespace botanist {

SyntaxNodeOld* Analyser::createNewNode(const SyntaxNodeOld::Kind kind, SyntaxNodeOld* lhs, SyntaxNodeOld* rhs) {
    // フリーなノードを探す
    SyntaxNodeOld* freeNode = nullptr;
    // TODO: 要素数ハードコードの削除
    for (size_t i = 0; i < 64; i++) {
        auto* node = &(syntaxNodePool[i]);
        if (node->kind != SyntaxNodeOld::Kind::Empty) {
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

collection2::TreeNode<SyntaxNode>* Analyser::createNewNode(const SyntaxNode::Kind kind, collection2::TreeNode<SyntaxNode>* lhs, collection2::TreeNode<SyntaxNode>* rhs) {
    // ノードプールからノードを確保
    auto* newNode = syntaxNodeTree.retainNode({kind, currentTokenNode->element.content, currentTokenNode->element.length});
    if (newNode == nullptr) {
        return nullptr;
    }

    // 確保したノードに左辺・右辺を接続して返す
    newNode->lhs = lhs;
    newNode->rhs = rhs;
    return newNode;
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
