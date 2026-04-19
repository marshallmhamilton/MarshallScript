#include "util.h"
#include "tokens.h"

bool Utils::IsIntegar(string s) {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char* p;
	auto v = strtol(s.c_str(), &p, 10);

	return (*p == 0);
};

bool Utils::IsWhiteSpace(char c) {
	return (c == '\n' or c == ' ' or c == '\t');
}

bool Utils::InList(std::list<string> l, string v) {
	return std::find(l.begin(), l.end(), v) != l.end();
}

bool Utils::InList(std::list<char> l, char v) {
	return std::find(l.begin(), l.end(), v) != l.end();
}

Token Utils::VarToToken(VarValue var) {

    if (auto p = std::get_if<None>(&var)) {
        return Token("", ERROR);
    }
    if (auto p = std::get_if<int64_t>(&var)) {
        return Token(std::to_string(*p), NUMBER);
    }
    if (auto p = std::get_if<double>(&var)) {
        return Token(std::to_string(*p), NUMBER);
    }
    if (auto p = std::get_if<string>(&var)) {
        return Token(*p, STRING);
    }
    if (auto p = std::get_if<LoopEndType>(&var)) {
        return Token(std::to_string(*p), BREAK);
    }
    if (auto p = std::get_if<bool>(&var)) {
        return Token(std::to_string(*p), BOOL);
    }
    return Token("", ERROR);
}

VarValue Utils::TokenToVar(Token token) {
    switch (token.type) {
    case NUMBER: {
        if (Utils::IsIntegar(token.value)) {
            return std::stoi(token.value);
        }
        else {
            return std::stof(token.value);
        }
        break;
    }
    case BOOL: {
        return token.value == "1";
        break;
    }
    case ERROR: {
        return NIL;
        break;
    }
    default: {
        return token.value;
        break;
    }
    }
}

vector<Token> Utils::EvaluateBrackets(vector<Token> & tokens){
        vector<Token> instructions = {};
        int num = 0;
        int depth = 1;
        for (int i = 1; i < tokens.size(); i++) {
            Token token = tokens[i];
            num++;
            if (token.type == BRACKET and token.value == "{") depth += 1;

            if (token.type == BRACKET and token.value == "}") depth -= 1;


            if (depth == 0) break;


            instructions.push_back(token);
        }
        if (depth != 0) {

            stringstream s;
            s << "Unfinished bracket!";
            throw std::runtime_error(s.str());


        }
        tokens.erase(tokens.begin(), tokens.begin() + num);
        tokens.erase(tokens.begin());



        return instructions;
 }

int Utils::EvaluateParenthesis(vector<Token>& tokens, int begin) {
    for (int i = begin; i < (int)tokens.size(); ++i) {
        Token& token = tokens[i];
        if (token.type == PARENTHESIS && token.value == "L") {
            int pos = EvaluateParenthesis(tokens, i + 1);
            if (pos < 0) return -1;

            vector<Token> v(tokens.begin() + i + 1, tokens.begin() + pos);
            Token result = Evals::EvaluateTokens(v);
            tokens.erase(tokens.begin() + i + 1, tokens.begin() + pos + 1);
            tokens[i] = result;
        }
        else if (token.type == PARENTHESIS && token.value == "R") {
            return i;
        }
    }
    return -1;
}

vector<VarValue> Utils::EvaluateArguments(vector<Token>& tokens, Environment env, int start) {
    vector<Token> instructions = {};
    int num = 0;
    int depth = 1;


    for (int i = start; i < tokens.size(); i++) {
        Token token = tokens[i];
        num++;
        if (token.type == PARENTHESIS and token.value == "L") depth += 1;

        if (token.type == PARENTHESIS and token.value == "R") { depth -= 1; num++; }


        if (depth == 0) break;


        instructions.push_back(token);
    }


    tokens.erase(tokens.begin() + start, tokens.begin() + num);
    tokens.erase(tokens.begin() + start);
    if (depth != 0) {

        stringstream s;
        s << "Unfinished parenthesis!";
        throw std::runtime_error(s.str());


    }


    //77
    vector<VarValue> args = {};

    while (!instructions.empty()) {
        vector<Token> argVal = {};
        int num = -1;

        for (Token token : instructions) {
            num += 1;

            if (token.type == COMMA) break;
            argVal.push_back(token);





        }
        instructions.erase(instructions.begin(), instructions.begin() + num + 1);

        Token finalToken = Evals::SimplifyExpr(argVal, env);
        args.push_back(Utils::TokenToVar(finalToken));

    }

    return args;
}

vector<string> Utils::evalFunctionArgs(vector<Token> tokens) {
    if (tokens[0].type == PARENTHESIS and tokens[0].value == "L") {
        tokens.erase(tokens.begin());
        vector<string> argVal = {};
        while (!tokens.empty()) {
            if (tokens[0].type == TEXT) {
                argVal.push_back(tokens[0].value);


            }
            else if (tokens[0].type == PARENTHESIS and tokens[0].value == "R")
                break;
            else {
                throw std::runtime_error("Invalid syntax in function args");
            }

            tokens.erase(tokens.begin());
            if (tokens[0].type == PARENTHESIS and tokens[0].value == "R")
                break;


            if (tokens[0].type != COMMA)
                throw std::runtime_error("Invalid syntax in function args");
            tokens.erase(tokens.begin());





        }
        return argVal;



    }
}

Token Utils::changeVarType(VarValue val, TokenType convert) {
    Token first = Utils::VarToToken(val);
    if (convert == STRING) {
        return Token(first.value, STRING);
    }
    if (convert == NUMBER) {
        if (Utils::IsIntegar(first.value)) {
            return Token(std::to_string(std::stoi(first.value)), NUMBER);
        }
        else {
            return Token(std::to_string(std::stof(first.value)), NUMBER);
        }

    }
    if (convert == BOOL) {
        if (first.value != "" and first.type != NUMBER) {
            return Token("1", BOOL);

        }
        if (first.type == NUMBER) {
            return Token(std::to_string((first.value != "0")), BOOL);
        }
        else {
            return Token("0", BOOL);
        }
    }
}