//
// 数式構成要素のパース
//

#ifndef PARSE_ELEMENT_H
#define PARSE_ELEMENT_H

#include <cctype>
#include <cstddef>
#include <cstring>
#include <iostream>

#include "token.hpp"

/**
 * @brief 与えられた文字列が有効な正の10進実数値として認識できる相対終端位置を返す
 *
 * @param str 文字列
 * @return size_t 数値として認識できる位置
 *
 * @note 先頭に数値を発見できなかった場合は0が返ります。たとえばstrに "X123" が渡された場合は0, "1.23" が渡された場合は4, "12X3" が渡された場合は2が返ります。
 */
size_t parseNumber(char const* str);

/**
 * @brief 与えられた文字列が有効な演算子として認識できる相対終端位置を返す
 *
 * @param str 文字列
 * @return size_t 演算子として認識できる位置
 *
 * @note 加減乗除(+, -, *, /), 比較(<, >), ビットシフト(<<, >>) 演算子をサポートしています。先頭に演算子を発見できなかった場合は0が返ります。
 */
size_t parseOperator(char const* str);

/**
 * @brief 与えられた文字列が括弧として認識できる相対終端位置を返す
 *
 * @param str 文字列
 * @return size_t 括弧として認識できる位置
 *
 * @note 現在の実装では、この関数は単純に引数が開き括弧または閉じ括弧のいずれかに合致するかを調べます。
 */
size_t parseBracket(char const* str);

#endif /* PARSE_ELEMENT_H */
