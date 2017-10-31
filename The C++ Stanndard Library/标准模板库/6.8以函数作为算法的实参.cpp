以函数作为算法的实参

    有些算法可以接受用户自定义的辅助性函数，借以提高弹性和能力。这些函数将被算法内部调用。 

以函数作为算法实参的实例示范 

    最简单的例子莫过于for_each()算法了。它针对区间内的每一个元素，调用一个由用户指定的函数。下面是个例子：

#include "stdafx.h"
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

// function that prints the passed argument
void print (int elem)
{
    cout << elem << ' ';
}

int main()
{
    vector<int> coll;

    // insert elements from 1 to 9
    for (int i=1; i<=9; ++i) {
        coll.push_back(i);
    }

    // print all elements
    for_each (coll.cbegin(), coll.cend(),  // range
              print);                      // operation
    cout << endl;
}
    这里的for_each()函数针对区间[coll.cbegin(),coll.cend())内的每个元素调用print()函数。输出如下：
    1 2 3 4 5 6 7 8 9
    算法以数种态度来面对这些辅助函数：有的视为可有可无，有的视为必要。你可以利用它们来指定查找准则、排序准则，或定义某种操作使其在“容器元素转换至另一容器”时被调用。 
    下面是另一个例子：
    
#include "stdafx.h"
#include <set>
#include <vector>
#include <algorithm>
#include <iterator>
#include <iostream>
#include "print.hpp"

using namespace std;

int square(int value)
{
	return value * value;
}
int main()
{
	std::set<int>		coll1;
	std::vector<int> coll2;

	for (int i = 1;i<=9;++i)
	{
		coll1.insert(i);
	}
	PRINT_ELEMENTS(coll1, "initialized: ");

	std::transform(coll1.cbegin(), coll1.cend(),
		std::back_inserter(coll2),
		square);

	PRINT_ELEMENTS(coll2, "squared:	");
}

    此例之中，square()的作用是将coll1内的每一个元素以平方运算，然后转移到coll2（如图6.11所示）。程序输出如下：
    initialized: 1 2 3 4 5 6 7 8 9
    suqred:      1 4 9 16 25 36 49 64 81

判断式（Predicate）

    Predicate（判断式）是一种特殊的辅助函数。所谓predicate，它会返回布尔值（Boolean），常被用来指定作为排序准则或查找准则。Predicate可能有一或两个操作数，视具体情形而定。
    注意，并非任何返回布尔值的单参函数或双参函数都是合法的predicate。STL要求，面对相同的值，predicate必须得出相同结果。这条戒律使得那些“被调用时会改变自己内部状态”的函数出局。详见10.1.4节第483页。 

Unary Predicate（单参判断式）

    Unary predicate会检查唯一实参的某项特性。典型例子如下，用来查找第一个质数：
#include <list>
#include <algorithm>
#include <iostream>
#include <cstdlib>      // for abs()
using namespace std;

// predicate, which returns whether an integer is a prime number
bool isPrime (int number)
{
    // ignore negative sign
    number = abs(number);

    // 0 and 1 are no prime numbers
    if (number == 0 || number == 1) {
        return false;
    }
        
    // find divisor that divides without a remainder
    int divisor;
    for (divisor = number/2; number%divisor != 0; --divisor) {
        ;
    }

    // if no divisor greater than 1 is found, it is a prime number
    return divisor == 1;
}

int main()
{
    list<int> coll;

    // insert elements from 24 to 30
    for (int i=24; i<=30; ++i) {
        coll.push_back(i);
    }

    // search for prime number
    auto pos = find_if (coll.cbegin(), coll.cend(),  // range
                        isPrime);                    // predicate
    if (pos != coll.end()) {
        // found
        cout << *pos << " is first prime number found" << endl;
    }
    else {
        // not found
        cout << "no prime number found" << endl;
    }
}
    在这个例子中，算法find_if()在给定区间内寻找使“传入之单参判断式（unary predicate）”运算结果为true的第一个元素。本例中的predicate是isPrime()函数，它会检查某数是否为质数。通过它，这个算法可以返回给定区间内的第一个质数。
    如果没有任何元素能够匹配这个（质数）条件，find_if()算法就返回区间终点（也就是第二实参)。本例 中24到30之间确实存在一个质数，所以程序输出：    
        29 isfirst prime number found
Binary Predicate（双参判断式）
    Binary predicate的典型用途是，比较两个实参的特定属性。例如，为了以你自己的原则对元 排序，你必须以一个简单的predicate形式提供这项原则。如果元素本身不支持operator <， 或如果你想使用不同的排序原则，这就派上用场了。 
    下面这个例子，根据每个人的姓名，对一组元素进行排序：
#include "stdafx.h"
#include <algorithm>
#include <deque>
#include <string>
#include <iostream>

using namespace std;

class Person
{
public:
	string firstname() const;		//const 成员函数，意味着无法修改函数内成员，而const对象必须调用const成员函数
	string lastname() const;
	
};

//binary function predicate:
//-return whether a person is less than another person
bool personSortCriterion(const Person& p1, const Person& p2)
{
	//a person is less than another person
	//- if the last name is less
	//-if the last name is equal and the first name is less
	return p1.lastname() < p2.lastname() ||
		(p1.lastname() == p2.lastname() &&
			p1.firstname() < p2.firstname() < p2.firstname());
}
int main()
{
	deque<Person> coll;
	
	sort(coll.begin(), coll.end(),		//range
		personSortCriterion);			//sort citerion
}

    注意，你也可以使用函数对象（function object）来实现一个排序准则。这种做法的优点是，制作出来的准则将是个类型（type），可用来作为“声明一个以某种类型为排序准则的set”之类的事情。
    

 









