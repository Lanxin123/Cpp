关键字 decltype 

    新关键字decltype可让编译器找出表达式（expression) 类型。这其实就是常被要求的 typeof的特性体现。只不过原有的typeof缺乏一致性又不完全，C++11 才引入这么一个关键字。举个例子： 
    std::map<std:：string,float> coll; 
    decltype(coll)::value_type elem; 
    decltype的应用之一是声明返回类型（见下），另一个用途是在metaprogramming (超编程）或用来传递一个lambda类型。 
