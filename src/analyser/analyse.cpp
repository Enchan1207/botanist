/// @file
/// @brief 構文木構成
///

#include "botanist/analyser.hpp"

namespace botanist {

using SyntaxNodeKind = SyntaxNode::Kind;

size_t Analyser::analyse() {
    // 状態をリセット
    tokenIndex = 0;
    initialize();
    currentTokenNode = tokenList.head();

    // 構文木生成
    root = expression();

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

SyntaxNode* Analyser::expression() {
    auto* node = unary();

    bool canLoopContinue = true;
    while (canLoopContinue) {
        if (forward("+", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = unary();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 加算ノードを構成
            node = createNewNode(SyntaxNodeKind::Add, node, rhs);
            continue;
        }

        if (forward("-", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = unary();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 減算ノードを構成
            node = createNewNode(SyntaxNodeKind::Subtract, node, rhs);
            continue;
        }

        canLoopContinue = false;
    }

    return node;
}

SyntaxNode* Analyser::unary() {
    auto* node = factor();

    bool canLoopContinue = true;
    while (canLoopContinue) {
        if (forward("*", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = factor();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 乗算ノードを構成
            node = createNewNode(SyntaxNodeKind::Multiply, node, rhs);
            continue;
        }

        if (forward("/", 1)) {
            // 右辺が計算できることを確認
            auto* rhs = factor();
            if (rhs == nullptr) {
                node = nullptr;
                canLoopContinue = false;
                continue;
            }

            // 除算ノードを構成
            node = createNewNode(SyntaxNodeKind::Divide, node, rhs);
            continue;
        }

        canLoopContinue = false;
    }

    return node;
}

SyntaxNode* Analyser::factor() {
    if (forward("(", 1)) {
        auto* node = expression();

        // 閉じ括弧があってほしい
        if (!forward(")", 1)) {
            return nullptr;
        }
        return node;
    }

    if (expect(Token::Kind::Number)) {
        auto newNode = createNewNode(SyntaxNodeKind::Number);
        advanceTokenList();
        return newNode;
    }

    return nullptr;
}

}  // namespace botanist
