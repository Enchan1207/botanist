//
// 数式構成要素のパース
//

#include "parse_elements.hpp"

size_t parseNumber(char const* str) {
    /**
     * @brief 数値パース状態
     */
    enum class ParseState {
        // パース開始
        Ready,

        // 実数部
        Real,

        // 小数部
        Fractional,

        // 数値の終了
        Terminate
    };
    ParseState currentState = ParseState::Ready;
    long cursor = 0;
    while (currentState != ParseState::Terminate) {
        switch (currentState) {
            case ParseState::Ready:
                // 数値なら状態遷移 それ以外なら終わる
                if (isdigit(str[cursor])) {
                    cursor++;
                    currentState = ParseState::Real;
                    continue;
                }
                currentState = ParseState::Terminate;
                break;

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

size_t parseOperator(char const* str) {
    // 二項演算子
    const char* binaryOperators[] = {"<<", ">>", ""};
    const char** binaryOpPtr = binaryOperators;
    while (strcmp(*binaryOpPtr, "") != 0) {
        char buf[3] = {*str, *(str + 1), '\0'};
        if (strcmp(buf, *binaryOpPtr) == 0) {
            return 2;
        }
        binaryOpPtr++;
    }

    // 単項演算子
    const char unaryOperators[] = {'+', '-', '*', '/', '<', '>', '\0'};
    const char* unaryOpPtr = unaryOperators;
    while (*unaryOpPtr != '\0') {
        if (*str == *unaryOpPtr) {
            return 1;
        }
        unaryOpPtr++;
    }

    return 0;
}

size_t parseBracket(char const* str) {
    return (*str == '(' || *str == ')') ? 1 : 0;
}
