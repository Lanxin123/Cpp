Smart Pointer(智能指针)

    c语言让我们知道，pointer很重要，却是麻烦的来源。使用pointer的理由之一是在惯常的作用域边界之外拥有reference语义。然而，确保“pointer的寿命”和“其所指向的对象的寿命” 一致，却是件棘手的事，特别是当多个pointer指向同一对象时。
    例如，为了让多个集合（见第7章）拥有同一对象，你必须把指向该对象的pointer放进那些集合内，而且当其中一个pointer被销毀时不该出现问题，也就是不该出现所谓的dangling pointer (空悬指针) 
    或多次删除被指向对象；最后一个pointer被销毁时也不该出现问题（不该造成资源泄漏〔resource leak〕)。 
    避免上述问题题的一个通常做法是使用smart pointer (智能指针)。它们被称为smart是因为它们对于上述问题提供解决方案。举个例子，smart pointer有可能如此smart以至于能够“知道”它自己是不是“指向某物”的最后一个pointer,
    并运用这样的知识，在它的确是该对象的最后一个拥有者而且它被删除时，销毁它所指向的对象。 
    然而请注意，只提供一个smart pointer class是不够的。Smart pointer可以在很多方面发挥其智能，并满足不同重点，但你可能会为它的smartness付出代价。注意，对于一个独特的smart pointer,还是有可能误用pointer或写出容易出错的行为。 
    自C++11起，C++标准库提供两大类型的smart pointer： 
    1. Class shared_ptr实现共享式拥有（shared ownership) 概念。多个 smart pointer可以指向相同对象，该对象和其相关资源会在“最后一个reference被销毁”时被释放。为了在结构较复杂的情境中执行上述工作，
    标准库提供了weak_ptr、bad_ weak_ptr和enable_shared_from_this 等辅助类。 
    2. Class unique_ptr 实现独占式拥有（exclusive ownership) 或严格拥有（strict ownership)概念，保证同一时间内只有一个smart pointer可以指向该对象。你可以移交拥有权它对于避免资源泄漏(resource leak)
    例如“以new创建对象后因为发生异常而忘记调用delete” 特别有用。 
    C++98只让C++标准库提供一个smart pointer class : auto_ptr<>，其设计是为了执行现今的unique_ptr所提供的服务。然而由于当时缺乏语法特性如“针对构造和陚值”的move语义，以及其他瑕疵，这个class不易被理解且容易出错。
    因此在TR1引入class S hared_ptr， C++11 引入 class unique_ptr 之后，class auto_ptr 成为 C++11 中被正式反对（deprecated) 的成分，意味着你不该再使用它，除非你手上还有一些老旧代码需要编译。 
    所有smart pointer class都被定义于头文件〈memory〉内。

Class shared_ptr

    几乎毎一个稍有分量的程序猿需要“在相同时间的多处地点处理或使用对象”的能力。为此，你必须在程序的多个地点指向(refer to)同一对象。虽然C++语言提供reference 和 pointer,还是不够，因为你往往必须确保当“指针对象”的最末一个reference
    被删除时该对象本身也被删除，毕竟对象被删除时析构函数可以要求某些操作，例如释放内存或归还资源等等。
    所以我们需要“当对象再也不被使用时就被清理”的语义。Class shared-ptr提供了这样的共享式拥有语义。也就是说，多个shared_ptr可以共享（或说拥有）同一对象。对象的最末一个拥有者有责任销毁对象，并清理与该对象相关的所有资源。 
    如果对象以new产生，默认情况下清理工作就由delete完成。但你可以（并且往往必须）定义其他清理办法。你可以定义你自己的析构策略。举个例子，如果你的对象是个以new[]分配而得的array,你必须定义你自己的delete[]加以清理。
    其他例子还包括刪除相应资源，如 handle、lock、associated temporary file (相关临时文件）等。 
    总而言之，shared_ptr 的目标就是，在其所指向的对象不再被需要之后（而非之前), 自动释放与对象相关的资源。

使用shared_ptr

    你可以像使用任何其他point一样使用shared_ptr。你可以赋值、拷贝、比较他们，也可以使用操作符*和->访问其所指向的对象。看看下面这个例子:

#include "stdafx.h"
#include <iostream>
#include<string>
#include <vector>
#include <memory>
using namespace std;

int main()
{
	//two shared pointers representing two persons by their name
	shared_ptr<string> pNico(new string("nico"));
	shared_ptr<string> pJutta(new string("jutta"));

	//capitalize person names
	(*pNico)[0] = 'N';
	pJutta->replace(0, 1, "J");

	//put them multiple times in a container
	vector<shared_ptr<string>> whoMadeCoffee;
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pNico);
	whoMadeCoffee.push_back(pJutta);
	whoMadeCoffee.push_back(pNico);

	//print all elements
	for (auto ptr : whoMadeCoffee)
	{
		cout << *ptr << " ";
	}
	
	cout << endl;

	//overwrite a name again
	*pNico = "Nicolai";

	//print all elements again
	for (auto ptr : whoMadeCoffee)
	{
		cout << *ptr << " ";
	}
	cout << endl;

	//print some interral data
	cout << "use_count: " << whoMadeCoffee[0].use_count()<< endl;

	return 0;
}

    上述程序在包含<memory>（其内定义shared_ptr class）之后，声明两个shared_ptr并设初值，各自指向string:
        shared_ptr<string> pNico(new string("nico"));
        shared_ptr<string> pJutta(new string("jutta"));
    注意，由于“接受单一pointer作为唯一实参”的构造函数是explicit，所以这里你不能够使用赋值符，因为那样的话会被视为需要一个隐式转换。然而新式初始化语法是被接受的：
        shared_ptr<string> pNico = new string("nico");  //ERROR
        shared_ptr<string> pNico{new string("nico")};   //OK
    也可以使用便捷函数make_shared():
        shared_ptr<string> pNico = make_shared<string>("nico");
        shared_ptr<string> pJutta = make_shared<string>("jutta");
    这种建立方式比较快，也比较安全，因为它使用一次而不是二次分配：一次针对对象，另一次针对“shared pointer用以控制对象”的sahred data。
    另一种写法是，你可以先声明shared pointer，然后对它赋值一个new pointer。然而你不可使用assignment操作符，必须改用reset():
        shared_ptr<string> pNico4;
        pNico4 = new string("nico");        //ERROR:no assignment for ordinary pointers
        pNico4.reset(new string("nico"));   //OK
    接下来两行示范shared pointer的用法，就像使用寻常的pointer:
        (*pNico)[0] = 'N';
        pJutta->replace(0,1"J");
    运用 operator*，你取得了pNico指向的对象，然后将一个大写的‘N’指派为其第一字符。运用operator->，你得意取用pJutta所指向的对象内的一个成员，而此处成员函数replace()允许你替换substring。

    接下来，我们多次安插这两个pointer,放进一个类型为vector<>的容器。但凡容器，总是为传入的元素创建属于容器自己的拷贝（copy),所以如果我们直接安插string，插入的其实是string的拷贝。
    然而由于我们放进去的是pointer to string,被复制的是那些pointer,于是容器内含多个“指向同一对象”的reference。这意味着如果我们改动对象，对象在容器内的所有身影（occurrence) 都会被改动。因此，在替换string pNico的内容后： 
    *pNico  =  "Nicolai"; 
    该对象的所有身影都改而指向新值，如图5.1所示，并显现于程序输出结果： 
    Jutta  Jutta  Nico  Jutta  Nico 
    Jutta  Jutta  Nicolai  Jutta  Nicolai 
    use.count: 4 
    最后一行输出乃是“针对vector内的第一个shared pointer” 调用use_count()的结果。 use_count()会产出“某个shared pointer所指对象”的当前拥有者数量。
    如你所见，这个vector的第一个元素所指向的对象有四个主人，pJutta和它的三份被安插到容器内的拷贝。 
    程序终点处，当string的最后一个拥有者被销毁，shared pointer对其所指向的对象调用delete。这样的刪除动作(deletion) 并非一定发生在作用域终点。

定义一个Deleter
    
    我们可以声明属于自己的deleter,例如让它在“删除被指向对象”之前先打印一条信息： 
        shared_ptr<string> pNico(new string("nico"), 
                                [](string* p) { 
                                    cout << "delete" << *p << endl; 
                                    delete p; 
                                });
        ...
        pNico = nullptr;        //pNico does not refer to the string any longer
        whoMadeCoffee.resize(2);//all copies of the string in pNico are destroyed
    这里传递一个lambda作为shared_ptr:构造函数的第二实参。如果 令pNico以这种方式声明，当其最末一个拥有者彼销毁时，会调用这个lambda函数。先前 程序搭配此次修改，那么，在前述所有语句之后调用resizeO,会打印出： 
        delete Nicolai 
    如果我们先改变vector的大小，然后将nullptr成另一个对象赋值给pNico,效果相同。
        shared_ptr<>的另一种应用处在388页，示范了两个容器如何共享元素。

对付Array
    请注意，shared_ptr提供的default deleter调用的是delete，不是delete[]。这意味着只有当shared pointer拥有“由new建立起来的单一对象”，default deleter方能适材适所。
    很不幸，为array建立一个shared_ptr是可能的，却是错误的:
        std::shared_ptr<int>p(new int[10], 
                            [](int* p){
                                delete[] p;
                            });
    也可以使用为unique_ptr而提供的辅助函数作为deleter,其内调用 delete []： 
        std:：shared_ptr<int> p(new int[10], 
                                std::default_delete<int[]>())； 
    然而请注意，shared_ptr和unique_ptr以稍稍不同的方式处理deleter。例如unique_ptr允许你只传递对应的元素类作为template实参，但这对shared_ptr就不可行：
        std::unique_ptr<int [] > p(new int [10])； //OK 
        std::shared_ptr<int [] > p(new int [10])； //ERROR:   does not compile 
    此外，对于unique_ptr，你必须明确给予第二个template实参，指出你自己的deleter: 
        std:：unique_ptr<int,void(*)(int*)> p(new int[10], 
                                                    [](int* p) { 
                                                        delete[] p; 
                                                    });
    也请注意，shared_ptr不提供operator[]。至于unique_ptr,它有一个针对array的偏特化版本，该版本提供operator[]取代operator*和operator->。之所以有此差异是因为，unique_ptr在效能和弹性上进行优化。

其他析构策略

    最末一个拥有者---亦即shared pointer---结束生命时，如果清理工作不仅仅是删除内存，你必须明确给出自己的deleter。你可以指定属于自己的析构策略。
    第一个例子：假设我们想确保“指向某临时文件”之最末一个reference被销毁时，该文件即被移除。可以这么做：
#include "stdafx.h"
#include <string>
#include <fstream>  //for ofstream
#include <memory>   //for shared_ptr
#include <cstdio>   //for remove()
using namespace std;

class FileDeleter
{
private:
	std::string filename;
public:
	FileDeleter(const std::string& fn)
		: filename(fn){
	}
	void operator() (std::ofstream* fp) {
		fp->close();										//close.file
		std::remove(filename.c_str());		//delete.file
	}
};

int main()
{
	//create and open temporary file:
	std::shared_ptr<std::ofstream>fp(new std::ofstream("tmpfile.txt"),
		FileDeleter("tmpfile.txt"));
	
}
    以上，首先初始化一个shared_ptr，令它指向一个new新建的输出文件。传进去的FileDeleter负责确保一件事：与此shared pointer的最末一个拷贝失去此输出文件的拥有权时，该文件将被关闭并被<cstdic> 提供的C标准函数remove()移除。
    由于remove()需要文件名，所以我们以文件名为实参传给FileDeleter的构造函数。 
    第二个例子展示了如何使用shared_ptr处理共享内存（shared  memory)：
//此文件只可以在linux环境下运行
#include<memory>        //for shared_ptr
#include<sys/mman.h>    //for shared memory
#include<fcntl.h>
#include<unistd.h>
#include<cstring>       //for strerror()
#include<cerrno>        //for errno
#include<string>
#include<iostream>
class SharedMemoryDetacher
{
    public:
        void operator() (int* p){
            std::cout<<"unlink/tmp1234"<<std::endl;
            if(shm_unlink("/tmp1234") !=0){
                std::cerr<<"OOPS: shm_unlink() failed"<<std::endl;
            }
        }
};

std::shared_ptr<int> getSharedIntMemory(int num)
{
    void* mem;
    int shmfd = shem_open("/tmp1243",O_CREAT|O_RDWR,S_IRWXU|S_IRWXG);
    if(shmfd<0){
        throw std::string(strerror(errno));
    }

    if(ftruncate(shmfd,num*sizeof(int))==-1){
        throw std::string(strerror(errno));
    }
    mem = mmap(nullptr,num*sizeof(int),PROT_READ|PROT_WRITE,
                MAP_SHARED,shmfd,0);
    if(mem == MAP_FAILED){
        throw std::sting(strerror(errno));
    }
    return std::shared_ptr<int>(static_cast<int*>(mem),
                                SharedMemoryDetacher());
}

int main()
{
    //get and attach shared memory for 100 ints:
    std::shared_ptr<int> smp(getSharedIntMemory(100));

    //init the shared memory
    for(int i=0;i<100;i++)
        smp.get()[i] = i*42;
    
    //deal with shared memory somewhere else:
    std::cout<<"<return>"<<sd::endl;
    std::cin.get();

    //release shared memory here:
    smp.reset();
}

    首先定义一个 deleter SharedMemoryDetacher,用来卸除（detach) shared memory。这个 deleter 会释放被 getSharedlntMemory() 取得并附着（attach) 的shared memory。
    为确保这 个deleter会在shared memory最末一次被使用后被调用，我们在“getSharedIntMemory() 为已附着内存建立起一个Shared_ptr” 时把它当作第二实参传递： 
        return std::shared_ptr<int>(static_cast<int*>(mem), 
                                    SharedMemoryDetacher()) ； //calls shmdt() 
    也可以改用lambda (以下略过前缀std::): 
        return shared_ptr<int>(static_cast<int*>(mem), 
                            [](int *p) { 
                                cout <<"unlink /tmpl234" << endl;
                                if(shm_unlink("/tmp1234") !=0){
                                    std::cerr<<"OOPS: shm_unlink() failed"<<std::endl;
                                 });
    注意，传入的deleter不允许抛出异常，因此这电我们只写出一个错误信息给std::cerr。 由于shm_unlink()的签名式相符，所以你甚至可直接使用shm_unlink()作为deleter如果你不打算检查其返回值的话： 
        return std:：shared_ptr<int>(static_cast<int*>(mem), 
                            shm_nnliiik)； 
    注意，shared_ptr只提供operator *和operator->，指针运算和operator[]都末提供。因此，如果想访问内存，你必须使用get()获得被shared_ptr包覆（wrapped)的内部指针，提供完全的指针语义： 
        smp.get()[i] = i» 4 2 ; 
    上述结果等同于以下调用： 
        (&*smp)[i] = i*42; 
    对于上述两个例子，另一个实现技术也件更千净利落：只建立一个new class X, 让其构造函数执行初始工作，让其析构函数执行清理工作。然后你就只是使用shared_ptr每以new建立的class X对象。这样的好处是，你可定义一个更直观的接口，
    例如为一个表现shared memory的对象定义一个operator []。然而随后你应该谨慎地想想copy和assignment操作， 如果不能肯定，就禁用（disable) 它们。

Class weak_ptr
    使用shared_ptr主要是为了避免操心pointer 指向的资源。先前已说过，shared_ptr会自动释放“不再被需要的对象”的相应资源。
        然而，某些情势下，这样的行为无法运作或不被期望：
    例子之一是所谓的cyclic reference (环式指向)。如果两对象使用shared_ptr互相指向对方，而一旦不存在其他reference指向它们时，你想释放它们和其相应资源。这种情况下shared_ptr不会释放数据，因为每个对象的use_count()仍是1。
    此时你或许会想使用寻常的pointer，但这么做却又得自行照顾和管理“相应资源的释放”。 
    另一个例子发生在你“明确想共享但不愿拥有”某对象的情况下。你要的语义是：reference的寿命比其所指对象的寿命更长。因此sliared__ptr绝不释放对象，而寻常pointer可能不会注意到它们指向的对象已经不再有效，导致“访问已被释放的数据”的风险。
    于是标准库提供了 class weak_ptr ,允许你“共享但不拥有”某对象。这个class会建立起 一个shared pointer。一旦最末一个拥有该对象的shared pointer失去了拥有权，任何weak pointer都会自动成空（empty)。因此，在default和copy构造函数之外，
    class weak_ptr只 提供“接受一个shared_ptr” 的构造函数。 
    你不能够使用操作符*和->访问weak_ptr指向的对象。而是必须另外建立一个shared pointer。这是合理的设计，基于两个理由： 
    1.在weak pointer之外建立一个shared pointer可因此检查是否（仍）存在一个枏应对象。 如果不操作会抛出异常或建立一个empty shared pointer (实际究竞哪种行为乃取决手 你所执行的是哪一种操作)。 
    2.当指向的对象正被处理时，shared pointer无法被释放。 
    基于以上所述，class weak_ptr只提供小量操作，只够用来创建、复制、赋值weak pointer, 以及转换为一个shared pointer，或检查自己是否指向某对象。

使用Class weak_ptr
    考虑以下例子：
#include "stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

class Person {
public :
	string name;
	shared_ptr<Person> mother;
	shared_ptr<Person> father;
	vector<shared_ptr<Person>> kids;

	Person(const string& n,
		shared_ptr<Person> m = nullptr,
		shared_ptr<Person> f = nullptr)
		: name(n), mother(m), father(f) {
	}

	~Person() {
		cout << "delete" << name << endl;
	}
};

shared_ptr<Person> initFamily(const string& name)
{
	shared_ptr<Person> mom(new Person(name + "'s mom"));
	shared_ptr<Person> dad(new Person(name + "'s dad"));
	shared_ptr<Person> kid(new Person(name, mom, dad));
	mom->kids.push_back(kid);
	dad->kids.push_back(kid);
	return kid;
}

int main()
{
	shared_ptr<Person> p = initFamily("nico");

	cout << "nico's family exists" << endl;
	cout << "- nico is shared" << p.use_count() << " times " << endl;
	cout << "- name of 1st kid of nico's mom: "
		<< p->mother->kids[0]->name << endl;
		
	p = initFamily("jim");
	cout << "jim's family exists" << endl;
}
    这里的class Person拥有一个name和若干reference指向其他Person---也就是父母 (mother 和 father)及 kids (那是个vector)。 
    首先，initFamily()建立三个Person: mom、dad 和kid,根据传入的实参将所有姓名初始化；此外还设定kid的父母，并将kid插入其父母的kids容器内。最终initFamily()返回kids图5.2展示了 initFamily()调用后的结果，以及陚值给p之后的结果。 
    如你所见，p是指向上述家庭的最来一个handle。而在内部，每个Person对象都有着 reference从kid指向其父母以及反向指向。因此，在p被陚值之前，nico被共享三次。现在，如果我们释放手上最末一个指向该家庭的handle---也许是对p指派一个新person
    或一个nullptr，也许是main()结束时离开了p作用域——没有任何Person会被释放，因为它们都仍至少被一个shared pointer指向。于是每个Person的析构函数（会打印出"delete name")从未被调用:
        nico‘s family exists
        - nico shared 3 times
        - name of 1st kid of nicos mom:nico
        jim’s family exists
    这种情况下改用weak_ptr会带来帮助。我们可以声明kids为一个内含众多weak_ptr的vector:
    class Person { 
    public: 
    string name; 
    shared_ptr<Person> mother； 
    shared_ptr<Person> father; 
    vector<weak_ptr<Person>> kids ； // weak pointerIII Person (const stringft n, 
    shared_ptr<Person> m = nullptr, 
    shared_ptr<Person> f = nullptr) 
    :name(n), mother(m), father(f) { 
    } 
    ~Person() {
        cout<<"delete"<<name<<endl;
    }
    这么做之后，我们就可以打破shared pointer形成的循环（cycle),使得在一个方向上（从kid 到 parent)用的是 shared pointer，而从 parent 到 kids 则运用weak pointer (图5.3中的虚线)。 

    经过这番改造，程序输入如下：
     nico's family exists
     - nico shared 1 times
     - name of 1st kid of nicos mom: nico
     delete nico 
     delete nico's dad 
     delete nico's mom 
     jim's family exists 
     delete jim 
     delete jim's dad 
     delete jim's mom 
    一旦我们失去“指向某个家庭”的handle---也许因为它被赋予一个新值，也许因为离开了 main()--- 这个家庭中的kid对象也就失去了其最末一个拥有者(use.count()产出1)， 导致其父母也都失去了最末拥有者。于是，最初以new建立的所有对象，现在都被delete, 因此它们的析构函数都会被调用。 
    注意，使用weak pointer时，我们必须轻微改变被指对象的访问方式。不应该再调用 
        p->mother->kids[0]->name 
    现在必须在式子内加上lock() ： 
        p->mother->kids[0].lock()->name
    这会导致新产生一个“得自于kids容器内含之weak_ptr” 的shared_ptr。如果无法进行这样的改动---例如由于对象的最末拥有者也在此时释放了对象---lock()会产出一个 empty shared_ptr。 这种情况下调用*或-> 操作符会引发不明确行为。 
        如果不确定隐身于weak pointer背后的对象是否仍旧存活，你有以下数个选择： 
    1.调用expired(),它会在weak_ptr不再共享对象时返回true。这等同于检査use_count() 是否为0，但速度较快。 
    2.可以使用相应的shared_ptr构造函数明确将weak_ptr转换为一个shared_ptr。如果被指对象已经不存在，该构造函数会拋出一个bad_weak_ptr 异常，那是一个派生自 std::exception 的异常，其what() 会产出"bad_weak_ptr"。
    3.调用 use_count(),询问相应对象的拥有者数tt。如果返回0表示不存在任何有效对象。然而请注意，通常只应为了调试而调用use_count(), C++标准库明确告诉我们：“use_count()并不总是很有效率。” 
    下面举个例子:
        try {    
            shared_ptr<string> sp(new string("hi"));    //create shared pointer
            weak_ptr<string> wp = sp;                   //create weak pointer out of it
            sp.reset()；                                //release object of shared pointer
            cout << wp.use_count() << endl；            //prints: 0 
            cout << boolalpha << wp.expired()<<endl;    // prints: true
            shared_ptr<string> p(wp);                   //throw std::bad_weak_ptr 
        }
        catch (const std::exception& e) { 
            cerr << "exception: " << e.what()<<endl；    //prints bad_weak_ptr
        }

误用Shared Pointer

    虽然Shared_ptr 强化了程序安全，但由于对象的相应资源往往被自动释放，当对象不再被使用时有可能出现问题。刚刚讨论过的一个问题便是，由于循环依赖(cyclic dependency) 造成的“dangling pointer”（空荡指针)。 
    另一个可能出现的问题是，你必须确保某对象只被一组（one group of) shared pointer 拥有。下面的代码是错的：
        int *p = new int;
        shared_ptr<int> sp1(p);
        shared_ptr<int> sp2(p);         //ERROR: two shared pointers manage allocated int
    问题出在sp1和sp2都会在丢失p的拥有权时释放相应资源（亦即调用delete）。一般而言，“两组拥有权”意味着，相应资源的释放会被执行两次---每当每一组最末一个拥有者“丢失其拥有权”或“被销毁”就会执行一次。因此，
    总是在创建对象和其相应资源的那一刻直接设立smart pointer。
        shared_ptr<int> sp1(new int);
        shared_ptr<int> sp2(sp1);   //OK
    这个问题也可能间接发生。在先前介绍过的例子中，假设我们打算为Person引入一个成员函数，用来建立“从kid指向parent”的reference及其反向reference:
        shared_ptr<Person> mom(new Person(name+"'s mom"));
        shared_ptr<Person> dad(new Person(name+"'s dad"));
        shared_ptr<Person> kid(new Person(name));
        kid->setParentsAndTheirKids(mom,dad);
    下面是一份朴素直观的setParentsAndTheirKids()版本
        class Person{
            public:
                ...
                void setParentsAndTheirKids (shared_ptr<Person> m = nullptr,
                                             shared_ptr<Person> f = nullptr){
                    mother = m;
                    father = f;
                    if(m != nullptr)
                    {
                        m->kids.push_back(shared_ptr<Person>(this));    //ERROR
                    }
                    if(f != nullptr)
                    {
                        f->kids.push_back(shared_ptr<Person>(this));    //ERROR
                    }
            }
            ...
        }
    问题出在“得自this的那个shared pointer”的建立。之所以那么做是因为，我们想要设置mother和father两成员的kids。但为了那么做，我们需要一个shared pointer指向kid，而那是我们手上没有的。然而，
    根据this建立起一个新的shared pointer并不能解决事情，因为这么一来就是开启一个新的拥有者团队（a new group of oweners）。
    对付这个问题的办法之一是，将指向kid的那个shared pointer传递为第三实参。但C++标准库提供了另一个选项：class std::enable_shared_from_this<>。 
    你可以从class std:：enable_shared_from_this<>派生你自己的class，表现出“被shared pointer管理”的对象做法是将class名称当作template实参传入。
    然后你就可以使用一个派生的成员函数shared_from_this()建立起一个源自this的正确shared_ptr。
class Person : public enable_shared_from_this<Person> {
public:
	string name;
	shared_ptr<Person> mother;
	shared_ptr<Person> father;
	vector<weak_ptr<Person>> kids;  // weak pointer !!!

	Person(const string& n)
		: name(n) {
	}

	void setParentsAndTheirKids(shared_ptr<Person> m = nullptr,
		shared_ptr<Person> f = nullptr) {
		mother = m;
		father = f;
		if (m != nullptr) {
			m->kids.push_back(shared_from_this());      //OK
		}
		if (f != nullptr) {
			f->kids.push_back(shared_from_this());      //OK
		}
	}

	~Person() {
		cout << "delete " << name << endl;
	}
};
    注意，你不能在构造函数内调用shared_from_this() (好吧，其实是可以的，但会导致运行期错误):
class Person ： public std:：enable_shared_from_this<Person> { 
public: 
    Person (const stringft n, 
        shared_ptr<Person> m = nullptr, 
        shared_ptr<Person> f = nullptr) 
    :name(n), mother(m), father(f) { 
    if (m ！= nullptr) { 
        m->kids.push_back(shaured^f rom.this()) ； //ERROR 
    }
    if (f ！= nullptr) { 
        f->kids.push.back(shared.from_this()) ；   //ERROR
    }
};
    问题在于shred_ptr本身被存放于Person的base class，也就是enable_shared_from_this<>内部的一个private成员中，在Person构造结束之前。
    所以，在初始化shared pointer的那个对象的构造期间，绝对无法建立shared pointer的循环引用（cyclic reference）。你必须以两个步骤完成它---以某种方式。

细究Shared Pointer和Weak Pointer
    
    让我们总结展示 shared/weak pointer提供的完整接口。

细究Class shared_ptr
    class shared_ptr提供的是带有“共享式拥有”语义的smart pointer概念。无论何时当shared pointer的最末一个拥有者被销毁，相应对象就会被deleted(或是其相应资源被清理)。 
    Class sharet_ptr<>被模板化，模板参数是“原始pointer所指对象”的类型： 
    namespace std { 
        template <typename T> 
        class shared_ptr
        {
        public: 
            typedef T element_type；
            ...
        };
    }

    元素类型可以是void，意味着shared pointer共享的对象有着一个未具体说明的类型，例如 void*
    一个empty shared_ptr并不能分享对象拥有权，所以use_count()返回0。然而请注意，由于设计了一个特殊构造函数，使得empty shared pointer还是可以指向对象。 
    表5.3和表5.4列出了 shared pointer提供的所有操作。 
    一旦拥有权被转移至一个“已拥有其他对象”的shared pointer, deleter就会针对先前被拥有的那个对象被调用---如果那个shared pointer是最末一个拥有者的话。如果shared pointer取得一个新值（不论是由于被陚予新值或由于调用reset()),
    相同事情发生：如果 shared pointer先前拥有某对象，而且它是最末一个拥有者，相应的deleter (或delete)会为此对象而被调用。再请注意，传入的deleter不该拋出异常。 
    Shared pointer可能使用不同的对象类型，前提是存在一个隐式的pointer转换。基于此，构造函数、赋值操作符和reset()都是member template,比较操作符则针对不同类型模板化。 
    每一个比较操作符所比较的都是shared pointer内部那个raw pointer（初始指针） (也就是说，它们对get()的返回值调用同操作符)。它们全都有“接受nullptr为实参”的重载版本。因此你可以检査是否带有一个合法pointer,甚至检查是否raw pointer小干或大于nullptr。 
    构造函数收到的实参如果是个weak_ptr而且是empty (亦即expired()产出true)， 会抛出bad.weak_ptr异常。

    get_deleter()会取得一个pointer to deleter（如果有定义deleter的话），要不就取得nullptr。只要shared pointer还拥有那个deleter，该pointer就有效。然而为了取得deleter，你必须以其类型作为template实参，例如：
        auto del = [](int* p){
            delete p;
        };
        std::shared_ptr<int> p(new int,del);
        decltype(del)* pd = std::get_deleter<decltype(del)>(p);
    注意，shared pointer并未提供release()操作用以放弃拥有权并将对象控制权交给调用者。

更复杂的shared_ptr操作
    这里的少数操作可能并不显著。它们的大部分在[N2351:SharedPtr]中都有介绍。
    //暂略

细究Class weak_ptr
    weak_ptr是shared的帮手，用来共享对象但不拥有对象。它的use_count()返回的是对象呗shared_ptr拥有的次数，至于weak_ptr对它的共享次数是不计的。而且weak_ptr可以为空（empty）---如果它不是以一个shared_ptr为初值或其对应对象的最末一个拥有者被删除就会发生这种情况。
    Classweak_ptr<>也被模板化了，以“初始pointer指向之对象”的类型为模板参数:
    namespace std{
        template<typename T>
        class weak_ptr
        {
            public:
                typedef T element_type；
                ...
        };
    }
    表5.5列出了weak pointer提供的所有操作。 
        Default 构造函数会创建出一个empty weak pointer,那意味着expired()的结果是true。由于lock()会产出一个shared pointer，因此在那个shared pointer寿命期间，该对象的
        使用次数会多1。这是处理weak pointer所共享的对象的唯一途径。

线程安全(Thread-Safe)的Shared Pointer接口
    一般而言，shared pointer并非线程安全。因此，为避免data race(数据竞争)造成的不明确行为，当你在多个线程中以shared pointer指向同一对象，你必须使用诸如mutex或lock等技术。
    不过当某个线程改变对象时，其他线程读取其使用次数并不会导致data race，虽然的值有可能不是最新的。现实中我们的确可以让一个线程检查使用次数而让另一个线程处理对象。
    相当于寻常pointer之原子性（atomic）C-style接口，为shared pointer设计的重载版本允许并发处理shared pointer。注意，并发访问的是pointer而非其指向的值。
    例如：
    std::shared_ptr<X> global;  //initially nullptr

    void foo()
    {
        std::shared_ptr<X> local{new X};
        ...
        std::atomic_store(&global,local);
    }

    表5.6列出了高层接口。

Class unique_ptr
    
    unique_ptr是C++标准库自C++11起开始提供的类型。它是一种在异常发生时可帮助避免资源泄露的smart pointer。一般而言，这个smart pointer实现了独占式拥有概念，意味着它可确保一个对象
    和其相应资源同一时间只被一个pointer拥有。一旦拥有者被销毁或变成empty，或开始拥有另一个对象，先前拥有的那个对象就会被销毁，其任何相应资源亦会被释放。
    Class unique_ptr继承class auto_ptr，后者由C++98引入但已不再被认可。Class unique_ptr提供了一个更简明干净的接口，比auto——pointer更不易出错。

Class unique_ptr 的目的 
    函数往往以下列方式运作：    
    1.获得某些资源 
    2.执行某些操作 
    3.将取得的资源释放掉 
    如果限定local对象，那么函数进入点获得的资源会在函数结束前被动释放，因为那些local对象的析构函数会被调用。但如果资源是显式获得，不与任何对象捆绑，它们就必须被明确释放掉。
    程序使用pointer时，资源往往被显示管理（managed explicitly)。 
    以此方式使用pointer的典型例子是，以new和delete创建和销毁对象： 
    void f() 
    { 
        ClassA* ptr = new ClassA;    //create an object explicitly
        ...                          //perform some operations
        delete ptr;                  //clean up(destroyed the object explicitly)
    }
    这个函数将成为麻烦制造者。一个明显的问题是，有可能忘记delete对象，特别是如果你在函数中有个return语句。另一个较不明显的危险是它可能拋出异常，那将立刻退离(exit) 函数，
    末尾的delete语句也就没机会被调用，导致内存泄漏，或更一般性地说，资源泄漏。 

    为了避免如此的资源泄露，通常函数会捕捉所有异常，例如：

    void f() 
    { 
        ClassA* ptr = new ClassA;    //createanobjectexplicitly 
        try {                       
            ...                      //perform some operations
        }
        catch(...){                  //for any exception
            delete ptr;              //~clean up   
            throw;                   //~rethrow the exception
        }   

        delete ptr;                  //clean up on normal end
    }
    为了在异常发生时能够适当处理好对象的删除，代码变得比较复杂，而且累赘。如果第二个对象也以此方式处理，或需要一个以上的catch子句，情况会变得更糟。这是一种不好的编程风格，应该避免，因为它复杂而又容易出错。
    对此，smart pointer 可以带来帮助。Smart pointer可以在它自身被销毁---不论是正常结束或异常结束。Class unique_ptr就是这样一个smart pointer。
    unique_ptr是“其所指向之对象”的唯一拥有者。自然而然地，当unique_ptr被销毁，其所指向的对象也就动被销毁。unique_ptr的必要条件就是，它指向的对象只有一个拥有者。
    用unique_ptr改写先前的例子如下： 
    //header file for unique_ptr 
    #include <memory> 

    void f() 
    { 
        //create and initialize an unique_ptr 
        std::unique<ClassA> ptr(new ClassA); 
        ...                 //perform some operations
    } 
    这就够了，再也不需要delete语句和catch 子句。

使用unique_ptr
    
    unique_ptr有着与寻常pointer非常相似的接口,操作符*用来提领（dereference）指向对象，操作符-> 用来访问成员---如果被指向的对象来自class或struct: 
    //create and initialize(pointer to)string: 
    std::unique_ptr<std::string> up(new std::string("nico"))；

    （*up）[0] = 'N';           //replace first character
     up->append("lai");        //append some characters
     std::cout<<*up<<std::endl;//print whole string
    然而它不提供pointer算数如++等，这被视为优点，因为pointer算术运算往往是麻烦的来源。
    注意，class unique_ptr<>不允许你以斌值语法将一个寻常的pointer当作初值。因此你必须直接初始化unique_ptr，像下面这样： 
    std: :unique_ptr<int> up = new int;    //ERROR 
    std: :unique_ptr<int> up (new int);    //OK 
    unique.ptr不必一定拥有对象，它也可以是empty。例如当它被default构造函数创建出来：
    std::unique_ptr<std::string> up; 
    你也可以对它赋予nullptr或调用reset()： 
    up = nullptr； 
    up.reset()； 
    此外，你可以调用release(),获得unique_ptr拥有的对象并放弃拥有权，于是调用者现在对该对象有了责任： 
    std:：unique_ptr<std::string> up(new std::string ("nico"))； 
    ...
    std::string* sp = up.release(); //up loses ownership 
    你可以调用操作符bool()用以检查是否unique pointer拥有（任何）对象： 
    if (up) *{   //if up is not empty 
        std::cout << *up << std::endl； 
    }
    你也可以拿unique pointer和nullptr比较，或查询unique.ptr内的raw pointer---如果 unique.ptr来未拥有任何对象，就会获得一个nullptr：
    if(up != nullptr)       //if up is not empty
    if(up.get() != nullptr) //if up is not empty

移转unique_ptr的拥有权 

    unique_ptr提供的语义是“独占式拥有”。然而其实责任在程序员，由他/她确保“没有任何两个unique pointer以同一个pointer作为初值”： 
    std::string* sp = new std::string("hello"); 
    std::unique_ptr<std::string>upl (sp); 
    std::unique_ptr<std::string>up2(sp) ; //ERROR:  upl and up2 own same data 
    很不幸这是个运行期错误，程序员必须避免这样的过失。 
    这导致这样的问题：unique_ptr的copy构造函数和assignment操作符如何运作？答案很简单：你不可以对unique pointer执行copy或assign---如果你要的是寻常的copy语义。然而你可以使用自C++11起提供的move语义。这种情况下上述的copy构造函数或assignment操作符会将拥有权移交给另一个unique pointer。
    举个例子，考虑这样使用copy构造函数： 
    //initialize a unique_ptr with a new object 
    std::unique_ptr<ClassA> up1(new ClassA)； 
    //copy the unique_ptr 
    std::unique_ptr<ClassA> up2(up1)； // ERROR: not possible 
    //transfer owner ship of the unique_ptr 
    std::unique_ptr<ClassA> up3(std::move(upl)); //OK 
    第一个语句结束后，up1拥有以new创建的对象。第二个语句试阁调用copy构造函数，但发生编译期错误，因为up2不可以成为原对象的另一个拥有者，毕竟一次只允许存在一个拥有者。第三个语句则是将拥有权从upl移至up3。在那之后up3就拥有了先前以new建立起来的对象，而upl不再拥有它。一旦up3被销毁，
    以new ClassA创建出来的对象也就被deleted。   
    Assignment操作符的行为和上面所说很类似： 
    //initialize a unique_ptr with a new object 
    std::unique_ptr<ClassA> upl(new ClassA)； 
    std:: unique_ptr<ClassA> up2； //create another unique_ptr 

    up2 = up1；                    //ERROR:not possible

    up2 = std::move(up1);          //assign the unique_ptr
    这里的move assignment将拥有权由up1移交给up2.于是up2拥有了先前被up1拥有的对象。
    
    如果上述赋值动作之前up2原本拥有对象，会有一个delete动作被调用，刪除该对象： 
    //initialize a unique_ptrwith a new object 
    std::unique_ptr<ClassA> up1(new ClassA); 
    //initialize another unique_ptr with a new object 
    std::unique_ptr<ClassA> up2(new ClassA)； 

    up2 = std::move(up1);           //move assign the unique_ptr
                                    //~delete object owned by up2
                                    //~transfer ownership from up1 to up2
    赋值nullptr也是可以的，和调用reset()效果相同： 
    up = nullptr; //deletes the associated object、if any 
源头和去处（Source and Sink) 
    拥有权的移转暗暗指出了 unique-ptr的一种用途：函数可利用它们将拥有权转移给其他函数。这会发生在两种情况下： 
    1.函数是接收端。如果我们将一个由std::move()建立起来的unique_ptr以rvalue reference身份当作函数实参，那么被调用函数的参数将会取得unique _ptr的拥有权。因此，如果该函数不再转移拥有权，对象会在函数结束时被deleted: 
    void sink (std::unique_ptr<ClassA> up)    //sink() gets ownership
    {
        ...
    }

    std::unique_ptr<ClassA> up(new ClassA);
    ...
    sink(std::move(up));    //up loses ownership
    ...
    2.函数是供应端。当函数返回一个unique_ptr，其拥有扠会转移至调用端场景（calling context）内。下面的例子展示了这项技术： 
    std::unique_ptr<ClassA> source()
    { 
        std::unique_ptr<ClassA> ptr (new ClassA); //ptr owns the new object 
        ...
        return ptr;                 //transfer owenership to calls function
    }

    void g()
    {
        std::unique_ptr<ClassA> p;

        for(int i = 0;i < 10;i++){
            p = source();       //p gets ownership of the returned object
                                //(previously returned object of f() gets deleted)
            ...
        }
    }//last-owned object of p gets deleted
    每当source()被调用，就会以new创建对象并返回给调用者，夹带着其拥有权。返问值被陚值给P，于是拥有权也被移转给P。在循环的第二次迭代中，对p陚值导致p先前拥有的对象会被删除。一旦g()结束，p被销毁，导致最后一个由p拥有的对象被析构。
    无论如何都不会发生资源泄露。即使抛出异常，任何拥有数据的unique _ptr也能 确保数据被删除。 
    在这里，sourceO的return语句不需要std::move()的原因是，C++ll语言规定, 编译器应该自动尝试加上move。 

unique_ptr被当作成员 
    在class内使用unique_ptr可避免资源泄漏。如果你使用unique_ptr取代寻常pointer,就不再需要析构函数，因为对象被删除会连带使所有成员被删除。此外unique_ptr也可协助避免“对象初始化期间因拋出异常而造成资源泄漏”。
    注意，只有当一切构造动作都完成了，析构函数才有可能被调用。因此一旦构造期间发生异常，只有那些已完全构造好的对象，其析构函数才会被调用。所以，对于“拥有多个raw pointer”的class，如果构造期间第一个new成功而第二个失败，就可能导致资源泄漏。例如：

class ClassB { 
    private： 
        ClassA* ptrl;   //pointer members
        ClassA* ptr2;   
    public:
        //constructor that initializes the pointers 
        //-will causere source leak if second new throws 
ClassB (int vail, int val2) 
    :ptr1(new ClassA(vail)), ptr2(new ClassA(val2)) { 
    }
        //copy constructor 
        //-might cause resource leak if second new throws 
ClassB (const ClassB& x) 
: ptr1 (new ClassA(*x.ptrl)), ptr2(new ClassA(*x.ptr2)) { 
    }










