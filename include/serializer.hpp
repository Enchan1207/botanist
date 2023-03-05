/// @file
/// @brief 構文ツリーの直列化
///

#ifndef BOTANIST_SERIALIZER_H
#define BOTANIST_SERIALIZER_H

#include <collection2/list.hpp>

#include "syntaxtree.hpp"

namespace botanist {

class Serializer {
   private:
    /// @brief 構文ノードリストを管理する配列
    collection2::Node<SyntaxNode*> internalNodesData[64];

    /// @brief 直列化された構文ノードのリスト
    collection2::List<SyntaxNode*> syntaxNodeList;

    /**
     * @brief 構文ノードをスタックマシンで処理できる形に並べ替える
     *
     * @param node 対象となるノード
     */
    void serializeNode(SyntaxNode* node);

   public:
    Serializer() : syntaxNodeList(internalNodesData, sizeof(internalNodesData) / sizeof(internalNodesData[0])){};

    /**
     * @brief 構文ツリーをスタックマシンで処理できる形に並べ替え、先頭へのポインタを返す
     *
     * @param node ツリーのルートノード
     * @return collection2::Node<SyntaxNode*>* シリアライズされたツリーの先頭へのポインタ
     */
    collection2::Node<SyntaxNode*>* serializeTree(SyntaxNode* rootNode);
};

}  // namespace botanist

#endif