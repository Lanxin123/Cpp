Pair 和 Tuple

    C++标准库的第1版，C++98,提供了一个简单的class,用来处理类型不同的两个（一对)值，而不需为它们再定义特别的class。当标准函数需要返回一个value pair，或者当容器元素是个key/value pair时，就会用上这样一个C++98 class。 
    TR1引入了一个tuple class,它将上述概念延伸，接受任意（但仍有限）个元素。标准 库实现保证可移植的范围是：允许tuple带有最多10个类型不同的元素。 
    到了 C++11，tuple class 被重新实现，采用 variadic template 概念。现在我们有了一个标准的tuple class，可用于任意大小的异质集合（heterogeneous collection)。而与此同时，class pair仍旧为两个元素服务，用来组合一个双元素的tuple。 
    不过，C++11的pair class也有很大扩展，某种程度上展示了C++语言及其标准库的提升。

Pair

    Class pair可将两个value视为一个单元。C++标准库内多处用到了这个class。尤其容器 map、multimap , unordered_map 和 unordered_multimap 就是使用 pair 来管理其以 key/value pair形式存在的元素。
    任何函数如果需返回两个value,也需要用到pair，例如minmax()。 
    Struct pair定义于<utility>，提供表5.1所列的各项操作。原则上你可以对pair<>执行 create、copy/assign/swap 及 compare 操作。此外它还提供 first_type 和 second_type 类型定义式，用来表示第一value和第二value的类型。

元素访问

    为了让程序能够处理pair的两个值，它提供了 “直接访问对应数据成员”的能力。事实上由于它是个struct而不是class,以至于所有成员都是public: 
    namespace std { 
    template <typename T1, typename T2> 
    struct pair { 
        //member 
        T1 first; 
        T2 second;
        ...
        };
    };
    举个例子，欲实现一个泛型函数模板（generic function template) , 用以将一个value pair写 入一个stream内，你必须这么做:
    //generic output operator for pairs(limited solution) 
    template <typename Tl, typename T2> 
    std::ostream& operator << (std::ostream& strm, 
                                const std::pair<Tl,T2>& p)  
    {
        return strm<<"["<<p.first<<","<<p.second<<"]";
    }
    另外，自C++11起，你可以对pair使用一份tuple-like接口。因此，你可以使用tuple_size<>::value获得元素个数，
    使用tuple_element<>::type获得某指定元素类型，也可以使用get()获得first或second:
    
    
	typedef std::pair<int, float> IntFlotPair;
	IntFlotPair p(42, 3.14);

	std::get<0>(p);										//yields p.first
	std::get<1>(p);										//yields p.second
	std::tuple_size<IntFlotPair>::value;				//yields 2
	std::tuple_element<0, IntFlotPair>::type;		    //yields int
    
构造函数和赋值  

    Default构造函数生成一个pair时，以两个“被default构造函数个别初始化”的元素作为初值。根据语言规则，基础类型（如 int）的
    default构造函数也可以引起适当的初始化动作，所以：
        std::pair<int,float〉p； //initializep f irstandp.second withzero 
    
    就是以int()和float()来初始化p。这两个构造函数都传回零值。3.2.1节第37页曾经讨论过基础类型的显式初始化动作。 
    Copy构造函数同时存在两个版本，版本1接受相同类型的pair，版本2是个member template,在“构造过程中需要隐式类型转换”时被调用。
    如果pair对象被复制，调用的是被隐式合成的那个copy构造函数。例如： 
    void f(std::pair<int,const char*>);
    void g(std::pair<const int,std::string>); 
    ...
    void foo() { 
        std::pair<int,const char*> p(42,"hello"); 
        f (p);   // OK: calls implicitly generated copy constructor 
        g (p);    // OK: calls template constructor          
    }
    自C++11开始，如果pair<>用到了某个类型而该类型只有一个nonconstant copy构造函数， 将不再编译成功：
        class A 
        {
            public: 
            ...
            A (A&) ； //copyconstructorwithnonconstantreference
            ...
        }；
        std::pair<A,int> p;     //Error since C++11
    自C++11起，assignment操作符也以member template形式出现，使隐式类型转换得以进行。此外也支持move semantic---搬移first和second元素。

逐块式构造（Piecewise Construction）
    Class pair<>提供三个构造函数，用以初始化first和second成员： 
    namespace std { 
        template <typename Tl, typename T2> 
        struct pair { 
            ...
            pair(const T1& x, const T2& y)； 
            template<typename U, typename V> pair(U&& x, V&& y)； 
            template<typename... Args1, typename... Args2> 
                pair(piecewise_construct_t, 
                    tuple<Argsl...> first_args, 
                    tuple<Args2...> second_args)；
            ...
        };
    }
    前两个构造函数提供的是惯常行为：传递一个实参给first,另一个实参给second,并且涵盖对move semantic和隐式类型转换的支持。第三个构造函数有点特别，它允许传递两个tuple (那是一种“拥有不定个数之元素&元素类型各不相同”的对象），
    但以另一种方式处理它们。正常而言，如果传递1或2个tuple,最前面两个 构造函数允许初始化一个pair，其first和/或second是tuple。但第三个构造函数使用 tuple,将其元素传递给:first和second的构造函数。为了强迫执行这样的行为，你必须传递std::piecewise_construct作为额外的第一实参。举个例子： 

#include "stdafx.h"
#include <iostream>
#include <utility>
#include <tuple>


using namespace std;

class Foo
{
public:
	Foo(tuple<int, float>)
	{
		cout << "Foo::Foo(tuple)" << endl;
	}
	template<typename... Args>
	Foo(Args... args)
	{
		cout << "Foo::Foo(args...)" << endl;
	}
};

int main()
{
	//create tuple t:
	tuple<int, float> t(1, 2.22);

	//pass the  tuple as a whole to the constructor of Foo:
	pair<int, Foo> p1(42, t);

	//pass the elements of the tuple to the constructor of Foo:
	pair<int, Foo> p2(piecewise_construct, make_tuple(42), t);

	return 0;
}


    只有当std::piecewise_construct被当作第一实参，class Foo才会被迫使用那个“接受tuple的元素(一个int和一个float)而非tuple整体”的构造函数。这意味着，此例中被调用的是Foo那个“实参数量不定（varargs)的构造函数”。如果提供了 Foo::Foo(int,float)的话，被调用的将是它。 
    如你所见，两个实参都必须是tuple才会强迫导致这个行为。因此，第一实参42被显式转换为一个tuple,用的是make_tuple()(你也可以改传std: ：tuple(42) )。 
    注意，这种初始化形式的必要性发生在当我们需要安放（emplace())一个新元素到 (unordered) map 或 multimap中时。

便捷函数make_pair() 

    Template函数make _pair ()使你无须写出类型就能生成一个pair对象。举个例子，你不必这样写： 
        std：pair<int,char>(42,'@') 
    可以写成这样： 
        std: :make_pair(42,'@')
    在C++11之前，这个函数的声明和定义如T: 
    namespace std { 
        //create value pair only by providing the values 
        template <template Tl, template T2> 
        pair<Tl,T2> make_pair (const T1& x, const T2& y) { 
        return pair<Tl,T2>(x,y); 
        }
    }
    然而自C++11起，事情变得比较复杂，因为这个class也需要应付move semantic。所以自 C++1丨起，OH•松准库宣称maktpairO声明如下： 
    namespace std { 
        //create value pair only by providing the values 
        template <template Tl, template T2> 
        pair<Vl,V2> make_pair (T1&& x, T2t&& y)； 
    }
    其中返回值的细节和它们的类型V1和V2,取决于x和y的类型。无须深入细节，C++ standard 明确指出，如来可能的话make_pair()使用move语义，否则就使用copy语义。此外它会蛀蚀(decay) 实参 ，致使make_pair("a" , "xy")产出一个 pair<const char*,const char*> 而非一个 pair<const char [2] ,const char [3] > 
    当我们必须对一个“接受pair为实参”的函数传递两个value时，make_pair()尤显方便，请看下例： 
    void f(std::pair<int,const char*>)； 
    void g(std::pair<const int,std::string>)； 
    ...
    void foo() { 
     f(std::make_pair (42, "empty")) ；//pass two values as pair 
     g(std::make_pair (42, "chair")) ; //pass two values as pair with type conversions 
    }
    从本例可以看出，make_pair() 即使在类型并不准确吻合的情况下也能借由template构造函数提供的支持顺利运作。当你使用map和muiltimap时，经常需要这样的能力。     
    注意，自C++11开始，你也可以使用初值列(initializer list)： 
    f({42,"Mempty"})； //pass two valuesaspair 
    g({42,"chair"}) ；  //pass nvo valuesaspairwithtypeconversions
    然而一个表达式如果明白指出类型，便带有一个优势：产生出来的pair将有绝对明确的类型。例如： 
        std:：pair<int,float>(42,7.77)
    所得结果不同于
        std::make_pair(42,7.77);
    后者所生成的pair的第二元素的类型是double (因为“无任何限定符的浮点字面常量”的类型被视为 double)。当我们使用重载函数（overloaded function)或template时，确切的类型非常重要。例如，为了提高效能，程序员可能同时提供分别针对float和double的重载函数（overloaded function）或 template,这时候确切的类型就非常重要了。 
    面对C++11新语义，你可以借由强迫使用move semantic或reference semantic来影响 make_pair()的产出类型。如果你的选择是move semantic,只需使用std::move()声明被传递的实参不再使用： 
    std::string s, t； 
    ...
    auto p = std::make_pair(std::move(s)，std::move(t))； 
    ...// s and t are no longerused 
    如果你的选择是reference semantic，就必须使用 ref(),那会强迫形成一个reference 类型，或使用cref()强迫形成一个constant reference类型（二者都由<functional>提供）。
    下面的语句中，有个pair指向某个int两次，因此最终i的值是2:

#include "stdafx.h"
#include <iostream>
#include <utility>
#include <functional>


using namespace std;


int main()
{
	int i = 0;                                                  
	auto p = std::make_pair(std::ref(i), std::ref(i));  //creates pair<int&,int&>
	++p.first;                                          //increments i
	++p.second;                                         //pairs i:2
	std::cout << "i: " << i << std::endl;
	return 0;
}

    自C++11起，你也可以使用定义于<tuple>内的tie()接口，抽出pair的value:
#include "stdafx.h"
#include <iostream>
#include <utility>
#include <tuple>


int main()
{
	std::pair<char, char> p = std::make_pair('x', 'y');	//pair of two chars

	char c;
	std::tie(std::ignore, c) = p;   //extract second value into c(ignore first one)

	return 0;
}

    事实上这里的pair p被赋值给一个tuple，后者的第二value是个reference，指向c。


Pair运用实例 
    C++标准库大量使用pair。例如（unordered)map和multimap容器的素类型便是pair,也就是一对key/value。map和multimap的一般性描述见7.8节第331页。6.2.2节第179页有 一个pair类型的运用实例。 
    C++标准库中凡“必须返回两个value” 的函数也都使用pair对象。

Tuple(不定数的值组)

    Tuple是TR1引入的东西，它扩展了pair的概念，拥有任意数量的元素。也就是说，tuple 呈现出一个异质元素列(heterogeneous list of elements),其中每个类型都可以被指定，或来自编译期推导。 
    然而，由干TR1用的是C++98 语言特性，也就不可能定义出一个“参数个数不定”的 template。基于这个原因，实现必须具体指明“一个tuple吋拥有的”所有可能的元素个数。 TR1对此的建议是至少10个实参，这意味着tuple往往被定义如下，甚至某賤实现会提供 更多的template参数： 
    template <typename TO = ..., typename Tl =…，typename T2 =..., 
            typename T3 =     typename T4 = ..., typename T5 =..., 
            typename T6 = ..., typename T7 = ..., typename T8 =..., 
            typename T9 =...> 
    class tuple； 
    这显示，class tuple拥有至少10个类型各异的template参数，每个都带有实现赋予的默认类型。未用到的tuple元素也有个默认类型，但没有作用。这实际上就是variadic template的仿效品，只不过又累赘又有限制。 
    来到C++11，variadic template被引入进来，使template得以接受任何数读的template实参。于是，出现在<tuple>中的class tuple声明式现在就被简化如下： 
    namespace std { 
        template <typename... Types> 
        class tuple；
    } 

Tuple的操作

    原则上，tuple掊口十分直观： 
    •通过明白的声明，或使用便捷函数make_tuple(),你可以创建一个tuple。 
    •通过 get<>() function template,你可以访问tuple的元素。 
    下面是其接口的一个基本示例：












