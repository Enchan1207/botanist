/// @file
/// @brief 数式トークナイザ
///

#ifndef BOTANIST_TOKENIZER_H
#define BOTANIST_TOKENIZER_H

#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <collection2/list.hpp>

#include "token.hpp"

namespace botanist {

/// @brief 数式を項や演算子、括弧など意味のある単位に分割する
class Tokenizer final {
   private:
    /// @brief トークンリストのノードを管理する配列
    collection2::Node<Token> internalTokensData[64];

    /// @brief 数式から構成されたトークンのリスト
    collection2::List<Token> tokenList;

    /// @brief 最後に行ったトークナイズ処理の結果
    bool isLastTokenizationSucceeded = false;

    /**
     * @brief トークンリストを初期化する
     */
    void initializeTokenList();

    /**
     * @brief 与えられた文字列がトークンとして成立するかを調べ、成立する場合は種類と長さを返す
     *
     * @param str 文字列
     * @param kind 成立したトークンの種類の格納先
     * @param length トークン長の格納先
     *
     * @return bool トークンとして成立するかどうか
     */
    bool tryParse(char const* str, Token::Kind& kind, size_t& length) const;

    /**
     * @brief 与えられた文字列が有効な正の10進実数値として認識できる相対終端位置を返す
     *
     * @param str 文字列
     * @return size_t 数値として認識できる位置
     *
     * @note 先頭に数値を発見できなかった場合は0が返ります。
     *       たとえばstrに "X123" が渡された場合は0, "1.23" が渡された場合は4が返ります。
     */
    size_t tryParseAsNumber(char const* str) const;

    /**
     * @brief 与えられた文字列が有効な演算子として認識できる相対終端位置を返す
     *
     * @param str 文字列
     * @return size_t 演算子として認識できる位置
     *
     * @note 加減乗除, 比較, ビットシフト演算子をサポートしています。
     *       先頭に演算子を発見できなかった場合は0が返ります。
     */
    size_t tryParseAsOperator(char const* str) const;

    /**
     * @brief 与えられた文字列が括弧として認識できる相対終端位置を返す
     *
     * @param str 文字列
     * @return size_t 括弧として認識できる位置
     *
     * @note 現在の実装では、引数が'('または')'のいずれかに合致するかを調べるのみです。
     */
    size_t tryParseAsBracket(char const* str) const;

   public:
    explicit Tokenizer()
        : tokenList(internalTokensData, sizeof(internalTokensData) / sizeof(internalTokensData[0])){};

    /**
     * @brief 与えられた数式を、数値や演算子、括弧等のトークンに分割する
     * @param formula 数式
     *
     * @return size_t 正常にパースできなかった式の位置
     * @note トークナイズに成功した場合は0が返ります。
     */
    size_t tokenize(char const* formula);

    /**
     * @brief トークンのリストを取得
     *
     * @return collection2::Node<Token>* 最初のトークンへのポインタ
     * @note 直近で行われたトークナイズに失敗した場合はnullptrが返ります。
     */
    collection2::Node<Token>* tokens() const {
        return isLastTokenizationSucceeded ? tokenList.head() : nullptr;
    }

    /**
     * @brief トークンリストのダンプ
     *
     * @param colorlized 出力にANSIエスケープシーケンスによる色付けを行うか
     */
    void dumpTokenList(bool colorlized = true) const;
};

}  // namespace botanist

#endif /* BOTANIST_TOKENIZER_H */
