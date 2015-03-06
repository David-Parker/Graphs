#include <iostream>
using namespace std;

#define _String template <typename STR>

_String
inline void print(STR msg) {
	cout << msg;
}