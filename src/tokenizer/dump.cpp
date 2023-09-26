/// @file
/// @brief トークンリストのダンプ
///

#include <iostream>

#include "botanist/tokenizer.hpp"

namespace botanist {

void Tokenizer::dumpTokenList(bool colorlized) const {
    if (!isLastTokenizationSucceeded) {
        std::cerr << "The last tokenization was failed. please invoke again after tokenize valid formula." << std::endl;
        return;
    }
    auto* tokenNode = tokens();

    while (tokenNode != nullptr) {
        const auto token = tokenNode->element;

        // トークンの種類によって色を変える
        if (colorlized) {
            switch (token.kind) {
                case Token::Kind::Number:
                    std::cout << "\033[0m";
                    break;

                case Token::Kind::Operator:
                    std::cout << "\033[32;1m";
                    break;

                case Token::Kind::Bracket:
                    std::cout << "\033[34;1m";
                    break;

                default:
                    std::cout << "\033[0m";
                    break;
            }
        }

        // トークンの内容
        char contentBuffer[32] = {0};
        memcpy(contentBuffer, token.content, token.length);
        std::cout << contentBuffer << " ";
        if (colorlized) {
            std::cout << "\033[0m";
        }

        tokenNode = tokenNode->next;
    }
    std::cout << std::endl;
}

}  // namespace botanist
