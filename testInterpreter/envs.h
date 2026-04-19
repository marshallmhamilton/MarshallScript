#ifndef CALLBACKS_H
#define CALLBACKS_H
#include "main.h"
#include "tokens.h"

class Token;

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

	Environment(Environment* parentEnv = nullptr);

	Environment(vector<string> a, Environment* parentEnv = nullptr);

	bool IsAncestorOfType(EnvironmentType t);

	void setArgs(vector<VarValue> v);

	VarValue Call();

	VarValue CallCallback(Callback c);

	void setVar(string id, VarValue val);

	void changeVar(string id, const VarValue& val);

	VarValue getVar(string id);

	Callback findCallback(string id);

	bool callbackExsists(string id);

	bool varExsists(string id);

	void setCallback(string id, Callback callback);
};


#endif