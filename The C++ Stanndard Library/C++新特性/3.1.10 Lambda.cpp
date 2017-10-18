Lambda

    C++11引入了 lambda,允许inline函数的定义式被用作一个参数，或是一个local对象。 

    所谓lambda是一份功能定义式，可被定义于语句（statement)或表达式（expression)内部。因此你可以拿lambda当作inline函数使用。 
    最小型的lambda函数没有参数，什么也不做，如下： 
    []{ 
    std::cout << "hello lambda" <<  std::endl; 
    } 
    可以直接调用它： 
    { 
    std::cout <<  "hello lambda11"  << std: :endl; 
    };
    l();    

    如你所见，lambda总是由一个所谓的lambda introducer引入：那是一组方括号，你可以在其内指明一个所谓的capture，用来在lambda内部访问“nonstatic外部对象”。
    如果无须访问外部数据，这组方括号可以为空，就像本例所示。Static对象，诸如std::cout，是可被使用的。 
    在lambda introducer和lambda body之间，你可以指明参数或mutable，成一份异常明细（exception specification)或attribute specifier以及返回类型。
    所有这一切都可有可无，但如果其中一个出现了，参数所需的小括号就必须出现。因此lambda语法也可以是 
    [...]{...}
    或 
    [...](...) 
    Lambda可以拥有参数，指明于小括号内，就像任何函数一样： 
    auto l = [] (const std::stringft s) { 
    std::cout «  s «  std::endl； 
    }； 
    l("hello lambda") ；    //prints "hello lambda" 
    然而请注意，lambda不可以是template。你始终必须指明所有类型。 
    Lambda也可以返回某物。但你不需要指明返回类型，该类型会根据返回值被推导出来。例如下面的lambda的返回类型是int: 
    []{ 
    return 42; 
    }
    如果一定想指明一个返回类型，可使用新式C++语法，一般函数也可以用它。例如下面的lambda返回42.0: 
    [] () -> double { 
    return 42; 
    }
    这么一来你就必须指明返回类型，放在实参所需的小括号以及字符->之后。 
    在参数和返回类型指示(return specification)或函数体之间，你也可以写出一份异常明细（exceptionspecification),就像你能够为一般函数所做的那样。然而目前已不鼓励为函数撰写异常明细。 
    Capture (用以访问外部作用域） 
    在lambda introducer (每个lambda最开始的方括号）内，你可以指明一个capture用来处理外部作用域内未被传递为实参的数据： 
    [=]意味着外部作用域以by value方式传递给lambda。因此当这个lambda被定义时，你 可以读取所有可读数据（readable data)，但不能改动它们。 
    [&]意味着外部作用域以by reference方式传递给lambda。因此当这个lambda被定义时， 你对所有数据的涂写动作都合法，前提是你拥有涂写它们的权力。 
    也可以个别指明lambda之内你所访问的每一个对象是by value或by reference。因此你可以对访问设限，也可以混合不同的访问权力。例如下面这些语句： 
    int x=0; 
    int y=42; 
    auto qqq = [x, &y] { 
    std::cout    "x: "  << x << std::endl； 
    std::cout    "y: "  << y << std::endl; 
    ++y;  //OK 
    }； 
    x = y = 77; 
    qqq(); 
    qqq(); 
    std::cout << "final y: " << y << std::endl; 

    会产生以下输出：

    x:0 
    y:77 
    x:0 
    y:78 
    final y: 79 
    由于x是因by value而获得一份拷贝，在此lambda内部你可以改动它，但若调用++x是通不过编译的。y以by reference方式传递，所以你可以涂写它，并且其值的变化会影响外部; 所以调用这个lambda 二次，会使指定值77被累加。 
    你也可以写[=,&y]取代[x, &y],意思是以by reference方式传递y,其他所有实参则以by value方式传递。 
    为了获得 passing by value 和 passing by reference 混合体，你可以声明 lambda 为 mutable。 下例中的对象都以by value方式传递，但在这个lambda所定义的函数对象内，你有权力涂写传入的值。例如： 
    int id = 0; 
    auto f = [id] () mutable { 
        std::cout << "id:" << id <<  std::endl; 
        ++id;    //OK 
    };
    id = 42;
    f()； 
    f()； 
    f(); 
    std :: cout << id << std::endl； 
    会产生以下输出： 
    id: 0 
    id: 1 
    id: 2 
    42 
    你可以把上述lambda的行为视同下面这个function object ： 
    class { 
    private： 
        int id;  //copy ofoutsideid 
    public: 
        void operator() () { 
        std::cout <<"id: " << id << std::endl;
        ++id;    //OK
        }
    };

    由干mutable的缘故，operator ()被定义为一个non-const成员函数，那意味着对id的涂写是可能的。所以，有了 mutable，lambda变得stateful,即使slate是以by value方式传递。 
    如果没有指明mutable (—般往往如此)，operator ()就成为一个const成员函数，那么对于对象你就只能读取，因为它们都以by value方式传递。有一个例子使用 lambda并使用mutable,该处会讨论可能出现的问题。 

    Lambda的类型 

    Lambda的类型，是个不具名function object (或称functor)。每个lambda表达式的类型是 独一无二的。因此如果想根据该类型声明对象，可借助于template或auto。
    如果你实在需要写下该类型，可使用decltype，例如把一个lambda作为hash function或ordering准则或sorting准则传给associative (关联式）容器或unordered (不定序）容器。 
    或者你也可以使用C++标准库提供的std: :function<> class template,指明一个一般化类型给 functional programming使用。这个 class template 提供了 “明确指出函数的返回类型为lambda”的唯一办法： 

// lang/lambdal. cpp 
#include<functional> 
#include<iostream> 
std::function<int(int,int)> returnLambda () 
{
    return [] (int x,int y)
    {
        return x*y;        
    }
}

int main()
{
    auto lf = returnLambda();
    std::cout << lf(6,7) <<std::endl;
}

程序的输出当然是:
    42
