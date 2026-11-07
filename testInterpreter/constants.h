#ifndef CONSTANTS_H
#define CONSTANTS_H


#include "main.h"

const string version = "0.3.2";

const std::list<char> TEXT_CHARS = std::list<char>{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
'q','r','s','t','u','v','w','x','y','z','_','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V',
'W','X','Y','Z'
};

const std::list<char> OPERATORS = std::list<char>{
     '+','-','*','/', '%','='
};

const std::list<string> KEYWORDS = std::list<string>{
     "let","fn","return","if","while","break"
};

const std::list<string> TYPENAMES = std::list<string>{
     "string","number","bool"
};

const std::list<string> DEFS = std::list<string>{
    "=","+=","-=","*=","/="
};

const std::list<char> LOGIC_DEF_CHARS = std::list<char>{
    '=','<','>','!','a','o','n','d','r'
};

const std::list<string> LOGIC_DEFS = std::list<string>{
    "==","<=",">=","<",">","!=","and","or"
};

#endif // !CONSTANTS_H