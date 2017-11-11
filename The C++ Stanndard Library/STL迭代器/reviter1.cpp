#include "stdafx.h"
#include<list>
#include <algorithm>
#include <iostream>

using namespace std;
void print(int elem)
{
	cout << elem << ' ';
}
int main()
{
	//create list with elements from  1 to 9
	list<int> coll = { 1,2,3,4,5,6,7,8,9 };

	//print all elements in normal order
	for_each(coll.begin(), coll.end(),			//range
		print);									//operation
	cout << endl;

	//print all elements in reverse order
	for_each(coll.rbegin(), coll.rend(),		//range
		print);									//operations
	cout << endl;

}
