#ifndef TOKENS_H
#define TOKENS_H
#include "main.h"
#include "envs.h"


class Token {
public:
	string value;
	TokenType type;

	inline Token(string val, TokenType t);

	inline Token();
};

class Expression {
public:
	Token result;
	inline Expression(Token l, Token r, Token op);
};

class LogicExpression {
public:
	Token result;
	inline LogicExpression(Token l, Token r, Token op);
};
class Environment;
struct Evals {
    static Token EvaluateExpression(std::vector<Token>& tokens, int i);
    static Token EvaluateLogicExpression(std::vector<Token>& tokens, int i);
	
	static Token EvaluateTokens(vector<Token> token);
	static Token SimplifyExpr(vector<Token> tokens, Environment& env);
};

#endif // !TOKENS_H
