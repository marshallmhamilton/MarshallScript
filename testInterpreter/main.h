#ifndef MAIN_H
#define MAIN_H

#include "deps.h"

using std::string;
using std::stringstream;
using std::vector;
using std::map;
using std::array;

enum None {
    NIL
};
enum LoopEndType {
    NOEND, END
};

class Table;

using VarValue = std::variant<std::monostate, int64_t, double, string, bool, LoopEndType, std::shared_ptr<Table>, None>;


#endif