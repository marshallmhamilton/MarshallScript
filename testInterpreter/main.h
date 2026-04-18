#ifndef MAIN_H
#define MAIN_H

#include "deps.h"

using std::string;
using std::stringstream;
using std::vector;



enum None {
    NIL
};
enum LoopEndType {
    NOEND, END
};
using VarValue = std::variant<std::monostate, int64_t, double, string, bool, LoopEndType, None>;


enum TokenType {
    ERROR,
    OPERATOR,
    NUMBER,
    KEYWORD,
    TEXT,
    STRING,
    BOOL,
    ENDL,
    VARIABLE,
    PARENTHESIS,
    DEF,
    CALLBACK,
    BRACKET,
    LOGIC,
    BREAK,
    COMMA
};

#include "tokens.h"
#include "util.h"
#include "constants.h"
#endif