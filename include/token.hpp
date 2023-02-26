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

    // 演算子、括弧等、数値以外のシンボル
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

    // 次のトークンへのポインタ
    Token* next;

    // トークンが持つ内容
    char* content;

    // 文字数
    size_t length;
};

#endif /* TOKEN_H */
