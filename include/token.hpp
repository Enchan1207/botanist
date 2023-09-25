/// @file
/// @brief 数式トークン
///

#ifndef BOTANIST_TOKEN_H
#define BOTANIST_TOKEN_H

#include <stddef.h>
#include <stdint.h>

namespace botanist {

/// @brief 式を構成するトークン
struct Token {
    /// @brief トークンの種類
    enum class Kind : uint8_t {
        /// @brief リスト内に存在しないトークン
        Empty,

        /// @brief 数値
        Number,

        /// @brief 演算子
        Operator,

        /// @brief 括弧
        Bracket,

        /// @brief それ以外のシンボル
        Symbol,

        /// @brief トークン列の終端
        Terminator
    };

    /// @brief このトークンの種類
    Kind kind = Kind::Empty;

    /// @brief トークンが持つ内容
    const char* content = nullptr;

    /// @brief contentの長さ
    size_t length = 0;

    Token() = default;

    /**
     * @brief トークンを生成
     *
     * @param kind このトークンの種類
     * @param content トークンが持つ内容
     * @param length contentの長さ
     */
    Token(Kind kind, const char* content, size_t length)
        : kind(kind), content(content), length(length){};
};

}  // namespace botanist

#endif /* BOTANIST_TOKEN_H */
