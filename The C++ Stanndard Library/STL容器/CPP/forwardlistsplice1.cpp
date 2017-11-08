#include "stdafx.h"
#include <forward_list>
#include "print.hpp"
using namespace std;

int main()
{
	forward_list<int> l1 = { 1, 2, 3, 4, 5 };
	forward_list<int> l2 = { 97, 98, 99 };

	// find 3 in l1
	auto pos1 = l1.before_begin();
	for (auto pb1 = l1.begin(); pb1 != l1.end(); ++pb1, ++pos1) {
		if (*pb1 == 3) {
			break;  // found
		}
	}

	// find 99 in l2
	auto pos2 = l2.before_begin();
	for (auto pb2 = l2.begin(); pb2 != l2.end(); ++pb2, ++pos2) {
		if (*pb2 == 99) {
			break;  // found
		}
	}

	// splice 3 from l1 to l2 before 99
	l2.splice_after(pos2,l1,  // destination
		pos1);     // source

	PRINT_ELEMENTS(l1, "l1: ");
	PRINT_ELEMENTS(l2, "l2: ");
}






