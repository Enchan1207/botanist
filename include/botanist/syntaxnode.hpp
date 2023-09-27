/// @file
/// @brief 構文木
///

#ifndef BOTANIST_SYNTAX_NODE_H
#define BOTANIST_SYNTAX_NODE_H

#include <stdint.h>

#include "token.hpp"

namespace botanist {

/// @brief 構文木のノード
struct SyntaxNode {
    /// @brief ノードの種類
    enum class Kind : uint8_t {
        /// @brief ツリー中に存在しないノード
        Empty,

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

        /// @brief 無効なノード
        Invalid
    };

    /// @brief このノードの種類
    Kind kind = Kind::Empty;

    /// @brief ノードが持つ内容
    const char* content = nullptr;

    /// @brief contentの長さ
    size_t length = 0;

    SyntaxNode() = default;

    /**
     * @brief 値を渡してノードオブジェクトを初期化
     *
     * @param kind ノードの種類
     * @param content ノードが持つ内容
     * @param length contentの長さ
     */
    SyntaxNode(const SyntaxNode::Kind kind, const char* content, size_t length)
        : kind(kind), content(content), length(length){};
};

}  // namespace botanist

#endif /* BOTANIST_SYNTAX_NODE_H */
