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
