//
// 数式のパース
//

#ifndef PARSER_H
#define PARSER_H

#include <collection2/list.hpp>
#include <cstddef>

#include "token.hpp"

/**
 * @brief 与えられた文字列が有効な正の10進実数値として認識できる相対終端位置を返す
 *
 * @param str 文字列。任意の数字から始まっている必要があります。
 * @return size_t 数値として認識できる位置
 *
 * @note strに "X123" が渡された場合は0, "1.23" が渡された場合は4, "12X3" が渡された場合は2が返ります。
 */
size_t parseNumber(char const* str);

/**
 * @brief 与えられた数式を、数値や演算子、括弧等のトークンに分割する
 *
 * @param formula 数式
 */
void tokenize(collection2::List<Token>& list, const char* formula);

#endif /* PARSER_H */
