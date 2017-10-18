虽旧犹新的语言特性 

    虽然C++98已超过10岁，程序员仍然会对其中某些语言特性感到惊讶。 

    Nontype Template Parameter (非类型模板参数） 

    Type parameter (类型参数）之外，我们也可以为template使用nontype parameter (非类型参数)。这样的参数亦被视为template类型的一部分。例如对于标准的class bitset<>，
    你可以传递bit个数作为template实参。下面的语句定义了两个bitfield: 一个带有32 bit,另一个带有 50 bit。 
    bitset<32> flags32;    //bitsetwith32bits 
    bitset<50> flags50;   //bitsetwith50bits 
    这些bitset有着不同的类型，因为它们使用不同的template实参。因此你不可对上述二者迸 行賦值或比较，除非它们之间有一个相应的类型转换。 
    Default Template Parameter (模板参数默认值） 
    Class template可以拥有默认实参。例如下面的声明式允许我们在声明class MyClass对象时 指定1或2个template实参： 
    template <typename T, typename container = vector<T>>
    class  MyClass； 
    如果你只传入一个实参，第二实参会采用默认值：
        MyClass<int> x1;    //equivalentto:MyClass<int,vector<int>>
    注意，默认的template实参可以其前一个实参为依据而定义。 

关键字typename 

    关键字typename用来指明紧跟其后的是个类型。考虑下面的例子： 
    template <typename T> 
    class MyClass { 
        typename T::SubType * ptr； 
        ...
    }； 
    在这里typename用来阐明 “SubType是个类型，定义于classT内”。因此ptr是个pointer， 指向类型T::SubType。如果没有typename, SubType会被视为一个static成员，于是 
    T：：SubType * ptr 
    被视为“类型为T的数值SubType” 乘以ptr。 
    基于“SubType必须是个类型”这样的限定，任何被用来替换T的类型，都必须提供一 个内层类型SubType。例如，以类型q作为此处的一个template实参的唯一可能是，类型Q有个内层类型SubType: 
    class Q { 
        typedef int SubType；
        ...
    };
    MyClass<Q> x;    //OK 
    此例之中，MyClass<(J>的ptr成员将会成为一个pointer toint。内层类型也可以是个抽象 数据类型（abstract data t y p e ) , 例如是个 class: 
    class Q { 
        class SubType； 
        ...
    }； 
    注意，如果你需要指明“template的某个标识符（identifier)其实是个类型”，那么总是需要 typename,即便它“如果不是个类型就毫无意义”。因此，C++的一般性规则就是，template 内的任何标识符都被视为一个value，除非为它加上typename。 
    此外在template声明式中typename也可被用来取代class: 
    template <typename T> class MyClass； 

Member Template (成员模板） 

    Class的成员函数口了以是template。然而 member template不可以是virtual。举个例子： 

    class MyClass { 
    template <typename T> 
    void f(T); 
    }； 
    上述的MyClass::f 声明了 “一组”成员函数，其参数可以是任意类型。你可以传递任何实参给它们，只要该实参的类型提供“f()用到的所有操作”。 
    这一语言特性往往被用来支持class template内的成员之间的自动类型转换。例如下面 的定义式中，assign()的实参x必须和“调用assign()” 的那个对象的类型完全相同： 
    template <typename T> 
    class MyClass {
    private: 
        T value； 
    public: 
        void assign (const MyClass<T>& x) { //x must have same type as*this 
        value = x.value;
    ...
    }; 

    如果assign()调用者和其实参（都是对象）的template类型不同，是不可以的，即便你提供了那些类型之间的自动转换： 
    void f() 
    {
        MyClass<double> d; 
        MyClass<int> i； 
        d.assign(d) ；  //OK 
        d.assign(i) ；  //ERROR: i is MyClass<int> 
                    //       but MyClass<double> is required
    }
    但如果你为成员函数提供一个不一样的template类型，你就可以从“必须精确吻合”的规则中解脱。现在，这个成员函数的template实参可具备任何template类型，只要该类型“可被赋值”： 
    template <typename T> 
    class MyClass { 
    private: 
        T value; 
    public:
        template <typename X>               //member template
        void assgin(const MyClass<X>& x)    //allows different template types
        {
            value = x.getValue();
        }
        T getValue() const 
        {
            return value;
        }
        ...
    };
    void f()
    {
        MyClass<double> d;
        MyClass<int> i;

        d.assign(d);    //OK
        d.assign(i);    //OK(int is assginable to double)
    }

    注意，现在assign()的实参x,其类型不同于*this的类型，因此你不可直接取用 MyClass()的private和protected成员。取而代之，你必须使用诸如本例getValue()之类的东西。 
    Member template的一个特殊形式是命谓template构造函数。这种东西通常被提供用于 “对象被复制时给予隐式类型转换”的能力。注意，template构造函数并不压制copy构造函数的隐式声明。
    如果类型完全吻合，隐式的copy构造函数会被生成出来，并被调用。例如: 
    template <typename T> 
    class MyClass { 
    public: 
    //copy constructor with implicit type conversion 
    //-does not suppress implicit copy constructor 
    template <typename U> 
    MyClass   ( const  MyClass<U>& x)；
    ...
    };
    void f()
    {
        MyClass<double> xd;
        ...
        MyClass<double> xd2(xd);    //calls implicitly generated copy constructor
        MyClass<int>    xi(xd);     //calls template constructor
        ...
    }

    这里，xd2 的类型完全相同于xd，因此其初始化是通过被隐式生成的copy构造函数完成。 xi的类型不同于xd，因此其初始化是通过template构造函数完成。所以，如果你实现出一个template构造函数，
    请不要忘记也提供一个default构造函数---如果后者的默认行为不符合你的需要。还有一个member template的例子。 

Nested (嵌套式）Class Template 

    嵌套类(Nested class)也可以是 template:
    template <typename T> 
    class MyClass {
        ... 
        template <typename T2> 
        class NestedClass; 
        ...
    };
