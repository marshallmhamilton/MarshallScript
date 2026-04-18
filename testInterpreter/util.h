#ifndef UTIL_H
#define UTIL_H
#include "main.h"

struct Utils {

	static bool IsIntegar(string s);

	static bool IsWhiteSpace(char c);

	

	template <typename T>
	static bool InList(std::list<T> l, T v) {
		return (std::find(l.begin(), l.end(), v) != l.end());
	}
};

#endif // UTIL_H