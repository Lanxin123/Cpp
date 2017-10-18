带领域的（Scoped)   Enumeration 

    C++11 允许我们定义 scoped enumeration   又称为 stronge numeration 成enumeration class 这是C++ enumeration value ( 或称 enumeroator)的一个较干净的实现 。例如： 
    enum class Salutation :char {mr, ms, co, none}; 
    重点在于，在enum之后指明关键字class。 
    Scoped enumeration 有以下优点： 
    •绝不会隐式转换至/自int。 
    •如果数值（例如mr)不在enumeration被声明的作用域内，你必须改写为Salutation::mr。 
    •你可以明显定义低层类型（underlying type,本例是char)并因此获得一个保证大小（如果你略去这里的“：char”，默汄类型是int)。 
    •提前声明（forward declaration)  enumeration type 是可能的，那会消除 “为了新的enumerations value 而重新编译”的必要---如果只有类型被使用的话。 

    注意，有了 type trait std：：underlying-type，你可以核定（evaluate) 一个 enumeration type 的低层类型。 
    标准异常的差错状态值（error condition value)也是个scoped enumerator。