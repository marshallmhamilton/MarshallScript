#ifndef EXECUTOR_H
#define EXECUTOR_H
#include "main.h"
#include "tokens.h"
#include "envs.h"
     
struct Executor {
	
	static VarValue Execute(vector<Token> tokens, Environment env);

};


#endif
