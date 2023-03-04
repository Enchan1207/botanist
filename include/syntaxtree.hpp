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
     * @brief 値を渡してノードオブジェクトを初期化
     *
     * @param kind ノードの種類
     * @param lhs 左辺
     * @param rhs 右辺
     * @param content ノードが持つ内容
     * @param length contentの長さ
     */
    SyntaxNode(const SyntaxNode::Kind kind,
               SyntaxNode* lhs,
               SyntaxNode* rhs,
               const char* content,
               size_t length)
        : kind(kind),
          lhs(lhs),
          rhs(rhs),
          content(content),
          length(length){};
};

}  // namespace botanist

#endif /* BOTANIST_SYNTAX_TREE_H */
