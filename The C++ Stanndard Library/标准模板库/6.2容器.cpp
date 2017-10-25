容器（Container）
    容器用来管理一大群元素。为了适应不同需要，STL提供了不同的容器，如图6.2所示。

    总的来说，容器可分为三大类：
    1.序列式容器（Sequence container），这是一种有序（ordered）集合，其内每个元素均有确凿的位置---取决干插人时机和地点，与元素值无关。如果你以追加方式对一个集合置入6个元素，它们的排列次序将和置入次序一致。
    STL提供了5个定义好的序列式容器：array、vector、deque、list和forward_list。 
    2.关联式容器（Associative container） , 这是一种已排序(sorted}集合，元素位置取决于其value（或key---如果元素是个key/value pair）和给定的某个排序准则。如果将6个元素置入这样的集合中，它们的值将决定它们的次序，
    和插入次序无关。STL提供4个关联式容器：set、multiset、map和multimap。 
    3.无序容器（Unordered （associative） container），这是一种无序集合（unordered collection）, 其内每个元素的位置无关紧要，唯一重要的是某特定元素是否位于此集合内。元素值或其安插顺序，都不影响元素的位置，
    而且元素的位置有可能在容器生命中被改变。如果你放6个元素到这种集合内，它们的次序不明确，并且可能随时间而改变。STL内含4个预定义的无序容器：unordered_set、unordered_multiset、unordered_map和unordered_multimap。

    Unordered容器由TR1引入，对术语带来了一些混淆。官方说法是“unordered容器被归类为一种unordered associative容器”，这导致associative容器的意义变得有点不清不楚：它到底是（ordered）associative容器和
    unordered associative容器的整体性术语，抑或是 unordered容器的对立面？答案取决于上下文语境。本书中当我说associative容器时，我的意思是“旧式的” sorted associative容器，而当我说unordered容器时，
    我不会在中间加上 associative这个字。 
    这里介绍的3个容器分类，只是根据“元素次序”而完成的一种逻辑分类。从这个角度看，一个associative容器也可以被视为一个特殊种类的sequence容器，因为sorted容器有能力根据某个排序准则来安排元素次序。
    有可能你期待的正是这个，特别是如果你曾经用过其他程序库或集合类（collection class）, 例如Smalltalk或NIHCL所提供的那些，它们的sorted集合乃是派生自ordered集合。然而，STL的集合类型彼此之间完全不同，
    有着十分不同的实现，彼此没有派生关系。我们即将看到： 
    Sequence容器通常被实现为array或linked list。 
    Associative容器通常被实现为binary tree。 
    Unordered容器通常被实现为hash table。 
    严格地说，C++标准库并未规定任何容器必须使用任何特定的实现，然而由于C++ standard对于行为和复杂度的明确要求，使得这方面的变化余地不多。所以现实中各实现之间只可能存在微小细节上的差异。 
    当你面临如何选择适当的容器时，排序以外的能力也必须考虑。事实上，关联式容器 自动对其元素排序，并非意味着它们就是用来排序的。你也可以对序列式容器的元紊徘序。 自动徘序带来的主要优势是，查找元素时可获得较佳效率。
    更明确地说，你可以放心使用 二分査找法（binary search）, 它具备对数（logarithmic）复杂度，而非线性复杂度。这是什么意思呢？如果你想在1000个元素中查找某元素，平均而言只需10次比较，而非500次比较。因此自动排序只不过是关联式容器的一个（好的）副作用而已。 
    
    下面各小节详细讨论各种容器类：包括容器的典型实现，及其好处和缺点。（译注：严格说来，C++ standard并未定义某一种容器的具体实现。）第7章会谈到容器类的确切行为， 描述它们共有和特有的能力，并详细分析其成员函数。7.12节第392页详细讨论了何时使用哪一种容器。    

序列式容器（Sequence Container）

    STL内部预先定义好了一下序列式容器：
    Array (其 class 名为array) 
    Vector 
    Deque 
    List (singly/doubly linked)
    以下讨论从vector开始，因为array是TR1新引入的，进入C++标准库的时间比较短，而且它有一些特殊属性，与其他STL容器不共通。

    Vector
    Vector将其元素置于一个dynamic array中管理。它允许随机访问，也就是说，你可以利用索引直接访问任何一个元素。在array尾部附加元素或移除元素都很快速，但是在array的中段或起始段安插元素就比较费时。
    因为安插之后所有元素都必须移动，以保持原本的相对次序。
        以下例子这对整数类型定义一个vector，插入6个元素，然后打印所有元素：
#include "stdafx.h"
#include <iostream>
#include <vector>
using namespace std;

int main()
{
	vector<int> coll;		//vector container for integer elements

	//append elements with values 1 to 6
	for (int i = 1; i <= 6; ++i) {
		coll.push_back(i);
	}

	//print all elements followed by a space
	for (int i=0;i<coll.size();++i)
	{
		cout << coll[i] << ' ';
	}
	cout << endl;

}

    Vector的头文件通过以下语句包含进来：
        #include<vector>
    以下声明式建立了一个“元素类型为int”的vector:
        vector<int> coll;
    由于没有任何初始化参数，所以default构造函数将它构造为一个空集合 。push_back()为容器附加新元素:
        coll.push_back(i);
    所有序列式容器都提供这个成员函数，因为尾附一个元素永远是可能的，而且效率相当高。
    size()成员函数返回容器的元素个数：
        for(int i=0;i<coll.size();++i){
            ...
        }
    所有容器类都提供这个函数，唯一例外是singly linked list（class forward_list）。
    你可以通过subscript（下标）操作符[]，访问vector内的某个元素:
        cout<<coll[i]<<' ';
    元素被写至标准输出装置，所以整个程序的输出如下：
        1 2 3 4 5 6
Deque
    所谓deque（发音类似“check”），是“double-ended queue”的缩写。它是一个dynamic array，可以向两端发展，因此不论在尾部或头部安插元素都十分迅速。在中间部分安插元素则比较费时，因为必须移动其他元素。
        以下例子声明了一个元素为浮点数的deque，并在容器头部安插1.1至6.6共6个元素，最后打印出所有元素。
    最后打印出所有元素。
#include "stdafx.h"
#include <iostream>
#include <deque>
using namespace std;

int main()
{
	deque<float> coll;		//deque container for floating-point elements

	//insert elements form 1.1 to 6.6 each at the front
	for (int i=1;i<=6;++i)
	{
		coll.push_front(i*1.1);		//insert at the front
	}

	//print all elements  followed by a space
	for (int i=0;i<coll.size();++i)
	{
		cout << coll[i] << ' ';
	}
	cout << endl;
}
    本例中，下面这一行将被deque的头文件包含进来：
    #include <deque>
    下面的声明式则是产生一个空的浮点数集合：
    deque<float> coll;
    push_front()函数用来安插元素:
        coll.push_front(i*1.1);
    push_front()会将元素安插于集合前端。注意，这种安插方式造成的结果是，元素排放次序与安插次序恰好相反，因为每个元素都安插于一个元素前面。程序输出如下：
        6.6 5.5 4.4 3.3 2.2 1.1
    你也可以使用成员函数push_back()在deque尾端附加元素。Vector并未提供push_front()， 因为其时间效率不佳（在vector头端安插一个元素，需要先移动全部元素）。
    一般而言，STL 容器只提供具备良好时间效率的成员函数，所谓“良好”通常意味着其复杂度为常量或对数，以免程序员调用性能很差的函数。

Array
    一个array对象乃是在某个固定大小的array（有时成为一个static array或C array）内管理元素。因此，你不可以改变元素个数，只能改变元素值。你必须在建立时就指明其大小。
    Array也允许随机访问，意思是你可以直接访问任何一个元素---只要你指定相应的索引。
        下面的例子定义出了一个array，元素是string：
#include "stdafx.h"
#include <iostream>
#include <string>
#include <array>
using namespace std;

int main()
{
	//arrart container of 5 string el
	array<string, 5> coll = { "hello","world" };

	//print each element with its index on a line
	for (int i = 0; i < coll.size(); ++i) {
		cout << i << ": " << coll[i] << endl;
	}
}
    下面这行将array的头文件包含进来：
        #include<array>
    以下声明式会建立一个array，带有5个类型为string的元素：
        arra<string,5> coll
    默认情况下这些元素都被元素的default构造函数初始化。这意味着，对基础类型而言，初值不明确（undefined）
    然而本程序用了一个初值列（initializer list），这东西允许我们以一系列值将对象初始化于创建期。自C++11起这种初始化方法受到每一种容器的支持，所以当然我们也可以在vector和deque中使用它。
    既然如此，基础类型用的是zero omoyoa;ization， 意味着基础类型保证被初始化为0。 
    程序中借由size()和下标操作符[]，将所有元素联合其索引逐行写至标准输出装置。 整个程序输出如下：
    0：hello
    1：world
    2：
    3：
    4：
    如你所见，这个程序输出5行，因为我们定义的array带有5个string。根据初值列的设定，头2个元素被初始化为"hello"和"world"，其余元素则拥有默认值，也就是空字符串。
    注意，元素个数是array类型的一部分。因此array<int,5>和array<int,10>是两个不同的类型，你不能对此二者进行赋值或比较。

List
    从历史角度看，我们只有一个list class。然而自C++11幵始，STL竟提供了两个不同的 list 容器：class list<> 和 class forward_list<>。因此，list 可能表示其中某个 class,或者是个总体术语，
    代表上述两个list class。然而就某种程度来说，forward list只不过是受到更多限制的list，现实中二者的差异并不怎么重要。因此当我使用术语list，通常我指的是class list<>,
    它的能力往往超越class forward_list<>。如果特別需要指出class forward_list<>,我会使用术语forward list。所以本节讨论的是寻常的list,是一开始就成为STL一部分的那个东西。 
    list<>由双向链表(doubly linked list)实现而成。这意味着list内的每个元素都以一部分内存指示其前导元素和后继元素。 
    List不提供随机访问，因此如果你要访问第10个元素，你必须沿着链表依次走过前9 个元素。不过，移动至下一个元素或前一个元素的行为，可以在常量时间内完成。因此一般的元素访问动作会花费线性时间，
    因为平均距离和元素数量成比例。这比vector和deque 提供的摊提式(amortized)常量时间，效率差很多。 
    List的优势是：在任何位置上执行安插或删除动作都非常迅速，因为只需改变链接(link)就好。这表示在list中段处移动元素比在vector和deque快得多。 
    以下例子产生一个空list,用以放置字符，然后将‘a’至‘z’，的所有字符插人其中，利用循环海次打印并移除集合的第一个元素，从而打印出所有元素：

#include "stdafx.h"
#include <list>
#include <iostream>
using namespace std;

int main()
{
	list<char> coll;		//list container for character elements

	//append elements from 'a' to 'z'
	for (char c = 'a';c<='z';++c)
	{
		coll.push_back(c);
	}
	//print all elements:
	//~use range-base for loop
	for (auto elem : coll)
	{
		cout << elem << ' ';
	}
	cout << endl;
}












    







