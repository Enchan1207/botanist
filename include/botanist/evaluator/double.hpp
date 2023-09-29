/// @file
/// @brief 構文評価器 (倍精度浮動小数点数)
///

#ifndef BOTANIST_EVALUATOR_DOUBLE_H
#define BOTANIST_EVALUATOR_DOUBLE_H

#include <stdlib.h>
#include <string.h>

#include <collection2/list.hpp>
#include <collection2/stack.hpp>

#include "botanist/syntaxnode.hpp"

namespace botanist {

/// @brief 構文評価器 (倍精度浮動小数点数)
class DoubleEvaluator final {
   private:
    /// @brief 計算に使用するスタック
    collection2::Stack<double>& calcStack;

    /**
     * @brief 構文木ノードのcontentを読み、数値化して返す
     *
     * @param node 対象のノード
     * @param value 数値化した値の格納先
     * @return bool 変換に失敗した場合はfalseが返ります。
     */
    bool getValueFromNode(const SyntaxNode& node, double& value) const;

   public:
    explicit DoubleEvaluator(collection2::Stack<double>& calcStack) : calcStack(calcStack){};

    /**
     * @brief 引数に与えられた直列化済みの構文木を解析し、結果を返します。
     *
     * @param nodeList 直列化された構文ノードのリスト
     * @return double 計算結果
     * @note 内部の計算は倍精度浮動小数点(`double`)により行われるため、誤差が発生する場合があります。
     */
    double evaluate(collection2::List<SyntaxNode>& nodeList);
};

}  // namespace botanist

#endif