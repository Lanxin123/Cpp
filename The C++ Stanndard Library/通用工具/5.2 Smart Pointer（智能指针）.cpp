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

    几乎毎一个稍有分量的程序郎需要“在相同时间的多处地点处理或使用对象”的能力。为此，你必须在程序的多个地点指向(refer to)同一对象。虽然C++语言提供reference 和 pointer,还是不够，因为你往往必须确保当“指针对象”的最某一个reference
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














































