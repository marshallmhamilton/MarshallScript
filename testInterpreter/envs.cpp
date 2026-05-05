#include "envs.h"
#include "util.h"
#include "executor.h"
#include "main.h"
Callback::Callback(vector<Token>& tokens, vector<string> a) {
    instructions = Utils::EvaluateBrackets(tokens);
    args = a;
}

Callback::Callback() {}

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

bool Environment::callbackExsists(string id) {
    auto it = callbacks.find(id);
    if (it != callbacks.end()) {
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