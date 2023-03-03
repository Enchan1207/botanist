/// @file
/// @brief トークナイズ
///

#include "tokenizer.hpp"

namespace botanist {

collection2::Node<Token>* Tokenizer::tokenize() {
    auto formulaPtr = formula;
    while (*formulaPtr) {
        // 空白や改行など、トークンに無関係なものは読み飛ばす
        if (isspace(*formulaPtr)) {
            formulaPtr++;
            continue;
        }

        // なんらかのトークンにマッチするか調べる
        Token::Kind expectedKind = Token::Kind::Invalid;
        size_t expectedLength = 0;
        if (!tryParse(formulaPtr, expectedKind, expectedLength)) {
            // TODO: エラーをもう少しわかりやすく
            return nullptr;
        }

        // トークンリストに追加
        tokens.append(Token(expectedKind, formulaPtr, expectedLength));
        formulaPtr += expectedLength;
    }

    return tokens.head();
}

}  // namespace botanist
