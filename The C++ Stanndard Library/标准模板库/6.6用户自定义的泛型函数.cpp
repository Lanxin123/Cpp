用户自定义的泛型函数（User-Defined Generic Function）
    STL是一个可扩充框架。它的意思是你可以写你自己的函数和算法，用以处理集合内的元素。当然这些操作函数也可以是泛化的（generic）。然而，若要在这些操作函数内声明一个合法的迭代器，必须与容器类型相应，因为不同的容器有不同的迭代器。
    为了协助写出泛型函数，每一个容器类型都提供有若干内部的类型定义。考虑以下例子：
#include <iostream>
#include <string>

// PRINT_ELEMENTS()
// - prints optional string optstr followed by
// - all elements of the collection coll
// - in one line, separated by spaces
template <typename T>
inline void PRINT_ELEMENTS (const T& coll,
                            const std::string& optstr="")
{
    std::cout << optstr;
    for (const auto&  elem : coll) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
}
    这个例子定义了一个泛型函数，打印出一个可有可无的string，后面紧接着是给定的容器的全部元素。
    在C++11之前，一个“遍历所有元素”的循环看起来如下:

    typename T::const_iterator pos;
    for(pos = coll.begin();pos!=coll.end();++pos){
        std::cout<<*pos<<' ';
    }
    这里声明pos为拥有“被传入的容器”的类型内的某个迭代器类型。注意，typename在此是必要的，用以表示const-iterator是个类型，而不是类型T内的某个static成员（见3.2 节第34页对typename的介绍)。 
    除了 iterator和const_iterator，容器还提供了其他类型，用来协助写出泛型函数。 举个例子，它们提供了元素类型，使你能够处理元素的临时拷贝。详见9.5.1节第468页。 
    在PRINT_ELEMENTS函数中，可有可无的第二实参是个string，用作被写出的每一个元素的前缀（prefix）。因此，使用PRINT_ELEMENTS(),你可以像这样注释或介绍你的输出： 
        PRINT.ELEMENTS (coll, "all elements:"); 
    我之所以在此介绍这个函数，因为本书常常用到它，用来打印容器内的所有元素，这确实是一个很简单的调用形式。
    //的确很简单。。。。








