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
    //真的理解了。。。

关联式（Associative）及无序（Unordered）容器的更多实例

    介绍了迭代器后，现在我们可以引出一些实例，使用关联式容器，但不使用C++11新添的语言特性如range-based for循环、auto和初值列（initializer list）。这里使用的特性对某些特殊需求而言也是有用的。 

使用C++11之前的Set 

    第一个例子示范如何安插元素进入set内，并使用迭代器打印它们（不使用C++11新增特性)：

#include "stdafx.h"
#include <set>
#include <iostream>
using namespace std;

int main()
{
	//type of the collection
	typedef std::set<int> IntSet;

	IntSet coll;			//set container for int values

	//insert elements from 1 to 6 in arbitrary order
	//-note that there are two calls of insert() with value 1
	coll.insert(3);
	coll.insert(1);
	coll.insert(5);
	coll.insert(4);
	coll.insert(1);
	coll.insert(6);
	coll.insert(2);

	//print all elements
	//-iterate over all elements
	IntSet::const_iterator pos;
	for (pos = coll.begin(); pos != coll.end(); ++pos) {
		std::cout << *pos << ' ';
	}
	std::cout << std::endl;
}
    一如以往，include指示符定义了set的所有必要类型和操作：
        #include<set>
    既然容器的类型要用到好几次，不妨先定义一个短一点的名字：
        typedef set<int> IntSet;
    这个语句定义的IntSet类型，其实就是“元素类型为int的一个set” 。这种类型有默认的排序准则，以操作符 < 为依据，对元素进行排序。 这意味着元素将以递增方式排列。
    如果希望以递减方式排列，或希望使用一个全然不同的排序准则，可以将该准则传入作为第二个template参数。下面的例子就是将元素以递减方式排列：
        typedef set<int,greater<int>> IntSet;
    以上所用的greater<>是一个预定义的函数对象（function object），6.10.2节第239页会讨论它。如果排序准则只使用对象的某部分数据如ID，做法见10.1.1节第476页。
    //老样子。。。一定要看。。。。
    所有关联式容器都提供了insert()成员函数，用来安插新元素：
        coll.insert(3);
        coll.insert(1);
        ...
    C++11 允许我们这么写：
        coll.insert({3,1,5,4,1,6,2});
    每一个被安插的元素就会根据排列准则自动被放到正确位置。不可使用序列容器才会有的push_back()或push_front()，它们在这里毫无意义，因为set不允许你指定新元素的位置。
    所有元素（不论任何次序）安插完毕后，容器的状态如图6.6所示。元素以排序后（sorted）的状态存放于内部tree结构中。任何元素（节点）的左子树的所有元素都小于右子树的所有元素（此处“小于是指就当前排序准则而言”）。
    Set不允许存在重复元素，所以容器里头只有一个1。//二叉查找树
    
    运用先前list例中所用的相同循环来打印set内的元素，以迭代器走访全部元素，并逐一打印：
        InSet::const_iterator pos;
        for(pos = coll.begin();pos!=coll.end();++pos){
            cout<<*pos<<' ';
        }
    
        //ps：依照的是层序遍历
    由于迭代器是容器定义的，所以无论容器内部结构如何复杂，它都知道如何行事。举个例子，如果迭代器指向第三元素，操作符++变回将它移动到上端的第四元素，再一次++，便会将它移动到下方第五个元素（如图6.7所示）。程序输出如下：
        1 2 3 4 5 6
    如果你想使用multiset而不是set, 唯一需要改变的就是容器类型（set和multiset的定义被置于同一个头文件内）：
        typedef multiset<int> IntSet； 
    由于multiset允许元素重复存在，因此可包含两个数值皆为1的元素。输出结果如下：
        1 1 2 3 4 5 6

使用无序的Multiset

    让我们仔细观察另一个例子，遍历一个无序（unordered）multiset的所有元素时会发生什么事：
#include "stdafx.h"
#include <unordered_set>
#include <iostream>
using namespace std;

int main()
{
	//unordered multiset container for int values
	std::unordered_multiset<int> coll;

	//insert some elements
	coll.insert({ 1,3,5,7,11,13,17,19,23,27,1 });

	//print all elements
	for (auto elem : coll)
	{
		std::cout << elem << ' ';
	}
	std::cout << std::endl;

	//insert one more element
	coll.insert(25);

	//print all elements again
	for (auto elem : coll) {
		std::cout << elem << ' ';
	}
	std::cout << std::endl;
}
    这些元素的顺序是不明确的，取决于hash table的内部布局，也取决于hashing函数。甚至即使元素先被排序后才被安插，它们在容器内还是有着不明确的顺序。
    如果加入更多元素，也可能改变既有元素的次序。下面只是这个程序的一个可能的输出结果：
        11 23 1 1 13 3 27 5 17 7 19
        23 1 1 25 3 27 5 7 11 13 17 19
    如你所见，这样的次序实在是杂乱无规则科研，而且在你的平台上执行这个程序可能获得又不一样的次序。就算指添加一个元素，也可能全盘改变次序。唯一保证的是，内容（value）相同的元素会相邻。
        当我们遍历所有元素并打印它们：
        for(auto elem : coll){
            std::cout<<elem<<' ';
        }
    相当于
        for(auto pos = coll.begin();pos != coll.end();++pos){
            auto elem = *pos;
            std::cout<<elem<<' ';
        }
    再一次，for循环内的迭代器pos拥有一个由容器提供的类型，所以它“知道”如何逐一遍历所有元素。基于上一页的输出结果，当程序中的迭代器首次被用来打印所有元素时，那个无序（unordered） multiset的内部结构可能正如图6.8所示。 
        如果我们改用无序（unordered） set： 
        std:：unordered_set<int> coll; 
    由于它不允许元素重复，输出可能如下： 
        11 23 1 13 3 27 5 17 7 19 
        11 23 1 13 25 3 27 5 17 7 19 

迭代器种类（Iterator Category) 
    除了基本操作，迭代器还有其他能力。这些能力取决f容器的内部结构。STL总是只提供效率比较出色的操作，因此如果容器允许随机访问（例如vector或deque）,它们的迭代器也必定能进行随机操作（例如直接让迭代器指向第5元素）。 
        根据能力的不同，迭代器被划分为五种不同类别。STL预先定义好的所有容器，其迭代器均属于以下三种分类：
    1.前向迭代器（Forward iterator） 只能够以累加操作符（increment operator）向前迭代。 
      Class forward_list的迭代器就属此类。其他容器如 unordered_set、unordered_multiset、unordered_map和unordered_multimap也都至少是此类别（但标准库其实为它们提供的是双向迭代器〔forward iterator〕，见7.9.1节第357页）。//什么鬼。。。 
    2.双向迭代器（Bidirectional iterator）顾名思义它可以双向行进：以递增(increment) 运算前进或以递减（decrement）运算后退。
      list、set、multiset、map和multimap提供的迭代器都属此类。 
    3.随机访问迭代器（Random-access iterator）它不但具备双向迭代器的所有厲性，还具备随机访问能力。更明确地说，它们提供了迭代器算术运算的必要操作符（和寻常指针的算术运算完全对应)。
      你可以对迭代器增加或减少一个偏移量、计算两迭代器间的距离，或使用 < 和 > 之类的relational (相对关系）操作符进行比较。vector、deque、 array和string提供的迭代器都属此类。 
除此之外，STL还定义了两个类别： 
    •输入型迭代器（Input iterator）向前迭代时能够读取/处理value。Input stream迭代器就是这样一个例子（见6.5.2节第212页)。 
    •输出型迭代器（Output iterator）向前迭代时能够涂写value。Inserter(见6.5.1节第210 页）和output stream迭代器（见6.5.2节第212页）都属此类。 
    
    9.2节第433页洋细讨论上述每一种类别。  

    为了写出尽可能与容器类型无关的泛型代码，你最好不要使用随机访问迭代器 （random-access iterator）的特有操作。例如，以下动作可在任何容器上运作： 
        for (auto pos = coll.beginO; pos !=coll. end()；++pos){
            ...
        }
    但下面这样的代码就不是所有容器都使用了：
        for (auto pos = coll.beginO; pos < coll .end ()； ++pos) {
            ...
        }
    
    两者的唯一区别在于测试循环条件时，第二例使用 <，第一例使用 !=。要知道，只有 random-access iterator才支持operator < ，所以第二例的循环对于list、set和map无法运作。为了写出适用于任何容器如泛型代码，你该使用operator != 而非operator < 。
    不过如此一来，代码的安全性可能稍有损伤，因为如果pos的位置在end()的后面，你未必便能发现（关于STL使用上的可能错误，见6.12节第245页）。究竟使用哪种方式，取决干当时情况， 取决于个人经验，取决于你。 
    为了避免误解，我再强调一句。我说的是类别（category）, 不是类（class）。所谓类别，只是定义迭代器的能力，无关乎迭代器的类型（type）。STL的泛型概念可以在纯抽象层面上运作，也就是说，任何东西只要行为“像” 一个双向迭代器，它就是一个双向迭代器。

//学东西要先知面后知片，不可片面。。。。

