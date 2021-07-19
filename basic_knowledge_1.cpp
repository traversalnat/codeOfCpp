#include<iostream>
#include<algorithm>
using namespace std;

void sort(int (&v)[8]);

// 1. 变量初始化
// 能够由初始化符号推断类型可以使用 auto (c++11)
auto ch = 'x';
auto i = 123;
// 使用{} 初始化变量避免类型隐式转换
int i2 = {134};

// 仅C++11 中支持不加等号的做法
// 但是在C++11 中效果是一样的
double d1 {12.2};

// 以下在C++11 中会报错, 低版本中仅显示 warning
//int i3 = {12.3};

// 2. 常量以及常量表达式
const int dmv = 17;
int var = 17;


double square(double x) { return x * x; }
// 定义成 constexpr 的函数必须非常简单, 仅一条计算某个值得 return 语句
constexpr double constexpr_square(double x) { return x * x; }

int main() {
	// const 常量可以在运行时求值, 用于"承诺不修改这个值"
	const double max = 1.4 * square(var);
	// constexpr 常量表达式, 表示“在编译时求值”
	// 不带函数的
	constexpr double s1 = 9 * 9;
	// 带函数的, 要求函数必须定义为 constexpr, 
	// 且表达式必须在编译时能够求出值
	constexpr double s2 = 1.4 * constexpr_square(dmv);

	cout << s1 <<endl;
	cout << s2 <<endl;
	cout << max <<endl;
	cout << "Hello, world!\n";
	cout << ch <<endl;
	cout << i <<endl;
	cout << i2 <<endl;

	// for_each
	for (auto x : {1, 2, 3, 4, 5, 6, 7, 8})
		cout << x;
	cout << endl;

	// 引用, 不拷贝元素的值, 只是引用元素
	int v[] = {1, 2, 3, 4, 5, 6, 7, 8};
	for (auto& x : v) {
		++x;	// 引用 v[i], 相当于 v[i]++
	}
	// 使用引用的排序函数, 不用传递指针, 也就无需使用 *运算
	sort(v);

	for (auto x : v) 
		cout << x;
	cout<<endl;

	// 空指针
	// 替代 NULL, 防止 0 和 NULL 混合
	double *lst = nullptr;
	if (lst == nullptr) {
		cout << "nullptr" << endl;
	}

}

// 传递数组依然需要指定数组的宽度
void sort(int (&v)[8]) {
	for (auto x : v) {
		cout << x;
	}
	cout<<endl;
}

