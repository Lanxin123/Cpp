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