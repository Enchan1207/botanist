/// @file
/// @brief 解析補助関数
///

#include "analyser.hpp"
#include "syntaxtree.hpp"

namespace botanist {

SyntaxNode* Analyser::getNewNode() {
    // TODO: 要素数ハードコードの削除
    for (size_t i = 0; i < 64; i++) {
        // フリーなノードを探す
        if (syntaxNodePool[i].kind != SyntaxNode::Kind::Invalid) {
            continue;
        }
        return &(syntaxNodePool[i]);
    }
    return nullptr;
}

bool Analyser::consumeIf(const Token::Kind kind) {
    if (cursor->element.kind == kind) {
        cursor++;
        return true;
    }
    return false;
}

bool Analyser::consumeIf(const char* content, const size_t length) {
    // 比較対象の方が短いことはありえない
    if (length > cursor->element.length) {
        return false;
    }

    // 一致しなくなったらfalseを返す
    for (size_t i = 0; i < length; i++) {
        if (*(cursor->element.content + i) != *(content + i)) {
            return false;
        }
    }

    cursor++;
    return true;
}

}  // namespace botanist
