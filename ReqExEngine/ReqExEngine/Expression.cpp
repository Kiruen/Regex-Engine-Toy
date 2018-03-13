#include "Expression.h"
using namespace std;

//-----------------------------------------------------

Expression::Expression(ExprType type) :
	//subExprs(vector<shared_ptr<Expression>>()),
	Type(type)
{

}

Expression::~Expression() { }

//-----------------------------------------------------

//初始化字符表达式池(不能只写前面的访问符)
CharExpr::CharPool CharExpr::pool;

CharExpr::CharExpr(wchar_t ch) :
	Expression(ExprType::Char),
	ch(ch)
{

}

shared_ptr<CharExpr> CharExpr::GetInstance(wchar_t ch)
{
	auto res = pool.find(ch);
	if (res != pool.end())
		return res->second;
	else
	{
		auto sp_expr = shared_ptr<CharExpr>(new CharExpr(ch));
		pool[ch] = sp_expr;
		return sp_expr;
	}
}

CharExpr::~CharExpr() { }

//-----------------------------------------------------

AndExpr::AndExpr() :
	Expression(ExprType::And),
	subExprs(vector<shared_ptr<Expression>>())
{

}

//void AndExpr::AddExpr(Expression* expr)
//{
//	subExprs.push_back(shared_ptr<Expression>(expr));
//}

void AndExpr::AddExpr(shared_ptr<Expression> sp_expr)
{
	subExprs.push_back(sp_expr);
}

AndExpr::~AndExpr() { }

//-----------------------------------------------------

OrExpr::OrExpr() :
	Expression(ExprType::Or),
	optExprs(vector<shared_ptr<Expression>>())
{

}

//void OrExpr::AddExpr(Expression* pexpr)
//{
//	optExprs.push_back(shared_ptr<Expression>(pexpr));
//}

void OrExpr::AddExpr(shared_ptr<Expression> sp_expr)
{
	optExprs.push_back(sp_expr);
}

OrExpr::~OrExpr() { }

//-----------------------------------------------------

RepExpr::RepExpr(shared_ptr<Expression> punit, int least, int most) :
	Expression(ExprType::Rep),
	unit(punit),
	least(least),
	most(most)
{

}

RepExpr::~RepExpr() { }

//-----------------------------------------------------

CharSetExpr::CharSetExpr(bool negative) :
	Expression(ExprType::Set),
	negative(negative),
	ranges()
{

}

CharSetExpr::~CharSetExpr()
{

}

void CharSetExpr::AddRange(int left, int right)
{
	ranges.insert({left, right});
}

shared_ptr<CharSetExpr> CharSetExpr::GetPreparedSet(int ch)
{
	return make_shared<CharSetExpr>();
}



