/// @file
/// @brief 構文木の直列化
///

#ifndef BOTANIST_SERIALIZER_H
#define BOTANIST_SERIALIZER_H

#include <collection2/list.hpp>
#include <collection2/tree.hpp>

#include "syntaxnode.hpp"

namespace botanist {

/// @brief 構文木をスタックマシンが実行可能な形に変換する
class Serializer {
   private:
    /// @brief 直列化された構文ノードへのポインタを格納するリスト
    collection2::List<SyntaxNode>& syntaxNodeList;

    /**
     * @brief ノードを直列化してリストに積む
     *
     * @param nodePtr
     */
    void serialize(const collection2::TreeNode<SyntaxNode>* nodePtr);

   public:
    explicit Serializer(collection2::List<SyntaxNode>& syntaxNodeList) : syntaxNodeList(syntaxNodeList){};

    /**
     * @brief 構文木をスタックマシンで処理できる形に並べ替え、ノードリストを更新する
     *
     * @param rootNode 構文木のルートノードへのポインタ
     */
    void serializeTree(const collection2::TreeNode<SyntaxNode>* rootNode);

#ifndef HEADLESS

    /**
     * @brief 直列化されたノードリストをダンプ
     */
    void dumpSeralizedNodeList() const;

#else
    void dumpSeralizedNodeList() = delete;
#endif
};

}  // namespace botanist

#endif