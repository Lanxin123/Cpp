Type Trait和Type Utility//类型特点和类型效用

    C++标准库中几乎每样东西都以template为根基。为了更多地支持template编程（有时亦称为超编程 [metaprogramming]），标准库提供了template通用工具，
    协助应用程序开发人员和程序库作者。
    Type Trait，由TR1引入而在C++11中大幅扩展的一项机制，定义出因type而异的行为。它们可被用来针对type优化代码，以便提供特别能力。

Type Trait的目的
    所谓type trait，提供一种用来处理type属性（property of a type）的办法。它是个template可在编译期根据一或多个template实参（通常也是type）产出一个type或value。
        请看以下例子：
        templatr<typename T>
        void foo(const T& val)
        {
            if(std::is_pointer<T>::value){
                std::cout<<"foo() called for a pointer"<<std::endl;
            }
            else{
                std::cout<<"foo()  called for a value"<<std::endl;
            }
            ...
        }
    这里的trait std::is_poiter定义于<type_traits>，用来检查T是否是个pointer type。事实上is_pointer<>产出的若非type true_type就是type false_type，而::value若非
    产出true就是产出false。因此如果参数val是个pointer，上述的foo()会输出。
        foo() called for a pointer
    然而请注意，你不能这么做:
        template<typename T>
        void foo(const T& val)
        {
            std::cout<<(std::is_pointer<T>::value ? *val : val)
                     <<std::endl;
        }    
    原因是，代码拓展后会同时有着*val和val。如果你传递一个int使得编译期表达式is_pointer<T>::value产出false，代码会被扩展为：
        cput<<(false ? *val : val)<<endl;
    这是通不过编译的，因为当“当val是这个整数”时*val是一个无效表达式。
    但是你可以这么做：
    //foo() implementation for pointer types:指针类型的实现
    template<typename T>
    void foo_impl (const T& val,std::true_tye)
    {
        std::cout<<"foo() called for pointer to "<<*val
                 <<std::endl;
    }

    //foo() implementation for non-pointer types:
    template<typename T>
    void foo_impl(const T& val, std::false_type)
    {
        std::cout<<"foo() called for value to"<<val
                 <<std::endl;
    }

    template<typename>
    void foo(const T& val)
    {
        foo_impl(val,std::is_pointer<T>());
    }
    以上，在foo()内的表达式
        std::is_pointer<T>()
    于编译器产出std::true_type或std::false_type，于是确定哪一个foo_impl()重载版本被实例化(instantiated)。
    为什么这比“提供两个foo()重载版本，其一针对寻常type，另一个针对pointer type”好呢？
    template<typename T>
    void foo(const T& val);         //general implementation    一般实现

    template<typename T>
    void foo<T*> (const T& val);    //partial specialization for pointers 指针的部分特化
    答案之一是，有时候，太多重载版本并无必要。一般而言，type trait的威力更多来自于一个事实：它们是泛型代码(generic code)的基石，下面两个例子可以说明这个观点。

处理共通类型(Common Type)
    Type trait的另一个运用实例是，处理两个或多个类型中的“共通类型”。那是什么呢？那是一个可以“用来对付两个不同类型的值”的类型，前提是的确存在这么一个共通类型。举个例子，
    不同类型的两个值的总和或最小值，就该使用这个所谓的共通类型。否则，如果想实现一个函数，判断不同类型的两值中的最小值，其返回值类型该是什么呢？
        template<typename T1,typename T2>
        min(const T1& x,const T2& y);//???
    如果运用type trait，仅仅使用std::common_type<>就可以解决问题:
        template<typename T1, typename T2>
        typename std::common_type<T1,T2>::type min(const T1& x, const T2& y);
    如果两个实参都是int或都是long，或一个是int另一个是long，则std::common_type<T1,T2>::type会产出int。如果实参之一是string而另一个是字符串字面常量（类型为const char*），查出的结果是std::string。
    怎么办到的？好吧，它只是使用了操作符 ?: 的规则而已，那个操作符必须根据两个操作数的类型产出一个结果类型。事实上std::common_type<>是这么实现的:
        template<typename T1,typename T2>
        struct common_type<T1,T2>{
            typedef decltype(true ? declval<T1>() : declval<T2>()) type;
        };
    其中decltype是C++11提供的一个新关键字，用以导出一个表达式的类型，declval<>则是个辅助性trait，依据传入的类型提供一个值，但不去核算它（特就是为该值产生一个 ravlue reference）。 
    于是，如果操作符？：能够找出一个共通类型，common_type<>就会产出它。如果找不到，你还是可以提供一个common_type<>重载版本（这正是chrono程序库的作为，使它得以结合duration〔时间段〕。

