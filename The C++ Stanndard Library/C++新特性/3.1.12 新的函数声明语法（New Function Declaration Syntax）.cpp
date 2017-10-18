新的函数声明语法（New Function Declaration Syntax) 

    有时候，函数的返回类型取决于某个表达式对实参的处理。然而类似 
        template <typename T1, typename T2> 
        decltype(x+y) add(Tl x, T2 y)； 
    在C++11之前是不可能的，因为返回式所使用的对象尚未被引入，或未在作用域内。 
    但是在C++11，你可以将一个函数的返回类型转而声明于参数列之后： 
    template <typename T1, typename T2> 
    auto add(T1 x, T2 y) -> decltype(x+y)； 
    这种写法所采用的语法，和 “ 为lambda声明返回类型”是一样的。 