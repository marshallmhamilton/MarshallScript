#include "executor.h"
#include "util.h"
#include "tokens.h"
#include "table.h"

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
            switch (currentLine[0].Keyword) {
                case KeywordTypes::LET: {
                    if (currentLine.size() > 1 and currentLine[1].type == TEXT) {
                    
                        string varName = currentLine[1].value;
                        Token def = currentLine[2];
                        
                        if (!def.type == TokenType::DEF or def.value != "=") break;
                        
                        currentLine.erase(currentLine.begin(), currentLine.begin() + 3);


                        Token finalToken = Evals::SimplifyExpr(currentLine, env);
                        env.setVar(varName, Utils::TokenToVar(finalToken));
                        break;

                    }
                    break;
                }
                case KeywordTypes::FN: {
                    if (currentLine[1].type == TEXT) {

                        string funcName = currentLine[1].value;
                        currentLine.erase(currentLine.begin());

                        currentLine.erase(currentLine.begin());
                        env.setCallback(funcName, Callback(tokens, Utils::evalFunctionArgs(currentLine)));

                        break;


                    }
                    break;

                }
                case KeywordTypes::RETURN: {
                    if (!env.IsAncestorOfType(Environment::FUNCTION)) {
                        stringstream s;
                        s << "Illegal use of return!";
                        throw std::runtime_error(s.str());
                        break;
                    }
                    currentLine.erase(currentLine.begin());
                    Token finalToken = Evals::SimplifyExpr(currentLine, env);
                    return  Utils::TokenToVar(finalToken);
                    break;
                }
                case KeywordTypes::IF: {
               
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
                case KeywordTypes::WHILE: {
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
                case KeywordTypes::BREAK: {
                    if (env.IsAncestorOfType(Environment::LOOP)) {
                        return END;
                    }
                    break;
                }
            }
            
            /*else if (currentLine[0].value == "input" and currentLine[1].type == TEXT) {
                string text;
                std::cin.clear();

                std::getline(std::cin, text);
                string varName = currentLine[1].value;
                env.changeVar(varName, text);
                currentLine.erase(currentLine.begin(), currentLine.begin() + 1);
            }*/
            
             
            break;
        }
        case TEXT: {
            string varName = currentLine[0].value;

            
            if (env.callbackExsists(varName) || env.defaultCallbackExsists(varName)) {

                Evals::SimplifyExpr(currentLine, env);

            }

            else if (env.varExsists(varName)) {
                Token def = currentLine[1];
                if (def.type == PERIOD || def.type == TABLEDEC) {

                    bool end = false;
                    std::list<string> l;
                    currentLine.erase(currentLine.begin());  
                    while (!end) {
                        Token tblDef = currentLine[0];
                        
                        
                        switch (tblDef.type) {
                            case TABLEDEC: {
                                
                                vector<Token> evaled = Utils::EvaluateTable(currentLine, 0);
                               
                                currentLine.erase(currentLine.begin());
                               
                                l.push_front(Evals::SimplifyExpr(evaled, env).value);
                               
                               
                                
                                break;
                            }
                            case PERIOD: {
                                currentLine.erase(currentLine.begin());
                                l.push_front(currentLine[0].value);

                                break;
                            }
                            case DEF: {

                                currentLine.erase(currentLine.begin());
                                if (tblDef.value != "=") {

                                    stringstream s;
                                    s << "Invalid definition! when using tables can only use '='";
                                    throw std::runtime_error(s.str());
                                }
                                end = true;
                                break;
                            }
                            default: {

                                stringstream s;
                                s << "Invalid table";
                                throw std::runtime_error(s.str());
                                break;
                            }

                        }

                    } 
                    
                   
                    
                    if (auto p = std::get_if<std::shared_ptr<Table>>(&env.getVarRef(varName))) {
                        Table & tbl = *p->get();
                        tbl.ChangeValue(Evals::SimplifyExpr(currentLine,env),l);

                    }
                    break;
                    
                }
                else if (def.type != TokenType::DEF) {
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
