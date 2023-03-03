/// @file
/// @brief 数式アナライザ
///

#ifndef BOTANIST_ANALYSER_H
#define BOTANIST_ANALYSER_H

#include <cctype>
#include <collection2/list.hpp>
#include <cstddef>
#include <cstring>

#include "token.hpp"

namespace botanist {

class Analyser final {
   private:
    /// @brief 最初のトークンへのポインタ
    const collection2::Node<Token>* firstTokenPtr;

    /// @brief アナライザが現在見ているトークンリストの位置
    collection2::Node<Token>* currentToken;

   public:
    /**
     * @brief トークンリストを与えてアナライザを初期化
     *
     * @param firstToken 最初のトークンへのポインタ
     */
    explicit Analyser(collection2::Node<Token>* firstToken) : firstTokenPtr(firstToken), currentToken(firstToken){};

    /**
     * @brief トークナイズされた数式から構文ツリーを生成
     */
    void analyse();
};

}  // namespace botanist

#endif /* BOTANIST_ANALYSER_H */
