迭代器之适配器（Iterator Adapter）
    
    迭代器（Iterator）是一个春抽象概念：任何东西，只要其行为类似迭代器，它就是一个迭代器。因此，你可以撰写一些类（class）具备迭代器接口，有着各不同的行为。C++标准库提供了数个预定义的特殊迭代器，提供了数个预定义的特殊迭代器，
    亦即所谓迭代器适配器（iterator adapter）。它们不仅是辅助性质而已，它们赋予整个迭代器抽象概念更强大的威力。
        以下数小节简介下面各种迭代器适配器（iterator adapter）：    
    1. Insert iterator（安插型迭代器）
    2. Stream iterator （串流迭代器）
    3. Reverse iterator （逆向迭代器）
    4. Move iterator （搬移迭代器）（自C++11起）
    9.4节第488页对它们有更翔实的说明。

Insert Iteator（安插型迭代器）

    迭代器适配器的第一个例子是insert iterator,或称inserter。它可以使算法以安插（insert）方式而非覆写（overwrite）方式运作。使用它可以解决算法的“目标空间不足问题”。是的，它会促使目标区间的大小按需求成长。
        Insert iterator内部将接口做了新的定义：
    如果你对某个元素陚值（assign）, 会引发“对其所属集合的安插（insert）动作”。至于插入位置是在容器的最前或最后，或某特定位置上，要视三种不同的insert iterator而定。 
    单步前进（step  forward） 不会造成任何动静（是个no-op）。 
    请看下面这个例子：
#include "stdafx.h"   
#include <algorithm>
#include <iterator>
#include <list>
#include <vector>
#include <deque>
#include <set>
#include <iostream>
using namespace std;

int main()
{
    list<int> coll1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    // copy the elements of coll1 into coll2 by appending them
    vector<int> coll2;
    copy (coll1.cbegin(), coll1.cend(),    // source
          back_inserter(coll2));           // destination

    // copy the elements of coll1 into coll3 by inserting them at the front
    // - reverses the order of the elements
    deque<int> coll3;
    copy (coll1.cbegin(), coll1.cend(),    // source
          front_inserter(coll3));          // destination

    // copy elements of coll1 into coll4
    // - only inserter that works for associative collections
    set<int> coll4;
    copy (coll1.cbegin(), coll1.cend(),    // source
          inserter(coll4,coll4.begin()));  // destination
}

    此例运用了三种预定义的insert iterator： 
    1.Back inserter （安插子容器最末端）其内部调用push_back(),在容器末端插入元素（此即“追加”动作）。例如以下语句完成后，coll1的所有元素都会被附加到coll2中： 
        copy (colli.cbeginO , colli.cendO ,   //source 
            back_inserter(coll2));            //destination
     当然，只有在提供有push_back()的容器中back inseter才能派上用场。在C++标准库中这样的容器有vector、deque、list和string。
    2.Front inserter （安插于容器最前端）其内部调用push_front()，将元素安插于容器最前端。例如以下语句将coll1的所有元素插入coll3:
        copy(coll1.cbegin(),coll1.cend(),     //source
            front_inserter(coll3));           //destination
     注意，这种动作翻转了被安插元素的次序。如果你先安插1，再向前安插2，那么1会排列在2的后面。
     Front inserter只能用于提供有push_frout()的容器，在C++标准库中这样的容器是deque、list 和 forward_list。
    3. General inserter这种一般性的inserter，简称inserter，它的作用是在“初始化时接受之第二实参”所指位置的前方插入元素。它内部调用成员函数insert()，并以新值和新位置作为实参传入。所有STL容器都提供insert()成员函数，
     因此，这是唯一可用于关联式容器身上的一种预定义的inserter。等等，我不是说过，在关联式容器和无序容器身上安插新元素时不能指定位置吗？ 在关联式容器中元素的位置由元素值决定，而在无序容器中，元素的位置根本难以确定！
     好，我解释一下，很简单：在关联式容器和无序容器中，你所给的位置只是一个提示，帮助容器确定从什么地方幵始查找正确的安插位置。容器有完全的自由决定要不要忽略它。9.6节第471页将介绍一个用户自定义的inserter,对关联式容器和无序容器特别有用。 
     表6.1列出了 insertiterator的功能。9.4.2节第454页将介绍更多细节。 

Stream Iterator（串流迭代器）
    Stream iterator被用来读/写stream。它们提供了必要的抽象性，使得来自键盘的输入像是个集合（collection），你能够从中读取内容。同样道理，你也可以把一个算法的输出结果重新导向某个文件或屏幕上。
    下面展示STL威力的一个典型例子。和一般C/C++程序相比，本例仅用数条语句，就完成了大量的复杂工作：
#include "stdafx"
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
using namespace std;

int main()
{
    vector<string> coll;

    // read all words from the standard input
    // - source: all strings until end-of-file (or error)
    // - destination: coll (inserting)
    copy (istream_iterator<string>(cin),    // start of source
          istream_iterator<string>(),       // end of source
          back_inserter(coll));             // destination

    // sort elements
    sort (coll.begin(), coll.end());

    // print all elements without duplicates
    // - source: coll
    // - destination: standard output (with newline between elements)
    unique_copy (coll.cbegin(), coll.cend(),           // source
                 ostream_iterator<string>(cout,"\n")); // destination
}
    这个程序只用三条语句就完成了一系列工作：从标准输入设备读取所有输入文字，然后排序，然后将它们打印到屏幕。让我们逐一思考这三条语句。语句 
        copy (istream_iterator<string>(cin), 
            istream_iterator<string>(), 
            back_inserter(coll))； 
    用到两个input stream iterator： 
    1.表达式 
        istream_iterator<string>(cin) 
    会产生一个可从“标准输入串流（standard input stream） cin”读取数据的stream iterator。其中的template实参string表示这个stream iterator 专司读取该和类型的元素（string类型将在第13章介绍）。这些元素通过一般的operator >>被读取进来。
    因此每当算法企图处理下一元素时，istream iterator就会将这种企图转化为以下行动： 
        cin>>string 
    针对string而执行的input操作符通常读取以空白分隔的文字（见13.2.10节第677页），因此上述箅法的行为将是“逐字读取”（word by word)。 
    2.表达式 
        istream_iterator<string>() 
    会调用istream iterator的default构造函数，产生一个代表“串流结束符”(end-of-stream) 的迭代器，这个东西代表的意义是：你不能再从中读取任何东西。
    只要那个不断逐步前进的第一实参异于第二实参，算法copy()就持续动作。这里的end-of-stream迭代器正是作为区间终点之用，因此这个算法便从cin读取所有string,直到读无可读为止（可能是因为到达了 end-of-stream，也可能是因为读入过程发生错误）。
    总而言之，算法的数据来源是“来自cin的所有文字”。在back inserter的协助下，这些文字被拷贝并插入coll中。 
        接下来的sortO算法对所有元素排序： 
        sort (coll.begin() , coll.end())； 
    最后，语句 
        unique_copy (coll.cbegin(), coll.cend(), 
                    ostream_iterator<string>(cout,"\n"))； 
    将所有元素复制到目的端cout。处理过程中，算法uniq_copy()会消除吡邻的重复值。其中的表达式 
        ostream_iterator<string>(cout,"\n") 
    会产生一个 output stream iterator,通过 operator<< 向 cout 写入 string。cout之后的第二实参（可有可无）被用作元素之间的分隔符。本例指定为一个换行符，因此每个元素都被 打印于独立的一行。 
    这个程序内的所有组件都是template，所以你可以轻易改变程序，对其他类型如整数或更复杂的对象进行排序。9.4.3节第460页更详细地介绍了iostream iterator，并附带了更多实例。 
    本例使用一个声明式和三条语句，将来自标准输入设备的所有文字（单词）排序。你还可以更进一步，只用一个声明式和一条语句就搞定一切。示例见1节第394页。

Reverse Iteator（反向迭代器）

    Reverse iterator会造成算法逆向操作，其内部将对increment（递增）操作符的调用转换为对decrement（递减）操作符的调用，反之亦然。所有提供双向（bidrectional）或随机访问（random-access）迭代器的容器（也就是forward_list之外的所有序列式容器和所有关联式容器）
    都可以通过它们的成员函数rbegin()和rend()产生一个反向迭代器。自C++11起还提供了一组对应成员函数rebgin()和rend()产生一个反向迭代器。自C++11起还提供了一组对应成员函数crbegin()和crend()，它们会返回只读反向迭代器。
    forward_list和所有无序容器都没有提供回向迭代（backward-iteration）接口，即 rbegin(), rend()等等。原因是那些容器内部实现只是使用singly linked list串起所有元素。
    看下面的例子：

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
using namespace std;

int main()
{
	vector<int> coll;

	for (int i = 1; i <= 9; ++i) {
		coll.push_back(i);
	}

	copy(coll.crbegin(), coll.crend(),
		ostream_iterator<int>(cout, " "));
	cout << endl;
}
    下列表达式返回一个由coll定义的只读（read-only）reverse iterator:
        coll.crbegin()
    这个迭代器可作为“对coll的元素逆向遍历”的起点。它指向集合的结尾位置（也就是最末元素的下一位置）。因此，以下表达式返回的是最后一个元素的值： 
        *coll. crbegin() 
    对应地，下列表达式返回一个reverse iterator,可作为“对集合coll的元素逆向遍历”的终点。
        coll.crend() 
    它一样是指向“逾尾”（past the end）位置，只不过方向相反，所指的是容器内第一元素的前一个位置。 
    再次提醒，千万不要对着一个“并未表现出有效元素”的位置使用操作符*  (或->)。 
    以下表达式就是一例： 
        *coll. crend() 
    它带来什么结果无人知晓，*coll.end()或*coll.cend()也一样。 
    采用reverse iterator，所有算法便可以不需特殊处理就以相反方向操作容器，这自然是美事一桩。使用operator++前进至下一元素，被转化为

    使用operator--后退至前一元素。本例中的copy() “从尾到头”地遍历coll的每一个元素。程序输出如下：
        9 8 7 6 5 4 3 2 1
    你可以将一般迭代器转换为reverse iterator，反之亦可。然而，对于具体某个迭代器而言，这样的转换会改变其所指对象。这些细节将在9.4.1节第448页提出。

Move Iterator（搬移迭代器）
    这种迭代器始自C++11，用来将任何“对低层元素（underlying element）的访问”转换为一个move操作。也就是说，他们允许从一个容器移动元素至另一个容器，不论是在构造函数内或是在运用算法时。详见9.44节466页。

