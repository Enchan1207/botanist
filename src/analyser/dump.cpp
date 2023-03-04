
/// @file
/// @brief 構文ツリーのダンプ
///

#include <iostream>
#include <sstream>
#include <string>

#include "analyser.hpp"
#include "syntaxtree.hpp"

namespace botanist {

void Analyser::dumpSyntaxNode(SyntaxNode* node) const {
    // アドレスを取得しておく
    auto nodeAddress = static_cast<const void*>(node);
    std::stringstream nodeAddressStream;
    nodeAddressStream << nodeAddress;
    std::string addressStr = nodeAddressStream.str();

    // ノードの内容を出力
    char identifierBuffer[node->length + 1];
    memset(identifierBuffer, '\0', node->length + 1);
    switch (node->kind) {
        case SyntaxNode::Kind::Number:
            memcpy(identifierBuffer, node->content, node->length);
            break;

        case SyntaxNode::Kind::Add:
            identifierBuffer[0] = '+';
            break;

        case SyntaxNode::Kind::Subtract:
            identifierBuffer[0] = '-';
            break;

        case SyntaxNode::Kind::Multiply:
            identifierBuffer[0] = '*';
            break;

        case SyntaxNode::Kind::Divide:
            identifierBuffer[0] = '/';
            break;

        default:
            break;
    }
    std::cout << "  " << addressStr << "[" << identifierBuffer << "]" << std::endl;

    // ノードの接続関係を説明
    if (node->lhs != nullptr) {
        dumpSyntaxNode(node->lhs);

        auto lhsNodeAddress = static_cast<const void*>(node->lhs);
        std::stringstream lhsNodeAddressStream;
        lhsNodeAddressStream << lhsNodeAddress;
        std::string lhsNodeAddressStr = lhsNodeAddressStream.str();

        std::cout << addressStr << " --> " << lhsNodeAddressStr << std::endl;
    }

    if (node->rhs != nullptr) {
        dumpSyntaxNode(node->rhs);

        auto rhsNodeAddress = static_cast<const void*>(node->rhs);
        std::stringstream rhsNodeAddressStream;
        rhsNodeAddressStream << rhsNodeAddress;
        std::string rhsNodeAddressStr = rhsNodeAddressStream.str();

        std::cout << addressStr << " --> " << rhsNodeAddressStr << std::endl;
    }
}

}  // namespace botanist
