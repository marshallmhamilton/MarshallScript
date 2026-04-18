#ifndef CONSTANTS_H
#define CONSTANTS_H


#include "main.h"

using VarValue = std::variant<std::monostate, int64_t, double, string, bool, LoopEndType, None>;
const string version = "0.1.0";
#endif // !CONSTANTS_H