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
