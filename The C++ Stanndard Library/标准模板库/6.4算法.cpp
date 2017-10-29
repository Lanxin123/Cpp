算法（Algorithm）
    
    为了处理容器内的元素，STL提供了提供了一些标准算法，包括查找、排序拷贝。重新排序、修改、数值运算等基本而普遍的算法。
    算法并非容器类的成员函数，而是一种搭配迭代器使用的全局函数。这么做有一个重要优势：所有算法只需实现一份，就可以对所有容器运作，不必为每一种容器量身定制。算法甚至可以操作不同类型（type）之容器内的元素，也可以与用户自定义的容器搭配。
    这个概念大幅降低了代码量，提高了程序库的能力和弹性。 
    注意，这里所阐述的并非面向对象思维（OOP paradigm) , 而是泛型函数编程思维 (generic functional programming paradigm）。在面向对象编程（OOP）概念里，数据与操作合为一体，在这里则被明确划分开来，再通过特定的接口彼此互动。
    当然这需要付出代价： 首先是用法有失直观，其次某些数据结构和算法之间并不兼容。更有甚者，某些容器和算法虽然勉强兼容却毫无用处（也许导致很糟的效率）。因此，深入学习STL概念并了解其缺陷，显得十分重要，唯其如此方能取其利而避其害。我将在本章剩余篇幅中，通过一些实例来介绍它们。 
    让我们从STL算法的简单运用入手。以下展现了若干算法的使用形式：
#include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
	//create vector with elements from 1 to 6 in arbitrary order
	vector<int> coll = { 2,5,4,1,6,3 };

	//find and print minimum and maximum elements
	auto minpos = min_element(coll.cbegin(), coll.cend());
	cout << "min: " << *minpos << endl;
	auto maxpos = max_element(coll.cbegin(), coll.cend());
	cout << "max: " << *maxpos << endl;

	//sort all elements
	sort(coll.begin(), coll.end());

	//find the first element with value 3
	//-no cbegin()/cend() because later we modify the elements pos3 reers to
	auto pos3 = find(coll.begin(), coll.end(),	//range
		3);															//value
	
	//reverse the order of the found element with value 3 and all following elements
	reverse(pos3, coll.end());

	//print all elements
	for (auto elem : coll)
	{
		cout << elem << ' ';
	}
	cout << endl;
}

    为了能够调用算法，首先必须包含头文件<algorithm>（某些算法需要特别的头文件， 见11.1节第505页）：
        #include <algorithm> 
    最先出现的算法是min_element()和max_element()。调用它们时必须传入两个实参，定义出欲处理的元素范围。如果想处理容器内所有元素，可使用cbegin()和cend()，或 begin()和end()。两个算法都返回一个迭代器，分别指向最小或最大元素。因此，语句 
        auto minpos = min_element(coll.cbegin(),coll.cend());
    中，算法min_element()返回最小元素的位置（如果最小元素不止一个，则返回第一个的位置）。以下语句打印出该元素：
        cout<<"min: "<<*minpos<<endl;
    当然，你也可以把上述两个动作合并于单一语句：
        cout<<*min_element(coll.cbegin(),coll.cend())<<endl;
    接下来出现的算法是sort()。顾名思义，它将“由两个实参指出来”的区间内的所有元素加以排序。你可以（选择性地）传入一个排序准则，默认使用operator<。因此，本例容器内的所有元素以递增方式排列。
        sort(coll.begin(),coll.end());
    排序后的容器元素次序如下：
        1 2 3 4 5 6
    注意，这里不可使用cbegin()和cend()，因为sort会改动元素的value，但const_iterator不允许如此。    
    //cbegin和cend返回的是只读
    再来便是算法find()。它在给定范围内査找某值。本例在整个容器内寻找数值为3的第一个元素。 
        auto pos3 = find (coll.begin() , coll.end(),  //range
                          3);                       //value
    如果find()成功，返回一个迭代器指向目标元素。如果失败，返回第二实参所指示的区间末端，本例是coll的逾尾（past-the-end）迭代器。本例在第三个元素位置上发现数值3，因此完成后pos3指向coll的第三个元素。
    本例所展示的最后一个算法是reverse()，将区间内的元素反转放置：
        reverse(pos3,coll.end());
    这个动作会将第三元素至最末元素的排列次序逆转。由于这是一种改动，我们必须使用一个非常量迭代器，这就是为什么我调用find()并传入begin()和end()，而不是传入cbegin()和cend()。否则pos3就是个const_iterator，把它传给reverse()会导致错误。
        程序输出如下：
        min: 1
        max: 6
        1 2 6 5 4 3
    注意，这个例子运用了若干C++11特性。如果你手上的平台不支持C++11所有特性，下面是一个等效程序:
    #include "stdafx.h"
#include <algorithm>
#include <vector>
#include <iostream>
using namespace std;

int main()
{
	//create vector with elements from 1 to 6 in arbitrary order
	vector<int> coll;
	coll.push_back(2);
	coll.push_back(5);
	coll.push_back(4);
	coll.push_back(1);
	coll.push_back(6);
	coll.push_back(3);

	//find an print minimum and maximum elements
	vector<int>::const_iterator minpos = min_element(coll.begin(), coll.end());

	cout << "min: " << *minpos << endl;
	vector<int>::const_iterator maxpos = max_element(coll.begin(), coll.end());

	cout << "max: " << *maxpos << endl;

	//sort all elements
	sort(coll.begin(), coll.end());

	//find the first element with value 3
	vector<int>::iterator pos3;
	pos3 = find(coll.begin(), coll.end(),
		3);

	//reverse the order of the found element with value 3 and all following elements
	reverse(pos3, coll.end());

	//print all elements
	vector<int>::const_iterator pos;
	for (pos = coll.begin();pos!=coll.end();++pos)
	{
		cout << *pos << ' ';
	}
	cout << endl;
}

    //不得不说C++11的auto还是生了不少代码。。。
    两个版本之间的区别在于： 
    •不能使用初值列（initializer list）来为vector设初值。 
    •不提供成员函数cbegin()和cend()，你必须代以begin()和end()，但还是可以使用 const_iterator。 
    •不能使用auto,你必须明白声明迭代器。
    •不能使用range-based for循环，你必须以迭代器输出每一个元素。

区间（Range) 
    所有算法都是用来处理一或多个区间内的元素。这样的区间可以（但非必须）涵盖容器内的 全部元素。为了操作容器元素的某个子集，我们必须将区间首尾当作两个实参（argument） 传给算法，而不是一口气把整个容器传递进去。 
    这样的接口灵活又危险。调用者必须确保经由两实参定义出来的区间是有效的（valid)。 所谓有效就是，从起点出发，逐一前进，能够到达终点。也就是说，程序员自己必须确保两个迭代器隶属同一容器，而且前后的放置是正确的，否则结果难料，可能引起无穷循环，
    也可能访问到内存禁区。就此而言，迭代器就像寻常指针一样危险。不过请注意，所谓“结果难料”（或说行为不明确〔undefined behavior〕）意味着任何STL实现均可自由选择合适的方式来处理此类错误。稍后你会发现，确保区间有效并不像听起来那么简单。与此相关的一些细节见6.12节第245页。 
    所有算法处理的都是半开区间（half-open range)--包括起始元素的位置但不包括末尾元素的位置。传统的数学表示法是：
        [begin,end)
    或
        [begin,end[
    本书采用第一种形式。
    半幵区间的优点主要是单纯，可免除对空集做特殊处理（见6.3节第189页)。当然， 金无足赤，世上没有完美的设计。请看下面的例子：
#include "stdafx.h"
#include <algorithm>
#include <list>
#include <iostream>
using namespace std;

int main()
{
	list<int> coll;

	// insert elements from 20 to 40
	for (int i = 20; i <= 40; ++i) {
		coll.push_back(i);
	}

	// find position of element with value 3
	// - there is none, so pos3 gets coll.end()
	auto pos3 = find(coll.begin(), coll.end(),    // range
		3);                          // value

									 // reverse the order of elements between found element and the end
									 // - because pos3 is coll.end() it reverses an empty range
	reverse(pos3, coll.end());

	// find positions of values 25 and 35
	list<int>::iterator pos25, pos35;
	pos25 = find(coll.begin(), coll.end(),  // range
		25);                       // value
	pos35 = find(coll.begin(), coll.end(),  // range
		35);                       // value

								   // print the maximum of the corresponding range
								   // - note: including pos25 but excluding pos35
	cout << "max: " << *max_element(pos25, pos35) << endl;

	// process the elements including the last position
	cout << "max: " << *max_element(pos25, ++pos35) << endl;
}
    本例首先以20至40的整数作为容器初值。查找元素值3的任务失败后，find()返回区间的结束位置（本例为coll.end())并赋值给pos3。以此作为稍后调用reverse()时的区间起点，纯粹是空摆架子，因为其结果相当于： 
        reverse (coll.end() , coll.end()); 
    这其实就是反转一个空区间，当然亳无效果了（亦即所谓的“no-op”）。 
    如果使用find()获取某个子集的第一和最后元素，你必须考虑一点：半开区间并不包含最后一个元素。所以上述例子中首次调用max—elemeiit()： 
        max_element (pos25, pos35) 
    找到的是34,而不是35:
        max: 34
    为了处理最后一个元素，你必须把该元素的下一位置传给算法： 
        max_element (pos25, ++pos35) 
    这样才能得到正确的结果：
        max: 35
    注意，本例用的是list容器，所以你只能以++取得pos35的下一个位置。如果面对的是 vector 或 deque 的随机访问迭代器（random-access iterators），你可以写pos35+1。
    这是因为随机访问迭代器允许迭代器算术运算（参见6.3.2节第198页和9.2.5节第438页)。 
    当然，你可以使用pos25和pos35来查找其间的任何东西。记住，为了让查找动作含pos35,必须将pos35的下一位置传入，例如： 
    //increment pos35 to search with its value included 
    ++pos35； 
    pos30() = find (pos25, pos35,  //range    
                    30);          //value
    if(pos30 == pos35){
        cout<<"30 is NOT in the subrange"<<endl;
    }
    else{
        cout<<"30 is in the subrange"<<endl;
    }
    本节的所有例子都可以正常运作的前提是，你知道pos25在pos35 之前。否则[pos25 ,pos35)就不是个有效区间。如果你对于“哪个元素在前，哪个元素在后”心中没谱，事情就麻烦了，说不定会导致不明确行为。
    现在假设你并不知道元素25和元素35的前后关系，甚至连它们是否存在也心存疑虑。 如果你手上是随机访问迭代器（random-access iterator），你可以使用operator < 进行检查： 
    if (pos25 < pos35) { 
        //only [pos25,pos35) is valid
        ...
    } 
    else if (pos35 < pos25) { 
        //only [pos35’pos25) is valid 
        ...
    }
    else { 
        //both are equal, so both must be end() 
        ...
    }
    如果你手上并非随机访问迭代器，那还真没什么直截了当的办法可以确定哪个迭代器在前。 你只能在“起点和某个迭代器”之间，以及“该迭代器和终点”之间，寻找另外那个迭代器。此时你的解决方法需要一些变化： 
    不是一口气在整个区间中查找两值，而是试着料及，哪个值先来到。例如：
        pos25 = find(coll.begin(),coll.end(),       //range
                    25);                            //value
        pos35 = fin(coll.begin(),pos25,             //range
                    35);                            //value
        if(pos25 != coll.end() && pos35!=pos25){
            //pos35 is in front of pos25
            //so,only[pos35,pos35) is valid
            ...
        }
        else{
            //25 and/ or 35 not found
            ...
        }

    和前例不同的是，本例并非在coll的整个区间内査找35,而是先在起点和pos25之间寻找，如果一无所获，再在pos25之后的区间寻找。其结果当然使你得以完全掌握哪个位置 在前面、哪个子区间有效。 
    这么做并不是很有效率。当然还有其他高招，可以直接找到25或35首次出现位置。 你可以运用find_if()，传给它一个lambda (见3丄10节第28页）定义出一个准刖，评估 coll内的每一个元素： 
        pos = find.if (coll. begin(), coll.end(), //range 
                        [](int i){              //criterion
                            return i==25 || i==35;
                        });
        if(pos == coll.end()){
            //no element with value 25 or 35 found
            ...
        }
        else if(*pos == 25){
            //element with value 25 comes first
            pos23 = pos;
            pos35 = find(++pos,coll.end(),        //range  
                        35);                      //value
            ...
        }
        else {
            //element with value 35 comes first
            pos35 = pos;
            pos25 = find(++pos,coll.end(),        //range
                        25);                      //value
            ...
        }
    在这里，特殊的lambda表达式
        [](int i){
            return i==25 || i == 35;
        }
    被用作一个准则，允许查找第一个“带有value 25 或 value35”的元素。6.9节第229页已介绍过如何在STL中使用lambda，10.3节第499页有更详细的讨论。
    
处理多重区间（Multiple Ranges）
    有数个算法可以（或说需要）同时处理多重区间。通常你必须设定第一个区间的起点和终点，至干其他区间，只需设定起点即可，终点通常可由第一区间的元素数量推导出来。例如以下程序片段中，equal()从头开始逐一比较coll1和coll2的所有元素：
    if(equal(coll1.begin(),coll1.end(), //first range
              coll2.begin())){          //second range
                  ...                   
              }
    于是，coll2之中之中参与比较的元素数量，间接取决于coll1内的元素数量，间接取决于coll1内的元素数量（如图6.9所示）。
    这是我们收获一个重要心得：如果某个算法用来处理多重区间，那么当你调用它时，务必确保第二（以及其他）区间所拥有的元素个数至少和第一区间所拥有的元素个数至少和第一区间内的元素个数相同。特别是执行涂写动作时，务必确保目标区间够大。
    考虑下面这个程序：
#include "stdafx.h"
#include <algorithm>
#include <list>
#include <vector>
using namespace std;

int main()
{
	list<int> coll1 = { 1,2,3,4,5,6,7,8,9 };
	vector<int> coll2;

	//RUTIME ERROR:
	//-overwrites nonexisting elements in the destination
	copy(coll1.cbegin(), coll1.cend(),	//source
		coll2.begin());	//destination
    ...
}
    这里调用了copy()算法，将第一区间内的全部元素拷贝至目标区间。如上所述，第一区间的起点和终点都已指定，第二区间只指出起点。然而，由于该算法执行的是覆写动作(overwrite)而非安插动作（insert） , 所以目标区间必须拥有足够的元素被覆写，
    否则就会像这个例子一样，导致不明确行为。如果目标区间内没有足够的元素供覆写，通常意味着你会覆写co112.end()之后的任何东西，幸运的话程序立即崩溃---这起码还能让你知道出错了。你可以强制自己获得这种幸运：使用STL安全版本。在这样的版本中所有不明确行为都会被导向一个错误处理程序（error-handling procedure）。见6.12.1节第247页。 

    想要避免上述错误，你可以（1）确认目标区间内有足够的元素空间，或是（2）采用insert iterator。Insert iterator将在6.5.1节第210页介绍。我首先解释如何修改目标区间使它拥有足够空间。
    为了让目标区间够大，你要不一开始就给它一个正确大小，要不就显式变更其大小。 这两个办法都只适用于序列式容器（vector 、deque 、list 和forward_list)。关联式容器根本不会有此问题，因为关联式容器不可能被当作覆写式算法（overwriting algorithm) 的操作目标（原因见6.7.2节第221页)。以下例子展示了如何扩充容器大小： 
#include "stdafx.h"
#include <algorithm>
#include <list>
#include <vector>
#include <deque>
using namespace std;

int main()
{
	list<int> coll1 = { 1,2,3,4,5,6,7,8,9 };
	vector<int> coll2;

	//resize destination to have enough room for the overwriting algorithum
	coll2.resize(coll1.size());

	//copy elements from first into second collection 
	//-overwrites existing elements in destination
	copy(coll1.cbegin(), coll1.cend(),		//source
		coll2.begin());									//destination

	//create third collection with enough room
	//-initial size is passed as parameter
	deque<int> coll3(coll1.size());

	//copy elements from first into third collection
	copy(coll1.cbegin(), coll1.cend(),		//source
		coll3.begin());									//destination
}
    在这里，resize()的作用是改变coll2的元素个数：
        coll2.resize(coll.size());
    coll3则是在初始化时就指明要有足够空间，以容纳coll1中的全部元素：
        deque<int> coll3(coll1.size());
    注意，这两种方式都会产出新元素并赋予初值。这些元素由default构造函数初始化，没有任何实参。你可以传递额外的实参给构造函数和resize()，这样就可以按你的意愿将新元素初始化。







    


