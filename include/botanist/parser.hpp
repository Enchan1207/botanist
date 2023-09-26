//
// 数式パーサ
//

#ifndef BOTANIST_PARSER_H
#define BOTANIST_PARSER_H

#include <collection2/list.hpp>

#include "analyser.hpp"
#include "serializer.hpp"
#include "tokenizer.hpp"

namespace botanist {

/// @brief 数式をパースして評価する
class Parser final {
   private:
    /// @brief トークナイザ
    Tokenizer tokenizer;

    /// @brief アナライザ
    Analyser analyser;

    /// @brief シリアライザ
    Serializer serializer;

   public:
    /**
     * @brief 入力された数式を評価する
     *
     * @param formula 数式
     * @return SyntaxNode* 評価器に入力できるよう変換されたノード列
     */
    collection2::Node<SyntaxNode*>* parse(char const* formula) {
        if (tokenizer.tokenize(formula) != 0) {
            return nullptr;
        }
        if (analyser.analyse(tokenizer.tokens()) != 0) {
            return nullptr;
        }
        return serializer.serializeTree(analyser.rootNode());
    }
};

}  // namespace botanist

#endif /* BOTANIST_PARSER_H */
