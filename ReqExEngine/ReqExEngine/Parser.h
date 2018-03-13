#ifndef _KParser
#define _KParser

#include <iostream>
#include <set>
#include "Expression.h"

class Parser
{
	const static std::set<wchar_t> reserved;
	static std::map<wchar_t, wchar_t> ESCtoChar;
public:
	typedef std::shared_ptr<Expression> PExpr;

	Parser(std::wstring &expr);
	~Parser();

	//PExpr ParseChar();
	PExpr ParseUnit();
	PExpr ParseBlock();
	//PExpr ParseAnd();
	//PExpr ParseOr();
	//PExpr ParseOptional();
	//PExpr ParseRepeat();
	PExpr ParseCharSet();
	//转换一个转移字符,ch将被转换成绝对值
	PExpr ConvertESC(int ch);

	int PeekChar();
	int ReadChar(bool moveon = true);
	int ReadNum();
	bool SkipChar(wchar_t ch);

private:
	const std::wstring srcExp;
	const std::wstring::const_iterator endPos;
	std::wstring::iterator currPos;
};

#endif