/// @file
/// @brief 構文木構成
///

#include "botanist/analyser.hpp"

namespace botanist {

using SyntaxNodeKindOld = SyntaxNodeOld::Kind;
using SyntaxNodeKind = SyntaxNode::Kind;

size_t Analyser::analyse() {
    // 状態をリセット
    tokenIndex = 0;
    initializeNodePool();
    currentTokenNode = tokenList.head();

    // 構文木生成
    root = expressionOld();

    // 生成中に不正なトークンを踏んだ
    if (root == nullptr) {
        return tokenIndex > 0 ? tokenIndex : 1;
    }

    // トークンを使い切っていない
    if (currentTokenNode != nullptr) {
        root = nullptr;
        return tokenIndex > 0 ? tokenIndex : 1;
    }

    return 0;
}

SyntaxNodeOld* Analyser::expressionOld() {
    auto* node = unaryOld();

    bool canLoopContinue = true;
    while (canLoopContinue) {
        if (forward("+", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = unaryOld();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 加算ノードを構成
            node = createNewNode(SyntaxNodeKindOld::Add, node, rhs);
            continue;
        }

        if (forward("-", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = unaryOld();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 減算ノードを構成
            node = createNewNode(SyntaxNodeKindOld::Subtract, node, rhs);
            continue;
        }

        canLoopContinue = false;
    }

    return node;
}

SyntaxNodeOld* Analyser::unaryOld() {
    auto* node = factorOld();

    bool canLoopContinue = true;
    while (canLoopContinue) {
        if (forward("*", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = factorOld();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 乗算ノードを構成
            node = createNewNode(SyntaxNodeKindOld::Multiply, node, rhs);
            continue;
        }

        if (forward("/", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = factorOld();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 除算ノードを構成
            node = createNewNode(SyntaxNodeKindOld::Divide, node, rhs);
            continue;
        }

        canLoopContinue = false;
    }

    return node;
}

SyntaxNodeOld* Analyser::factorOld() {
    if (forward("(", 1)) {
        auto* node = expressionOld();

        // 閉じ括弧があってほしい
        if (!forward(")", 1)) {
            return nullptr;
        }
        return node;
    }

    if (expect(Token::Kind::Number)) {
        auto newNode = createNewNode(SyntaxNodeKindOld::Number);
        advanceTokenList();
        return newNode;
    }

    return nullptr;
}

}  // namespace botanist
