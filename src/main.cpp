//
// formula_parser
//

#include <collection2/list.hpp>
#include <cstddef>
#include <iostream>

#include "parser.hpp"

using namespace collection2;

int main(int argc, char const* argv[]) {
    // 対象の式は実行引数経由で渡されることを想定
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " formula" << std::endl;
        return 1;
    }
    char const* formula = argv[1];

    // トークンに分割する
    std::cout << "  formula: " << formula << std::endl;
    const size_t tokenDataSize = 32;
    Node<Token> tokenData[tokenDataSize];
    List<Token> tokenList(tokenData, tokenDataSize);
    tokenize(tokenList, formula);

    // 書き出してみる
    std::cout << "tokenized: ";
    Token token;
    while (tokenList.remove(0, &token) == OperationResult::Success) {
        // トークンの種類によって色を変える
        switch (token.kind) {
            case TokenKind::Number:
                std::cout << "\033[2m";
                break;

            case TokenKind::Operator:
                std::cout << "\033[32;1m";
                break;

            case TokenKind::Bracket:
                std::cout << "\033[1m";
                break;

            default:
                std::cout << "\033[0m";
                break;
        }

        // トークンの内容
        char contentBuffer[32] = {0};
        const auto contentSizeOfToken = token.length;
        memcpy(contentBuffer, token.content, contentSizeOfToken);
        std::cout << contentBuffer << " \033[0m";
    }
    std::cout << std::endl;

    return 0;
}
