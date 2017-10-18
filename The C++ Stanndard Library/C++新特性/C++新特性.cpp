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
    举个例子，string的move构造函数只是将既有的内部字符数组（existing internal character array) 赋予 (assign)新对象，而非建立一个新array然后复制所有元素。
    同样情况也适用于所有集合class:不再为所有元素建立一份拷贝，只需将内部内存（internal memory ) 赋予新对象就行。如果move构造函数不存在，copy构造函数就会被用上。 
    另外，你必须确保对于被传对象（其value被“偷取”）的任何改动---特别是折构---都不至于冲击新对象（如今拥有value)的状态。因此，你往往必须清除被传入的实参的内容，
    例如将nullptr赋值给“指向了容器元素”的成员函数。 
    将“move semantic被调用”的对象的内容清除掉，严格说来并非必要，但不这么做的话会造成整个机制几乎失去用途。事实上，一般而言，C++标准库的class保证了，在一次 move之后，
    对象处于有效但不确定的状态。也就是说，你可以在move之后对它赋予新值， 但当前值是不确定的。STL容器则保证了，被搬移内容者，搬移后其值为空。 
    同样地，任何 nonlrivial class 都该同时提供一个 copy assignment 和一个move assignment 操作符：

    class X{
        public:
            X& operator = (const X& lvalue);//copy assignment operator
            X& operator = (X&& rvalue);     //move assignment operator
            ...
    };
    对于string和集合，上述操作符可以简单交换（swapping)内部内容和资源就好。然而你也应该清除*this的内容，因为这个对象可能持有资源（如lock),因而最好很快释放它们。 
    再强调一次，move semantic并不要求你那么做，但这是C++标准库容器所提供的一种优越质量。
    最后，请注意两个相关议题：（1) rvalue和lvalue reference的重载规则：（2)返回rvalue referencee。

    Rvalue 和 Lvalue Reference 的重载规则 

    Rvalue 和 lvalue 的重载规则(overloading rule)如下
    •如果你只实现 
        void foo(X&)； 
        而没有实现void foo(X&&),行为如同C++98: foo()可因lvalue但不能因rvalue被调用。 
    •如果你实现 
        void foo(const X&)； 
        而没有实现void foo(X&&),行为如同C++98: foo()可因lvalue也可因rvalue被调用。
    •如果你实现 
        void foo(X&)； 
        void foo(X&&)； 
    或 
        void foo(const X&)； 
        void foo(X&&)； 
        你可以区分“为rvalue服务”和“为lvalue服务”。“々rvakie服务”的版本被允I午且应 该提供move语义。也就是说，它可以“偷取”实参的内部状态和资源。 
    •如果你实现 
        void foo(X&&)； 
        但既没有实现void foo(X&)也没有实现void foo(const X&), foo()可因rvalue被调用，但当你尝试lvalue调用它，会触发编译错误。因此，这里只提供move语义。
    这项能力被程序库中诸如unique pointer 、file stream 或 string stream所用。 
    以上意味着，如果class未提供move语义，只提供惯常的copy构造函数和copy assignment 操作符，rvalue reference可以调用它们。因此，std::move()意味着“调用move语义（若有提供的话），否则调用copy语义”。 

返回 Rvalue Reference 

    你不需要也不应该move()返回值。C++standard指出，对干以下代码： 
    X foo ()    
    {
        X x;
        ...
        return x;
    }
    保证有下列行为： 
    •如果X有一个可取用的copy成move构造函数，编译器可以选择略去其中的copy版本。 这也就是所谓的(named) return value optimization/*返回值优化*/((N)RVO)，这个特性其至在C++11之前就获得了大多数编译器的支持。 
    •否则，如果X有一个move构造函数，X就被moved  (搬移)。 
    •否则，如果X有一个copy构造函数，X就被copied  (复制)。 
    •否则，报出一个编译期错误（compile-time error)。 
    也请注意，如果返回的是个local nonstatic对象，那么返回其rvalue reference是不对的：
    X&& foo () 
    {
        X x;
        ...
        return x;   //ERROR: return reference to nonexisting object
    }
    是的，rvalue reference也是个reference，如果返回它而它指向（referring to) local对象，意味着你返回一个reference却指向一个不再存在的对象。是否对它使用std::move()倒是无关紧要。

新式字符串字面常量

    自C++11起，你可以定义raw string/*初始字符串*/和multibyte/wide-character等字符串字面常量。

    Raw String Literal
    Raw string允许我们定义字符序列（character sequence)，做法是确切写下其内容使其成为一 个raw character sequence。于是你可以省下很多用来装饰特殊字符的escape/*泄露*/符号。 
    Raw string以R"(幵头，以)"结尾，可以内含line break/*换行符*/。例如一个用来表示“两个反斜线和一个n” 的寻常字面常量可定义如下：

    "\\\\n"//转义字符 \\n转义
    也可以定义它为如下raw string literal:
    R"(\\n)"
    如果要在raw string内写出)“,可使用定义符（delimiter）。因此，一个raw string的完整语法是R“delim(...)delim”，其中delim是个字符序列(character sequence)，最多16个基本字符，
    不可含反斜线（backslash）、空格（whitespace）和小括号（parenthesis）。

    举个例子，下面的raw string literal 
        R"nc(a\ 
            b\nc()" 
            )nc"； 
    等同于以下的寻常string literal： 
    "a\\\n           b\\nc()\"\n            " 
    这样的string内含一个a、一个反斜线、一个新行字符（newline)、若干空格（space)、一个b、一个反斜线、一个n、一个c、一对小括号、一个双引号、一个新行字符，以及若干空格。
        定义正则表达式（regular expression）时raw string literal特别有用。

    编码的（Encoded) String Literal //字符串字面值

    只要使用编码前缀(encoding prefix),你就可以为string literal定义一个特殊的字符编码 (character encoding)下面这些编码前缀都预先定义好了 ： 
    u8定义一个UTF-8编码。UTF-8 string literal以UTF-8编定的某个给定字符起头，字符类型为 const char。 
    u定义一个string literal,带着类型为chiarl6_t的字符。 
    U定义一个string literal，带着类型为char32_t的字符。 
    L定义一个wide string literal,带着类型为wchar_t的字符。 
    例如： 
    L” hello ” //defines.‘hello ‘‘aswchar_t stringliteral 
    Raw string幵头那个R的前面还町以放置一个编码前缀。 


关键字 noexcept

    C++11提供了关键字noexcept，用来指明某个函数无法----或不打算---抛出异常。例如： 
    void foo () noexcept； 
    声明了 foo()不打算拋出异常。若有异常未在foo()内被处理---亦即如果foo()拋出异常---程序会被终止，然后std::terminate()被调用并默汄调用std::abort()//程序被终止。


关键字 constexpr

    自C++11起，constexpr可用来让表达式核定于编译期。例如：
    constexpr int square(int x)
    {
        return x*x;
    }
    float a [square (9)]； // OK since C++11: a has 81 elements

崭新的Template特性

    Variadic Template

    自C++11起, template可拥有那种“得以掊受个数不定之template实参”的参数。此能力称为 variadic template。举个例子，你可以写出这样的print()，得以在调用它时给予，不定个 
    数的实参且各具不同类型：
    void print()
    {
    }

    template <typename T, typename... Types> 
    void print (const T& firstArg, const Types&... args) 
    { 
        std:: cout << firstArg << std::endl; //print first argument 
        print(args...);                      //call print() for remaining arguments
    }
    如果传入1或多个实参，上述的 function template 就会被调用，它会把第一实参区分幵来，允许第一实参被打印，然后递归调用print()并传入其余实参。你必须提供一个nontemplate重载函数print(),才能结朿整个递动作。 

    举个例子，如下调用 
    print (7.5, "hello", std::bitset<16>(377), 42)； //bitset整数值序列化
    会导致以下输出： 
    7.5 
    hello 
    0000000101111001 
    42 
    注意，目前还在讨论以下例子是否也有效，原因是，正规而言，一个“单实参的variadic 形式”与一个“单实参的nonvariadic形式”形成歧义；然而编译器通常接受这样的代码： 
    template <typename T> 
    void print (const T& arg)
    { 
        std::cout << arg << std::endl； 
    } 
    template <typename T, typename... Types> 
    void print (const T& firstArg, const Types&... args)
    {   
        std:: cout << firstArg << std:: endl;  //print first argument
        print(args...);                        //call print() for remaining arguments
    }

    在 variadic template 内，sizeof...(args)会生成实参个数。 
    Class std::tuple<>大量使用了这一特性。 

Alias Template (带别名的模板.或者叫 Template Typedef) 

    自 C++11 起，支持 template (partial) type definition。然而由于关键字 typename 用于此处 时总是出于某种原因而失败，所以这里引人关键字using,并因此引入一个新术语 
    alias template。举个例子，写出以下代码 
    template <typename T> 
    using Vec = std::vector<T,MyAlloc<T>>;  //standard vector using own allocator 
    后, 
    Vec<int> coll; 
    就等价于 
    std::vector<int,MyAlloc<int>>   coll; 

    其他的Template新特性 

    自C++11起，function template 可拥有默认的 template 实参。此外，local type可被当作template实参。 


Lambda

    C++11引入了 lambda,允许inline函数的定义式被用作一个参数，或是一个local对象。 

    所谓lambda是一份功能定义式，可被定义于语句（statement)或表达式（expression)内部。因此你可以拿lambda当作inline函数使用。 
    最小型的lambda函数没有参数，什么也不做，如下： 
    []{ 
    std::cout << "hello lambda" <<  std::endl; 
    } 
    可以直接调用它： 
    { 
    std::cout <<  "hello lambda11"  << std: :endl; 
    };
    l();    

    如你所见，lambda总是由一个所谓的lambda introducer引入：那是一组方括号，你可以在其内指明一个所谓的capture，用来在lambda内部访问“nonstatic外部对象”。
    如果无须访问外部数据，这组方括号可以为空，就像本例所示。Static对象，诸如std::cout，是可被使用的。 
    在lambda introducer和lambda body之间，你可以指明参数或mutable，成一份异常明细（exception specification)或attribute specifier以及返回类型。
    所有这一切都可有可无，但如果其中一个出现了，参数所需的小括号就必须出现。因此lambda语法也可以是 
    [...]{...}
    或 
    [...](...) 
    Lambda可以拥有参数，指明于小括号内，就像任何函数一样： 
    auto l = [] (const std::stringft s) { 
    std::cout «  s «  std::endl； 
    }； 
    l("hello lambda") ；    //prints "hello lambda" 
    然而请注意，lambda不可以是template。你始终必须指明所有类型。 
    Lambda也可以返回某物。但你不需要指明返回类型，该类型会根据返回值被推导出来。例如下面的lambda的返回类型是int: 
    []{ 
    return 42; 
    }
    如果一定想指明一个返回类型，可使用新式C++语法，一般函数也可以用它。例如下面的lambda返回42.0: 
    [] () -> double { 
    return 42; 
    }
    这么一来你就必须指明返回类型，放在实参所需的小括号以及字符->之后。 
    在参数和返回类型指示(return specification)或函数体之间，你也可以写出一份异常明细（exceptionspecification),就像你能够为一般函数所做的那样。然而目前已不鼓励为函数撰写异常明细。 
    Capture (用以访问外部作用域） 
    在lambda introducer (每个lambda最开始的方括号）内，你可以指明一个capture用来处理外部作用域内未被传递为实参的数据： 
    [=]意味着外部作用域以by value方式传递给lambda。因此当这个lambda被定义时，你 可以读取所有可读数据（readable data)，但不能改动它们。 
    [&]意味着外部作用域以by reference方式传递给lambda。因此当这个lambda被定义时， 你对所有数据的涂写动作都合法，前提是你拥有涂写它们的权力。 
    也可以个别指明lambda之内你所访问的每一个对象是by value或by reference。因此你可以对访问设限，也可以混合不同的访问权力。例如下面这些语句： 
    int x=0; 
    int y=42; 
    auto qqq = [x, &y] { 
    std::cout    "x: "  << x << std::endl； 
    std::cout    "y: "  << y << std::endl; 
    ++y;  //OK 
    }； 
    x = y = 77; 
    qqq(); 
    qqq(); 
    std::cout << "final y: " << y << std::endl; 

    会产生以下输出：

    x:0 
    y:77 
    x:0 
    y:78 
    final y: 79 
    由于x是因by value而获得一份拷贝，在此lambda内部你可以改动它，但若调用++x是通不过编译的。y以by reference方式传递，所以你可以涂写它，并且其值的变化会影响外部; 所以调用这个lambda 二次，会使指定值77被累加。 
    你也可以写[=,&y]取代[x, &y],意思是以by reference方式传递y,其他所有实参则以by value方式传递。 
    为了获得 passing by value 和 passing by reference 混合体，你可以声明 lambda 为 mutable。 下例中的对象都以by value方式传递，但在这个lambda所定义的函数对象内，你有权力涂写传入的值。例如： 
    int id = 0; 
    auto f = [id] () mutable { 
        std::cout << "id:" << id <<  std::endl; 
        ++id;    //OK 
    };
    id = 42;
    f()； 
    f()； 
    f(); 
    std :: cout << id << std::endl； 
    会产生以下输出： 
    id: 0 
    id: 1 
    id: 2 
    42 
    你可以把上述lambda的行为视同下面这个function object ： 
    class { 
    private： 
        int id;  //copy ofoutsideid 
    public: 
        void operator() () { 
        std::cout <<"id: " << id << std::endl;
        ++id;    //OK
        }
    };

    由干mutable的缘故，operator ()被定义为一个non-const成员函数，那意味着对id的涂写是可能的。所以，有了 mutable，lambda变得stateful,即使slate是以by value方式传递。 
    如果没有指明mutable (—般往往如此)，operator ()就成为一个const成员函数，那么对于对象你就只能读取，因为它们都以by value方式传递。有一个例子使用 lambda并使用mutable,该处会讨论可能出现的问题。 

    Lambda的类型 

    Lambda的类型，是个不具名function object (或称functor)。每个lambda表达式的类型是 独一无二的。因此如果想根据该类型声明对象，可借助于template或auto。
    如果你实在需要写下该类型，可使用decltype，例如把一个lambda作为hash function或ordering准则或sorting准则传给associative (关联式）容器或unordered (不定序）容器。 
    或者你也可以使用C++标准库提供的std: :function<> class template,指明一个一般化类型给 functional programming使用。这个 class template 提供了 “明确指出函数的返回类型为lambda”的唯一办法： 

// lang/lambdal. cpp 
#include<functional> 
#include<iostream> 
std::function<int(int,int)> returnLambda () 
{
    return [] (int x,int y)
    {
        return x*y;        
    }
}

int main()
{
    auto lf = returnLambda();
    std::cout << lf(6,7) <<std::endl;
}

程序的输出当然是:
    42


关键字 decltype 

    新关键字decltype可让编译器找出表达式（expression) 类型。这其实就是常被要求的 typeof的特性体现。只不过原有的typeof缺乏一致性又不完全，C++11 才引入这么一个关键字。举个例子： 
    std::map<std:：string,float> coll; 
    decltype(coll)::value_type elem; 
    decltype的应用之一是声明返回类型（见下），另一个用途是在metaprogramming (超编程）或用来传递一个lambda类型。 

新的函数声明语法（New Function Declaration Syntax) 

    有时候，函数的返回类型取决于某个表达式对实参的处理。然而类似 
        template <typename T1, typename T2> 
        decltype(x+y) add(Tl x, T2 y)； 
    在C++11之前是不可能的，因为返回式所使用的对象尚未被引入，或未在作用域内。 
    但是在C++11，你可以将一个函数的返回类型转而声明于参数列之后： 
    template <typename T1, typename T2> 
    auto add(T1 x, T2 y) -> decltype(x+y)； 
    这种写法所采用的语法，和 “ 为lambda声明返回类型”是一样的。 

带领域的（Scoped)   Enumeration 

    C++11 允许我们定义 scoped enumeration   又称为 stronge numeration 成enumeration class 这是C++ enumeration value ( 或称 enumeroator)的一个较干净的实现 。例如： 
    enum class Salutation :char {mr, ms, co, none}; 
    重点在于，在enum之后指明关键字class。 
    Scoped enumeration 有以下优点： 
    •绝不会隐式转换至/自int。 
    •如果数值（例如mr)不在enumeration被声明的作用域内，你必须改写为Salutation::mr。 
    •你可以明显定义低层类型（underlying type,本例是char)并因此获得一个保证大小（如果你略去这里的“：char”，默汄类型是int)。 
    •提前声明（forward declaration)  enumeration type 是可能的，那会消除 “为了新的enumerations value 而重新编译”的必要---如果只有类型被使用的话。 

    注意，有了 type trait std：：underlying-type，你可以核定（evaluate) 一个 enumeration type 的低层类型。 
    标准异常的差错状态值（error condition value)也是个scoped enumerator。

新的基础类型（New Fundamental Data Type) 
    以下的新式基本数据类型，定义于C++11: 
    •  charl6_t 和 char32_t 
    •  long long 和 unsigned long long 
    •  std::nullptr_t 


虽旧犹新的语言特性 

    虽然C++98已超过10岁，程序员仍然会对其中某些语言特性感到惊讶。 

    Nontype Template Parameter (非类型模板参数） 

    Type parameter (类型参数）之外，我们也可以为template使用nontype parameter (非类型参数)。这样的参数亦被视为template类型的一部分。例如对于标准的class bitset<>，
    你可以传递bit个数作为template实参。下面的语句定义了两个bitfield: 一个带有32 bit,另一个带有 50 bit。 
    bitset<32> flags32;    //bitsetwith32bits 
    bitset<50> flags50;   //bitsetwith50bits 
    这些bitset有着不同的类型，因为它们使用不同的template实参。因此你不可对上述二者迸 行賦值或比较，除非它们之间有一个相应的类型转换。 
    Default Template Parameter (模板参数默认值） 
    Class template可以拥有默认实参。例如下面的声明式允许我们在声明class MyClass对象时 指定1或2个template实参： 
    template <typename T, typename container = vector<T>>
    class  MyClass； 
    如果你只传入一个实参，第二实参会采用默认值：
        MyClass<int> x1;    //equivalentto:MyClass<int,vector<int>>
    注意，默认的template实参可以其前一个实参为依据而定义。 

关键字typename 

    关键字typename用来指明紧跟其后的是个类型。考虑下面的例子： 
    template <typename T> 
    class MyClass { 
        typename T::SubType * ptr； 
        ...
    }； 
    在这里typename用来阐明 “SubType是个类型，定义于classT内”。因此ptr是个pointer， 指向类型T::SubType。如果没有typename, SubType会被视为一个static成员，于是 
    T：：SubType * ptr 
    被视为“类型为T的数值SubType” 乘以ptr。 
    基于“SubType必须是个类型”这样的限定，任何被用来替换T的类型，都必须提供一 个内层类型SubType。例如，以类型q作为此处的一个template实参的唯一可能是，类型Q有个内层类型SubType: 
    class Q { 
        typedef int SubType；
        ...
    };
    MyClass<Q> x;    //OK 
    此例之中，MyClass<(J>的ptr成员将会成为一个pointer toint。内层类型也可以是个抽象 数据类型（abstract data t y p e ) , 例如是个 class: 
    class Q { 
        class SubType； 
        ...
    }； 
    注意，如果你需要指明“template的某个标识符（identifier)其实是个类型”，那么总是需要 typename,即便它“如果不是个类型就毫无意义”。因此，C++的一般性规则就是，template 内的任何标识符都被视为一个value，除非为它加上typename。 
    此外在template声明式中typename也可被用来取代class: 
    template <typename T> class MyClass； 

Member Template (成员模板） 

    Class的成员函数口了以是template。然而 member template不可以是virtual。举个例子： 

    class MyClass { 
    template <typename T> 
    void f(T); 
    }； 
    上述的MyClass::f 声明了 “一组”成员函数，其参数可以是任意类型。你可以传递任何实参给它们，只要该实参的类型提供“f()用到的所有操作”。 
    这一语言特性往往被用来支持class template内的成员之间的自动类型转换。例如下面 的定义式中，assign()的实参x必须和“调用assign()” 的那个对象的类型完全相同： 
    template <typename T> 
    class MyClass {
    private: 
        T value； 
    public: 
        void assign (const MyClass<T>& x) { //x must have same type as*this 
        value = x.value;
    ...
    }; 

    如果assign()调用者和其实参（都是对象）的template类型不同，是不可以的，即便你提供了那些类型之间的自动转换： 
    void f() 
    {
        MyClass<double> d; 
        MyClass<int> i； 
        d.assign(d) ；  //OK 
        d.assign(i) ；  //ERROR: i is MyClass<int> 
                    //       but MyClass<double> is required
    }
    但如果你为成员函数提供一个不一样的template类型，你就可以从“必须精确吻合”的规则中解脱。现在，这个成员函数的template实参可具备任何template类型，只要该类型“可被赋值”： 
    template <typename T> 
    class MyClass { 
    private: 
        T value; 
    public:
        template <typename X>               //member template
        void assgin(const MyClass<X>& x)    //allows different template types
        {
            value = x.getValue();
        }
        T getValue() const 
        {
            return value;
        }
        ...
    };
    void f()
    {
        MyClass<double> d;
        MyClass<int> i;

        d.assign(d);    //OK
        d.assign(i);    //OK(int is assginable to double)
    }

    注意，现在assign()的实参x,其类型不同于*this的类型，因此你不可直接取用 MyClass()的private和protected成员。取而代之，你必须使用诸如本例getValue()之类的东西。 
    Member template的一个特殊形式是命谓template构造函数。这种东西通常被提供用于 “对象被复制时给予隐式类型转换”的能力。注意，template构造函数并不压制copy构造函数的隐式声明。
    如果类型完全吻合，隐式的copy构造函数会被生成出来，并被调用。例如: 
    template <typename T> 
    class MyClass { 
    public: 
    //copy constructor with implicit type conversion 
    //-does not suppress implicit copy constructor 
    template <typename U> 
    MyClass   ( const  MyClass<U>& x)；
    ...
    };
    void f()
    {
        MyClass<double> xd;
        ...
        MyClass<double> xd2(xd);    //calls implicitly generated copy constructor
        MyClass<int>    xi(xd);     //calls template constructor
        ...
    }

    这里，xd2 的类型完全相同于xd，因此其初始化是通过被隐式生成的copy构造函数完成。 xi的类型不同于xd，因此其初始化是通过template构造函数完成。所以，如果你实现出一个template构造函数，
    请不要忘记也提供一个default构造函数---如果后者的默认行为不符合你的需要。还有一个member template的例子。 

Nested (嵌套式）Class Template 

    嵌套类(Nested class)也可以是 template:
    template <typename T> 
    class MyClass {
        ... 
        template <typename T2> 
        class NestedClass; 
        ...
    };

基础类型的明确初始化（Explicit Initialization for Fun-damental Type) 

    如果你使用“一个明确的构造函数调用，但不给实参”这样的语法，基础类型会被设定初值为0:
        int i1;             //undefined value
        int i2 = int();     //initialized with zero
        int i3{};           //initialized with zero(since C++11)
    这个特性使你得以写出“确保无论任何类型，其值都有一个确凿的默认值”的template code。例如下面的函数中，初始化机制保证了 “ x 如果是基础类犁，会被初始化为0”： 
    template <typename T> 
    void f() 
    { 
        T x = T();
        ... 
    } 
    如果一个template强迫设K初值为0，其值就是所谓的zero initialized，否则就是default initialized。 

main ( ) 定义式 

    我还想澄清一个重要而常被误解的核心语言点：唯一 “正确且其移植性”的main()。拫据C++标准，main()只有两种定义式具备可移植性，那就是 
    int main()
    {
        ...
    } 
    和
    int main(int argc,char* argv[])
    {
        ...
    }

    其中的argv，也就足命令行实参（command-line argument)所形成的array，也可定义为 char**。注意其返回类型必须是int。 
    你可以（但非必要）以一个return语句终结main()。不同于C, C++ 定义了一个隐晦的 
        return 0; 
    于main()终点。这意味着任何程序离幵main()时可以不写return语句。0以外的任何值都代表某种失败。为此，本书中我的所有例子都没在main()终点放上一个return语句。 
    如果不想以“ main()返回”方式结束C++程序，通常应该调用exit()、quick_exit() (C++11之后）或terminate()。












