Move语义和Rvalue Reference
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
