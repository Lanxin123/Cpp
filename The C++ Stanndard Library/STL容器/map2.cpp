#include "stdafx.h"
#include<map>
#include<string>
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
	// create map / associative array
	// - keys are strings
	// - values are floats
	typedef map<string, float> StringFloatMap;

	StringFloatMap stocks;			//create empty conainer

	//insert some elements
	stocks["BASF"] = 369.50;
	stocks["VW"] = 413.50;
	stocks["Daimler"] = 819.00;
	stocks["BMW"] = 834.00;
	stocks["Siemens"] = 842.20;

	//print all elements
	StringFloatMap::iterator pos;
	cout << left;	//left-adjust values
	for (pos = stocks.begin();pos!=stocks.end();++pos)
	{
		cout << "stock: " << setw(12) << pos->first
			<< "price: " << pos->second << endl;
	}
	cout << endl;

	//boom(all prices doubled)
	for (pos = stocks.begin();pos != stocks.end();++pos)
	{
		pos->second *= 2;
	}

	//print all elements
	for (pos = stocks.begin();pos!=stocks.end();++pos)
	{
		cout << "stock: " << setw(12) << pos->first
			<< "price: " << pos->second << endl;
	}
	cout << endl;

	//renmae key from "VM" to "Volkswagen"
	// - provided only by exchanging element
	stocks["Volkswagen"] = stocks["VW"];
	stocks.erase("VW");

	//print all elements
	for (pos = stocks.begin(); pos!=stocks.end();++pos)
	{
		cout << "stock: " << setw(12) << pos->first
			<< "price: " << pos->second << endl;
	}
	cout << endl;

}

