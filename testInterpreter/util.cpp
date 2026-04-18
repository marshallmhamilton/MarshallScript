#include "util.h"


bool Utils::IsIntegar(string s) {
	if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

	char* p;
	strtol(s.c_str(), &p, 10);

	return (*p == 0);
};

bool Utils::IsWhiteSpace(char c) {
	return (c == '\n' or c == ' ' or c == '\t');
}
