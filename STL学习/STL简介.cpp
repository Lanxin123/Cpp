//几个例子
#include <iostream>
#include <vector>
using namespace std;
int main(){
    vector<int>vi;
    int a;
    while(true)
    {
    cout<<"输入一个整数，按0停止输入：";
    cin>>a;
    if(a==0)
    break;
    vi.push_back(a);   
    vector<int>::iterator iter;
    for(iter=vi.begin();iter!=vi.end();++iter)
    cout<<*iter;      
    }
    return 0;
    }

#include <iostream> 
#include <string>
#include <vector> 
using namespace std; 
int main(){
string str="shiyang"; 
vector <string> vecstr; 
vecstr.push_back(str);
vector <string> ::iterator iter= vecstr.begin(); 
cout<<*iter<<endl;
return 0;
}
#include   <stdlib.h>   
#include   <windows.h>   
#include   <conio.h>   
#include   <map>   //STL
#include   <functional> //STL  
#include   <algorithm>   //STL
#include   <iostream>   
using namespace std;   
typedef  map<int,int*> m_iip;   
typedef  map<int,char*> m_icp;   
class  f_c{   
  int _i;   
  public:   
  f_c(int i):_i(i){
          
  }   
  void operator()(m_iip::value_type ite)   
  {   
  cout<<_i++<<"\t"<<ite.first<<" shi"<<endl;   
  }   
  void operator()(m_icp::value_type ite)  
  {   
  cout<<_i++<<"\t"<<ite.first<<" yang"<<endl;  
  }   
  };   
  void f(int i,int c)   
  {   
      
  }   
  int main(int argc,char* argv[]){
  m_iip  iip;   
  m_icp  icp;   
  int i=0;   
  iip.insert(make_pair(34,&i));   
  iip.insert(make_pair(67,&i));   
  iip.insert(make_pair(5,&i));   
  iip.insert(make_pair(342,&i));    
  char d=0;   
  icp.insert(make_pair(12,&d));   
  icp.insert(make_pair(54,&d));   
  icp.insert(make_pair(6,&d));   
  icp.insert(make_pair(92,&d));   
  for_each(iip.begin(),iip.end(),f_c(8));   
  for_each(icp.begin(),icp.end(),f_c(65));//   
  return 0;   
  }

#include <iostream>
#include <list>
#include <numeric>
#include <algorithm>
using namespace std;
//创建一个list容器的实例LISTINT
typedef list<int> LISTINT;

//创建一个list容器的实例LISTCHAR
typedef list<int> LISTCHAR;

int main(){
    //--------------------------
    //用list容器处理整型数据
    //--------------------------
    //用LISTINT创建一个名为listOne的list对象
    LISTINT listOne;
    //声明i为迭代器
    LISTINT::iterator i;

    //从前面向listOne容器中添加数据
    listOne.push_front (2);
    listOne.push_front (1);

    //从后面向listOne容器中添加数据
    listOne.push_back (3);
    listOne.push_back (4);

    //从前向后显示listOne中的数据
    cout<<"listOne.begin()--- listOne.end():"<<endl;
    for (i = listOne.begin(); i != listOne.end(); ++i)
        cout << *i << " ";
    cout << endl;

    //从后向前显示listOne中的数据
LISTINT::reverse_iterator ir;
    cout<<"listOne.rbegin()---listOne.rend():"<<endl;
    for (ir =listOne.rbegin(); ir!=listOne.rend();ir++) {
        cout << *ir << " ";
    }
    cout << endl;

    //使用STL的accumulate(累加)算法
    int result = accumulate(listOne.begin(), listOne.end(),0);
    cout<<"Sum="<<result<<endl;
    cout<<"------------------"<<endl;

    //--------------------------
    //用list容器处理字符型数据
    //--------------------------

    //用LISTCHAR创建一个名为listOne的list对象
    LISTCHAR listTwo;
    //声明i为迭代器
    LISTCHAR::iterator j;

    //从前面向listTwo容器中添加数据
    listTwo.push_front ('A');
    listTwo.push_front ('B');

    //从后面向listTwo容器中添加数据
    listTwo.push_back ('x');
    listTwo.push_back ('y');

    //从前向后显示listTwo中的数据
    cout<<"listTwo.begin()---listTwo.end():"<<endl;
    for (j = listTwo.begin(); j != listTwo.end(); ++j)
        cout << char(*j) << " ";
    cout << endl;

    //使用STL的max_element算法求listTwo中的最大元素并显示
    j=max_element(listTwo.begin(),listTwo.end());
    cout << "The maximum element in listTwo is: "<<char(*j)<<endl;
}

#include <iostream>
#include <list>

using namespace std;
typedef list<int> INTLIST;

//从前向后显示list队列的全部元素
void put_list(INTLIST list, char *name)
{
    INTLIST::iterator plist;

    cout << "The contents of " << name << " : ";
    for(plist = list.begin(); plist != list.end(); plist++)
        cout << *plist << " ";
    cout<<endl;
}

//测试list容器的功能
int main(){
//list1对象初始为空
    INTLIST list1;   
    //list2对象最初有10个值为6的元素 
    INTLIST list2(10,6); 
    //list3对象最初有9个值为6的元素 
    INTLIST list3(list2.begin(),--list2.end()); 

    //声明一个名为i的双向迭代器
    INTLIST::iterator i;

    //从前向后显示各list对象的元素
    put_list(list1,"list1");
    put_list(list2,"list2");
    put_list(list3,"list3");
    
//从list1序列后面添加两个元素
list1.push_back(2);
list1.push_back(4);
cout<<"list1.push_back(2) and list1.push_back(4):"<<endl;
    put_list(list1,"list1");

//从list1序列前面添加两个元素
list1.push_front(5);
list1.push_front(7);
cout<<"list1.push_front(5) and list1.push_front(7):"<<endl;
    put_list(list1,"list1");

//在list1序列中间插入数据3个9 
list1.insert(++list1.begin(),3,9);
cout<<"list1.insert(list1.begin(),3,9):"<<endl;
    put_list(list1,"list1");

//测试引用类函数
cout<<"list1.front()="<<list1.front()<<endl;
cout<<"list1.back()="<<list1.back()<<endl;

//从list1序列的前后各移去一个元素
list1.pop_front();
list1.pop_back();
cout<<"list1.pop_front() and list1.pop_back():"<<endl;
    put_list(list1,"list1");

//清除list1中的第2个元素
list1.erase(++list1.begin());
cout<<"list1.erase(++list1.begin()):"<<endl;
    put_list(list1,"list1");

//对list2赋值并显示
list2.assign(8,1);
cout<<"list2.assign(8,1):"<<endl;
    put_list(list2,"list2");

//显示序列的状态信息
cout<<"list1.max_size(): "<<list1.max_size()<<endl;
cout<<"list1.size(): "<<list1.size()<<endl;
cout<<"list1.empty(): "<<list1.empty()<<endl;

//list序列容器的运算
    put_list(list1,"list1");
    put_list(list3,"list3");
cout<<"list1>list3: "<<(list1>list3)<<endl;
cout<<"list1<list3: "<<(list1<list3)<<endl;

//对list1容器排序
list1.sort();
    put_list(list1,"list1");
    
//结合处理
list1.splice(++list1.begin(), list3);
    put_list(list1,"list1");
    put_list(list3,"list3"); 
}





