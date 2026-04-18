#include "util.h"

struct Util {
	static bool IsIntegar(string s) {
		if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

		char* p;
		strtol(s.c_str(), &p, 10);

		return (*p == 0);
	};
	static bool IsWhitespace(char c) {
		return (c == '\n' or c == ' ' or c == '\t');
	}
	static bool InList(std::list<char> l, char c) {
		return (std::find(l.begin(), l.end(), c) != l.end());
	};
};