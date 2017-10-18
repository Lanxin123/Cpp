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