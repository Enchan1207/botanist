/// @file
/// @brief 数式アナライザ
///

#ifndef BOTANIST_ANALYSER_H
#define BOTANIST_ANALYSER_H

#include <collection2/list.hpp>
#include <collection2/tree.hpp>

#include "syntaxnode.hpp"
#include "token.hpp"

namespace botanist {

/// @brief トークナイズされた数式を解析し、構文木を形成する
class Analyser final {
   private:
    /// @brief トークンリスト
    const collection2::List<Token>& tokenList;

    /// @brief アナライザが現在見ているトークンリストのノード
    collection2::Node<Token>* currentTokenNode = nullptr;

    /// @brief 構文木
    collection2::Tree<SyntaxNode>& syntaxNodeTree;

    /// @brief トークンリストがどこまで進んだか
    size_t tokenIndex = 0;

    /// @brief 構文解析結果のルートノード
    collection2::TreeNode<SyntaxNode>* root = nullptr;

    /**
     * @brief ノードプールから未割り当てのノードを探し、現在のトークンの情報を割り当てて返す
     *
     * @param kind 設定するノード種別
     * @param lhs 左辺値
     * @param rhs 右辺値
     * @return collection2::TreeNode<SyntaxNode>* 構成されたノードへのポインタ
     * @note ノードプールがいっぱいになっている場合はnullptrが返ります。
     *       ノードの内容はメンバ `currentTokenNode` より取得されます。
     */
    collection2::TreeNode<SyntaxNode>* createNewNode(
        const SyntaxNode::Kind kind,
        collection2::TreeNode<SyntaxNode>* lhs = nullptr,
        collection2::TreeNode<SyntaxNode>* rhs = nullptr);

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
     * @note 長さが一致している必要はありません(C++20におけるstarts_with関数相当の処理).
     *       また、第二引数はNULを考慮しません(`+`を評価したい場合は `expect("+", 1)` を呼び出してください)。
     */
    bool expect(const char* content, const size_t length = 1) const;

    /**
     * @brief 今見ているトークンの種類と引数が一致する場合はトークンカーソルを進める
     *
     * @param kind 比較するトークンの種類
     * @return bool トークンと一致しない場合はfalseが返ります。
     */
    bool forward(const Token::Kind kind);

    /**
     * @brief 今見ているトークンの内容と引数が一致する場合はトークンカーソルを進める
     *
     * @param content 比較するトークンの内容
     * @param length contentの文字数
     * @return bool トークンと一致しない場合はfalseが返ります。
     *
     * @note 長さが一致している必要はありません(C++20におけるstarts_with関数相当の処理).
     *       また、第二引数はNULを考慮しません(`+`を評価したい場合は `forward("+", 1)` を呼び出してください)。
     */
    bool forward(const char* content, const size_t length = 1);

    /**
     * @brief トークンカーソルを進める
     */
    void advanceTokenList() {
        if (currentTokenNode == nullptr) {
            return;
        }
        currentTokenNode = currentTokenNode->next;
        tokenIndex++;
    }

    /**
     * @brief ノードプールを初期化する
     */
    void initializeNodePool() {
        syntaxNodeTree.initializeTreeNodePool();
        root = nullptr;
    }

    /**
     * @brief アナライザが見ているトークンを式の開始とみなしてパースを試みる
     *
     * @return collection2::TreeNode<SyntaxNode>* パース結果
     */
    collection2::TreeNode<SyntaxNode>* expression();

    /**
     * @brief アナライザが見ているトークンを単一の項とみなしてパースを試みる
     *
     * @return collection2::TreeNode<SyntaxNode>* パース結果
     */
    collection2::TreeNode<SyntaxNode>* unary();

    /**
     * @brief アナライザが見ているトークンを単一の因子とみなしてパースを試みる
     *
     * @return collection2::TreeNode<SyntaxNode>* パース結果
     */
    collection2::TreeNode<SyntaxNode>* factor();

   public:
    explicit Analyser(
        const collection2::List<Token>& tokenList,
        collection2::Tree<SyntaxNode>& syntaxNodeTree)
        : tokenList(tokenList), syntaxNodeTree(syntaxNodeTree){};

    /**
     * @brief トークナイズされた数式から構文木を生成
     *
     * @return size_t 正常にパースできなかったトークンの位置
     * @note 生成に成功した場合は0が返ります。
     */
    size_t analyse();

    /**
     * @brief 構文木のルートノードを取得
     *
     * @return collection2::TreeNode<SyntaxNode>* 構文木のルートノード
     * @note 直近で行われた構文木の生成に失敗した場合はnullptrが返ります。
     */
    collection2::TreeNode<SyntaxNode>* rootNode() {
        return root;
    }

#ifndef HEADLESS

    /**
     * @brief 構文木をダンプ
     * @note ルートノードがない場合ダンプは実行されません。
     */
    void dumpSyntaxTree() const;

    /**
     * @brief 構文ノードをダンプ
     *
     * @param nodePtr 対象のノードへのポインタ
     */
    void dumpSyntaxNode(collection2::TreeNode<botanist::SyntaxNode>* nodePtr) const;

#else
    void dumpSyntaxTree() const = delete;
    void dumpSyntaxNode(collection2::TreeNode<botanist::SyntaxNode>* nodePtr) const = delete;
#endif
};

}  // namespace botanist

#endif /* BOTANIST_ANALYSER_H */
