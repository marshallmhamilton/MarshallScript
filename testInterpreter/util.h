#ifndef UTIL_H
#define UTIL_H
#include "main.h"

#include "tokens.h"

struct Utils {

	static bool IsIntegar(string s);

	static bool IsWhiteSpace(char c);

	static bool InList(std::list<string> l, string v);

	static bool InList(std::list<char> l, char v);

	static Token VarToToken(VarValue var);

	static VarValue TokenToVar(Token t);

	static vector<Token> EvaluateBrackets(vector<Token>& tokens);

	static int EvaluateParenthesis(vector<Token>& tokens, int begin);

	static vector<VarValue> EvaluateArguments(vector<Token>& tokens, Environment env, int start);

	static vector<string> evalFunctionArgs(vector<Token> tokens);

	static Token changeVarType(VarValue val, TokenType convert);
};

#endif // UTIL_H