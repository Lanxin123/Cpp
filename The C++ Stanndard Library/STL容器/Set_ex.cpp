#include "stdafx.h"
#include<set>
#include <algorithm>
#include <iterator>
#include <iostream>

int main()
{
	std::multiset<int> coll{0,1,3,1,1,1,2};

	//remove first element with passed value
	std::multiset<int> ::iterator pos;
	pos = coll.find(1);
	if (pos != coll.end())
	{
		coll.erase(pos);
	}
	for (auto elem : coll)
	{
		std::cout << elem<<" ";
	}
}
