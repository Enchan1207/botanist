//
// 式のトークナイズ
//

#include <cctype>
#include <cstdlib>
#include <iostream>

#include "parser.hpp"

void tokenize(char const* formula) {
    auto p = formula;
    while (*p) {
        // 空白や改行など、トークンに無関係なものは読み飛ばす
        if (isspace(*p)) {
            p++;
            continue;
        }

        // 演算子
        if (*p == '+' || *p == '-' || *p == '*' || *p == '/') {
            std::cout << "operator: " << *p << std::endl;
            p++;
            continue;
        }

        // 括弧
        if (*p == '(' || *p == ')') {
            std::cout << "bracket: " << *p << std::endl;
            p++;
            continue;
        }

        // 数値
        if (isdigit(*p)) {
            std::cout << "digit: " << *p << std::endl;
            p++;
            continue;
        }

        // トークナイズできない何か
        std::cerr << "ERROR! unexpected charactor: " << *p << std::endl;
        break;
    }

    std::cout << "tokenize finished." << std::endl;
}
