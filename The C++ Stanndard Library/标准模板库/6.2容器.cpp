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
    就像先前的例子一样，头文件<list>内含list的声明。以下定义一个“元素类型为字符”的list： 
    list<char> coll; 
    为了打印所有元素，我使用一个range-based for循环，这种循环自C++11之后提供，允许对每个元素执行指定的语句。List并不提供作为“元素直接访问”之用的操作符[]。这是因为list并不提供随机访问，因此操作符[]会带来低下的效率。 
    在此循环中，当前正被处理的coll元素的类型被声明为auto。因此elem的类型被自动推导为char，因为coll是个char集合（auto 类型推导详见3.1.2节第14页）。另一种做法是明白声明elem的类型： 
    for (char elem ： coll) { 
        ...
    }
    注意，elem永远是当前正被处理的元素的一个拷贝（copy)。虽然你可以改动它，但其影响只限于“针对此元素而调用的语句”，coll内部并没有任何东西被改动。如果你想改动传入的集合的元素，你必须将elem声明为一个非常量的reference: 
    for (auto& elem : coll) { 
    ...  //any modification of elem modifies the current element in coll 
    就像函数参数那样，通常你应该使用一个常量reference以避免发生一次copy操作。因此,下面的function template输出“被传入的容器内的所有元素”： 
    template <typename T> 
    void printElements (const T& coll) 
    for (const auto& elem : coll) { 
        std::cout << elem << std::endl；
    } 
    在C++11之前你必须使用迭代器来访问所有元素。稍后才会介绍迭代器。
    在C++11之前，打印所有元素的另一种做法（不使用迭代器）是逐一地“打印而后移 除”第一元素，直到此list之中不再有任何元素： 

#include "stdafx.h"
#include <list>
#include <iostream>
using namespace std;

int main()
{
	list<char> coll;		//list container for character elements

	//append elements from'a' to 'z'
	for (char c = 'a'; c <= 'z'; ++c) {
		coll.push_back(c);
	}

	//print all elements
	//~while there are elements
	//~print and remove the first element
	while (!coll.empty())
	{
		cout << coll.front() << ' ';
		coll.pop_front();
	}
	cout << endl;
}

    成员函数empty()的返回值告诉我们容器中是否还有元素。只要这个函数返回false(也就是说，容器内还有元素），循环就继续进行： 
    while (!coll .empty()) { 
        ...
    }
    循环之内，成员函数front()返回第一个元素： 
    cout << coll.front() << ' '； 
    pop_front()函数删除第一个元素： 
    coll.pop_front()； 
    注意，pop_front()并不会返回被删除元素，所以你无法将上述两个语句合而为一。 
    程序的输出结果取决于所用的字集。如果是ASCII字集，输出如下： 
    a...z//就是a到z

Forward List 

    自C++11之后，C++标准库提供了另一个list容器：forward list。 forward_list<>是一个由元素构成的单向（singly) linked list。 就像寻常list那样，每个元素有自己一段内存， 为了节省内存，它只指向下一元素。
    因此，forward list原则上就是一个受限的list,不支持任何“后退移动”或“效率低下”的操作。基于这个原因，它不提供成员函数如push_back()乃至size()。因此，list
    现实中，这个限制比乍听之下甚至更尴尬棘手。问题之一是，你无法查找某个元素然后删除它，或是在它的前面安插另一个元素。因为，为了删除某个元素，你必须位于其前一元素的位置上，因为正是那个元素才能决定一个新的后继元素。
    也因此，forward list对此 提供了一个特殊成员函数，见7.6.2节第305页。
        下面是forward list的一个小例子:
#include "stdafx.h"
#include <forward_list>
#include <iostream>
using namespace std;

int main()
{
	//create forward-list container for some prime numbers
	forward_list<long> coll = { 2,3,5,7,11,13,17 };

	//resize two times
	//- note: poor performance
	coll.resize(9);
	coll.resize(10, 99);

	//print all elements:
	for (auto elem : coll)
	{
		cout << elem << ' ';
	}
	cout << endl;
}
    一如以往，我们使用forward list的头文件<forward_list>定义一个类型为forward_list的集合，以长整数（long integer） 为元素，并以若干质数为初值： 
    forward_list<long> coll = { 2, 3， 5, 7, 11, 13, 17 >; 
    然后使用resize()改变元素个数。如果数量成长，你可以传递一个额外参数，指定新元素值。否则就使用默认值（对基础类型而言是0)。注意，resize()是一个昂贵的动作，
    它具备线性复杂度，因为为了到达尾端你必须一个一个元素地前进，走遍整个list。不过这是一个几乎所有sequence容器都会提供的操作，就暂时忽略它那可能的低劣效率吧。
    只有array不提供resize()，因为其大小固定不变。 
        像先前对待list那样，我们使用一个range-based for循环打印所有元素。输出如下： 
        2 3 5 7 11 13 17 0 0 99

关联式容器（Associative Container) 
    关联式容器依据特定的排序准则，自动为其元素排序。元素可以是任何类型的value，也可以是key/value pair，其中key可以是任何类型，映射至一个相关value，而value也可以是任意类型。
    排序准则以函数形式呈现，用来比较value,或比较key/value中的key。默认情况下所有容器都以操作符<进行比较，不过你也可以提供自己的比较函数，定义出不同的排序准则。 
    通常关联式容器由二叉树（binary tree) 实现出来。在二叉树中，每个元素（节点）都有一个父节点和两个子节点；左子树的所有元素都比自己小，右子树的所有元素都比自己大。
    关联式容器的差别主要在于元素的种类以及处理重复元素时的方式（态度)。 
    关联式容器的主要优点是，它能很快找出一个具有某特定value的元素，因为它具备对数复杂度（logarithmic complexity）, 而任何循序式容器的复杂度是线性。
    因此，使用关联式 容器，面对1000个元素，平均而言你将有10次而不是500次比较动作。然而它的一个缺点是，你不能直接改动元素的value,因为那会破坏元素的自动排序。 
        下面是STL定义的关联式容器： 
    Set元素依据其value自动排序，每个元素只能出现一次，不允许重复。 
    
    Multiset和set的唯一差别是：元素可以重复。也就是multiset可包括多个“value相同”的元素。 
    
    Map每个元素都是key/value pair,其中key是排序准则的基准。每个key只能出现一次，不允许重会。Map也可被视为一种关联式数组（associative array），也就是“索引可为任意类型”的数组。 
    
    Multimap和map的唯一差别是：元素可以重复，也就是multimap允许其元素拥有相同的key。Multimap可被当作字典（dictionary)使用。 

    所有关联式容器都有一个可供选择的template实参，指明排序准则：默认采用操作符 <。排序准则也被用来测试等同性（equivalence）: 如果两个元素的value/key互不小干对方，则两者被视为重复。 
    你可以将set视为一种特殊的map:元素的value等同于key。实际产品中所有这些关联式容器通常都由二叉树（binary  tree） 实现而成。 

Set 和 MuUiset 实例 

    下面是第一个例子，使用multiset:
#include "stdafx.h"
#include <string>
#include <set>
#include <iostream>
using namespace std;

int main()
{
	multiset<string> cities{
		"Braunschweig","Hanover","Frankfurt","New York",
		"Chicago","Toronto","Paris","Frankfurt"
	};
	
	//print each element:
	for (const auto& elem:cities)
	{
		cout << elem << "	";
	}
	cout << endl;

	//insert additional values:
	cities.insert({ "London","Munich","Hanover","Braunschweig" });

	//print each element:
	for (const auto& elem: cities)
	{
		cout << elem << "	";
	}
	cout << endl;

}

    包含头文件<set>后，我们可以声明cities是个以string为元素的multiset: 
        multiset<string> cities 
    声明之后，若干字符串以初值列（initializer list）形式成为元素初值并于稍后被安插到容器内。为打印所有元素，我们使用一个range-based for循环。
    注意我们声明元素为const auto&,意思是希望从容器推断元素类型并免除为“被此循环叫唤”的每个元素制造一份拷贝。 
    在这个容器内部，所有元素都已经过排序，、因此第一份输出如下： 
    Braunschweig  Chicago  Frankfurt  Frankfurt  Hanover  New York 
    Paris  Toronto 
    第二份输出如下： 
    Braunschweig   Braunschweig  Chicago  Frankfurt  Frankfurt  Hanover  Hanover  London  Munich  New York  Paris  Toronto
    如你所见，由于这里用的是一个multiset而不是set，因此允许元素重复。如果我们生命的是set而不是multiset，每个value就指挥被打印一次。
    如果我们用的是一个unordered multiset,元素的次序就无法明确得知了。

Map和Multimap实例
    下面的例子示范了如何使用map和multimap:
#include "stdafx.h"
#include <string>
#include <map>
#include <iostream>
using namespace std;

int main()
{
	multimap<int, string> coll;		//container for int/string values

	//insert some elements in arbitrary order
	//- a value with key 1 gets insert twice
	coll = { {5,"tagged"},
	{2,"a"},
	{1,"this"},
	{4,"of"},
	{1,"is"},
	{3,"multimap"} };

	//print all element values
	//- element member second is the value
	for (auto elem : coll)
	{
		cout << elem.second << ' ';
	}
	cout << endl;
}
    包含<amp>之后，我们声明一个map，其元素拥有一个int作为key和一个string作为value：
        multimap<int,string> coll;
    由于map和multimap的元素都是key/value pair，因此声明式、元素安插、元素访问等动作略有不同:
    •首先，为了初始化（或赋值或安插）元素，你必须传递key/value pair,本例以嵌套式初值列（nested initializerlist）完成，
     内层定义的是每个元素的key和value，外层定义出所有元素。因此{5,"tagged"}具体指明被安插的第一个元素内容。 
    •处理元素时你再一次需要和key/value pair打交道。每个元素的类型实际上是pair<const key,value>。Key之所以必须是常量，因为其内容如果被改动，会破坏元素的次序，
     而元素次序是由容器自动排序的。由于pair缺乏output操作符，你无法视它们为一个整体加以打印。因此你必须分别处理pair结构中的成员，他们分别名为first和second。
        下面的式子取得key/value pair的第二成分，也就是multimap元素的value：
        elem.second
        下面的式子取得key/value pair的第一成分，也就是multimap元素的key:
        elem.first
    最终，程序输出如下：
        this is a multimap of tagged strings
    C++11之前并未明确规定等效元素（equivalent element，也就是key相同的元素）的排列次序。所以在C++11之前，"this"和"is"的排列有可能与本书所显示的不同。但是
    C++11已经保证，新插入元素会被安插在multiset和multimap已有之等效元素的末尾。而且如果调用insert()、emplace()或erase()，这些等效元素的次序也保证稳定不变。

关联式容器的其他例子 

    6.2.4节第185页有一个示例，使用map做成一个所谓的关联式数組（associative array)。7.7节详细讨论set和multiset,附有更多例子。7.8节详细讨论map和multimap，也带有更多例子。 
    Multimap也可以用作字典（dictionary)。7.8.5节第348页有一个例子。

无序容器（Unordered Container）

    在无序（unordered）容器中，元素没有明确的排列次序。也就是如果安插3个元素，当你迭代容器内的所有元素时会发现，它们的次序有各种可能。如果安插第4个元素，
    先前3个元素的相对次序可能会被改变。我们唯一关心的是，某个特定元素是否位于容器内。甚至如果你有2个这种容器，其内有着完全相同的元素，元素的排列次序也可能不同。试着想象它是个袋子（bag)。 
    无序（unordered） 容器常以hash table实现出来（如图6.3所示），内部结构是一个“由 linked list组成”的array。通过某个hash函数的运算，确定元素落于这个array的位置。 
    Hash函数运算的目标是：让每个元素的落点（位置）有助于用户快速访问。

    任何一个元素，前提则是hash函数本身也必须够快。由于这样一个快速而完美的hash函数不一定存在（或不一定被你找到），抑或由于它造成 array 耗费巨额内存而显得不切实际，
    因此，退而求其次的hash函数有可能让多个元素落于同一位置上。所以设计上就让array的元素再被放进一个linked list中，如此一来array的每个位置（落点）就得以存放一个以上的元素。
    无序（unordered）容器的主要优点是，当你打算査找一个带某特定值的元素，其速度甚至可能快过关联式容器。事实上无序容器提供的是摊提的常量复杂度（amortized constant complexity），
    前提是你有一个良好的hash函数。然而提供一个良好的hash函数并非易事，你可能需要提供许多内存怍为bucket。 
        根据关联式容器的分类法，STL定义出下面这些无序容器：
    Unordered set是无序元素的集合，其中每个元素只可出现一次。也就是不允许元素重复。
    
    Unordered multiset和unordered set的唯一差别是它允许元素重复。也就是unordered multiset可能内含多个有着相同value的元素。 
    
    Unordered map的元素都是key/value pair。每个key只可出现一次，不允许重复。它也 可以用作关联式数组（associativearray),那是“索引可为任意类型”的array (详见6.2.4 节第185页)。 
    
    Unordered multimap 和 unordered map 的唯一差别是允许重复。也就是 unordered multimap 可能内含多个“拥有相同key”的元素。它可以用作字典（dictionary）。

    所有这些无序容器的class 都有若干可有可无的template实参，用来指明hash函数和等效准则（equivalence critieion），该准则被用来寻找给定值，以判断是否重复。默认的等效准则是操作符==。
    你可以把unordered set视为一种特殊unordered map，只不过其元素的value等同于key。现实中所有无序容器通常都使用hash table作为底层实现。

Unordered Set/Multiset 实例 
    下面是第一个例子，使用unordered multiset，元素是string:
#include "stdafx.h"
#include <unordered_set>
#include <string>
#include <iostream>
using namespace std;

int main()
{
	unordered_multiset<string> cities{
		"Braunschweig","Hanover","Frankfurt","New York",
		"Chicago","Toronto","Paris","Frankfurt"
	};

	//print each element:
	for (const auto& elem : cities) {
		cout << elem << "  ";
	}
	cout << endl;

	//inset additional values:
	cities.insert({ "London","Munich","Hanover","Braunschweig" });

	//print each element:
	for (const auto& elem : cities) {
		cout << elem << "	";
	}
	cout << endl;
}
    包含必要的头文件
        #include<unordered_set>
    后，我们可以声明一个“元素为string”的unordered set 并给予初值：
        unordered_multiset<string> cities{...};
    现在如果打印所有元素，出现的次序可能不同于程序中所给的次序，因为其次序是不明确的。唯一保证的是重复元素---者的却有可能因为我们用的是multiset---以其安插次序被组合在一起（因此其相对次序永远不变）。
    
    而任何安插动作都有可能改变这种次序，事实上任何操作只要可能引发rehashing就可能改变上述次序。
    次序究竟会不会变化，或变成怎样，取决于rehashing策略，而它在某种程度上可被程序猿影响。例如你可以保留足够空间，使得知道出现一定量的元素才发生rehashing。此外，
    为确保你在处理所有元素的同时还可以删除元素，C++ standard 保证，删除元素不会引发rehashing。但是删除之后的一次安插动作就可能引发rehashing。

    再次提醒，在C++11之前，你必须使用迭代器访问元素。//所以这成为了传统风格了吗。。。

Unordered Map 和 Multimap 实例 
    出现于第179页的例子是针对multimap设计的，却也适用于unordered multimap，只要你在include指示符中以unordered_map替换map并在容器声明式中以unordered_multimap替换multimap： 
    #include <unordered_map> 
    ...
    unordered_multimap<int,string〉coll；
    ... 
    唯一的不同是，本例的元素次序不明确。然而在大多数平台上，元素仍会被排序，因为默 认是以modulo操作符作为hash函数。把“排序后的次序”（sorted order)视为“不明确次 序”（undefined order)当然合法。不过上述的“排序”现象并不保证一定会有，而且如果你 添加更多元素，元素的次序也将可能不同。 
    下面是另一个unordered map例子。此例使用的unordered map,其key是个string而其 value 是个 double: 
#include "stdafx.h"
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

int main()
{
	unordered_map<string, double> coll{ {"tim",9.9},
										{"struppi",11.77}
									   ;

	//suqare the value of each element:
	for (pair<const string, double>& elem : coll) {
		elem.second *= elem.second;
	}

	//print each element(key and value):
	for (const auto& elem : coll) {
		cout << elem.first << " ：" << elem.second << endl;
	}
}
    包含必要的map、sting和iostream头文件后，我们声明一个unordered map，并以两个元素作为初始元素。这里运用了嵌套式初值列（nested initializer list），所以
        {"tim",9.9}
    和
        {"struppi",11.77}
    是被用来初始化map的两个元素。
        接下来，对每个元素的value执行平方运算：
        for(pair<const string,double>& elem : coll){
            elem.second *= elem.second;
        }
    我要再一次提示你，看得出来元素类型是由constant string和double组成的pair<>。因此我们无法改动元素的key,也就是其first成员： 
    for (pair<const string,double>& elem : coll) { 
        elem. first = ...;  // ERROR: keys^jfa mapare constant 
    }
    像此前很多例子一样，自C++11 开始，我们不再需要明白指出元素类型，因为在一个range-based for循环内，类型可被推到出来（根据容器）。基于此，
    负责输出所有元素的第二循环中使用auto。事实上，由于声明elem为const auto&，我们得以避免产生很多拷贝（copy）:
    for(const auto& elem : coll){
        cout<<elem.first<<": "<<elem.second<<endl;
    }
    这个程序的一个可能的输出是：
        struppi: 138.533
        tim: 98.01
    但不保证如此，因为实际次序不明确（无序）。如果我们改用一个寻常的map，就能保证 “带着key "struppi"” 的元素必定在“带着key"tim"” 的元素之前,
    因为map会以key为 根据对元素排序，而"struppi"小于"tim"。7.8.5 节第345页有另一个例子，使用map并以STL算法和lambda取代range-based for循环。

Unordered容器的其他例子
    所有无序（unordered） 容器都提供若干可有可无的template实参，用来指明诸如hash函数和等效比较式（equivalence comparison）。
    标准库为基础类型和string准备了一个默认的hash 函数，至于其他类型，我们必须提供自己的hash函数。
    下一节有个例子使用map作为所谓的关联式数组（associative array）。7.9 节详细讨论 unordered容器并附带其他例子。Unordered multimap也可用作字典（dictionary) ( 7.9.7节第383页有一个例子)。

关联式数组（Associative Array）

    不论map或unordered map，都是key/value pair形成的集合，每个元素带着独一无二的key。如此的集合也可被视为一个关联式数组（associate array），也就是“索引并非整数”的array。
    也因此，刚才说的两个容器都提供了下标操作符[]。
        考虑下面这个例子：
#include "stdafx.h"
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

int main()
{
	//type of the container: 
	// - unordered_map: elements are key/value pairs 
	//- string: keys have type string 
	//- float: values have type float 
	unordered_map<string, float> coll;

	//insert some elements into the collection
	//- using the synatx of an associative array
	coll["VAT1"] = 0.16;
	coll["VAT2"] = 0.07;
	coll["Pi"] = 3.1415;
	coll["an arbitrary number"] = 4983.223;
	coll["Null"] = 0;

	//change value
	coll["VAT1"] += 0.03;

	//print difference of VAT values
	cout << "VAT difference:  " << coll["VAT1"] - coll["VAT2"] << endl;

}
    声明容器类型时，必须同时指明key的类型和value的类型： 
    unordered_map<string,float> coil; 
    这意味着key是string而相关联的value是浮点数。 
    根据associative array的观念，你可以使用下标操作符[]访问元素。然而请注意，这个操作符的行为和寻常array的下标操作符不太一样：其索引不是整数，它接受一个新索引 (或曰key)并以此建立和安插一个新元素，
    该元素以该索引为key。因此你绝不能给予一个无效索引。 
    也就是说，此程序中的
        coll["VAT1"] = 0.16;
    语句乃是建立一个新元素，其key为"VAT1"而其value为0.16。 
    事实上，下面的这个式子会建立起一个新元素，拥有key "VAT1"并将value设为默认值（使用default构造函数:如果是基础类型则初值为0): 
        coll["VATl"] 
    而稍早那整个语句会访问此新元素的value,于是赋值操作符对它陚值0.16。 
    自C++11开始，你也可以使用at()访问元素的value，只要传给它key就行。这种情况下如果容器内找不到给定的key,会导致一个out_of_range异常： 
        coll at(“VAT1“) =0.16; //out_of_range exceptionifnoelementfound 
    语句 
        coll["VATl"] += 0.03;   
    或 
        coll["VATl"] - coll["VAT2"] 
    获得的是对元素的value的激写权力。最终，程序输出如下： 
    VAT difference: 0.12
    一如以往，使用unordered map和使用map的差异在于，元素在unordered map内的次序有各种可能，但元素在map内一定是被排序过的。
    但由于unordered map的操作属于“摊提式常置”（amortized constant)复杂度，map提供的则是“对数复杂度”，所以通常你宁可使用unordered map而放弃map，
    除非你需要排序，或你的环境不支持C++11特性以至于无法使用unordered map。这种情况下只需改变容器类型：将include指示符和容器声明中的unordered.拿掉即可。 
    7.8.3节第343页和7.9.5节第374页分别更详细地讨论了如何以map和unordered-map做出一个关联式数组（associative array）。

其他容器
String 
    你可以把string当作一种STL容器。说到string我指的是C++ stringclass (basic_string<>、 string和wstring))对象，第13章会介绍它们。String很类似vector，但其元素都是字符。 13.2.14节第684页对此有更详细的说明。 

寻常的 C-Style Array 
    //没删的页码一定要看哦。。。。
    另一种容器由C/C++语言内核提供，是个type而不是个class: 那就是寻常的array,或所谓的C-style array。声明它时应该给予一个固定大小，或一个来自malloc()或realloc()的动态大小。然而这种寻常array并不是STL容器，
    因为它们不提供成员函数如size()和 empty()。尽管如此，STL算法还是可以作用在它们身上。 
    寻常array的用法一点都不新奇。唯一的新亮点就是可以把算法作用于它们身上。 7.10.2节第386页会进行解释。 
    C++程序不再需要直接使用C-style array。Vector和array提供了寻常C-style array的所有特性，并具备更安全更方便的接口。详见7.2.3节第267页和7.3.3节第278页。 

用户自定义容器（User-Defined Container) 

    现实中你可以给予任何“与容器相仿（comainer-like)的对象” 一个相应的STL接口，使它得以迭代元素，或提供标准操作以运用元素内容。例如你可以引入一个class用以表现目录(directory),而你能够迭代其中各文件，视它们为元素并操纵运用它们。
    STL-container-like 接口的最佳候选是7.1节第254页所列的那些共通的容器操作。 
    不过，某些container-like对象无法与STL概念相呼应。例如“STL容器有一个起头和一个结尾”这一概念就令环式容器（circlular container,例如ring buffer）很难融入STL框架。

容器适配器（Container Adpter）
    
    除了以上数个根本的容器类，为满足特殊需求，C++标准库还提供了一些所谓你的容器适配器，它们也是预定义的容器，提供的是一定限度的接口，用以应付特殊需求。这些容器适配器都是根据基本容器实现而成，包括： 
    Stack名字足以说明一切。Stack容器对元桌采取LIFO (后进先出）管理策略。 
    Queue对元素釆取FIFO (先进先出）管理策略。也就是说，它是个寻常的缓冲区 (buffer)。 
    Priority queue其内的元素拥有各种优先权。所谓优先权乃是基于程序员提供的排序准则（默认为操作符 <）而定义。这种特殊容器的效果相当于这样一个缓冲区：“下一元素永远是容器中优先权最髙的元素”。如果同时有多个元素最高优先权，则其次序无明确定义。 
    从历史观点来看，容器适配器是STL的一部分。从程序猿的观点来看，它们只不过是一种 特别的容器类，使用“由STL提供的容器、迭代器和算法所形成的总体框架”。因此，容器适配器被我当作STL内核的外围，直到第12章才介绍。


        



















        

    







