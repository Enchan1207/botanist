/// @file
/// @brief 構文評価器
///

#ifndef BOTANIST_EVALUATOR_H
#define BOTANIST_EVALUATOR_H

#include <collection2/list.hpp>
#include <collection2/stack.hpp>
#include <cstdlib>
#include <cstring>

#include "syntaxtree.hpp"

namespace botanist {

/// @brief 構文評価器 (抽象基底クラス)
/// @tparam Result 計算結果の型
template <typename Result>
class Evaluator {
   public:
    /**
     * @brief 引数に与えられた直列化済みの構文木を解析し、結果を返します。
     *
     * @param node 直列化された構文木へのポインタ
     */
    virtual Result evaluate(collection2::Node<SyntaxNode*>* node) = 0;

    virtual ~Evaluator() = default;
};

/// @brief 構文評価器 (doubleによる実装)
class FPEvaluator final : public Evaluator<double> {
   private:
    /// @brief 計算スタックを管理する配列
    double internalTokensData[64] = {0};

    /// @brief 計算スタック
    collection2::Stack<double> calcStack;

    /**
     * @brief 構文木ノードのcontentを読み、数値化して返す
     *
     * @param node 対象のノード
     * @param value 数値化した値の格納先
     * @return bool 変換に失敗した場合はfalseが返ります。
     */
    bool getValueFromNode(const SyntaxNode* node, double& value) const;

   public:
    FPEvaluator() : calcStack(internalTokensData, sizeof(internalTokensData) / sizeof(internalTokensData[0])){};

    /**
     * @brief 引数に与えられた直列化済みの構文木を解析し、結果を返します。
     *
     * @param node 直列化された構文木へのポインタ
     * @return double 計算結果
     * @note 内部の計算は倍精度浮動小数点(`double`)により行われるため、誤差が発生する場合があります。
     */
    double evaluate(collection2::Node<SyntaxNode*>* node) override;
};

}  // namespace botanist

#endif