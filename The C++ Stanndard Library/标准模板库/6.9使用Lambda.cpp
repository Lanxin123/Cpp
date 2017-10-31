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

函数对象（Function Object) 

    传递给算法的“函数型实参”（functional argument）不一定得是函数，可以是行为类似函数的对象。这种对象称为函数对象（function object）, 或称为仿函数（functor）。

定义一个函数对象

    函数对象是泛型编程（generic programming） 强大威力和纯粹抽象概念的又一个例证。你可 以说，任何东西，只要其行为像函数，它就是个函数。因此如果你定义了一个对象，行为像函数，它就可以被当作函数使用。 
    好，那么，什么才算是具备函数行为（也就是行为像个函数）？所谓函数行为，是指可以“使用小括号传递实参，借以调用某个东西”。例如： 
        function (argl, arg2) ；  //a function call 
    如果你指望对象也可以如此这般，就必须让它们也有可能被“调用”---通过小括号的运用和实参的传递。没错，这是可能的（在C++中很少有什么是不可能的）。你只需定义operator ()，并给予合适的参数类型：
    Class X{
        public:
            //define "function call" operator:
            return-value operator()(arguments) const;
            ...
    };

    现在，你可以把这个class的对象当做函数来调用了：
        X fo;
        ...
        fo(arg1,arg2);              //call operator() for function object fo
    上述调用等同于：   
        fo.operator()(arg1,arg2);   //call operator() for function object fo
    下面是个完整例子，是先前6.8.1节第224页范例的改版（改用函数对象），其行为和使用寻常函数（而非函数对象）完全相同：

#include "stdafx.h"
#include<vector>
#include<algorithm>
#include <iostream>

using namespace std;

//simple function object that prints  the passed argument
class PrintInt {
public:
	void operator()(int elem) const {       
		cout << elem << ' ';
	}
};

int main()
{
	vector<int> coll;

	//insert elements from 1 to 9
	for (int i = 1;i<=9;++i)
	{
		coll.push_back(i);
	}

	//print all elements
	for_each(coll.cbegin(), coll.cend(),		//range
		PrintInt());							//operation
	cout << endl;
}

    Class PrintInt 的定义显示，你可以对其对象调用operator()并传入一个int实参。在语句
        for_each(coll.cbegin(), coll.cend(),	
		PrintInt());
    中的表达式
        PrintInt()
    创建出一个临时对象作为for_each()算法的一个实参。for_each()算法大值如下：
        namespace std{
            template<typename Iterator,typename operator>
            Operation for_each(Iterator act,Iterator end,Operation op)
            {
                while(act != end){      //as long as not reached the end
                    op(*act);           //-call op() for actual element
                    ++act;              //-move iterator to the next element
                }
                return op;
            }
        }
    for_each()使用临时对象op（一个函数对象），针对每个元素act调用op（*act）。如果第三实参op是个寻常函数，就以*act为实参调用之。如果第三实参是个函数对象，则以*act为实参，调用函数对象op的operator()。因为本例之中for_each()调用：
        PrintInt::operator()(*act) 
    你也许不以为然，也许认为函数对象看起来有点怪异、令人讨厌甚至毫无意义。的确，它们带来更复杂的代码，然而函数对象有其过人之处，比起寻常函数，它们有以下优点： 
    1.函数对象是一种带状态（with state）的函数。“行为像pointer” 的对象我们称之为 “smart pointer”，同样道理，“行为像function”的对象我们奇以称之为“smart function”， 因为它们的能力超越了operator()。函数对象可拥有成员函数和成员变量，
    这意味着函数对象拥有状态（state)。事实上，在同一时间点，相同类型的两个不同的函数对象所表述的相同机能（same functionality）, 可具备不同的状态。这在寻常函数中是不可能的。 另一个好处是，你可以在运行期初始化它们---当然必须在它们被使用（被调用）之前。
 
    2.每个函数对象有其自己的类型。寻常函数，唯有在其签名式（signature） 不同时，才算类型不同。而函数对象即使签名式相同，也可以有不同的类型。事实上由函数对象定义的每一个函数行为都有其自己的类型。这对于“运用template实现泛型编程”乃是一个卓越的贡献，
    因为这么一来我们便可以将函数行为当作template参数来运用。这使得不同类型的容器可以使用同类型的函数对象作为排序准则。也可确保你不会在“排序准则 不同”的集合（collection）间陚值、合并或比较。你甚至可以设计函数对象的继承体系， 
    以此完成某些特别事情，例如在一个总体原则下确立某些特殊情况。 
 
    3.函数对象通常比寻常函数速度快。就template概念而言，由于更多细节在编译期就已确定，所以通常可能进行更好的优化。所以，传入一个函数对象（而非寻常函数）可能获得更好的执行效能。
    
    本小节的剩余部分，我将给出数个例子，展示“函数对象”较之于“寻常函数”的优势所在。第10章专攻函数对象，有更多例子和细节；特别值得一提的是该章为你展示了“以函数行为（functional behavior）作为template参数”这一技术带来的好处。 
    
    假设你需要对集合（collection） 内的每个元素加上一个特定的值。如果你在编译期便确知这个值，可使用寻常函数完成工作： 
        void add10(int& elem)
        {
            elem+-10;
        }
        void f1()
        {
            vector<int> coll;
            ...
            for_each(coll.begin(),coll.end(),       //range
                     add10);                        //operation
        }
    
    如果你需要数个不同的特定值，而它们在编译期都已确定，你可以使用template：
        template<int theValue>
        void add(int& elem)
        {
            elem += theValue;
        }
        void f1()
        {
            vector<int> coll.end(),                 
            ...
            for_each(coll.begin(),coll.end(),       //range
                    add<10>);                       //operation
        }
    如果你得在执行时期处理这个特定值，那就麻烦了。你必须在函数被调用前先将这个数值传给该函数。这通常会导致生成若干全局变量，让“算法的调用者”和“算法所调用的函数”都能看到和用到它们。真是一团糟。
    如果你两次用到该函数，每次用到的特定值不同，而且都是在执行时期才处理那些特定值，那么寻常函数根本无能为力。你要么传入一个标记（tag），要么干脆写两个函数。你是否有过这样的经验：手握一个函数，它有个static变量用以记录状态（state），
    而你需要这个函数在同一时间内有另一个不同的状态（state）？于是你只好拷贝整份函数定义，化为两个不同的函数。这正是先前所说的问题。
    如果使用函数对象，我们就可以写出更具只能（smart）的函数达成目的。对象可以有自己的状态，可以被正确初始化。下面是个完整例子：

        
    






















