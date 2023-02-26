//
// 数式のパース
//

#ifndef PARSER_H
#define PARSER_H

#include "token.hpp"

/**
 * @brief 与えられた数式を、数値や演算子、括弧等のトークンに分割する
 *
 * @param formula 数式
 */
void tokenize(char const* formula);

#endif /* PARSER_H */
