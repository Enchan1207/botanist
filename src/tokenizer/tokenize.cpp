/// @file
/// @brief トークナイズ
///

#include "botanist/tokenizer.hpp"

namespace botanist {

size_t Tokenizer::tokenize(char const* formula) {
    initializeTokenList();

    auto formulaPtr = formula;
    while (*formulaPtr) {
        // 空白や改行など、トークンに無関係なものは読み飛ばす
        if (isspace(*formulaPtr)) {
            formulaPtr++;
            continue;
        }

        // なんらかのトークンにマッチするか調べる
        Token::Kind expectedKind = Token::Kind::Empty;
        size_t expectedLength = 0;
        if (!tryParse(formulaPtr, expectedKind, expectedLength)) {
            isLastTokenizationSucceeded = false;
            return static_cast<size_t>(formulaPtr - formula + 1);
        }

        // トークンリストに追加
        auto result = tokenList.append(Token(expectedKind, formulaPtr, expectedLength));
        if (result != collection2::OperationResult::Success) {
            isLastTokenizationSucceeded = false;
            return static_cast<size_t>(formulaPtr - formula + 1);
        }
        formulaPtr += expectedLength;
    }

    isLastTokenizationSucceeded = true;
    return 0;
}

}  // namespace botanist
