//��������
#include <iostream>
#include <vector>
using namespace std;
int main(){
    vector<int>vi;
    int a;
    while(true)
    {
    cout<<"����һ����������0ֹͣ���룺";
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
//����һ��list������ʵ��LISTINT
typedef list<int> LISTINT;

//����һ��list������ʵ��LISTCHAR
typedef list<int> LISTCHAR;

int main(){
    //--------------------------
    //��list����������������
    //--------------------------
    //��LISTINT����һ����ΪlistOne��list����
    LISTINT listOne;
    //����iΪ������
    LISTINT::iterator i;

    //��ǰ����listOne�������������
    listOne.push_front (2);
    listOne.push_front (1);

    //�Ӻ�����listOne�������������
    listOne.push_back (3);
    listOne.push_back (4);

    //��ǰ�����ʾlistOne�е�����
    cout<<"listOne.begin()--- listOne.end():"<<endl;
    for (i = listOne.begin(); i != listOne.end(); ++i)
        cout << *i << " ";
    cout << endl;

    //�Ӻ���ǰ��ʾlistOne�е�����
LISTINT::reverse_iterator ir;
    cout<<"listOne.rbegin()---listOne.rend():"<<endl;
    for (ir =listOne.rbegin(); ir!=listOne.rend();ir++) {
        cout << *ir << " ";
    }
    cout << endl;

    //ʹ��STL��accumulate(�ۼ�)�㷨
    int result = accumulate(listOne.begin(), listOne.end(),0);
    cout<<"Sum="<<result<<endl;
    cout<<"------------------"<<endl;

    //--------------------------
    //��list���������ַ�������
    //--------------------------

    //��LISTCHAR����һ����ΪlistOne��list����
    LISTCHAR listTwo;
    //����iΪ������
    LISTCHAR::iterator j;

    //��ǰ����listTwo�������������
    listTwo.push_front ('A');
    listTwo.push_front ('B');

    //�Ӻ�����listTwo�������������
    listTwo.push_back ('x');
    listTwo.push_back ('y');

    //��ǰ�����ʾlistTwo�е�����
    cout<<"listTwo.begin()---listTwo.end():"<<endl;
    for (j = listTwo.begin(); j != listTwo.end(); ++j)
        cout << char(*j) << " ";
    cout << endl;

    //ʹ��STL��max_element�㷨��listTwo�е����Ԫ�ز���ʾ
    j=max_element(listTwo.begin(),listTwo.end());
    cout << "The maximum element in listTwo is: "<<char(*j)<<endl;
}

#include <iostream>
#include <list>

using namespace std;
typedef list<int> INTLIST;

//��ǰ�����ʾlist���е�ȫ��Ԫ��
void put_list(INTLIST list, char *name)
{
    INTLIST::iterator plist;

    cout << "The contents of " << name << " : ";
    for(plist = list.begin(); plist != list.end(); plist++)
        cout << *plist << " ";
    cout<<endl;
}

//����list�����Ĺ���
int main(){
//list1�����ʼΪ��
    INTLIST list1;   
    //list2���������10��ֵΪ6��Ԫ�� 
    INTLIST list2(10,6); 
    //list3���������9��ֵΪ6��Ԫ�� 
    INTLIST list3(list2.begin(),--list2.end()); 

    //����һ����Ϊi��˫�������
    INTLIST::iterator i;

    //��ǰ�����ʾ��list�����Ԫ��
    put_list(list1,"list1");
    put_list(list2,"list2");
    put_list(list3,"list3");
    
//��list1���к����������Ԫ��
list1.push_back(2);
list1.push_back(4);
cout<<"list1.push_back(2) and list1.push_back(4):"<<endl;
    put_list(list1,"list1");

//��list1����ǰ���������Ԫ��
list1.push_front(5);
list1.push_front(7);
cout<<"list1.push_front(5) and list1.push_front(7):"<<endl;
    put_list(list1,"list1");

//��list1�����м��������3��9 
list1.insert(++list1.begin(),3,9);
cout<<"list1.insert(list1.begin(),3,9):"<<endl;
    put_list(list1,"list1");

//���������ຯ��
cout<<"list1.front()="<<list1.front()<<endl;
cout<<"list1.back()="<<list1.back()<<endl;

//��list1���е�ǰ�����ȥһ��Ԫ��
list1.pop_front();
list1.pop_back();
cout<<"list1.pop_front() and list1.pop_back():"<<endl;
    put_list(list1,"list1");

//���list1�еĵ�2��Ԫ��
list1.erase(++list1.begin());
cout<<"list1.erase(++list1.begin()):"<<endl;
    put_list(list1,"list1");

//��list2��ֵ����ʾ
list2.assign(8,1);
cout<<"list2.assign(8,1):"<<endl;
    put_list(list2,"list2");

//��ʾ���е�״̬��Ϣ
cout<<"list1.max_size(): "<<list1.max_size()<<endl;
cout<<"list1.size(): "<<list1.size()<<endl;
cout<<"list1.empty(): "<<list1.empty()<<endl;

//list��������������
    put_list(list1,"list1");
    put_list(list3,"list3");
cout<<"list1>list3: "<<(list1>list3)<<endl;
cout<<"list1<list3: "<<(list1<list3)<<endl;

//��list1��������
list1.sort();
    put_list(list1,"list1");
    
//��ϴ���
list1.splice(++list1.begin(), list3);
    put_list(list1,"list1");
    put_list(list3,"list3"); 
}





