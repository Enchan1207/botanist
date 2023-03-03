//
// formula_parser
//

#include <iostream>

#include "tokenizer.hpp"

using namespace collection2;
using TokenKind = botanist::Token::Kind;

int main(int argc, char const* argv[]) {
    // 対象の式は実行引数経由で渡されることを想定
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " formula" << std::endl;
        return 1;
    }
    char const* formula = argv[1];
    std::cout << formula << std::endl;

    // トークナイザに渡す
    botanist::Tokenizer tokenizer(formula);
    auto result = tokenizer.tokenize();
    if (result != 0) {
        for (size_t i = 0; i < result - 1; i++) {
            std::cerr << ' ';
        }
        std::cerr << "\033[31;1m^\033[0m" << std::endl;
        std::cerr << "Tokenize failed: unexpected character found at " << result << std::endl;
        return 1;
    }
    auto* tokenNode = tokenizer.tokens();

    while (tokenNode != nullptr) {
        const auto token = tokenNode->element;

        // トークンの種類によって色を変える
        switch (token.kind) {
            case TokenKind::Number:
                std::cout << "\033[0m";
                break;

            case TokenKind::Operator:
                std::cout << "\033[32;1m";
                break;

            case TokenKind::Bracket:
                std::cout << "\033[34;1m";
                break;

            default:
                std::cout << "\033[0m";
                break;
        }

        // トークンの内容
        char contentBuffer[32] = {0};
        memcpy(contentBuffer, token.content, token.length);
        std::cout << contentBuffer << " \033[0m";

        tokenNode = tokenNode->next;
    }
    std::cout << std::endl;

    return 0;
}
