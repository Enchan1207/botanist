/// @file
/// @brief 構文木の直列化
///

#include "botanist/serializer.hpp"

namespace botanist {

void Serializer::serializeTree(const collection2::TreeNode<SyntaxNode>* rootNode) {
    // リストを初期化
    while (syntaxNodeList.amount() > 0) {
        syntaxNodeList.remove(0, nullptr);
    }

    // serializeに投げて再帰的に直列化
    serialize(rootNode);
}

void Serializer::serialize(const collection2::TreeNode<SyntaxNode>* nodePtr) {
    if (nodePtr == nullptr) {
        return;
    }

    // リーフなら自分をpushして戻る
    if (nodePtr->isLeaf()) {
        syntaxNodeList.append(nodePtr->element);
        return;
    }

    // ノードならさらに下る
    serialize(nodePtr->lhs);
    serialize(nodePtr->rhs);

    // 最後に自分をpushする
    syntaxNodeList.append(nodePtr->element);
}

}  // namespace botanist
