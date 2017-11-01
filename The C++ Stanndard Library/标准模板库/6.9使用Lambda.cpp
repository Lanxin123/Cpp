使用 Lambda

    Lambda始自C++11，是一种“在表达式或语句内指明函数行为”的定义式（见3.1.10节第 28页）。这导致你可以定义对象，用以描述函数行为，并将这些对象以“inline 实参”形式传给算法作为predicate,或是作为其他用途。 
    例如下面这个语句： 
    //transform all elements to the power of 3
    std::transform (coll.begin(),coll.end(),    //source
                    coll.begin(),               //destination
                    [](double d){               //lambda as function object
                        return d*d*d;
                    });
    其中的表达式
     [](double d){return d*d*d;}
    就定义了一个lambda，表述一个函数对象（function object），返回某个double的三次方值。如你所见，这提供了一种能力可以直接描述“传给transform()”的某种函数行为。

Lambda的好处
    运用lambda具体指定“STL框架内部将采取的行为”，可以解决先前存在的许多缺点。假设你想要查找某集合内“数值在 x 和 y 之间”的第一个元素：
#include "stdafx.h"
#include <algorithm>
#include <deque>
#include <iostream>
using namespace std;

int main()
{
	deque<int> coll = { 1, 3, 19, 5, 13, 7, 11, 2, 17 };

	int x = 5;
	int y = 12;
	auto pos = find_if(coll.cbegin(), coll.cend(),  // range
		[=](int i) {                 // search criterion
		return i > x && i < y;
	});
	cout << "first elem >5 and <12: " << *pos << endl;
}
    当你调用find_if()，便以inline形式传递相应的predicate作为find_if()的第三实参：
        auto pos = find_if(coll.cbegin(),coll.cend(),
                           [=](int i){
                               return i > x && i < y;
                           });
    Lambda其实是个函数对象，此处它将接受一个整数i，返回“i是否大于x且小于y”：
        [=](int i){
            return i > x && i < y;
        }
    借由“在[=]内指明：作为你所表达的意思是，“在lambda被声明时已有效”的所有符号都以by value形式传进lambda体内。于是在这个lambda内部，你拥有对“main() 内部声明的变量x和y” 的读取权。
    如果改用[&]作为capture,那就是以by reference方式 传递value,致使lambda内得以改动它们的value （3.1.10节第29页对capture有更详细 的介绍）。 
    现在，拿这种做法比较“引入lambda” 前的做法（见下），我们同样查找“第一个>5且<12的元素”。 
    相比于手写的循环：
        //find first element > x and < y
        vector<int> :: iterator pos;
        for(pos = coll.begin();pos != coll.end();++pos){
            if(*pos > x && pos < y){
                break;// the loop
            }
        }
    你将得以使用预定义的算法，并且避免出现一个多少有点丑陋的break。
    相比于一个半手写函数，也就是一个predicate：
        bool pred(int i)
        {
            return i > x && i < y;
        }
        ...
        pos = find_if(coll.begin().coll.end(),      //range 
                       pred);                       //search cirterion
    行为细节被写于另外某处，因此你必须在屏幕上来回滚找出find_if()究竟真正找些什么（因为只有pred定义式才能提供答案），除非你拥有pred的说明文件和注释并且投以完全的信任。如果使用lambda，你就不会遭遇这种困扰。
    此外，C++编译器对lambda的优化效果高于它们对普通函数的优化效果。
    此情节中更难堪的是，对x和y的访问动作变得十分丑陋。C++11之前的常见做法是，使用一个函数对象（见6.10节第233页），那恰恰完完全全展示了我所谓的丑陋:
    class Pred
    {
        private:
            int x;
            int y;
        public:
            Pred(int xx,int yy) : x(xx),y(yy){

            }
            bool operator()(int i)const {
                return i > x && i < y;
            }
    };
    ...
    pos = find_if(coll.begin(),coll.end(),          //range
                 Pred(x,y));                        //search criterion                
    相比于使用binder（见6.10.3节第233页：
    pos = find_if(coll.begin(),coll.end(),          //range
                  bind(logical_and<bool>(),         //search cirterion
                  bin(greater<int>(),_1,x),
                  bind(less<int>(),_1,y)));
    如果采用lambda，你就不需要了解上述表达式，毕竟那语法还是蛮艰难的。
    扼要地说，lambda为“运用STL算法”提供了最高级、第一等的方便性、可读性、快速性和可维护性。

以Lambda作为排序准则（Sorting Criterion）

    再举个例子，使用lambda定义一个排序准则，用来对一个“由Person构成的vector”排序（6.8.2节第228页也一个程序，乃是以函数定义出排序准则）。

#include "stdafx.h"
#include <algorithm>
#include <deque>
#include <iostream>
#include <iostream>
using namespace std;

class Person {
public:
	string firstname() const;
	string lastname() const;
};

int mian()
{
	deque<Person> coll;

	//sort Persons according to lastname(and firstname):
	sort(coll.begin(), coll.end(),
		[](const Person& p1, const Person& p2) {
		return p1.lastname() < p2.lastname() ||
			(p1.lastname() == p2.lastname() &&
				p1.firstname() < p2.firstname());
	});

}

Lambda的局限
    
    Lambda并非在每一方面都保持优势。让我们考虑使用lambda为关联式容器指出一个排序准则：
        auto com = [](const Person& p1, const Person& p2) {
		    return p1.lastname() < p2.lastname() ||
			    (p1.lastname() == p2.lastname() &&
				    p1.firstname() < p2.firstname());
        };
    ...
    std::set<Person,decltype(cpm)> coll(cmp);
    由干set声明式需要指明lambda类型，所以我们必须使用decltype（见3.1.11节第32 页），它会为一个lambda对象（上例的cmp)产出类型。注意，你也必须把lambda对象传给coll的构造函数，否则coll会调用被传入的徘序准则的default构造函数，
    而根据C++语言规则，lambda没有default构造函数也没有assignment操作符。基于这些局限，“以class定义某个函数对象作为排序准则”说不定还比较直观些。
    Lambda的另一个问题是，它们无法拥有“跨越多次调用”都能被保存下来的内部状态 (internal state）。 如果你需要这样的状态，必须在外围作用域中声明一个对象或变量，将它(搭配某种capture)以by-reference方式传lambda、与此相比，
    函数对象允许你封装内部状态（10.3.2节第500觅有更多细节和范例)。 
    尽管如此，你还是可以使用lambda来为无序（unordered）容器指出一个hash函数或一个等效准则。7.9.7节第379页有个例子。


    






















