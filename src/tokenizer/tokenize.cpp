/// @file
/// @brief トークナイズ
///

#include "tokenizer.hpp"

namespace botanist {

collection2::list_size_t Tokenizer::tokenize() {
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
            return static_cast<collection2::list_size_t>(formulaPtr - formula + 1);
        }

        // トークンリストに追加
        tokenList.append(Token(expectedKind, formulaPtr, expectedLength));
        formulaPtr += expectedLength;
    }
    return 0;
}

}  // namespace botanist
