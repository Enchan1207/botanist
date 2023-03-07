/// @file
/// @brief 補助関数
///

#include "tokenizer.hpp"

namespace botanist {

void Tokenizer::initializeTokenList() {
    auto result = collection2::OperationResult::Success;
    while (result == collection2::OperationResult::Success) {
        result = tokenList.pop(nullptr);
    }
}

bool Tokenizer::tryParse(char const* str, Token::Kind& kind, size_t& parsedTokenLength) const {
    // 演算子
    const auto positionAsOperator = tryParseAsOperator(str);
    if (positionAsOperator > 0) {
        kind = Token::Kind::Operator;
        parsedTokenLength = positionAsOperator;
        return true;
    }

    // 括弧
    const auto positionAsBracket = tryParseAsBracket(str);
    if (positionAsBracket > 0) {
        kind = Token::Kind::Bracket;
        parsedTokenLength = positionAsBracket;
        return true;
    }

    // 数値
    const auto positionAsNumber = tryParseAsNumber(str);
    if (positionAsNumber > 0) {
        kind = Token::Kind::Number;
        parsedTokenLength = positionAsNumber;
        return true;
    }

    // どれにも当てはまらない
    kind = Token::Kind::Invalid;
    return false;
}

size_t Tokenizer::tryParseAsNumber(char const* str) const {
    /**
     * @brief 数値パース状態
     */
    enum class ParseState : uint8_t {
        // パース開始
        Ready,

        // 実数部
        Real,

        // 小数第一位
        FirstDP,

        // 小数第二位以下
        LessDP,

        // 数値の終了
        Terminate
    };
    ParseState currentState = ParseState::Ready;
    long numberCursor = 0;
    while (currentState != ParseState::Terminate) {
        switch (currentState) {
            case ParseState::Ready:
                // 数値なら状態遷移 それ以外なら終わる
                if (isdigit(str[numberCursor])) {
                    numberCursor++;
                    currentState = ParseState::Real;
                    continue;
                }
                currentState = ParseState::Terminate;
                break;

            case ParseState::Real:
                // 数値なら続ける 小数点なら状態遷移 それ以外なら終わる
                if (isdigit(str[numberCursor])) {
                    numberCursor++;
                    continue;
                }
                if (str[numberCursor] == '.') {
                    numberCursor++;
                    currentState = ParseState::FirstDP;
                    continue;
                }
                currentState = ParseState::Terminate;
                break;

            case ParseState::FirstDP:
                // 小数点の直後は数値でなければならない
                if (isdigit(str[numberCursor])) {
                    numberCursor++;
                    currentState = ParseState::LessDP;
                    continue;
                }
                // そうでなければ、小数点の直前まで戻ってパースを終了する
                numberCursor--;
                currentState = ParseState::Terminate;
                break;

            case ParseState::LessDP:
                // 数値なら続ける
                if (isdigit(str[numberCursor])) {
                    numberCursor++;
                    continue;
                }
                currentState = ParseState::Terminate;
                break;

            case ParseState::Terminate:
                /* switch statement should be exhaustive */
                break;
        }
    }

    return numberCursor;
}

size_t Tokenizer::tryParseAsOperator(char const* str) const {
    // 二項演算子
    const char* binaryOperators[] = {"<<", ">>", ""};
    const char** binaryOpPtr = binaryOperators;
    while (strcmp(*binaryOpPtr, "") != 0) {
        // TODO: strの残り文字数管理しないと危ない (2文字まではいいけど、3文字の演算子・識別子をトークナイズできなくなる)
        const char buf[3] = {*str, *(str + 1), '\0'};
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

size_t Tokenizer::tryParseAsBracket(char const* str) const {
    return (*str == '(' || *str == ')') ? 1 : 0;
}

}  // namespace botanist
