//
// 式のトークナイズ
//

#include <cctype>
#include <cstdlib>
#include <iostream>

#include "parse_elements.hpp"
#include "parser.hpp"

using namespace collection2;

void tokenize(collection2::List<Token>& list, const char* formula) {
    auto p = formula;
    while (*p) {
        // 空白や改行など、トークンに無関係なものは読み飛ばす
        if (isspace(*p)) {
            p++;
            continue;
        }

        // 演算子
        const auto positionAsOperator = parseOperator(p);
        if (positionAsOperator > 0) {
            Token token;
            token.kind = TokenKind::Operator;
            token.content = p;
            token.length = positionAsOperator;
            list.append(token);
            p += positionAsOperator;
            continue;
        }

        // 括弧
        const auto positionAsBracket = parseBracket(p);
        if (positionAsBracket > 0) {
            Token token;
            token.kind = TokenKind::Bracket;
            token.content = p;
            token.length = positionAsBracket;
            list.append(token);
            p += positionAsBracket;
            continue;
        }

        // 数値
        const auto positionAsNumber = parseNumber(p);
        if (positionAsNumber > 0) {
            Token token;
            token.kind = TokenKind::Number;
            token.content = p;
            token.length = positionAsNumber;
            list.append(token);
            p += positionAsNumber;
            continue;
        }

        // トークナイズできない文字
        std::cerr << "ERROR! unexpected charactor: " << *p << std::endl;
        break;
    }
}
