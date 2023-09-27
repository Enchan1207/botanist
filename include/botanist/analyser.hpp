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

    /// @brief 構文木を構成するノードのプール
    SyntaxNodeOld syntaxNodePool[64];

    /// @brief トークンリストがどこまで進んだか
    size_t tokenIndex = 0;

    /// @brief 構文解析結果のルートノード
    SyntaxNodeOld* root = nullptr;

    /**
     * @brief ノードプールから未割り当てのノードを探し、発見できた場合は引数の内容を設定する
     *
     * @param kind 設定するノード種別
     * @param lhs 左辺値
     * @param rhs 右辺値
     * @return SyntaxNodeOld* 構成されたノードへのポインタ
     * @note ノードプールがいっぱいになっている場合はnullptrが返ります。
     * @note ノードの内容はメンバ `currentTokenNode` より取得されます。
     */
    SyntaxNodeOld* createNewNode(const SyntaxNodeOld::Kind kind, SyntaxNodeOld* lhs = nullptr, SyntaxNodeOld* rhs = nullptr);

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
        for (size_t i = 0; i < 64; i++) {
            auto* node = &(syntaxNodePool[i]);
            node->kind = SyntaxNodeOld::Kind::Empty;
        }
    }

    /**
     * @brief アナライザが見ているトークンを式の開始とみなしてパースを試みる
     *
     * @return SyntaxNodeOld* パース結果
     */
    SyntaxNodeOld* expression();

    /**
     * @brief アナライザが見ているトークンを単一の項とみなしてパースを試みる
     *
     * @return SyntaxNodeOld* パース結果
     */
    SyntaxNodeOld* unary();

    /**
     * @brief アナライザが見ているトークンを単一の因子とみなしてパースを試みる
     *
     * @return SyntaxNodeOld* パース結果
     */
    SyntaxNodeOld* factor();

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
     * @return SyntaxNodeOld* 構文木のルートノード
     * @note 直近で行われた構文木の生成に失敗した場合はnullptrが返ります。
     */
    SyntaxNodeOld* rootNode() {
        return root;
    }

#ifndef HEADLESS

    /**
     * @brief 構文木をダンプ
     *
     * @param node 対象のノード
     */
    void dumpSyntaxTree(SyntaxNodeOld* node) const;

    /**
     * @brief 構文ノードをダンプ
     *
     * @param node 対象のノード
     */
    void dumpSyntaxNode(SyntaxNodeOld* node) const;

#else
    void dumpSyntaxTree(SyntaxNodeOld* node) const = delete;
    void dumpSyntaxNode(SyntaxNodeOld* node) const = delete;
#endif
};

}  // namespace botanist

#endif /* BOTANIST_ANALYSER_H */
