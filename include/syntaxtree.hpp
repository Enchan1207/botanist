/// @file
/// @brief 構文ツリー
///

#ifndef BOTANIST_SYNTAX_TREE_H
#define BOTANIST_SYNTAX_TREE_H

#include <cstddef>

#include "token.hpp"

namespace botanist {

/// @brief 構文ツリーのノード
struct SyntaxNode {
    /// @brief ノードの種類
    enum class Kind : uint8_t {
        /// @brief 加算
        Add,

        /// @brief 減算
        Subtract,

        /// @brief 乗算
        Multiply,

        /// @brief 除算
        Divide,

        /// @brief 数値
        Number,

        /// @brief 無効な、またはツリー中に存在しないノード
        Invalid
    };

    /// @brief このノードの種類
    Kind kind = Kind::Invalid;

    /// @brief 左辺
    SyntaxNode* lhs = nullptr;

    /// @brief 右辺
    SyntaxNode* rhs = nullptr;

    /// @brief ノードが持つ内容
    const char* content = nullptr;

    /// @brief contentの長さ
    size_t length = 0;

    SyntaxNode() = default;

    /**
     * @brief トークンの持つ内容をノードにコピーする
     *
     * @param token コピー元のトークン
     */
    void applyDataFromToken(const Token* token) {
        content = token->content;
        length = token->length;
    }
};

}  // namespace botanist

#endif /* BOTANIST_SYNTAX_TREE_H */
