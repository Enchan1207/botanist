//
// formula_parser
//

#include <iostream>

#include "parser.hpp"

int main(int argc, char const* argv[]) {
    // 対象の式は実行引数経由で渡されることを想定
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " formula" << std::endl;
        return 1;
    }
    char const* formula = argv[1];

    // トークンに分割する
    tokenize(formula);

    return 0;
}
