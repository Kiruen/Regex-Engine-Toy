#ifndef _KRegex
#define _KRegex

#include <iostream>
#include <vector>
#include "Expression.h"
using namespace std;

namespace regex {
	class Regex {
	private:
		wstring expr;
		vector<wstring> res;
	public:
		Regex(wstring);
		~Regex();

		//bool IsChar(const wchar_t*, wchar_t);
		//void NextChar(const wchar_t*&, wchar_t);
		//bool IsCharSet(const wchar_t*);
		//void NextCharSet(const wchar_t*&);
	};
}

#endif