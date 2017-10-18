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
