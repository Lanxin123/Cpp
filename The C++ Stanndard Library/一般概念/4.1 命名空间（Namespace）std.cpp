命名空间（Namespace） std
    当你釆用不同的模块和/或程序库时，经常会出现名称冲突现象，这是因为不同的模块和程序库可能对不同的对象使用相同的标识符（identifier)。Namespace 正是用来解决这个问题的。所谓namespace是指标识符的某种可见范围。
    和class不同，namespace具有扩展幵放性，可发生于任何源码文件上。因此你可以使用一个namespace来定义若千组件，而它们可散布于多个实质模块（physical module) 内。这类组件的典型例子就是标准库，因为 C++11标准库使用了一个namespace。 
    事实上，C++标准库中的所有标识符都被定义于一个名为std的namespace内。C++11 还包含由TR1引入、被定义于namespace std::trl 内的所有标识符。此外，namespace posix被保留了，虽然它尚未被C++标准库使用。
    基于namespace带来的概念，欲使用C++标准库的任何标识符，你有三种选择： 
    1.直接指定标识符，例如写下std: :ostream而不是ostream。完整语句类似这样： 
    std:：cout << std::hex << 3.4 << std：:endl； 
    2.使用using declaration。例如以下程序片段使我们后续不必写出范围限定符Std::便可直接使用cout和endl: 
    using std：：cout； 
    using std::endl; 
    于是先前的例子可写成这样： 
    cout << std::hex << 3.4 << endl; 
    3.使用using directive,这是最简便的选择。如果对namespace std采用using directive,便可以让std内的所有标识符都有效（曝光），就好像它们被声明为全局符号一样。因此， 写下 
    using namespace std; 
    之后，就可以直接这样写： 
    cout << hex << 3.4 << endl; 
    但请注意，在复杂的代码中，这种方式可能导致意外的名称冲突，更糟的是，由于某些晦涩的重栽(overloading)规则甚至,导致不一样的行为。如果场合不够明确（例如在头文件中），应避免使用using directive。 
    