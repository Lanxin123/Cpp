Callable Object (可被调用的对象)
    C++ 标准库在不同的地方都用过术callable object，意思是“可被某种方式调用其某些函数”的对象。它可以是： 
    一个函数，接受额外传入的作为实参（argument) 。 
    一个指向成员函数的指针（a pointer to a member function),当你通过对象调用它，该对象被传递成为第一实参（必须是个reference或pointer)，其他实参则一一对应成员函数的参数。 
    一个函数对象（function object,该对象拥有operator ())，附带的args被传递作为实参。 
    一个lambda，严格地说它是一种函数对象。 
    例如： 
    void func (int x, int y)； 
    auto 1 = [] (int x, int y) {
                ...
    };

    class C
    {
        public:
            void operator() (int x,int y) const;//const的函数不能对其数据成员进行修改操作。
                                                //const的对象，不能引用非const的成员函数。
            void memfunc(int x,int y) const;
    };
    int main()
    {
        C c;
        std::shared_ptr<C> sp(new C);

        //bind()uses callable object to bind arguments:
        std::bind(func,77,33)();                //calls: func(77,33)
        std::bind(1,77,33)();                   //calls:l(77,33)
        std::bind(C(),42,77)();                 //calls:C::operator()(77,33)
        std::bind(&C::memfunc,c,77,33)();       //calls:c.memfunc(77,33)     
        std::bind(&C::memfunc,sp,77,33)();      //calls:sp->memfunc(77,33)

        //async() uses callable objects to start(background)tasks:
        std::async(func,42,77);                 //calls: func(42,77)
        std::async(l,42,77);                    //calls: l(42,77)
        std::async(c,42,77);                    //calls: c.operator()(42,77)
        std::async(&C::memfunc,&c,42,77);       //calls: c.memfunc(42,77)
        std::async(&C::memfunc,sp,42,77);       //calls: sp->memfunc(42,77)
    }