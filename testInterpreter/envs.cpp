#include "envs.h"
#include "util.h"
#include "executor.h"
#include "main.h"


Environment globalEnvironment = Environment();

Callback::Callback(vector<Token>& tokens, vector<string> a) {
    instructions = Utils::EvaluateBrackets(tokens);
    args = a;
}

Callback::Callback() {}

DefaultMethod::DefaultMethod(Function f, int maxa, int mina) {
    function = f;
    maxArgs = maxa;
    minArgs = mina;
}

Environment::Environment(Environment* parentEnv)
{
    parent = parentEnv;
};

Environment::Environment(vector<string> a, Environment* parentEnv) {
    args = a;
    parent = parentEnv;
}

bool Environment::IsAncestorOfType(EnvironmentType t) {
    if (parent == nullptr)
        return false;
    
    if (parent->type == t)
        return true;
    else {
        return parent->IsAncestorOfType(t);
    }
}

void Environment::setArgs(vector<VarValue> v) {
    int num = 0;

    for (string val : args) {

        variables.emplace(val, v[num]);
        num++;
    }

}

VarValue Environment::Call() {

    return Executor::Execute(callback.instructions, this);
}

VarValue Environment::CallCallback(Callback c) {
    return Executor::Execute(c.instructions, this);
}

void Environment::setVar(string id, VarValue val) {
    variables.emplace(id, val);
}

void Environment::changeVar(string id, const VarValue& val) {


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

VarValue Environment::getVar(string id) {
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
VarValue & Environment::getVarRef(string id) {
    auto it = variables.find(id);

    if (it != variables.end()) {

        return it->second;
    }
    else {
        if (parent != nullptr) {
            return parent->getVarRef(id);
        }
    }
}

Callback Environment::findCallback(string id) {
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

DefaultMethod Environment::findDefaultCallback(string id) {
    auto it = defaultMethods.find(id);
    if (it != defaultMethods.end()) {
        return it->second;
    }
    else {
        if (parent != nullptr) {
            return parent->findDefaultCallback(id);
        }
    }
}

bool Environment::callbackExsists(string id) {
    
    if (callbacks.find(id) != callbacks.end()) {
        return true;
    }
    else {
        if (parent != nullptr) {
            return parent->callbackExsists(id);
        }
        else {
            return false;
        }
    }
}

bool Environment::defaultCallbackExsists(string id) {
    
    if (defaultMethods.find(id) != defaultMethods.end()) {
        return true;
    }
    else {
        if (parent != nullptr) {
            return parent->defaultCallbackExsists(id);
        }
        else {
            return false;
        }
    }
}



bool Environment::varExsists(string id) {
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

void Environment::setCallback(string id, Callback callback) {
    callbacks.emplace(id, callback);
}

Token Print(vector<Token> args) {
    printf(args[0].value.c_str());
    printf("\n");
    return Token();
}
Token RawPrint(vector<Token> args) {
    printf(args[0].value.c_str());
    return Token();
}

Token Input(vector<Token> args) {
    if(args.size() >= 1) 
        printf(args[0].value.c_str());
    
    string text;
    std::cin.clear();
    std::getline(std::cin, text);
    return Token(text, STRING);

}

Token Random(vector<Token> args) {
    if (args.size() != 2) {
        stringstream s;
        s << "Invalid number of arguments!";
        throw std::runtime_error(s.str());
    } 
    try {
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(std::stoi(args[0].value), std::stoi(args[1].value));
        int randomNum = distrib(gen);
        return Token(std::to_string(randomNum), NUMBER);
    }
    catch (...) {
        throw std::runtime_error("attempted to compare invalid values in random function");
    }
    
}

Token MakeString(vector<Token> args) {
    return Utils::changeVarType(Utils::TokenToVar(args[0]), STRING);
}

Token MakeNumber(vector<Token> args) {
    return Utils::changeVarType(Utils::TokenToVar(args[0]), NUMBER);
}

Token DefaultMethod::Call(vector<Token> args) {
    if (!(args.size() >= minArgs && args.size() <= maxArgs)) {
        stringstream s;
        s << "Invalid number of args!";
        throw std::runtime_error(s.str());
    }
    return function(args);
}

