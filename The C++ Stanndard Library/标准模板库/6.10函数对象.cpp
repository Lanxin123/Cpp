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

#include "stdafx.h"
#include <list>
#include <algorithm>
#include <iostream>
#include  "print.hpp"

using namespace std;

//function object that adds the value with which it is initialized
class AddValue {

private:
	int theValue;				//the value to add

public:
	//constructor initializes the value to add
	AddValue(int v) : theValue(v) {
	}

	//the "function call" for the element adds the value
	void operator() (int& elem) const {
		elem += theValue;
	}

};

int main()
{
	list<int >  coll;

	//insert elements from 1 to 9
	for (int i = 1;i<=9;++i)
	{
		coll.push_back(i);
	}

	PRINT_ELEMENTS(coll, "initialized:			");

	//add value 10 to each element
	for_each(coll.begin(), coll.end(),				//range
		AddValue(10));										//operation

	PRINT_ELEMENTS(coll, "after adding 10:		");

	//add value of first element to each element
	for_each(coll.begin(), coll.end(),				//range
		AddValue(*coll.begin()));						//operation

	PRINT_ELEMENTS(coll, "after adding first element:	");

}
    初始化之后，集合内含数值1至9：
        initialized:                1 2 3 4 5 6 7 8 9
    第一次调用for_each()，将每个数值加10：
        for_each(coll.begin(),coll.end(),       //range       
                AddValue(10));                  //operation
    这里，表达式AddValue(lO)生出一个AddValue对象，并以10为初值。AddValue构造函数将这个值保存在成员theValue中。而在for_eacti()内部，针对coll的每一个元素调用(),实际上就是针对传入的那个AddValue临时对象调用operator(),并以容器元素作为实参。
    函数对象（AddValue对象）将每个元素加10。结果如下： 
        after adding 10:            11 12 13 14 15 16 17 18 19
    第二次调用for_each()亦采用相同技能，将第一元素值加到每个元素身上。首先使用第一元素值作为函数对象临时对象的初值：
        AddValue(*coll.begin())
    最后结果如下：
        after adding first element: 22 23 24 25 26 27 28 29 30
    11.4节第520页有这个例子的改进版，其中AddValue函数对象的类型被改为一个template，可接纳不同的加数。
    运用此项技术，先前所说“一个函数、两个状态” 的问题就可以用“两个不同的函数对象”解决掉。例如你可以声明两个函数对象，然后各自独立运用：
        AddValue addx(x);       //function object that adds value x
        AddValue addy(y);       //function object that adds value y

        for_each(coll.begin(),coll,end(),       //add value x to each element
                addx);      
        ...
        for_each(coll.begin(),coll.end(),       //add value y to each element
                addy);
        ...
        for_each(coll.begin(),coll.end(),       //add value x to each element
                addx);
    同样道理，你也可以提供一些成员函数，在函数对象生命期内查询或改变对象状态。10.1.3 节第482页有很好的示范。 
    注意，对干某些算法，C++标准库并未明确指出，针对各元素多久调用一次你所给定的函数对象。再者“同一函数对象的不同拷贝被传递给元素”也有可能发生。如果你使用函数对象作为predicate，这将可能造成某些难对付的结果。10.1.4节第483页讨论了这个问题。

预定义的函数对象 

    C++标准库内含若干预定义的函数对象，涵盖了许多基础运算。有了它们，很多时候你就不必费心自己去写函数对象了。一个典型的例子是作为排序准则的函数对象。Operator <之默认排序准则乃是调用less<>,所以，如果你声明： 
        set<int> coll; 
    会被扩展为   
        set<int,less<int>> coll;        //sort elements with <
    既然如此，想必你能猜到，反向排列这些元素将不是什么难事：
        set<int,greater<int>> coll;     //sort element with >
    另一个运用“预定义函数对象”的地点是STL算法。考虑下面这个例子：
#include "stdafx.h"
#include <deque>
#include <functional>
#include <algorithm>
#include <iostream>
#include  "print.hpp"

using namespace std;

int main()
{
	deque<int> coll = { 1,2,3,5,7,11,13,17,19 };

	PRINT_ELEMENTS(coll, "initialized:		");

	//negate all values in coll

	transform(coll.cbegin(), coll.cend(),						//source
		coll.begin(),											//destination
		negate<int>());											//operation
	PRINT_ELEMENTS(coll, "negated:		");

	//square all values in coll
	transform(coll.cbegin(), coll.cend(),						//first source
		coll.cbegin(),											//second source
		coll.begin(),											//destination			
		multiplies<int>());										//operation
	PRINT_ELEMENTS(coll, "squared:		");
}
    首先，预定义的函数对象，其头文件是<functional>： 
        #include <functional> 
    然后，两个预定义的函数对象被用来将coll内的每个元素反相然后平方。先是反相（设负）
        transform (coll.cbegin(), coll.cend(), //source 
                   coll.begin(),               //destination     
                   negate<int>());             //operation
    中，表达式
        negate<int>()
    根据预定义好的class template negate<>生成一个函数对象，将传入的int设为负。transform() 算法使用此一运算，将第一集合内所有元素处理后转移到第二集合。如果转移目的地就是 自己，那么这段代码就是“对集合内的每一个元素设负值”。 
    同样道理，我们使用函数对象multiplies对集合coll内的所有元素求平方值:
        transform (coll.cbegin() , coll. cend() , //first source
                   coll.cbegin(),                 //second source
                   coll.begin(),                  //destination
                   multiplies<int>();             //operation
    这里运用了transform()算法的另一种形式，以某个给定的运算，将两集合内的元素处理后的结果写入第三集合。由于本例的三个集合实际上是同一个，所以其内的每个元素都被计算了平方值，并写进集合内，改写原有值。 
        程序输出如下： 
        initialized: 1 2 3 5 7 11 13 17 19  
        negated：    -1 -2 -3 -5 -7 -11 -13 -17 -19 
        square:      1 4 9 25 49 121 169 289 361

Binder
    你可以使用特殊的（函数适配器），或所谓况binder，将预定义的函数对象和其他数值结合为一体。下面是个完整例子：
#include "stdafx.h"
#include <set>
#include <deque>
#include <algorithm>
#include <iterator>
#include <functional>
#include <iostream>
#include "print.hpp"
using namespace std;
using namespace std::placeholders;

int main()
{
	set<int, greater<int>> coll1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	deque<int> coll2;

	// Note: due to the sorting criterion greater<>() elements have reverse order:
	PRINT_ELEMENTS(coll1, "initialized: ");

	// transform all elements into coll2 by multiplying them with 10
	transform(coll1.cbegin(), coll1.cend(),      // source
		back_inserter(coll2),             // destination
		bind(multiplies<int>(), _1, 10));   // operation
	PRINT_ELEMENTS(coll2, "transformed: ");

	// replace value equal to 70 with 42
	replace_if(coll2.begin(), coll2.end(),       // range
		bind(equal_to<int>(), _1, 70),     // replace criterion
		42);                             // new value
	PRINT_ELEMENTS(coll2, "replaced:    ");

	// remove all elements with values between 50 and 80
	coll2.erase(remove_if(coll2.begin(), coll2.end(),
		bind(logical_and<bool>(),
			bind(greater_equal<int>(), _1, 50),
			bind(less_equal<int>(), _1, 80))),
		coll2.end());
	PRINT_ELEMENTS(coll2, "removed:     ");
}
    在这个例子中，语句
        transform(coll1.cbegin(),coll1.cend(),      //source
                   back_inserter(coll2),            //destination
                   bind(multiplies<int>,_1,10));    //operation
    将coll1的所有元素乘以10，然后以安插模式传送到coll2。为了定义相应操作，我们用上了bind()，它允许你借由低层的函数对象和占位符（placeholder）合成高层的函数对象。所谓占位符是“带有前缀下画线”的数值标识符（numeric identifier）。通过指定 
        bind(multiplies<int>(),_1,10) 
    便定义出一个函数对象，会将传入的第一实参乘以10。 
    你也可以使用这样一个函数对象将任何数值乘以10。例如以下语句会把990写至标准输出设备： 
        auto f = bind(multiplies<int>(),_1,10)； 
        cout << f(99) << endl; 
    现在，这样一个函数对象被传给transformO,成为其第四实参，它预期接受一个实参，现实中就是容器内的实际元素。最终结论是transform()为每个元素调用了“乘以10”函数，并将成果安插至co112。这些动作完成后，co112内含“coll1 所有元素乘以10”的结果。
    //_1占位符：占位符就是先占住一个固定的位置，等着你再往里面添加内容的符号。
        replace_if(coll2.begin(),coll2.end(),        //range
                    bind(equal_to<int>(),_1,70),     //replace criterion
                    42);                             //new value
    中，下列表达式，被用作一个判断准则（criterion）,指出哪种元素将被42替换：
        bind(equal_to<int>(),_1,70)
    在这里，bind()调用binary predicate equal_to，并以它所收到的第一参数作为后者的第一实参，以70作为后者的第二实参。于是，如果传入的实参（现实中是coll2的元素）等于70，被bind()点出来的那个函数对象便产生true。
    最终结果便是：整个语句将“等于70” 的每一个数值替换为42。 
    最后一例用的是个binder组合体，其中：
        bind(logical_and<bool>(),
            bind(greater_equal<int>(),_1,50,
            bind(less_equal<int>(),_1,80)))
    最终表现出的是个单参判断式（unary predicate）,拥有参数x，操作意义是x>=50&&x<=80。这个例子告诉你，你可以嵌套使用bind()来描述繁复的判断式（predicate）和函数对象。本例中remove_if()使用上述函数对象移除介于50和80之间的所有数值。
    但事实上remove_if()只是改变元素次序并返回一个逻辑新终点，不像coll2.erase()那样会真正除掉被移除的元素（详见6.7.1节第218页)。 
    程序输出如下：
        initialized: 9 8 7 6 5 4 3 2 1 
        transformed: 90 80 70 60 50 40 30 20 10 
        replaced:    90 80 42 60 50 40 30 20 10 
        removed：     90 42 40 30 20 10 
    注意，占位符（placeholder） 有自己的命名空间：std::placeholders。基于此，程序起始处有一个相应的using指示符，这才能够使用_1或_2作为binder的第一或第二参数。如果 没有使用任何using指示符，最后那个binder组合体就必须被明确地标示出来： 
        std:：bind(std::logical_and<bool>(), 
            std::bind(std::greater_equal<int>(),std::placeholders::_1,50), 
            std::bind(std::less_equal<int>(),std::placeholders::_1,80)) 
    上述编程方式导致所谓的functional composition（机能组合体）。有趣的是，所有函数对象通常被声明为inline。因此，虽然你使用的看起来是个函数，或是个抽象层，却能获得良好的效率。还有另一种定义函数对象的办法。举个例子，欲“针对集合内的每个元素”调用某成员函数，可以这么做： 
            for_each (coll.cbegin() , coll.cend() , //range 
            bind(&Person:: save,_1));    //operation:Person::save (elem) 
    函数对象bind会绑定（结合）一个你所给的成员函数，后者会因每一个元素而彼调用。此处被传递的元素是以占位符（placeholder) _1表现。于是针对集合coll的每个元素，class Person的成员函数save()被调用。当然，只有当这些元素有着Person类型或其派生类型，上述的一切才能有效运作。 
    10.2节第486页更详细列出并讨论了所有预定义的函数对象、函数适配器以及机能组合体（functional composition） 的诸多方面，并且给出了一份说明，告诉你如何写出你自己的函数对象。 
    TR1 之前另有其他binder和adapter用干函数合成：bindlstO、bind2nd()、ptr_fun()、mem_fun() 和mem_fun_ref ()等，但它们已不再被C++11支持，详见10.2.4节第497页。

函数对象 vs.Lambda 
    Lambda是一种隐式的（implicitly）预定义函数对象。正如6.9节第230页所言，lambda通常提供“用以定义使用于STL算法内的函数行为”的更直观做法。此外，lambda应该和函数对象一样快。 
    然而，lambda也有若干缺点： 
    •你无法让如此一个函数对象带有一个隐藏的内部状态（hidden internal state)。所有定义出状态的数据，都由调用端定义，并以一个capture传递之。 
    •如果一个lambda在许多地方被需要，那么“在函数行为被需要处才指明”的优点就有一部分褪色了，你可以为此定义一个lambda并将它陚予一个auto对象（见6.9节第232 页），但这比起“直接定义一个函数对象”是否较具可读性呢？或许这只是与口味有关。















        