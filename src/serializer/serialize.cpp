/// @file
/// @brief 構文木の直列化
///

#include "botanist/serializer.hpp"

namespace botanist {

collection2::Node<SyntaxNode*>* Serializer::serializeTree(SyntaxNode* rootNode) {
    serializeNode(rootNode);
    return syntaxNodeList.head();
}

void Serializer::serializeNode(SyntaxNode* node) {
    if (node == nullptr) {
        return;
    }

    // リーフなら自分をpushして戻る
    if (node->lhs == nullptr && node->rhs == nullptr) {
        syntaxNodeList.append(node);
        return;
    }

    // ノードならさらに下る
    serializeNode(node->lhs);
    serializeNode(node->rhs);
    syntaxNodeList.append(node);
}

}  // namespace botanist
