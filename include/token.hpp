//
// 数式トークン
//

#ifndef TOKEN_H
#define TOKEN_H

#include <cstddef>

/**
 * @brief トークン種別
 */
enum class TokenKind {
    // 数値
    Number,

    // 演算子
    Operator,

    // 括弧
    Bracket,

    // それ以外のシンボル
    Symbol,

    // トークン列の終端
    Terminator
};

/**
 * @brief 式を構成するトークン
 */
struct Token {
    // トークン種別
    TokenKind kind = TokenKind::Terminator;

    // トークンが持つ内容
    const char* content = nullptr;

    // contentの長さ
    size_t length = 0;

    Token() = default;

    /**
     * @brief トークンを生成
     *
     * @param kind トークン種別
     * @param content トークンが持つ内容
     * @param length contnetの長さ
     */
    Token(TokenKind kind, const char* content, size_t length) : kind(kind), content(content), length(length){};
};

#endif /* TOKEN_H */
