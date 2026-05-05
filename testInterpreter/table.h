#ifndef TABLE_H
#define TABLE_H
#include "main.h"
#include "tokens.h"
#include "envs.h"

class Table {
public:
	enum TableType {
		UNKNOWN, LIST, DICT
	};

	map<string, VarValue> values;
	vector<Token> tokens;
	TableType type = UNKNOWN;
	Environment& env;

	Table(vector<Token>& t, Environment& e);

	Table(map<string, VarValue>, Environment& e);

	void EvalDict(vector<Token>& line);
	
	void EvalList(vector<Token>& line);

	VarValue GetValue(const string tag);
};


#endif
