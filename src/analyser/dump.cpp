
/// @file
/// @brief 構文木のダンプ
///

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "botanist/analyser.hpp"

namespace botanist {

void Analyser::dumpSyntaxTree(SyntaxNode* node) const {
    std::cout << "flowchart TD" << std::endl;
    dumpSyntaxNode(node);
}

void Analyser::dumpSyntaxNode(SyntaxNode* node) const {
    // アドレスを取得しておく
    auto nodeAddress = static_cast<const void*>(node);
    std::stringstream nodeAddressStream;
    nodeAddressStream << nodeAddress;
    std::string addressStr = nodeAddressStream.str();

    // ノードの内容を出力
    char identifierBuffer[node->length + 1];
    memset(identifierBuffer, '\0', node->length + 1);
    std::cout << "  " << addressStr << "[";
    switch (node->kind) {
        case SyntaxNode::Kind::Number:
            memcpy(identifierBuffer, node->content, node->length);
            std::cout << identifierBuffer;
            break;

        case SyntaxNode::Kind::Add:
            std::cout << "+";
            break;

        case SyntaxNode::Kind::Subtract:
            std::cout << "-";
            break;

        case SyntaxNode::Kind::Multiply:
            std::cout << "*";
            break;

        case SyntaxNode::Kind::Divide:
            std::cout << "\"/\"";
            break;

        default:
            break;
    }
    std::cout << "]" << std::endl;

    // ノードの接続関係を説明
    if (node->lhs != nullptr) {
        dumpSyntaxNode(node->lhs);

        auto lhsNodeAddress = static_cast<const void*>(node->lhs);
        std::stringstream lhsNodeAddressStream;
        lhsNodeAddressStream << lhsNodeAddress;
        std::string lhsNodeAddressStr = lhsNodeAddressStream.str();

        std::cout << "  " << addressStr << " --> " << lhsNodeAddressStr << std::endl;
    }

    if (node->rhs != nullptr) {
        dumpSyntaxNode(node->rhs);

        auto rhsNodeAddress = static_cast<const void*>(node->rhs);
        std::stringstream rhsNodeAddressStream;
        rhsNodeAddressStream << rhsNodeAddress;
        std::string rhsNodeAddressStr = rhsNodeAddressStream.str();

        std::cout << "  " << addressStr << " --> " << rhsNodeAddressStr << std::endl;
    }
}

}  // namespace botanist
