//
// 式のトークナイズ
//

#include <cctype>
#include <cstdlib>
#include <iostream>

#include "parser.hpp"

size_t parseNumber(char const* str) {
    /**
     * @brief 数値パース状態
     */
    enum class ParseState {
        // 実数部
        Real,

        // 小数部
        Fractional,

        // 数値の終了
        Terminate
    };
    ParseState currentState = ParseState::Real;
    long cursor = 0;
    while (currentState != ParseState::Terminate) {
        switch (currentState) {
            case ParseState::Real:
                // 数値なら続ける 小数点なら状態遷移 それ以外なら終わる
                if (isdigit(str[cursor])) {
                    cursor++;
                    continue;
                }
                if (str[cursor] == '.') {
                    cursor++;
                    currentState = ParseState::Fractional;
                    continue;
                }
                currentState = ParseState::Terminate;
                break;

            case ParseState::Fractional:
                // 数値なら続ける それ以外なら終わる
                if (isdigit(str[cursor])) {
                    cursor++;
                    continue;
                }
                currentState = ParseState::Terminate;
                break;

            case ParseState::Terminate:
                /* switch statement should be exhaustive */
                break;
        }
    }

    return cursor;
}

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
            std::cout << "digit: ";
            size_t numLength = parseNumber(p);
            for (size_t i = 0; i < numLength; i++) {
                std::cout << *p;
                p++;
            }
            std::cout << std::endl;
            continue;
        }

        // トークナイズできない何か
        std::cerr << "ERROR! unexpected charactor: " << *p << std::endl;
        break;
    }

    std::cout << "tokenize finished." << std::endl;
}
