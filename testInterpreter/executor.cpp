#include "executor.h"
#include "util.h"

VarValue Executor::Execute(vector<Token> tokens, Environment env) {
    while (!tokens.empty()) {
        vector<Token> currentLine = {};
        int tokenNumber = 0;

        for (Token token : tokens) {

            tokenNumber += 1;
            if (token.type == ENDL) break;
            currentLine.push_back(token);

        }
        if (currentLine.empty()) {
            tokens.erase(tokens.begin(), tokens.begin() + tokenNumber);
            continue;
        }
        tokens.erase(tokens.begin(), tokens.begin() + tokenNumber);

        switch (currentLine[0].type) {
        case KEYWORD: {

            if (currentLine[0].value == "let" and currentLine.size() > 1 and currentLine[1].type == TEXT) {
                string varName = currentLine[1].value;
                Token def = currentLine[2];
                if (!def.type == DEF or def.value != "=") break;
                currentLine.erase(currentLine.begin(), currentLine.begin() + 3);


                Token finalToken = Evals::SimplifyExpr(currentLine, env);
                env.setVar(varName, Utils::TokenToVar(finalToken));


            }
            else if (currentLine[0].value == "print" and currentLine.size() > 1) {

                currentLine.erase(currentLine.begin());

                Token finalToken = Evals::SimplifyExpr(currentLine, env);

                std::cout << finalToken.value << "\n";
            }
            else if (currentLine[0].value == "rawprint" and currentLine.size() > 1) {

                currentLine.erase(currentLine.begin());

                Token finalToken = Evals::SimplifyExpr(currentLine, env);

                std::cout << finalToken.value;
                break;
            }
            else if (currentLine[0].value == "fn" and currentLine[1].type == TEXT) {

                string funcName = currentLine[1].value;
                currentLine.erase(currentLine.begin());

                currentLine.erase(currentLine.begin());
                env.setCallback(funcName, Callback(tokens, Utils::evalFunctionArgs(currentLine)));

                break;


            }
            else if (currentLine[0].value == "return") {
                if (!env.IsAncestorOfType(Environment::FUNCTION)) {
                    stringstream s;
                    s << "Illegal use of return!";
                    throw std::runtime_error(s.str());
                    break;
                }
                currentLine.erase(currentLine.begin());
                Token finalToken = Evals::SimplifyExpr(currentLine, env);
                return  Utils::TokenToVar(finalToken);
            }
            else if (currentLine[0].value == "input" and currentLine[1].type == TEXT) {
                string text;
                std::cin.clear();

                std::getline(std::cin, text);
                string varName = currentLine[1].value;
                env.changeVar(varName, text);
                currentLine.erase(currentLine.begin(), currentLine.begin() + 1);
            }
            else if (currentLine[0].value == "if") {
                currentLine.erase(currentLine.begin());
                Token result = Evals::SimplifyExpr(currentLine, env);

                currentLine.erase(currentLine.begin(), currentLine.begin() + 1);
                if (result.type != BOOL) {
                    stringstream s;
                    s << "Not a boolean";
                    throw std::runtime_error(s.str());
                    break;
                }
                Environment ifEnvironment = Environment(&env);
                ifEnvironment.callback = Callback(tokens);

                if (result.value == "1") {
                    VarValue var = ifEnvironment.Call();
                    Token resultToken = Utils::VarToToken(var);



                    if (resultToken.type != ERROR and env.IsAncestorOfType(Environment::FUNCTION)) {
                        return var;
                    }


                }
                break;

            }
            else if (currentLine[0].value == "while") {
                currentLine.erase(currentLine.begin());
                vector<Token> logic(currentLine);

                currentLine.erase(currentLine.begin(), currentLine.begin() + 1);
                Token result = Evals::SimplifyExpr(vector<Token>(logic), env);
                Environment environment = Environment(&env);
                environment.type = Environment::LOOP;
                environment.callback = Callback(tokens);
                while (result.value == "1") {

                    VarValue var = environment.Call();
                    Token resultToken = Utils::VarToToken(var);

                    if (resultToken.type != ERROR and env.IsAncestorOfType(Environment::FUNCTION)) {
                        return var;
                    }
                    if (resultToken.type == BREAK and resultToken.value == "1") {
                        break;
                    }

                    result = Evals::SimplifyExpr(vector<Token>(logic), env);

                }
                break;
            }
            else if (currentLine[0].value == "break" and env.IsAncestorOfType(Environment::LOOP)) {
                return END;
            }
            break;
        }
        case TEXT: {
            string varName = currentLine[0].value;


            if (env.callbackExsists(varName)) {

                Evals::SimplifyExpr(currentLine, env);

            }
            else if (env.varExsists(varName)) {
                Token def = currentLine[1];
                if (!def.type == DEF) {
                    stringstream s;
                    s << "No definition found for var: '" << varName << "'";
                    throw std::runtime_error(s.str());
                    break;
                }

                currentLine.erase(currentLine.begin(), currentLine.begin() + 2);


                Token result("", STRING);
                if (def.value == "=") {

                    result = Evals::SimplifyExpr(currentLine, env);
                }
                else if (def.value == "+=") {
                    Token token1 = Evals::SimplifyExpr(currentLine, env);
                    Token token2 = Utils::VarToToken(env.getVar(varName));
                    result = Evals::SimplifyExpr(vector<Token>{token2, Token("+", OPERATOR), token1}, env);
                }
                else if (def.value == "*=") {
                    Token token1 = Evals::SimplifyExpr(currentLine, env);
                    Token token2 = Utils::VarToToken(env.getVar(varName));
                    result = Evals::SimplifyExpr(vector<Token>{token2, Token("*", OPERATOR), token1}, env);
                }
                else if (def.value == "-=") {
                    Token token1 = Evals::SimplifyExpr(currentLine, env);
                    Token token2 = Utils::VarToToken(env.getVar(varName));
                    result = Evals::SimplifyExpr(vector<Token>{token2, Token("-", OPERATOR), token1}, env);
                }
                else if (def.value == "/=") {
                    Token token1 = Evals::SimplifyExpr(currentLine, env);
                    Token token2 = Utils::VarToToken(env.getVar(varName));
                    result = Evals::SimplifyExpr(vector<Token>{token2, Token("/", OPERATOR), token1}, env);
                }
                env.changeVar(varName, Utils::TokenToVar(result));

            }
            else {
                stringstream s;
                s << "No var '" << varName << "' exists";
                throw std::runtime_error(s.str());
                break;
            }
            break;
        }
        default: {
            stringstream s;
            s << "Unkown Starter token: " << currentLine[0].type << "(" << currentLine[0].value << ") \n";
            throw std::runtime_error(s.str());

        }

        }


    }

    return NIL;

};


//VarValue GenerateInstructions(vector<Token> tokens, Environment env) {
//

//}
