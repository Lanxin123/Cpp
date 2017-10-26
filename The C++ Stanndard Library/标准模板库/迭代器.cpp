迭代器（Iterator）
    
    自C++11起，我们可以使用一个range-based for循环来处理所有元素，然而如果只是要找出某元素，并不需要处理所有元素。我们应该迭代所有元素，直到找到目标。此外，我们成许希望将这个（被找到的元素的）位置存放在某处，以便稍后能够继续迭代或进行其他处理。
    因此我们需要这样的概念：以一个对象表现出容器元素的位置。这样的概念的确存在。实践这个概念的对象就是所谓的迭代器（iterator）。事实上：我们将会看到，range-based for循环其实就是此概念的一个便捷接口，也就是说，其内部使用迭代器对象迭代（遍历） 所有元素。 
        迭代器是一个“可遍历STL容器全部或部分元素”的对象。迭代器用来表现容器中的某一个位置。基本操作如下： 
    Operator * 返回当前位置上的元素值。如果该元素拥有成员，你可以通过迭代器直接以 操作符-> 取用它们。 
    Operator ++ 令迭代器前进至下一元素。大多数迭代器还可使用operator-- 退至前一元素。 
    Operators == 和 != 判断两个迭代器是否指向同一位置。 
    Operator = 对迭代器陚值（也就是指明迭代器所指向的元素的位置)。 
    这些操作和C/C++ “运用pointer操作寻常的array元素”时的接口一致。差别在于，迭代器是所谓的smart pointer，具有遍历复杂数据结构的能力，其内部运作机制取决干其所遍历的数据结构。因此，每一种容器都必须提供自己的迭代器。
    事实上每一种容器的确都将其迭代器以嵌套(nested)方式定义干class内部。因此各种迭代器的接口虽然相同，类型却各自不同。

    这直接引出了泛型程序设计的概念：所有操作都使用相同接口，纵使类型不同。因此，你可以使用template将泛型操作公式化，使之得以顺利运作那些“能够满足接口需求”的任何类型。
    所有容器类都提供一些基本的成员函数，使我们得以取得迭代器并以之遍历所有元素。这些函数中最重要的是：
    
    begin()返回一个迭代器，指向容器起点，也就是第一元素（如果有的话）的位置。
    end()返回一个迭代器，指向容器终点。终点位于最末元素的下一位置，这样的迭代器又称作“逾尾（past-the-end）”迭代器。


    于是，begin()和end()形成了一个半开区间（half-open range），从第一元素开始，到最末元素的下一位置结束（如图6.4所示）。半开区间有两个优点： 
    1.为“遍历元素时的loop结束时机”提供一个简单的判断依据。只要尚未到达end(), loop就可以继续进行。 
    2.不必对空区间（empty range）采取特殊处理手法。空区间的begin()就等于end()。 
    下面这个例子示范了迭代器的用法，将list容器的所有元素打印出来（这是6.2.1节173 页的list实例的变化版本，改用迭代器)。

#include "stdafx.h"
#include <list>
#include <iostream>
using namespace std;

int main()
{
	list<char> coll;			//list container for character elements 
	
	//append elements from 'a' to 'z'
	for (char c='a';c<='z';++c)
	{
		coll.push_back(c);
	}

	//print all elements:
	//iterate over all elements
	list<char>::const_iterator pos;
	for (pos = coll.begin();pos!=coll.end();++pos)
	{
		cout << *pos << ' ';//遇到了一个奇怪的bug当我输入的是'  '里面两个空格的时候，就会变成(int)8824,之前也没遇到过这种情况。。。
	}
	cout << endl;
}
    首先创建一个list，然后填入字符‘a’到‘z’，然后打印出所有元素。但这次不是使用range-base for循环:
    for(auto elem : coll){
        cout<<elem<<' ';
    }
    所有元素是被一个寻常的for循环打印，使用迭代器走遍容器内的每一个元素：
        list<char>::const_iterator pos;
        for(pos = coll.begin(); pos!=coll.end();++pos){
            cout<<*pos<<' ';
        }
    迭代器pos被声明于循环之前，其类型是“指向容器内的常量元素”的迭代器：
        list<char>::const_iterator pos;
    任何容器都定义有两种迭代器类型：
    1.container::iterator以“读/写”模式遍历元素。
    2.container::const_iterator以“只读”模式遍历元素。
    例如在class list中，它们的定义可能如下：
        namespace std{
            template <typename T>
            class list{
                public:
                    typedef... iterator;
                    typedef... const_iterator;
                    ...
            };
        }
    至于其中iterator和const_iterator的确切类型，则由实现（implementation）定义之。
        上述循环中，迭代器pos以容器的第一元素的位置为初值：
        pos = coll.begin()
    循环不断进行，只要pos尚未到达容器终点：
        pos != coll.end()
    这里的pos是与“逾尾（past-the-end）”迭代器做比较。当循环内部执行++pos语句，迭代器pos就会前进至下一元素、
    总而言之，pos从第一元素开始，逐次访问每一个元素，直到抵达终点为止（如图6.5所示）。如果容器内没有任何元素，coll.begin()等于coll.end(),循环根本不会执行。


    在循环内部，*pos代表当前（current）元素。本例将它输出至标准输出设备cout后, 又接着输出了一个空格。你不能改变元素内容，因为pos是个const_iterator,
    从这样一个迭代器的观点看，元素是常量，不能更改。不过如果你采用非常量（nonconstant）迭代器，而且元素本身的类型也是非常量（nonconstant),就可以通过迭代器来改变元素值。例如：
    //make all characters in the list uppercase
    list<char>::iterator pos;
    for(pos = coll.begin();pos != coll.end();++pos){
        *pos = toupper(*pos);
    }
    如果我们使用迭代器遍历（unordered) map和multimap的元素，pos会指向key/value pair。 那么表达式 
        pos->second 
    将取得key/value pair的第二成分，也就是元素的value,而表达式 
        pos->first 
    会取得其（constant) key。 

++pos vs. pos++ 

    注意，这里使用前置式递增（preincrement）,因为它比后置式递增（postincrement）效率髙。后者内部需要一个临时对象，因为它必须存放迭代器的原本位置并返回之，所以一般情况下最好使用++pos,
    不要pos++。也就是说，你应该避免这么写：
        for(pos = coll.begin();pos != coll.end();pos++){
                                                        //OK
        }
    这种效率改善几乎总是无关紧要的。所以，不要把这里的推荐解读为“你应该竭尽所能不计代价地做任何事情，只为了如此微小的效率损失”。程序的可读性，以及可维护性，远比效率优化重要。此处的重点是，
    在本例中你不会因为选用前置式累加而不选用后置式累加而付出代价，那么，当然我们宁可选择前置累加（和前置式递减）。

cbegin()和cend()
    
    自C++11开始，我们可以使用关键字auto (见3.1.2节第14页）代替迭代器的精确类型（前提是你在迭代器声明期间就初始化，使其类型可以取决干初值）。因此如果我们S接以 beginO初始化迭代器，就可以使用auto声明其类型： 
    for (auto pos = coll.begin(); pos != coll.end(); ++pos){
        cout << *pos << ' ' ;
    } 
    如你所见，使用auto的优点之一就是，程序比较浓缩精简。如果没有auto,在循环内声明 迭代器的动作应该如下： 
    for (list<char>::const_iterator pos = coll.begin()； 
        pos ！*coll.end();   
        ++pos) { 
        cout << *pos << ' ';
        } 
    另一个优点是，采用这种循环写法，万一容器类型有所改变，程序整体仍能保持较佳的强壮性。然而其缺点是，迭代器丧失常量性（constness），可能引发“计划外的赋值”风险。 因为 
        auto pos = coll.begin() 
    会使pos成为一个非常量迭代器，此乃因为begin()返回的是个类型为cont::iterator的对象。为确保仍可使用常量迭代器，自C++11起容器提供cbegin()和cend(),它们返回一个类型为cont::const.iterator的对象。 
    现在我来总结改善方案。自C++11起，一个允许“迭代容器内所有元素”的循环如果不使用range-based for循环，看起来应如下： 
    for (auto pos = coll.cbegin(); pos != coll.cend(); ++pos) {
        ...
    }

Range-Based for 循环vs.迭代器

    介绍过迭代器之后，我们可以解释range-based for循环其实不过是个便捷接口，用来迭代它“所接受到的集合区间”内的每一个元素。在循环体内，真实元素被“当前迭代器所指向value”初始化。
        因此
        for(type elem : coll){
            ...
        }
    被解释为
        for(auto pos = coll.begin(),end = coll.end();pos!=end;++pos){
            type elem = *pos;
            ...
        }
    现在我们可以了解为什么声明elem为一个constan reference可以避免非必要复制了。如果不那么做，elem会被初始化为*pos的拷贝。


    






