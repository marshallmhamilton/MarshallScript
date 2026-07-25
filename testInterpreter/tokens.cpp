#include "tokens.h"
#include "util.h"
#include "envs.h"
#include "constants.h"
#include "table.h"

Token::Token(string val, TokenType t) {
    value = val;
    type = t;
}
Token::Token() {
    value = "";
    type = ERROR;
}

Expression::Expression(Token left, Token right, Token op) {
    if (op.value == "+") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::stoi(left.value) + std::stoi(right.value)), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::stof(left.value) + std::stof(right.value)), NUMBER);
            }

        }
        else if (left.type == STRING or right.type == STRING) {
            std::stringstream ss;
            ss << left.value << right.value;
            result = Token(ss.str(), STRING);
        }

        return;
    }

    if (op.value == "-") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::stoi(left.value) - std::stoi(right.value)), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::stof(left.value) - std::stof(right.value)), NUMBER);
            }
        }
        return;
    }

    if (op.value == "*") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::stoi(left.value) * std::stoi(right.value)), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::stof(left.value) * std::stof(right.value)), NUMBER);
            }
        }
        return;
    }

    if (op.value == "/") {
        if (left.type == NUMBER and right.type == NUMBER) {
            result = Token(std::to_string(stof(left.value) / stof(right.value)), NUMBER);
            return;
        }
        return;
    }

    if (op.value == "//") {
        if (left.type == NUMBER and right.type == NUMBER) {
            int rf = static_cast<int>(std::floor(static_cast<float>(stof(left.value) / stof(right.value))));

            result = Token(std::to_string(rf), NUMBER);
            return;
        }
        return;
    }

    if (op.value == "**") {
        if (left.type == NUMBER and right.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string(std::pow(std::stoi(left.value), std::stoi(right.value))), NUMBER);
                return;
            }
            else {
                result = Token(std::to_string(std::pow(std::stof(left.value), std::stof(right.value))), NUMBER);
            }
        }
        return;
    }
    stringstream s;
    s << "Error in arithmatic operation: " << left.type << " " << op.value << " " << right.type;
    throw std::runtime_error(s.str());
    return;
}

LogicExpression::LogicExpression(Token left, Token right, Token op) {
    if (op.value == "==") {
        if (left.type == right.type) {
            result = Token(std::to_string((left.value == right.value)), BOOL);
        }
        else {
            result = Token("0", BOOL);

        }
        return;
    }
    if (op.value == "<=") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) <= stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) <= stof(right.value))), BOOL);
            }

        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == ">=") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) >= stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) >= stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == ">") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) > stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) > stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == "<") {
        if (left.type == right.type and left.type == NUMBER) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) < stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) < stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("0", BOOL);
        }
        return;
    }
    if (op.value == "!=") {
        if (left.type == right.type) {
            if (Utils::IsIntegar(left.value) and Utils::IsIntegar(right.value)) {
                result = Token(std::to_string((stoi(left.value) != stoi(right.value))), BOOL);
            }
            else {
                result = Token(std::to_string((stof(left.value) != stof(right.value))), BOOL);
            }
        }
        else {
            result = Token("1", BOOL);
        }
        return;
    }
    if (op.value == "and") {
        if (left.type == right.type and left.type == BOOL) {
            result = Token(std::to_string(((left.value == right.value) and (right.value == "1" and left.value == "1"))), BOOL);
            return;
        }
    }
    if (op.value == "or") {
        if (left.type == right.type and left.type == BOOL) {
            result = Token(std::to_string((left.value == "1" or right.value == "1")), BOOL);
            return;
        }
    }
    stringstream s;
    s << "Error in logic operation: " << left.type << " " << op.value << " " << right.type;
    throw std::runtime_error(s.str());
    return;
}

Token Evals::EvaluateExpression(std::vector<Token>& tokens, int i) {
    Token left = tokens.at(i - 1);
    Token op = tokens.at(i);
    Token right = tokens.at(i + 1);

    Token result = Expression(left, right, op).result;


    tokens[i] = result;
    tokens.erase(tokens.begin() + i + 1);
    tokens.erase(tokens.begin() + i - 1);


    return result;
}

Token Evals::EvaluateLogicExpression(std::vector<Token>& tokens, int i) {
    Token left = tokens.at(i - 1);
    Token op = tokens.at(i);
    Token right = tokens.at(i + 1);

    Token result = LogicExpression(left, right, op).result;


    tokens[i] = result;
    tokens.erase(tokens.begin() + i + 1);
    tokens.erase(tokens.begin() + i - 1);


    return result;
}

Token Evals::EvaluateTokens(vector<Token> tokens) {
    for (int i = 0; i < tokens.size(); i++) {
        Token& token = tokens[i];
        if (token.type == OPERATOR and (token.value == "**")) {

            EvaluateExpression(tokens, i);
        }

        for (int i = 0; i < tokens.size(); i++) {
            Token& token = tokens[i];
            if (token.type == OPERATOR and (token.value == "*" or token.value == "/" or token.value == "//")) {
                EvaluateExpression(tokens, i);
            }
        }
        for (int i = 0; i < tokens.size(); i++) {
            Token& token = tokens[i];
            if (token.type == OPERATOR and (token.value == "+" or token.value == "-")) {
                EvaluateExpression(tokens, i);
            }
        }

        for (int i = 0; i < tokens.size(); i++) {
            Token& token = tokens[i];
            if (token.type == OPERATOR and (token.value == "%")) {
                EvaluateExpression(tokens, i);
            }
        }

        for (int i = 0; i < tokens.size(); i++) {
            Token& token = tokens[i];
            if (token.type == LOGIC) {
                EvaluateLogicExpression(tokens, i);
            }
        }


        if (tokens.size() <= 1)
            return tokens.front();
        return EvaluateTokens(tokens);
    }
}


void evalTableVals(vector<Token>& tokens, int num, Environment& env, std::shared_ptr<Table> *p) {
    
        tokens.erase(tokens.begin() + num);

        switch (tokens[num].type) {
        case TABLEDEC:
            tokens[num] = Utils::VarToToken(p->get()->GetValue(Evals::SimplifyExpr(Utils::EvaluateTable(tokens, num), env).value));
            break;
        case PERIOD:
            tokens.erase(tokens.begin() + num);
            Token& tag = tokens[num];
            if (tag.type == TEXT) {

                tag = Utils::VarToToken(p->get()->GetValue(tag.value));
            }
            break;
        }
        Token& newtoken = tokens[num];
        if (newtoken.type == TABLE) {
            VarValue val = Utils::TokenToVar(newtoken);
            evalTableVals(tokens, num, env, std::get_if<std::shared_ptr<Table>>(&val));
        }



    }


Token Evals::SimplifyExpr(vector<Token> tokens, Environment& env) {


    int size = tokens.size();
    for (int num = 0; num < size; num++) {

        Token& token = tokens[num];
        if (token.type == TABLEDEC) {
            
            Table tbl(tokens,env);
            size = tokens.size();
            
            token = Utils::VarToToken(std::make_shared<Table>(tbl));
            
            continue;
        }
        if (token.type == TEXT) {


            
            
            if (env.varExsists(token.value)) {
                VarValue val = env.getVar(token.value);
                if (auto p = std::get_if<std::shared_ptr<Table>>(&val)) {
                    
                    evalTableVals(tokens, num, env, p);
                } else {

                    token = Utils::VarToToken(val);
                }
                
                size = tokens.size();

            }
            else if (env.callbackExsists(token.value)) {
                Callback callback = env.findCallback(token.value);
                Environment environment = Environment(callback.args, &env);

                vector<VarValue> argVals = Utils::EvaluateArguments(tokens, env, num + 2);
                tokens.erase(tokens.begin() + num + 1);


                environment.setArgs(argVals);


                environment.type = Environment::FUNCTION;
                environment.callback = callback;
                token = Utils::VarToToken(environment.Call());

                size = tokens.size();

            }
        /*    else if (Utils::InList(TYPENAMES, token.value)) {
                vector<VarValue> args = Utils::EvaluateArguments(tokens, env, num + 2);
                tokens.erase(tokens.begin() + num + 1);
                if (args.size() != 1) {
                    stringstream s;
                    s << "Invalid args for type: " << token.value;
                    throw std::runtime_error(s.str());
                }
                if (token.value == "string") {
                    token = Utils::changeVarType(args[0], STRING);
                }
                else if (token.value == "bool") {
                    token = Utils::changeVarType(args[0], BOOL);
                }
                else if (token.value == "number") {
                    token = Utils::changeVarType(args[0], NUMBER);
                }
                
            }*/
            else if (env.defaultCallbackExsists(token.value)) {
                DefaultMethod dm = env.findDefaultCallback(token.value);
                
                vector<Token> argVals = Utils::EvaluateArgumentTokens(tokens, env, num + 2);
                tokens.erase(tokens.begin() + num + 1);
               /* std::cout << "GAY\n";*/
               
                token = dm.Call(argVals);
                /*for (Token t : tokens) {
                    std::cout << t.type << '-' << t.value << '\n';
                }*/
                size = tokens.size();
            }
            else {
                stringstream s;
                s << "Unknown variable: " << token.value;
                throw std::runtime_error(s.str());
            }
        }
    }
    Utils::EvaluateParenthesis(tokens, 0);
    return EvaluateTokens(tokens);
}

//Token Token::GetValue(string v) {
//    if (type == TABLE) {
//        return Utils::VarToToken(values[v]).GetValue();
//    }
//    else {
//        return Token(value,type);
//    }
//}