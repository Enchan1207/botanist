//
// 数式パーサ
//

#ifndef PARSER_H
#define PARSER_H

#include <collection2/list.hpp>

#include "token.hpp"

/**
 * @brief 与えられた数式を、数値や演算子、括弧等のトークンに分割する
 *
 * @param formula 数式
 */
void tokenize(collection2::List<Token>& list, const char* formula);

#endif /* PARSER_H */
