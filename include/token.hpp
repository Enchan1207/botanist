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
    // 種別
    TokenKind kind;

    // トークンが持つ内容
    const char* content = nullptr;

    // contentの長さ
    size_t length = 0;
};

#endif /* TOKEN_H */
