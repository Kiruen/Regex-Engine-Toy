#ifndef _KExpression
#define _KExpression

#include <map>
#include <vector>
#include <set>
#include <memory>

enum ExprType { Unknown, Char, And, Or, Opt, Rep, Set };

class Expression
{
public:
	const ExprType Type;

	Expression(ExprType type);
	virtual ~Expression() = 0;
	//virtual wstring ToString() { return L"Unknown"; }
//private:
//	std::vector<std::shared_ptr<Expression>> subExprs;
};

class CharExpr : public Expression
{
public:
	typedef std::map<wchar_t, std::shared_ptr<CharExpr>> CharPool;

	~CharExpr() override;

	static std::shared_ptr<CharExpr> GetInstance(wchar_t ch);

private:
	const wchar_t ch;
	static CharPool pool;	//不能是const...否则不可变
	//隐藏构造函数
	CharExpr(wchar_t ch);
};

class AndExpr : public Expression
{
public:
	AndExpr();
	~AndExpr() override;

	//void AddExpr(Expression* expr);
	void AddExpr(std::shared_ptr<Expression> sp_expr);

private:
	std::vector<std::shared_ptr<Expression>> subExprs;
};

class OrExpr : public Expression
{
public:
	OrExpr();
	~OrExpr() override;

	//void AddExpr(Expression* expr);
	void AddExpr(std::shared_ptr<Expression> sp_expr);


private:
	std::vector<std::shared_ptr<Expression>> optExprs;
};

class RepExpr : public Expression
{
public:
	RepExpr(std::shared_ptr<Expression> punit, int least, int most);
	~RepExpr() override;
private:
	int least;
	int most;
	std::shared_ptr<Expression> unit;
};

class CharSetExpr : public Expression
{
public:
	typedef std::map<int, std::shared_ptr<CharSetExpr>> PreparedSet;
	CharSetExpr(bool negative = false);
	~CharSetExpr() override;

	void AddRange(int left, int right);

	static std::shared_ptr<CharSetExpr> GetPreparedSet(int ch);
private:
	//a-z: {'a', 'z'} ; a: {'a', 'a'} ; \d: {-1: -1}
	bool negative;
	std::set<std::pair<int, int>> ranges;
	static PreparedSet prepared;
};
#endif // !_Expr