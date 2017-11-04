#include "stdafx.h"
#include<list>
#include <string>
#include <algorithm>
#include <iostream>
#include  <iterator>
using namespace std;

int main()
{
	std::list<int> coll{1,5,6,7,8,9,5,4,5,5,5};
	coll.push_back(2);
	coll.push_back(3);
	
	cout << endl;
	
	copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, " "));

	cout << endl;

	coll.remove_if( [] (int i) {
		return i % 2 == 0;
	});
	copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, " "));

	cout << endl;

	for (auto& elem:coll)
	{
		cout << elem << endl;
	}

	coll.unique();

	copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, " "));

	cout << endl;

	std::list<int> coll2{ 1,1,1,1,1 };

	coll.splice(coll.cbegin(), coll2);
	copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, " "));

	//coll2已经空了,list 要么成功要么没有任何作用
	coll2.merge(coll2);
	copy(coll2.cbegin(), coll2.cend(), ostream_iterator<int>(cout, " "));

}