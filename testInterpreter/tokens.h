#ifndef TOKENS_H
#define TOKENS_H
#include "main.h"

class Token {
public:
	string value = "";
	TokenType type = static_cast<TokenType>(0);

	Token(string val, TokenType t);

	Token();
};

class Expression {
public:
	Token result;
	Expression(Token l, Token r, Token op);
};

class LogicExpression {
public:
	Token result;
	LogicExpression(Token l, Token r, Token op);
};
#endif // !TOKENS_H
