/// @file
/// @brief 数式アナライザ
///

#ifndef BOTANIST_ANALYSER_H
#define BOTANIST_ANALYSER_H

#include <cctype>
#include <collection2/list.hpp>
#include <cstddef>
#include <cstring>

#include "syntaxtree.hpp"
#include "token.hpp"

namespace botanist {

class Analyser final {
   private:
    /// @brief 最初のトークンへのポインタ
    const collection2::Node<Token>* firstTokenPtr;

    /// @brief アナライザが現在見ているトークンリストの位置
    collection2::Node<Token>* currentToken;

    /// @brief 構文ツリーを構成するノードのプール
    SyntaxNode syntaxNodePool[64];

    /**
     * @brief ノードプールから未割り当てのノードを探し、そのポインタを返す
     *
     * @return SyntaxNode* 使用可能なノードへのポインタ
     *
     * @note ノードプールがいっぱいになっている場合はnullptrが返ります。
     */
    SyntaxNode* createNewNode();

    /**
     * @brief 今見ているトークンの種類と引数が一致するか調べる
     *
     * @param kind 比較するトークンの種類
     * @return bool トークンと一致しない場合はfalseが返ります。
     */
    bool expect(const Token::Kind kind) const;

    /**
     * @brief 今見ているトークンの内容と引数が一致するか調べる
     *
     * @param content 比較するトークンの内容
     * @param length contentの文字数
     * @return bool トークンと一致しない場合はfalseが返ります。
     *
     * @note 長さが一致している必要はありません(C++20におけるstarts_with関数相当の処理). また、第二引数はNULを考慮しません(`+`を評価したい場合は `consume("+", 1)` を呼び出してください)。
     */
    bool expect(const char* content, const size_t length = 1) const;

    /**
     * @brief 今見ているトークンの種類と引数が一致する場合はトークンカーソルを進める
     *
     * @param kind 比較するトークンの種類
     * @return bool トークンと一致しない場合はfalseが返ります。
     */
    bool consume(const Token::Kind kind);

    /**
     * @brief 今見ているトークンの内容と引数が一致する場合はトークンカーソルを進める
     *
     * @param content 比較するトークンの内容
     * @param length contentの文字数
     * @return bool トークンと一致しない場合はfalseが返ります。
     *
     * @note 長さが一致している必要はありません(C++20におけるstarts_with関数相当の処理). また、第二引数はNULを考慮しません(`+`を評価したい場合は `consume("+", 1)` を呼び出してください)。
     */
    bool consume(const char* content, const size_t length = 1);

    /**
     * @brief アナライザが見ているトークンを式の開始とみなしてパースを試みる
     *
     * @return SyntaxNode* パース結果
     */
    SyntaxNode* expression();

    /**
     * @brief アナライザが見ているトークンを単一の項とみなしてパースを試みる
     *
     * @return SyntaxNode* パース結果
     */
    SyntaxNode* unary();

    /**
     * @brief アナライザが見ているトークンを単一の因子とみなしてパースを試みる
     *
     * @return SyntaxNode* パース結果
     */
    SyntaxNode* factor();

    /**
     * @brief 構文ノードをダンプ
     *
     * @param node 対象のノード
     */
    void dumpSyntaxNode(SyntaxNode* node) const;

   public:
    /**
     * @brief トークンリストを与えてアナライザを初期化
     *
     * @param firstToken 最初のトークンへのポインタ
     */
    explicit Analyser(collection2::Node<Token>* firstToken)
        : firstTokenPtr(firstToken), currentToken(firstToken){};

    /**
     * @brief トークナイズされた数式から構文ツリーを生成
     */
    void analyse();

    /**
     * @brief 構文ツリーをダンプ
     *
     * @param node 対象のノード
     */
    void dumpSyntaxTree(SyntaxNode* node) const;
};

}  // namespace botanist

#endif /* BOTANIST_ANALYSER_H */
