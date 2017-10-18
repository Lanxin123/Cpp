以auto完成类型自动推导
    auto i = 42;
    这是个推导而来的过程，因此一定需要一个初始化操作:
    auto i;             //ERROR:can't deduce the type of i

    //auto 是C语言的一个就关键字。和static一样用来声明某变量为局部的（local）。但它从未被真正的用过，因为
    //“不指明某物为static”也就隐含地声明了它是个auto。
