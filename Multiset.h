#pragma once
#include "RBTree.h"
#include "Pair.h"
#include <iostream>
#include <fstream>

using namespace std;

template <typename T>
class Multiset {
public:
	Multiset();
	~Multiset();
	Multiset(const T& data);
	Multiset(const Multiset<T>& other);
	Multiset<T>& operator=(const Multiset<T>& other);
	
	void add(const T& element);
	void add(const T& element, int count);

	void remove(const T& element);
	void removeAll(const T& element);

	bool isSubSet(Multiset<T>& m);
	Multiset<T>* merge(Multiset<T>& m);

	size_t count(const T& data);
	size_t count();

	void print();
	void restore();

	template <typename U>
	friend bool operator<(Multiset<U>& left, Multiset<U>& right);
	template <typename U>
	friend bool operator>(Multiset<U>& left, Multiset<U>& right);
	template <typename U>
	friend bool operator==(Multiset<U>& left, Multiset<U>& right);

	const size_t& operator[](const T& key);

private:
	RBTree<Pair<T>>* elements;
};

template <typename T>
Multiset<T>::Multiset() {
	elements = new RBTree<Pair<T>>;
}

template <typename T>
Multiset<T>::~Multiset() {
	delete elements;
}

template <typename T>
Multiset<T>::Multiset(const Multiset<T>& other) {
	elements = new RBTree<Pair<T>>;
	*elements = *other.elements;
}

template <typename T>
Multiset<T>::Multiset(const T& data) {
	elements = new RBTree<Pair<T>>;

	Pair<T> p(data);
	elements->insert(p);

	ofstream ofs("backup.dat", ios::binary);
	ofs << sizeof(*elements);
	ofs.write((char*)elements, sizeof(*elements));

	ofs.close();
}

template <typename T>
Multiset<T>& Multiset<T>::operator=(const Multiset<T>& other) {
	if (this != &other) {
		*elements = *other.elements;
	}

	return *this;
}

template <typename T>
void Multiset<T>::add(const T& element) {
	if (elements->search(element)) {
		elements->getElement(element).increaseCount();
	}
	else {
		Pair<T> p(element);
		elements->insert(p);
	}
	ofstream ofs("backup.dat", ios::binary);
	ofs << sizeof(*elements);
	ofs.write((char*)elements, sizeof(*elements));

	ofs.close();
}

template <typename T>
void Multiset<T>::add(const T& element, int count) {
	add(element);
	for (int i = 0; i < count - 1; i++) {
		elements->getElement(element).increaseCount();
	}

	ofstream ofs("backup.dat", ios::binary);
	ofs << sizeof(*elements);
	ofs.write((char*)elements, sizeof(*elements));
	
	ofs.close();
}

template <typename T>
void Multiset<T>::remove(const T& element) {
	if (elements->search(element)) {
		if (elements->getElement(element).getCount() < 2) {
			elements->remove(element);
		}
		else {
			elements->getElement(element).decreaseCount();
		}
	}

	ofstream ofs("backup.dat", ios::binary);
	ofs << sizeof(*elements);
	ofs.write((char*)elements, sizeof(*elements));

	ofs.close();
}

template <typename T>
void Multiset<T>::removeAll(const T& element) {
	if (elements->search(element)) {
		elements->remove(element);
	}

	ofstream ofs("backup.dat", ios::binary);
	ofs << sizeof(*elements);
	ofs.write((char*)elements, sizeof(*elements));

	ofs.close();
}

template <typename T>
size_t Multiset<T>::count() {
	return elements->getSize();
}

template <typename T>
size_t Multiset<T>::count(const T& element) {
	if (elements->search(element, &isEqual)) {
		return elements->getElement(element).getCount();
	}
	else {
		return 0;
	}
}

template <typename T>
const size_t& Multiset<T>::operator[](const T& key) {
	if (elements->search(key)) {
		return elements->getElement(key).getCount();
	}

	return 0;
}

template <typename T>
void Multiset<T>::print() {
	RBTree<Pair<T>>::Iterator it(elements);
	while (it.getTarget()) {
		cout << *it << endl;
		it.moveForward();
	}
}

template <typename T>
bool Multiset<T>::isSubSet(Multiset<T>& m) {
	RBTree<Pair<T>>::Iterator current(elements);
	while (current.getTarget()) {

		if (m.elements->search(*current)) {
			int count = m.elements->getElement(*current).getCount();
			if (current.getValue().getCount() <= count) {
				current.moveForward();
			}

			else {
				return false;
			}
		}

		else {
			return false;
		}
	}

	return true;
}


// free the memory in the main function needed
template <typename T>
Multiset<T>* Multiset<T>::merge(Multiset<T>& m) {
	Multiset<T>* merged = new Multiset<T>;
	*(merged->elements) = *elements;

	RBTree<Pair<T>>::Iterator current(m.elements);
	while (current.getTarget()) {
		merged->add(current.getValue().getKey(), current.getValue().getCount());
		current.moveForward();
	}

	return merged;
}

template <typename T>
bool operator<(Multiset<T>& left, Multiset<T>& right) {
	RBTree<Pair<T>>::Iterator current(left.elements);
	while (current.getTarget()) {

		if (right.elements->search(*current)) {
			int repeat = right.elements->getElement(*current).getCount();
			if (current.getValue().getCount() <= repeat) {
				current.moveForward();
			}

			else {
				return false;
			}
		}

		else {
			return false;
		}
	}

	return true;
}

template <typename T>
bool operator>(Multiset<T>& left, Multiset<T>& right) {
	return right < left;
}

template <typename T>
bool operator==(Multiset<T>& left, Multiset<T>& right) {
	RBTree<Pair<T>>::Iterator current(left.elements);
	if (left.count() == right.count()) {

		while (current.getTarget()) {
			if (right.elements->search(*current)) {
				int repeat = right.elements->getElement(*current).getCount();
				if (current.getValue().getCount() == repeat) {
					current.moveForward();
				}

				else {
					return false;
				}
			}

			else {
				return false;
			}
		}
		return true;
	}

	return false;
}

template <typename T>
void Multiset<T>::restore() {
	ifstream ifs("backup.dat", ios::binary);
	ifs.seekg(0, ios::beg);
	int size = ifs.tellg();

	if (size != 0) {
		ifs.seekg(0, ios::beg);

		int size = 0;
		ifs >> size;
		ifs.read((char*)&elements, size);
	}

	ifs.close();
}
