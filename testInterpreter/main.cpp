#include "main.h"


enum EnvironmentType {
    LOOP,
    FUNCTION,
    NONE
};
vector<Token> EvaluateBrackets(vector<Token>& tokens);
class Callback {
public:
    vector<string> args = {};
    vector<Token> instructions = {};
    Callback(vector<Token>& tokens,vector<string> a = {}) {
        instructions = EvaluateBrackets(tokens);
        args = a;
    }
    Callback() {
        
    }

    

};

class Environment;
VarValue GenerateInstructions(vector<Token> tokens, Environment environment);

class Environment {

public:
    Environment* parent;  
    EnvironmentType type = NONE;
    Callback callback;
    vector<string> args;
    
    std::unordered_map<string, VarValue> variables = {};
    std::unordered_map<string, Callback> callbacks = {};
    Environment(Environment* parentEnv = nullptr)
        : parent(parentEnv) {};
    
    Environment(vector<string> a, Environment* parentEnv = nullptr) {
        args = a;
        parent = parentEnv;
    }
    

    bool IsAncestorOfType(EnvironmentType t) {
        if (parent == nullptr)
            return false;

        if (parent->type == t)
            return true;
        else {
            return parent->IsAncestorOfType(t);
        }
    }

    void setArgs(vector<VarValue> v) {
        int num = 0;
        
        for (string val : args) {
            
            variables.emplace(val, v[num]);
            num++;
        }

    }

    VarValue Call() {
        
        return GenerateInstructions(callback.instructions, this);
    }

    VarValue CallCallback(Callback c) {
        return GenerateInstructions(c.instructions, this);
    }
    
    
    void setVar(string id, VarValue val) {
        variables.emplace(id, val);
    }
    void changeVar(string id, const VarValue& val) {
        
        
        auto it = variables.find(id);
        if (it != variables.end()) {
            variables[id] = val;
   
        }
        else {
            if (parent != nullptr) {
                parent->changeVar(id, val);
            }
        }
        
        
        
    }
    VarValue getVar(string id) {
        auto it = variables.find(id);
       
        if (it != variables.end()) {
            
            return it->second;
        }
        else {
            if (parent != nullptr) {
                return parent->getVar(id);
            }
            else {
                return false;
            }
        }
    }

    Callback findCallback(string id) {
        auto it = callbacks.find(id);
        if (it != callbacks.end()) {
            return it->second;
        }
        else {
            if (parent != nullptr) {
                return parent->findCallback(id);
            } 
        }
    }

    bool callbackExsists(string id) {
        auto it = callbacks.find(id);
        if (it != callbacks.end()) {
            return true;
        }
        else {
            if (parent != nullptr) {
                return parent->callbackExsists(id);
            } else{
                return false;
            }
        }
    }
    bool varExsists(string id) {
        auto it = variables.find(id);
        if (it != variables.end()) {
            return true;
        }
        else {
            if (parent != nullptr) {
                return parent->varExsists(id);
            }
            else {
                return false;
            }
        }
    }

    void setCallback(string id, Callback callback) {
        callbacks.emplace(id, callback);
    }

};

Environment globalEnviornment;


Token VarToToken(VarValue var) {
    
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

VarValue TokenToVar(Token token) {
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

vector<Token> Lexer(string text) {
    vector<Token> tokens = vector<Token>{};
    
    int len = text.length();
   

    for (int pos = 0; pos < len; pos++) {

        char c = text[pos];
        stringstream val;
        val << c;

        if (Utils::IsWhiteSpace(c))
            continue;

        if (Utils::InList(LOGIC_DEF_CHARS, c)) {
            
            
            for (int i = pos + 1; i < len; i++) {
                if (not Utils::InList(LOGIC_DEF_CHARS, text[i])) break;
                val << text[i];
                pos = i;
            }
            if (Utils::InList(LOGIC_DEFS, val.str())) {
                tokens.push_back(Token(val.str(), LOGIC));
                
                continue;
            }
            
        }

        if (Utils::InList(OPERATORS, c)) {
           
            for (int i = pos + 1; i < len; i++) {
                if (not Utils::InList(OPERATORS, text[i])) break;
                val << text[i];
                pos = i;
            }

            if (Utils::InList(DEFS, val.str())) {
                tokens.push_back(Token(val.str(), DEF));
                continue;
            }
            

            tokens.push_back(Token(val.str(), OPERATOR));

            continue;
        }

        

        if (Utils::InList(TEXT_CHARS, c)) {
               
            for (int i = pos + 1; i < len; i++) {

                if (not Utils::InList(TEXT_CHARS, text[i]) and not isdigit(text[i])) break;
                pos = i;

                val << text[i];
            }
        
            if (val.str() == "true" or val.str() == "false") {
                
                
                tokens.push_back(Token(std::to_string((val.str() == "true")), BOOL));
                continue;
            }
                
                

            if (Utils::InList(KEYWORDS, val.str())) {
                tokens.push_back(Token(val.str(), KEYWORD));
                continue;
            }

            tokens.push_back(Token(val.str(), TEXT));


            continue;
        }


        if (isdigit(c)) {
            bool period = false;
            for (int i = pos + 1; i < len; i++) {
                char ch = text[i];
                if (not isdigit(ch) and ch != '.') break;   
                if (ch == '.') {
                    if (period) break;                  
                    period = true;                       
                }
                val << ch;                               
                pos = i;                                 
            }
            tokens.push_back(Token(val.str(), NUMBER));

            continue;
        }

        if (c == '(' or c == ')') {
            val = stringstream();
            if (c == '(') val << "L";
            else val << "R";
            tokens.push_back(Token(val.str(), PARENTHESIS));
            continue;
        }
        if (c == '{' or c == '}') {
            tokens.push_back(Token(val.str(), BRACKET));
            continue;
        }

        if (c == ';') {
            tokens.push_back(Token(val.str(), ENDL));
            continue;
        }
        //77
        if (c == '"' or c == '\'') {
            val = stringstream();
            for (int i = pos + 1; i < len; i++) {
                char ch = text[i];
                pos = i;
                
                if (c == ch) break;
                val << ch;
            }
            tokens.push_back(Token(val.str(), STRING));
            continue;
        }

        if (c == ',') {
            tokens.push_back(Token(val.str(), COMMA));
            continue;

        }

        stringstream s;
        s << "Unexpected character: '" << c << "'";
        throw std::runtime_error(s.str());
    }


    return tokens;
}
Token EvaluateExpression(std::vector<Token>& tokens, int i) {
    Token left = tokens.at(i - 1);
    Token op = tokens.at(i);
    Token right = tokens.at(i + 1);

    Token result = Expression(left, right, op).result;

    tokens[i] = result;
    tokens.erase(tokens.begin() + i + 1);
    tokens.erase(tokens.begin() + i - 1);


    return result;
}

Token EvaluateLogicExpression(std::vector<Token>& tokens, int i) {
    Token left = tokens.at(i - 1);
    Token op = tokens.at(i);
    Token right = tokens.at(i + 1);

    Token result = LogicExpression(left, right, op).result;

 
    tokens[i] = result;
    tokens.erase(tokens.begin() + i + 1);
    tokens.erase(tokens.begin() + i - 1);


    return result;
}

Token EvaluateTokens(vector<Token> tokens) {




    for (int i = 0; i < tokens.size(); i++) {
        Token& token = tokens[i];
        if (token.type == OPERATOR and (token.value == "**")) {

            EvaluateExpression(tokens, i);
        }
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
int EvaluateParenthesis(vector<Token>& tokens, int begin) {
    for (int i = begin; i < (int)tokens.size(); ++i) {
        Token& token = tokens[i];
        if (token.type == PARENTHESIS && token.value == "L") {
            int pos = EvaluateParenthesis(tokens, i + 1);
            if (pos < 0) return -1;

            vector<Token> v(tokens.begin() + i + 1, tokens.begin() + pos);
            Token result = EvaluateTokens(v);
            tokens.erase(tokens.begin() + i + 1, tokens.begin() + pos + 1);
            tokens[i] = result;
        }
        else if (token.type == PARENTHESIS && token.value == "R") {
            return i;
        }
    }
    return -1;
}



vector<Token> EvaluateBrackets(vector<Token>& tokens) {
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

vector<VarValue> EvaluateArguments(vector<Token>& tokens, Environment env, int start);

Token changeVarType(VarValue val, TokenType convert) {
    Token first = VarToToken(val);
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

Token SimplifyExpr(vector<Token> tokens,   Environment& env) {
    
    
    int size = tokens.size();
    for (int num = 0; num < size; num++) {
        
        Token& token = tokens[num];
        if (token.type == TEXT) {

           
           
            if (env.varExsists(token.value)) {
                
                
                VarValue val = env.getVar(token.value);
                
                token = VarToToken(val);
            
            } else if (env.callbackExsists(token.value)) {
                Callback callback = env.findCallback(token.value);
                Environment environment = Environment(callback.args, &env);
                
                vector<VarValue> argVals = EvaluateArguments(tokens, env,num+2);
                tokens.erase(tokens.begin() + num + 1);
               

                environment.setArgs(argVals);
                

                environment.type = FUNCTION;
                environment.callback = callback;
                token = VarToToken(environment.Call());
               
                size = tokens.size();
                
            }
            else if (Utils::InList(TYPENAMES,token.value)) {
                vector<VarValue> args = EvaluateArguments(tokens, env, num + 2);
                tokens.erase(tokens.begin() + num + 1);
                if (args.size() != 1) {
                    stringstream s;
                    s << "Invalid args for type: " << token.value;
                    throw std::runtime_error(s.str());
                }
                if (token.value == "string") {
                    token = changeVarType(args[0], STRING);
                }
                else if (token.value == "bool") {
                    token = changeVarType(args[0], BOOL);
                }
                else if (token.value == "number") {
                    token = changeVarType(args[0], NUMBER);
                }
                size = tokens.size();
            }
            else {
                stringstream s;
                s << "Unknown variable: " << token.value;
                throw std::runtime_error(s.str());
            }
        }
    }
    EvaluateParenthesis(tokens,0);
    return EvaluateTokens(tokens);
}

vector<VarValue> EvaluateArguments(vector<Token>& tokens, Environment env ,int start) {
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
        
        Token finalToken = SimplifyExpr(argVal, env);
        args.push_back(TokenToVar(finalToken));

    }
    
    return args;
}
//777777
vector<string> evalFunctionArgs(vector<Token> tokens) {
    if (tokens[0].type == PARENTHESIS and tokens[0].value == "L") {
        tokens.erase(tokens.begin());
        vector<string> argVal = {};
        while (!tokens.empty()) {
            if (tokens[0].type == TEXT) {
                argVal.push_back(tokens[0].value);
                

            } else if (tokens[0].type == PARENTHESIS and tokens[0].value == "R")
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

VarValue GenerateInstructions(vector<Token> tokens, Environment env) {
    
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
                

                Token finalToken = SimplifyExpr(currentLine,env);
                env.setVar(varName, TokenToVar(finalToken));


            }
            else if (currentLine[0].value == "print" and currentLine.size() > 1) {
                
                currentLine.erase(currentLine.begin()); 

                Token finalToken = SimplifyExpr(currentLine,env);

                std::cout << finalToken.value << "\n";
            }
            else if (currentLine[0].value == "rawprint" and currentLine.size() > 1) {
                
                currentLine.erase(currentLine.begin());

                Token finalToken = SimplifyExpr(currentLine,env);

                std::cout << finalToken.value;
                break;
            }
            else if (currentLine[0].value == "fn" and currentLine[1].type == TEXT) {
                
                string funcName = currentLine[1].value;
                currentLine.erase(currentLine.begin());
                
                currentLine.erase(currentLine.begin());
                env.setCallback(funcName, Callback(tokens,evalFunctionArgs(currentLine)));

                break;
                

            }
            else if (currentLine[0].value == "return") {
                if (!env.IsAncestorOfType(FUNCTION)) {
                    stringstream s;
                    s << "Illegal use of return!";
                    throw std::runtime_error(s.str());
                    break;
                }
                currentLine.erase(currentLine.begin());
                Token finalToken = SimplifyExpr(currentLine,env);
                return TokenToVar(finalToken);
            }
            else if (currentLine[0].value == "input" and currentLine[1].type == TEXT) {
                string text;
                std::cin.clear();

                std::getline(std::cin, text);
                string varName = currentLine[1].value;
                env.changeVar(varName, text);
                currentLine.erase(currentLine.begin(),currentLine.begin()+1);
            } 
            else if (currentLine[0].value == "if") {
                currentLine.erase(currentLine.begin());
                Token result = SimplifyExpr(currentLine,env);
                
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
                    Token resultToken = VarToToken(var);
                    
                    
                    
                    if (resultToken.type != ERROR and env.IsAncestorOfType(FUNCTION)) {
                        return var;
                    }
                    
                    
                }
                break;
                
            } 
            else if (currentLine[0].value == "while") {
                currentLine.erase(currentLine.begin());
                vector<Token> logic(currentLine);
                
                currentLine.erase(currentLine.begin(), currentLine.begin() + 1);
                Token result = SimplifyExpr(vector<Token>(logic),env);
                Environment environment = Environment(&env);
                environment.type = LOOP;
                environment.callback = Callback(tokens);
                while (result.value == "1") {
                   
                    VarValue var = environment.Call();
                    Token resultToken = VarToToken(var);
                    
                    if (resultToken.type != ERROR and env.IsAncestorOfType(FUNCTION)) {
                        return var;
                    }
                    if (resultToken.type == BREAK and resultToken.value == "1") {
                        break;
                    }

                    result = SimplifyExpr(vector<Token>(logic),env);
                   
                }
                break;
            } 
            else if (currentLine[0].value == "break" and env.IsAncestorOfType(LOOP)) {
                return END;
            }
            break;
        }
            case TEXT: {
                string varName = currentLine[0].value;
                

                if (env.callbackExsists(varName)) {
                    
                    SimplifyExpr(currentLine, env);
                    
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

                    
                    Token result("",STRING);
                    if (def.value == "=") {

                        result = SimplifyExpr(currentLine,env);
                    }
                    else if (def.value == "+=") {
                        Token token1 = SimplifyExpr(currentLine,env);
                        Token token2 = VarToToken(env.getVar(varName));
                        result = SimplifyExpr(vector<Token>{token2, Token("+", OPERATOR), token1},env);
                    }
                    else if (def.value == "*=") {
                        Token token1 = SimplifyExpr(currentLine,env);
                        Token token2 = VarToToken(env.getVar(varName));
                        result = SimplifyExpr(vector<Token>{token2, Token("*", OPERATOR), token1},env);
                    }
                    else if (def.value == "-=") {
                        Token token1 = SimplifyExpr(currentLine,env);
                        Token token2 = VarToToken(env.getVar(varName));
                        result = SimplifyExpr(vector<Token>{token2, Token("-", OPERATOR), token1},env);
                    }
                    else if (def.value == "/=") {
                        Token token1 = SimplifyExpr(currentLine,env);
                        Token token2 = VarToToken(env.getVar(varName));
                        result = SimplifyExpr(vector<Token>{token2, Token("/", OPERATOR), token1},env);
                    }
                    env.changeVar(varName, TokenToVar(result));
                    
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
    
}


void readFile(std::ifstream file) {
    try {
        stringstream text;
        string line;
        if (file.is_open())
        {
            while (std::getline(file, line))
            {
                text << line;
            }
            file.close();
        }

        vector<Token> codeTokens = Lexer(text.str());

        GenerateInstructions(codeTokens, globalEnviornment);
    }
    catch (std::runtime_error err) {
        std::cout << "Error: " << err.what() << '\n';
    }
    catch (...) {
        std::cout << "Error\n";
    }
    
}

int main(int argc, char** argv)
{
    
   /* std::cout << "init" << '\n';
    readFile(std::ifstream("C:/InterpreterCode/test.txt"));*/
    if (argc >= 3 and string(argv[1]) == "run") {
        string filename = argv[2];
        
        readFile(std::ifstream(filename));
        return 0;
    } else if (argc == 2 and string(argv[1]) == "-version") {

        std::cout << "MarshallScript version " << version << '\n';

        return 0;
    }
    std::cerr << "Usage:\n Running a file: MarshallScript run <file>\n Version: MarshallScript -version\n";
   
    return 1;
    //return 0;
    
}