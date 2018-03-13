#include "Parser.h"
using namespace std;

const set<wchar_t> Parser::reserved = 
{ 
	'\\', '|', '(', ')',
	'{', '}', '?', '*',
	'+', '[', ']', '^',
	'.'
};

map<wchar_t, wchar_t> Parser::ESCtoChar = 
{
	{'t', '\t'}, { 'n', '\n' },{ 'r', '\r'}
};

Parser::Parser(wstring &expr) :
	srcExp(expr),
	endPos(expr.cend()),
	currPos(expr.begin())
{
	
}

Parser::~Parser()
{
}

//处理一块表达式，等价于处理一个四则运算表达式:
//将and视为*，or视为+，unit视为操作数，后缀是单目运算符
Parser::PExpr Parser::ParseBlock()
{
	auto and = make_shared<AndExpr>();
	auto or = make_shared<OrExpr>();
	//shared_ptr<OrExpr> or (new OrExpr);
	bool return_and = true;
	while (currPos < endPos)
	{
		auto unit = ParseUnit();
		if (!unit) 
			break;
		else if (SkipChar('?'))
			and ->AddExpr(make_shared<RepExpr>(unit, 0, 1));
		else if(SkipChar('*'))
			and ->AddExpr(make_shared<RepExpr>(unit, 0, -1));
		else if (SkipChar('+'))
			and ->AddExpr(make_shared<RepExpr>(unit, 1, -1));
		else if (SkipChar('{'))
		{
			int least = ReadNum();
			SkipChar(',');
			//TODO:处理{X,}
			int most = ReadNum();
			SkipChar('}');
			and->AddExpr(make_shared<RepExpr>(unit, least, most));
		}
		else
			and->AddExpr(make_shared<RepExpr>(unit, 1, 1));
			//and->AddExpr(unit);
		//若遇到低优先级的运算符，则将此批and放到or里面，再重置and
		if (SkipChar('|'))
		{
			or ->AddExpr(and);
			and = shared_ptr<AndExpr>(new AndExpr);
			return_and = false;
		}
	}
	if (return_and)
		return and;
	else
	{
		or ->AddExpr(and);
		return or ;
	}	
}

Parser::PExpr Parser::ParseUnit()
{
	if (SkipChar('('))
	{
		auto block = ParseBlock();
		if (block && SkipChar(')'))
			return block;
	}
	else if (SkipChar('['))
	{
		auto charset = ParseCharSet();
		return SkipChar(']') ? charset : nullptr;
	}
	//下一个字符为非直接转义字符的话(可以通过单纯地去掉反斜杠来转换)
	else if (PeekChar())
	{
		auto ch = ReadChar();
		return ch < 0 ? ConvertESC(ch) : CharExpr::GetInstance(ch);
	}
	else
		return nullptr;
}

//Parser::PExpr Parser::ParseChar()
//{
//	auto ch = ReadChar();
//	return ch ? CharExpr::GetInstance(ch) : nullptr;
//	auto lastPos = currPos;
//	wstring str;
//	while (currPos != endPos)
//	{
//		auto ch = ReadChar();
//		if (!ch)
//			break;
//		str.push_back(ch);
//		currPos++;
//	}
//	return nullptr;
//}

//Parser::PExpr Parser::ParseAnd()
//{
//	return nullptr;
//}
//
//Parser::PExpr Parser::ParseOr()
//{
//	return nullptr;
//}
//
//Parser::PExpr Parser::ParseOptional()
//{
//	return nullptr;
//}
//
//Parser::PExpr Parser::ParseRepeat()
//{
//	return nullptr;
//}

Parser::PExpr Parser::ParseCharSet()
{
	//对于转义的字符集，存储其范围代号，以后根据代号决定范围
	auto charset = make_shared<CharSetExpr>(SkipChar('^'));
	int ch = PeekChar();

	//当预读到非实义字符时，退出循环
	while (ch/* && ch != ']'*/)
	{
		ch = ReadChar();
		if (SkipChar('-'))
			charset->AddRange(ch, ReadChar());
		else
			charset->AddRange(ch, ch);
		ch = PeekChar();
	} 
	return charset;
}

int Parser::PeekChar()
{
	return ReadChar(false);
}

int Parser::ReadChar(bool moveon)
{
	auto lastPos = currPos;
	short ch = currPos >= endPos ? 0 : *(currPos++);
	if (ch)
	{
		if (ch == '.')
			ch = -ch;
		else if (ch == '\\')
		{
			ch = currPos >= endPos ? 0 : *(currPos++);
			if (ch && reserved.find(ch) == reserved.end())
				//此部分可以简化，但要小心:转义字符需要自己去判断
				ch = ESCtoChar.find(ch) == ESCtoChar.end()
					? -ch : ESCtoChar[ch];
		}
		else if (reserved.find(ch) != reserved.end())
			ch = 0;
	}
	if (!moveon)
		currPos = lastPos;
	return ch;
}

bool Parser::SkipChar(wchar_t ch)
{
	if (currPos < endPos && *currPos == ch)
	{
		currPos++;
		return true;
	}
	return false;
}

int Parser::ReadNum()
{
	int res = 0;
	wchar_t ch;
	do
	{
		ch = ReadChar();
		res = res * 10 + (ch - '0');
		ch = PeekChar();
	} while ('0' <= ch && ch <= '9');
	return res;
}

Parser::PExpr Parser::ConvertESC(int ch)
{
	ch = abs(ch);
	if (ESCtoChar.find(ch) != ESCtoChar.end())
		return CharExpr::GetInstance(ESCtoChar[ch]);
	else
		return CharSetExpr::GetPreparedSet(ch);
}

