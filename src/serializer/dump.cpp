/// @file
/// @brief 直列化された構文ノードリストのダンプ
///

#include <cstring>
#include <iostream>

#include "botanist/serializer.hpp"

namespace botanist {

void Serializer::dumpSeralizedNodeList() const {
    const auto* headNode = syntaxNodeList.head();

    // シリアライズしてからにしてくれ()
    if (headNode == nullptr) {
        std::cerr << "Please serialize before dump" << std::endl;
        return;
    }

    // 順に読む
    while (headNode != nullptr) {
        const auto* node = headNode->element;

        char contentStr[node->length + 1];
        memset(contentStr, '\0', node->length + 1);

        switch (node->kind) {
            case SyntaxNodeOld::Kind::Number:
                memcpy(contentStr, node->content, node->length);
                std::cout << "push " << contentStr << std::endl;
                break;

            case SyntaxNodeOld::Kind::Add:
                std::cout << "add" << contentStr << std::endl;
                break;
            case SyntaxNodeOld::Kind::Subtract:
                std::cout << "sub" << contentStr << std::endl;
                break;
            case SyntaxNodeOld::Kind::Multiply:
                std::cout << "mul" << contentStr << std::endl;
                break;
            case SyntaxNodeOld::Kind::Divide:
                std::cout << "div" << contentStr << std::endl;
                break;

            default:
                break;
        }

        headNode = headNode->next;
    }
}

}  // namespace botanist
