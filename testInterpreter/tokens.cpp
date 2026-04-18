#include "tokens.h"

class Token {

public:
    string value = "";
    TokenType type = ERROR;


    Token(string val, TokenType t) {
        value = val;
        type = t;
    }
    Token() {
        value = "";
        type = ERROR;
    }

};