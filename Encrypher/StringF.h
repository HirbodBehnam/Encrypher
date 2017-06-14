#include <algorithm>
#include <string>
#pragma once
using namespace std;

class StringF {
public:
	static string toLower(string str) {
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}
};