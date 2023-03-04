/// @file
/// @brief 構文ツリーの直列化
///

#include <cstring>
#include <iostream>

#include "serializer.hpp"

namespace botanist {

Serializer::Serializer(/* args */) {
}

Serializer::~Serializer() {
}

void Serializer::serialize(SyntaxNode* node) {
    if (node == nullptr) {
        return;
    }

    // リーフなら自分をpushして戻る
    if (node->kind == SyntaxNode::Kind::Number) {
        char nodeContentBuffer[node->length + 1];
        memset(nodeContentBuffer, '\0', node->length + 1);
        memcpy(nodeContentBuffer, node->content, node->length);
        std::cout << "push " << nodeContentBuffer << std::endl;
        return;
    }

    // ノードならさらに下り、最後に操作を説明する
    serialize(node->lhs);
    serialize(node->rhs);

    switch (node->kind) {
        case SyntaxNode::Kind::Add:
            std::cout << "add" << std::endl;
            break;

        case SyntaxNode::Kind::Subtract:
            std::cout << "sub" << std::endl;
            break;

        case SyntaxNode::Kind::Multiply:
            std::cout << "mul" << std::endl;
            break;

        case SyntaxNode::Kind::Divide:
            std::cout << "div" << std::endl;
            break;

        default:
            break;
    }
}

}  // namespace botanist
