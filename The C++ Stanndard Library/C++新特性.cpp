C++11语言新特性

Template表达式内的空格
    “在两个template表达式的闭符之间放一个空格“的要求已经过时了：
    vector<list<int> >;  //ok in each C++ version
    vector<list<int>>;   //ok since C++11

nullpter和std::nullptr_t
C++11允许你使用nullptr取代0或NULL，用来表示一个pointer（指针）指向所谓的no value
（此不同于拥有一个不确定值。）这个新特性特别能够帮助你在“nullpointer 被解释为一个整数值”
时避免误解。例如：
void f(int);
void f(void*);

f(0);               //calls f(int)
f(NULL);            //calls f(int) if NULL is 0,ambiguous otherwise
f(nullptr);         //calls f(void*)
nullptr是个新关键字。它被自动转换为各种pointer类型，但不会被转换为任何整数类型。
它拥有类型std::nullptr_t,定义与<cstddef>，所以你现在甚至可以重载函数令他们接受nullpointer。
注意，std::nullptr_t被视为一个基础类型。


以auto完成类型自动推导
auto i = 42;
这是个推导而来的过程，因此一定需要一个初始化操作:
auto i;             //ERROR:can't deduce the type of i

//auto 是C语言的一个就关键字。和static一样用来声明某变量为局部的（local）。但它从未被真正的用过，因为
//“不指明某物为static”也就隐含地声明了它是个auto。


一致性初始化（Uniform Initialization）与初值列（Initializer List）
在C++11之前，程序猿，特别是初学者，很容易被这个问题混淆：如何初始化一个变量或对象。
初始化可因为小括号、大括号或赋值操作符（assignment operator）的出现而发生。
    为此C++11引入了“一致性初始化“（uniform initialization）概念，意思是面的任何初始化动作，
    你可以使用相同元，也就是使用大括号。以下皆成立：
    int values[] {1,2,3};
    int value {1};
    std::vector<int> v{2,3,5,7,11,13,17};
    std::vector<std::string> cities{
        "Berlin","New York","London","Braunschweig","Cairo","Cologne"
    };
    std::complex<double> c{4.0,3.0};    //equivalent to c(4.0,3.0)
初值列（initializer list）会强迫造成所谓value initialization，意思是即使某个local变量属于
某种基础类型（那通常会有不明确的初值）也会被初始化为0（或nullptr--如果它是个pointer的话）：
    int i;      //i has undefined value
    int j{};    //j is initialized by 0
    int* p;     //p has undefined value
    int* q{};   //q is initialized by nullptr
然而请注意，窄化（narrowing）--也就是经度降低或造成数值变动--对大括号而言是不可成立的。例如：
    int x1(5.3);        //OK,but OUCH: x1 becomes 5
    int x2 = 5.3;       //OK,but OUCH: x2 becomes 5
    int x3{5.0};        //ERROR: narrowing
    int x4 = {5.3};     //ERROR: narrowing
    char c1{7};         //OK: even though 7 is an int,this is not narrowing
    char c2{99999};     //ERROR: narrowing(if99999 doesn't fit into a char)
    std::vector<int> v1{1,2,4,5};       //OK
    std::vector<int> v2{1,2.3,4,5.6};   //ERROR:narrowing doubles to ints
判定是否窄化转换时，C++11用避免许多不兼容性的做法是，依赖初值设定（initializer）的实际值
（actual value，如上例的7）而非只是依赖类型。如果一个值可被标的类型（target type）精确表述，其间的转换就不算窄化。
浮点数转换至整数，永远是一种窄化--即使是7.0转为7。

为了支持“用户自定义类型之初值列“(initializer lists for user-defined types)概念，
C++11提供了class template std::initializer_list<>，用来支持一系列值（a list of values）
进行初始化，或在“你想要处理一系列值（a list of values）”的任何地点进行初始化。例如:
void print(std::initializer_list<int> vals)
{
    for(auto p = vals.begin(); p!=vals.end(); ++p){ //process a list of values
        std::cout<<*p<<"\n";
    }
}

print({12,3,5,7,11,13,17}); //pass a list of values to print()
当“指明实参个数”和“指明一个初值列”的构造函数（ctor）同时存在，带有初值列的那个版本胜出:
class p
{
    public:
        P(int,int);
        P(std::initializer_list<int>);
};

P p(77,5);          //calls P::P(int,int)
P q{77,5};          //calls P::P(initializer_list)
P r{77,5,42};       //calls P::P(initializer_list)
P s = {77,5};       //calls P::P(initializer_list)
如果上述“带有一个初值列”的构造函数不存在，那么接受两个int的那个构造函数会被调用以初始化
q和s，而r的初始化将无效。
由于初值列的关系，explicit之于“接受一个以上实参”的构造函数也变得关系重大。
如今你可以令“多数值自动类型转换”不再起作用，即使初始化以 = 语法进行。
//explicit可以阻止不应该允许的经过转换构造函数进行的隐式转换的发生。声明为explicit的构造函数不能在隐式转换中使用。
class P
{
    public:
        P(int a,int b)
        {
            ...
        }
        explicit P(int a,int b,int c)
        {
            ...
        }
};

P x(77,5);          //OK
P y {77,5};         //OK
P z {777,5,42};     //OK
P v = {77,5};       //OK(implicit type conversion allowed)允许隐式转换
P w = {77,5,42};    //ERROR due to explicit (no imlicit type conversion allowed)如果去掉explicit则成功运行。

void fp(const P&);

fp({47,11});        //OK,implicit conversion of {47,11} into P
fp({47,11,3});      //ERROR due to explicit    
fp(P{47,11});       //OK,explicit conversion of {47,11} into P
fp(P{47,11,3});     //OK,explicit conversion of {47,11,3} into P 显式声明不存在隐式转换
同样地，explicit构造函数如果接受的是个初值列，会失去“初值列带有0个、1个或多个初值“的隐式转换能力。

Range-Based for 循环

C++11引入了一种崭新的for循环形式，可以逐一迭代某个给定的区间、数组、集合(range,array,or collection)
内的每一个元素。其他编程语言可能称次为foreach循环。其一般性语法如下:
for(decl : coll)
{
    statement
}
其中decl是给定之coll集合中的每个元素的声明：针对这些元素，给定的statement会被执行。例如下面针对传入的初值
列中的每个元素，调用给定的语句，于是在标准输出装置cout输出元素值：
for (int i : {2,3,5,7,9,13,17,19})
{
    std::cout<<i<<std::endl;
}
//输出2\n-19\n
如果要将vector vec的每个元素elem乘以3，可以这么做：
    std ::vector<double> vec;
    ...
    for (autok& elem : vec)
    {
        elem *= 3;
    }
这里“声明elem为一个reference”很重要，若不这么做，for循环中的语句会作用在元素的一份local copy身上（当然
或许有时候你想要这样）。
    这意味着，为了避免调用每个元素的copy构造函数和析构函数，你通常应该声明当前元素(current element)为一个
const reference。于是一个用来“打印某集合内所有元素”的泛型函数应该写成这样：
template <typename T>
void printElement(const T& coll)
{
    for(const auto& elem : coll)
    {
        std::cout<<elem<<std::endl;
    }
}
在这里，这种所谓range-based for 语句等同于：
{
    for (auto _pos = coll.begin();_pos != coll.end(); ++_pos)
    {
        const auto& elem = *_pos;
        std::cout<<elem<<std::endl;
    }
}
一般而言，如果coll提供成员函数begin()和end()，那么一个range-based for 循环声明为
for (decl : coll)
{
    statement
}
便等同于
{
    for(auto _pos = coll.begin(),_end = coll.end();_pos!=_end;++pos)
    {
        decl = *_pos;
        statement
    }
}
或者如果不满足上述条件，那么也等同于一下使用一个全局性begin()和end()且两者都接受coll为实参:
{
    for(auto _pos = begin(coll),_end= end(coll);_pos!=_end;++_pos)
    {
        decl = *_pos;
        statement
    }
}
于是，你甚至可针对初值列(initializer list)使用range-based for 循环，因为class template
std::initializer_list<>提供了成员函数begin()和end（）。
    此外还有一条规则，允许你使用寻常的、大小一致的C-style array，例如:
    int array[] = {1,2,3,4,5};

    long sum = 0;       //process sum of all elements
    for (int x : array)
    {
        sum += x;
    }

    for(auto elem : {sum,sum*2,sum*4})
    {//print some multipes of sum
        std::cout<<elem<<std::endl;
    }
输出：
    15
    30
    60

Move语言和Rvalue Reference
//节省空间和提升性能的一个很好的方法
C++11的一个最重要的特性就是，支持move semantic（搬迁语义）。这项特性更进一步进入了C++
主要设计目标内，用以避免非必要拷贝（copy）和临时对象（temporary）。
    void createAndInsert(std::set<X>& coll)
    {
        X x;    //creat an object of type X
        ...
        coll.insert(x); //insert it into the passend collection
    }
在这里我们将新对象插入集合（collection）中，后者提供一个成员函数可为传入的元素建立一份内部拷贝（internal copy）:
namespace std{
    template <typename T,...> class set {
        public:
         ...insert (const T& v);    //copy value of v
         ...
    };
}
这样的行为是有用的，因为集合（collection）提供value semantic及安插“临时对象”（temmporay object）或
“安插后会被使用或被改动的对象”的能力。
X x;
coll.insert(x);         //inserts copy of x                                     插入x
...
coll.insert(x+x);       //inserts copy of temporary rvalue                      插入拷贝临时右值
...
coll.insert(x);         //inserts copy of x(although x is not used any longer)  重复添加

然而，对于后两次x安插动作，更好的是指出“被传入值（特就是x+x的和以及x）不再被调用者使用”，
如此一来coll内部就无须为它建立一份copy且“某种方式move其内容进入新建元素中”。当x的复制成本高昂的时候--例如
它是个巨大的string集合--这会带来巨大的效能提高。
    自C++11起，上述行为成为可能，然而程序猿必须自行指明“move是可行的，除非被安插的那个临时对象还会被使用”。
虽然编译器自身也有可能找出这个情况（在某些浅薄无奇的情况下），允许程序猿执行这项工作毕竟可使这个特性被用于
逻辑上任何适当之处。先前的代码只需简单改成这样：
X x;
coll.insert(x);         //insert copy of x(OK,x is still used)
...
coll.insert(x+x);       //moves(or copies) contents of temporary rvalue
...
coll.insert(std::move(x));      //moves(or copies) contents of x into coll

有了声明于<utility>的std::move()，x可被moved而不再被copied。然而std::move()自身并不做任何moving工作，
它只是将其实参转成一个所谓的 rvalue reference，那是一种被声明为X&&的类型。这种新类型主张rvalue（不具名的
临时对象只能出现于复制操作的右侧）可被改动内容。这份契约的含义是，这是个不再被需要的（临时）对象，所以你可以
“偷”其内容和/或其资源。
    现在，我们让集合（collection）提供一个insert()重载版本，用以处理这些rvalue reference:
    namespace std{
        template<typename T,...>class set{
            public:
             ...insert(const T& x);     //for lvalues: copies the value
             ...insert(T&& x);          //for rvalues: moves the value
             ...
        };
    }
我们可以优化这个针对rvalue reference的重载版本，令它“偷取”x的内容。为了这么做，需要type of x的帮助，
因为只有type of x拥有接近本质的机会和权利。举个例子，你可以运用internal array和pointer of x来初始化备案差的元素。
如果class x 本身是个复杂类型，原本你必须为它注意赋值元素，现在这么做则会带来巨大的效能改善。欲初始化新的内部元素，
我们只需调用class X提供的一个所谓move构造函数，它“偷取”传入的实参值，初始化一个新的元素。所有复杂类型都应该提供这样一个
特殊构造函数---C++ 标准库也提供了---用来将一个既有元素的内容搬迁（move）到新元素中:
class X{
    public:
    X(const X& lvalue);            //copy constructor
    X(X&& rvalue);                 //move constructor
    ...
};










