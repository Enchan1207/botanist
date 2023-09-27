/// @file
/// @brief 解析補助関数
///

#include "botanist/analyser.hpp"

namespace botanist {

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
