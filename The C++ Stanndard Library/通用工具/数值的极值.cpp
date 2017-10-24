数值的极值

    一般说来，数值类型（numeric type） 有着与平台相依的极值。C++标准库借由template nummeric_limits提供这些极值，用以取代传统C语言所采用的预处理器常量（preprocessor constant)。
    你仍然可以使用后者，其中整数常量定义于<climits>和<limits.h> ，浮点常量定义于<climits>和<limits.h>。新的极值概念有两个优点，第一是提供了更好的类型安全性，第二是程序员可借此写出一些template以核定（evaluate)这些极值。 
    本节的剩余部分专门讨论极值问题。注意，C++ standard规定了各种类型必须保证的最小精度，如果你能够注惫并运用这些极值，就比较容易写出与平台无关的程序。这些最小值列于表5.8。

    //这个部分我并不想去沿照本书的发展写，因为它主要说Class numeric_limits<>的一些设计，在此我只想把书中的一个例子放在这里，供自己以后复习
#include "stdafx.h"
#include <iostream>
#include <limits>
#include <string>
using namespace std;

int main()
{
	// use textual representation for bool
	cout << boolalpha;

	// print maximum of integral types
	cout << "max(short): " << numeric_limits<short>::max() << endl;
	cout << "max(int):   " << numeric_limits<int>::max() << endl;
	cout << "max(long):  " << numeric_limits<long>::max() << endl;
	cout << endl;

	// print maximum of floating-point types
	cout << "max(float):       "
		<< numeric_limits<float>::max() << endl;
	cout << "max(double):      "
		<< numeric_limits<double>::max() << endl;
	cout << "max(long double): "
		<< numeric_limits<long double>::max() << endl;
	cout << endl;

	// print whether char is signed
	cout << "is_signed(char): "
		<< numeric_limits<char>::is_signed << endl;
	cout << endl;

	// print whether numeric limits for type string exist
	cout << "is_specialized(string): "
		<< numeric_limits<string>::is_specialized << endl;
}

    程序的输出结果与执行平台有关，下面是一份可能的输出:
        max(short)： 32767 
        max(int):   2147483647 
        max (long):  2147483647
    
        max(float):  3.40282e+38
        max(double): 1.797769e+308
        max(long double): 1.79769e+308

        is_signed(char): false                  //我的是true

        is_specialized(string): false           
        
