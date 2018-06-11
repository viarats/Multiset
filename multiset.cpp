// multiset.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Multiset.h"

using namespace std;

int main() {

	// examples:

	Multiset<int> m(3);
	m.add(5);
	m.add(7);
	m.add(6);
	m.add(9, 5);
	m.add(1);

	cout << "m:" << endl << endl;

	m.print();

	cout << endl << "--------------------------------" << endl << endl;

	cout << "m modified:" << endl << endl;

	m.remove(5);
	m.remove(7);
	m.removeAll(9);

	m.print();

	cout << endl << "--------------- + --------------" << endl << endl;

	Multiset<int> n;
	n.add(5);
	n.add(11, 2);
	n.add(6);
	n.add(8);

	cout << "n:" << endl << endl;

	n.print();

	cout << endl << "--------------- = --------------" << endl << endl;

	Multiset<int>* newMultiset;
	newMultiset = n.merge(m);
	
	newMultiset->print();

	delete newMultiset;

	cout << endl << "--------------------------------" << endl << endl;

	cout << "Count of elements in n: " << n.count() << endl;
	cout << "Repeat of element 11 in n: " << n[11] << endl;

	cout << "Is m subset of n? " << boolalpha << (m < n) << endl;

	cout << endl;

	return 0;
}