#include "tokens.h"
#include "main.h"
#include "util.h"
#include "table.h"
#include "envs.h"

void Table::EvalDict(vector<Token>& line) {
	Token firstToken = line[0];
	Token secondToken = line[1];
	if (firstToken.type == TEXT and secondToken.type == DEF) {
		line.erase(line.begin(), line.begin() + 2);
		if (secondToken.value == "=") {
			auto one = Evals::SimplifyExpr(line, env);
			VarValue two = Utils::TokenToVar(one);
			values[firstToken.value] = two;
		}
	}
}

void Table::EvalList(vector<Token>& line) {
	values[std::to_string(values.size())] = Utils::TokenToVar(Evals::SimplifyExpr(line,env));
}

VarValue Table::GetValue(const string tag) {
	auto it = values.find(tag);

	if (it != values.end()) {

		return it->second;
	} 
	stringstream s;
	s << "Could not find value '" << tag << "' in table!\n";
	throw std::runtime_error(s.str());
	return NIL;
}

Table::Table(vector<Token>& t, Environment& e) : env(e) {

	tokens = Utils::EvaluateTable(t);

	while (!tokens.empty()) {
		vector<Token> currentLine;

		int num = 0;
		int depth = 0;
		for (Token token : tokens) {
			num++;
			if (token.type == PARENTHESIS or token.type == BRACKET or token.type == TABLEDEC) {
				if (token.value == "{" or token.value == "[" or token.value == "(") 
					depth++;
				if (token.value == "}" or token.value == "]" or token.value == ")")
						depth--;

				}
				if (token.type == COMMA and depth == 0) break;
				currentLine.push_back(token);

			}
			tokens.erase(tokens.begin(), tokens.begin() + num);
			if (currentLine.empty())
				continue;

			switch (type) {
			case UNKNOWN:
				if (currentLine.size() > 2) {
					Token firstToken = currentLine[0];
					Token secondToken = currentLine[1];
					if (firstToken.type == TEXT and secondToken.type == DEF) {
						type = DICT;
						EvalDict(currentLine);
					}
					else {
						type = LIST;
						EvalList(currentLine);
					}
				}
				else {
					type = LIST;
					EvalList(currentLine);
				}
				break;
			case LIST:
				EvalList(currentLine);
				break;
			case DICT:
				EvalDict(currentLine);
				break;
			}


			

		}
	}


Table::Table(map<string, VarValue> v, Environment & e): env(e){
	values = v;
}

void Table::ChangeValue(Token value, std::list<string> tags) {
	Table currentTable = Table(values,env);
	Table& orginalValues = currentTable;
	while (tags.size() > 1) {
		string txt = tags.front();
		VarValue val = currentTable.GetValue(txt);
		Token tk = Utils::VarToToken(val);
		if (tk.type != TABLE) {
			stringstream ss;
			ss << "Value "<< txt << " is not a table!";
			throw std::runtime_error(ss.str());
		}
		else
			currentTable.values = tk.values;
		tags.pop_front();
	}
	currentTable.values[tags.front()] = Utils::TokenToVar(value);
	values = orginalValues.values;
}