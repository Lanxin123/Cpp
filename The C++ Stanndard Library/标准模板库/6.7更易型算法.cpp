更易型算法（Manipulating Algorithm) 

    译注：manipulating algorithm是指会“移除或重排,或修改”元素的算法，见6.7.2节第221 页。该页亦出现另一个同义术语：modifying algorithm。有些书籍（例如Generic Programming and the STL) 采用mutating algorithm一词。我将这些相同意义的术语都译为“更易型” 算法。 

    到目前为止，我已经介绍了 STL作为框架（framework）的整个概念：容器表述“管理集合数据”的种种不同概念，算法在这些集合内执行读取和涂写操作，迭代器则是容器和算法之间的黏合剂，由容器提供，允许你迭代所有元素，也允许你选择不同的方向，或采用特殊模式（如附加模式〔appending mode〕）。 
    然而现在，是说明STL框架的 BUT 的时候了：现实中存在某些限制和某些需要避开的事物，这都是应该让你知道的，其中许多和容器/元素的改动（modification）相伴相生。 
    有些算法（更明确地说是那些可能移除元素的算法）会改动标的区间。如果这种情况发生，特殊规定会起作用，本节将解释它们。这些特殊规则令人惊讶，并且显示了 STL  “把容器和算法以巨大的弹性分离”所付出的代价。

移除（Removing）元素

    算法remove()自某个区间删除元素。然后如果你用它来删除容器内的所有元素，其行为肯定会让你吃惊。例如：

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <list>
#include <iterator>
using namespace std;

int main()
{
	list<int> coll;
	
	//insert elements from 6 to 1 and 1 to 6
	for (int i = 1;i<=6;++i)
	{
		coll.push_front(i);
		coll.push_back(i);
	}
	
	//print all elements of the collection
	cout << "pre:  ";
	copy(coll.cbegin(), coll.cend(),				//source
		ostream_iterator<int>(cout, " "));	//destination
	cout << endl;
	
	//remove all elements with value 3
	remove(coll.begin(), coll.end(),				//range
		3);														//value

	//print all elements of the collection
	cout << "post: ";
	copy(coll.cbegin(), coll.cend(),				//source
		ostream_iterator<int>(cout, " "));	//destination
	cout << endl;
}
    对STL缺乏深层认识的人，看了这个程序，必然认为所有数值为3的元素都会从集合中被移除。然而，程序的输出却如下：
    pre:  6 5 4 3 2 1 1 2 3 4 5 6
    post: 6 5 4 2 1 1 2 4 5 6 5 6

    remove()并没有改变集合中的元素数量。cend()返回的是当初那个终点，size()返回的还是当初那个大小。不过某些事情还是有了变化：元素的次序改变了，有些元素被删除了。数值为3的元素被其后的元素覆盖了（如图6.10所示）。至于集合尾端那些未被覆盖的元素，
    原封不动---但从逻辑角度来说，那些元素已经不属于这个集合了。

    事实上，这个算法返回了一个新终点。你可以利用该终点获得新区间、缩减后的容器大小，或是获得被删除元素的个数。看看下面这个改进版本：
#include "stdafx"
#include <algorithm>
#include <iterator>
#include <list>
#include <iostream>
using namespace std;

int main()
{
    list<int> coll;

    // insert elements from 6 to 1 and 1 to 6
    for (int i=1; i<=6; ++i) {
        coll.push_front(i);
        coll.push_back(i);
    }

    // print all elements of the collection
    copy (coll.cbegin(), coll.cend(),
          ostream_iterator<int>(cout," "));
    cout << endl;

    // remove all elements with value 3
    // - retain new end
    list<int>::iterator end = remove (coll.begin(), coll.end(),
                                      3);

    // print resulting elements of the collection
    copy (coll.begin(), end,
          ostream_iterator<int>(cout," "));
    cout << endl;

    // print number of removed elements
    cout << "number of removed elements: "
         << distance(end,coll.end()) << endl;

    // remove "removed" elements
    coll.erase (end, coll.end());

    // print all elements of the modified collection
    copy (coll.cbegin(), coll.cend(),
          ostream_iterator<int>(cout," "));
    cout << endl;
}

    在这个版本中，remove()的返回值被陚予迭代器end: 
        list<int>::iterator end = remove (coll.begin(), coll.end(),3)； 
    这个end正是“被修改集合”经过元素移除动作后的新逻辑终点。接下来你便可以拿它当作新终点使用： 
        copy (coll.begin() , end, 
                ostream_iterator<int>(cout," "))； 
    另一种可能用法是，借由测定集合之逻辑终点和实际终点之间的距离，获得“被删除元素” 的数量: 
        cout << "number of removed elements:" 
            <<distance (end, coll.endO) << endl; 
    在这里，原本为了迭代器而设计的辅助函数distance()发挥了作用。它的功用是返回两个迭代器之间的距离。如果这两个迭代器都是随机访问迭代器（random-access iterator）, 你可以使用operator-直接计算其距离。不过本例所用的容器是list，只提供双向迭代器 (bidirectional iterator)。
    关干 distance()，详见 9.3.3 节第 445 页。
    如果真想把那些被删除的元素斩草除根，你必须调用该容器的相应成员函数。容器所提供的成员函数erase()正适用于此目的。erase()可以删除“实参所指示的区间”内的全部元素：
        coll.earse(end,coll.end());
    下面是整个程序的完整输出：
     6 5 4 3 2 1 1 2 3 4 5 6
     6 5 4 2 1 1 2 4 5 6
     number of removed elements: 2
     6 5 4 2 1 1 2 4 5 6
    如果你需要以单一语句来删除元素，可以这么做：
     coll.erase(remove(coll.begin(),coll.end(),
                        3),
                coll.end());
    为何算法不自己调用erase()呢？这个问题正好点出STL为获取弹性而付出的代价。通过“以迭代器为接口”，STL将数据结构和算法分离开来。然而迭代器只不过是“容器内部某一位置”的抽象概念而已。一般说来，迭代器对自己所属的容器一无所知。任何“以迭代器汸问容器”的算法，都不得（无法）通过迭代器调用容器类所提供的任何成员函数。 
    这个设计导致一个重要结果：算法的操作对象不一定得是“容器内的全部元素”所形成的区间，而可以是那些元素的子集。甚至算法可作用于一个“并未提供成员函数eraseO” 的容器身上（array就是个例子）。所以，为了达成算法的最大弹性，不强求“迭代器必须了解其容器细节”还是很有道理的。 
    注意，通常并无必要删除那些“已被移除”的元素。以逻辑终点取代容器的实际终点，通常就足以应对现实情况。你可以拿这个逻辑终点搭配任何算法演出。

更易Associative（关联式）和 Unordered（无序）容器

    更易型算法（指那些会移除〔remove〕、重排〔reorder〕、修改〔modify〕元素的算法）若用于关联式容器或无序容器，会出问题。关联式和无序容器不能被当作操作目标，原因很简单：如果更易型算法用干关联式和无序容器身上，会改变某位置上的值，进而破坏容器本身对次序的维护（对关联式容器而言是其已排序（sorted）特性，
    对无序容器而言则是其hash函数的运算结果)。为避免累及内部次序，关联式容器和无序容器的所有迭代器均被声明为指向常量（不变量）的value或key。如果你更动关联式容器或无序容器的元素，会导致编译错误。
    这使你无法在关联式容器身上运用移除型（removing）算法，因为这类算法实际上悄悄更易了元素：“被移除元素”被其后的“未被移除无素”覆盖。 
    现在问题来了，如何从关联容器和无序容器中删除元素？好吧，很简单：调用它们的成员函数！每一种关联式容器和无序容器都提供用以移除元素的成员函数。例如你可以调用erase():

#include "stdafx.h"
#include <set>
#include <algorithm>
#include <iterator>
#include <iostream>
using namespace std;

int main()
{
    // unordered set with elements from 1 to 9
    set<int> coll = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // print all elements of the collection
    copy (coll.cbegin(), coll.cend(),
          ostream_iterator<int>(cout," "));
    cout << endl;

    // Remove all elements with value 3
    // - algorithm remove() does not work
    // - instead member function erase() works
    int num = coll.erase(3);

    // print number of removed elements
    cout << "number of removed elements: " << num << endl;

    // print all elements of the modified collection
    copy (coll.cbegin(), coll.cend(),
          ostream_iterator<int>(cout," "));
    cout << endl;
}
    注意，容器类提供了多个不同的erase()成员函数。其中一种形式是以“欲删除之元素值” 为唯一实参，并返回被删除元素的个数（见8.7.3节第418页)。当然，在禁止元素重复的容器中（例如set、mapunordered_set 和 unordered_map)其返回永远只能是0或1。 
    本例程序输出如下： 
    1 2 3 4 5 6 7 8 9 
    number of removed elements： 1 
    1 2 4 5 6 7 8 9

算法 vs. 成员函数

    就算我们符合种种条件，得以使用某个算法，那也未必就一定是好。容器本身可能提供功能相似而效能更佳的成员函数。 
    一个极佳例子便是对list的元素调用remove()。算法本身并不知道它工作于list身上， 因此它在任何容器中都一样，做些四平八稳的工作：改变元素值，从而重新排列元素。如果它移除第一个元素，后面所有元素就会分别被设给各自的前一个元素。
    这就违反了list的主要优点——借由修改链接（link）而非实值（value） 来安插、移动、移除元素。 
    为了避免这么槽糕的表现，list针对所有更易型算法提供了一些对应成员函数。是的， 如果你使用list,就应该使用这些成员函数。此外请注意，这些成员函数真的移除了 “被移除”元素（译注：而不像先前所说只是某种搬移而已)，一如下例所示：
#include"stdafx.h"
#include <list>
#include <algorithm>
using namespace std;

int main()
{
    list<int> coll;

    // insert elements from 6 to 1 and 1 to 6
    for (int i=1; i<=6; ++i) {
        coll.push_front(i);
        coll.push_back(i);
    }

    // remove all elements with value 3 (poor performance)
    coll.erase (remove(coll.begin(),coll.end(),
                       3),
                coll.end());

    // remove all elements with value 4 (good performance)
    coll.remove (4);
}
    如果高效能是你的首要目标，你应该总是优先选用成员函数。问题是你必须先知道，某个容器确实存在效能明显突出的成员函数。面对list却使用removeO算法，决不会收到任何警告信息或报错通知。然而如果你决定使用成员函数，
    一旦换用另一种容器，就不得不更改代码。第11章的算法参考章节中，如果某个成员函数的效能优干某个算法，我会明白指










