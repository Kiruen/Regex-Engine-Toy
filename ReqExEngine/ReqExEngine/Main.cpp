#include <iostream>
#include <string>
#include "Parser.h"
using namespace std;

typedef bool(*Collector)(wchar_t);

void twoDimensionArrayWarmUp()
{
	int arr[2][2] = { { 1, 2 },{ 3, 4 } };	//一个指向指针的指针
	cout << arr << endl;					
	cout << arr[0] << endl;					//二级指针偏移0 * (2 * 4)字节处的值,还是个指针(跨行)
	cout << arr[1] << endl;					//二级指针偏移1 * (2 * 4)字节处的值,还是个指针
	cout << arr + 1 << endl;				//!一模
	cout << *(arr + 1) << endl;				//!一样.可见二级指针必须解两次引用！
	cout << *(*(arr + 1) + 1) << endl;		//二级指针偏移1 * (2 * 4)字节处的值(一个ptr)偏移4字节处的值(解两次引用)
											//这里可以证明*arr !== arr(前者:地址->值,后者:地址->地址,而它们的地址都是同一个值),
											//因为你不可能写成*(arr + 1 + 1)
											//解引用后指针的跨度可能会变
	cout << *arr[1] << endl;
	cout << *(arr[1] + 1) << endl;
}

bool IsEven(wchar_t ch)
{
	short numVal = ch - '0';
	return 0 <= numVal && numVal <= 10 && numVal % 2 != 0;
}

wstring getString(const wchar_t*& input, Collector coll)
{
	//并不能返回函数内部的引用！！所有对象均会在函数退栈后销毁
	wstring str;
	while (*input) {
		if (coll(*input))
			str += *input;
		input++;
	}
	return str;
}

int main()
{
	//twoDimensionArrayWarmUp();
	wstring sample = L"haha254886haha1234";
	/*wcout.imbue(std::locale("chs"));*/
	const wchar_t* ptr = sample.c_str();
	wstring res = getString(ptr, IsEven);
	wcout << ptr;
	//传递的引用必须是左值,即至少要有一个引用它的变量
	//res = getString(sample.c_str(), IsEven);
	wcout << res << endl;
	char ch = 0;
	if(ch)
		wcout << 0;
	res = L".?\\(+[^a-zabc\\t\\w]{18,22}|(b\\d|xvasfasdd*(\\t?1*)+)?";
	//当碰到'|'时，将and装入or中，然后and清空准备接下来新的and
	//for (size_t i = 0; i < 10; i++)
	//{
	Parser parser(res);
	auto exp = parser.ParseBlock();
	//}

	
	system("pause");
	return 0;
}
