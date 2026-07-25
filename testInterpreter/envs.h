#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "main.h"
#include "tokens.h"

class Table;


using Function = std::function<Token(vector<Token>)>;

Token Print(vector<Token> args);

Token RawPrint(vector<Token> args);

Token Input(vector<Token> args);

Token Random(vector<Token> args);

Token MakeString(vector<Token> args);

Token MakeNumber(vector<Token> args);

class DefaultMethod {
public:
	int maxArgs,minArgs = 0;
	Function function;

	DefaultMethod(Function f, int maxa,int mina);

	Token Call(vector<Token> args);
};

class Callback {
public:
	vector<string> args;
	vector<Token> instructions;
	
	Callback(vector<Token> & tokens, vector<string> a = {});

	Callback();
};

class Environment {

public:
	enum EnvironmentType {
		NONE,
		LOOP,
		FUNCTION
	};

	Environment* parent;
	EnvironmentType type = NONE;
	Callback callback;
	vector<string> args;


	std::unordered_map<string, VarValue> variables;
	std::unordered_map<string, Callback> callbacks;
	std::unordered_map<string, DefaultMethod> defaultMethods;

	Environment(Environment* parentEnv = nullptr);

	Environment(vector<string> a, Environment* parentEnv = nullptr);

	bool IsAncestorOfType(EnvironmentType t);

	void setArgs(vector<VarValue> v);

	VarValue Call();

	VarValue CallCallback(Callback c);

	void setVar(string id, VarValue val);

	void changeVar(string id, const VarValue& val);

	VarValue getVar(string id);

	VarValue & getVarRef(string id);

	Callback findCallback(string id);

	DefaultMethod findDefaultCallback(string id);

	bool callbackExsists(string id);
	
	bool defaultCallbackExsists(string id);

	bool varExsists(string id);

	void setCallback(string id, Callback callback);
};

extern Environment globalEnvironment;

#endif