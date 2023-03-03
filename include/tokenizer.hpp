/// @file
/// @brief 数式トークナイザ
///

#ifndef BOTANIST_TOKENIZER_H
#define BOTANIST_TOKENIZER_H

#include <cctype>
#include <collection2/list.hpp>
#include <cstddef>
#include <cstring>

#include "token.hpp"

namespace botanist {

class Tokenizer final {
   private:
    /// @brief 数式
    char const* formula;

    /// @brief トークンリストのノードを管理する配列
    collection2::Node<Token> internalTokensData[64];

    /// @brief 数式から構成されたトークンのリスト
    collection2::List<Token> tokenList;

    /**
     * @brief 与えられた文字列がなんらかのトークンとして成立するかを調べ、成立する場合はその長さを返す
     *
     * @param str 文字列
     * @param kind 成立したトークンの種類の格納先
     * @param parsedTokenLength トークン長の格納先
     *
     * @return bool トークンとして成立するかどうか
     */
    bool tryParse(char const* str, Token::Kind& kind, size_t& parsedTokenLength) const;

    /**
     * @brief 与えられた文字列が有効な正の10進実数値として認識できる相対終端位置を返す
     *
     * @param str 文字列
     * @return size_t 数値として認識できる位置
     *
     * @note 先頭に数値を発見できなかった場合は0が返ります。たとえばstrに "X123" が渡された場合は0, "1.23" が渡された場合は4, "12X3" が渡された場合は2が返ります。
     */
    size_t tryParseAsNumber(char const* str) const;

    /**
     * @brief 与えられた文字列が有効な演算子として認識できる相対終端位置を返す
     *
     * @param str 文字列
     * @return size_t 演算子として認識できる位置
     *
     * @note 加減乗除(+, -, *, /), 比較(<, >), ビットシフト(<<, >>) 演算子をサポートしています。先頭に演算子を発見できなかった場合は0が返ります。
     */
    size_t tryParseAsOperator(char const* str) const;

    /**
     * @brief 与えられた文字列が括弧として認識できる相対終端位置を返す
     *
     * @param str 文字列
     * @return size_t 括弧として認識できる位置
     *
     * @note 現在の実装では、この関数は単純に引数が開き括弧または閉じ括弧のいずれかに合致するかを調べます。
     */
    size_t tryParseAsBracket(char const* str) const;

   public:
    /**
     * @brief 数式を与えてトークナイザを初期化
     *
     * @param formula トークナイズする数式
     */
    explicit Tokenizer(char const* formula)
        : formula(formula),
          tokenList(internalTokensData, sizeof(internalTokensData) / sizeof(internalTokensData[0])){};

    /**
     * @brief 与えられた数式を、数値や演算子、括弧等のトークンに分割する
     *
     * @return collection2::list_size_t 正常にパースできなかった場合の位置
     * @note トークナイズに成功した場合は0、失敗した場合は何文字目で失敗したかが返ります。
     */
    collection2::list_size_t tokenize();

    /**
     * @brief 最初のトークンへのポインタを取得する
     *
     * @return collection2::Node<Token>* 最初のトークンへのポインタ
     */
    collection2::Node<Token>* tokens() {
        return tokenList.head();
    }
};

}  // namespace botanist

#endif /* BOTANIST_TOKENIZER_H */
