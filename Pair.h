#pragma once
#include <iostream>

using namespace std;

template <typename T>
class Pair {
public:
	Pair();
	Pair(const T& data);
	Pair(const Pair& other);
	Pair<T>& operator=(const Pair& other);

	template <typename U>
	friend bool operator<(const Pair<U>& p1, const Pair<U>& p2);
	template <typename U>
	friend bool operator<=(const Pair<U>& p1, const Pair<U>& p2);
	template <typename U>
	friend bool operator>(const Pair<U>& p1, const Pair<U>& p2);
	template <typename U>
	friend bool operator>=(const Pair<U>& p1, const Pair<U>& p2);
	template <typename U>
	friend bool operator==(const Pair<U>& p1, const Pair<U>& p2);
	template <typename U>
	friend bool operator!=(const Pair<U>& p1, const Pair<U>& p2);

	size_t getCount() const;
	T& getKey();

	void increaseCount();
	void decreaseCount();

	template <typename U>
	friend ostream& operator<<(ostream& out, const Pair<U>& p);

private:
	T key;
	size_t count;
};

template <typename T>
Pair<T>::Pair() {
	count = 0;
}

template <typename T>
Pair<T>::Pair(const Pair& other) {
	key = other.key;
	count = other.count;
}

template <typename T>
Pair<T>& Pair<T>::operator=(const Pair& other) {
	if (this != &other) {
		key = other.key;
		count = other.count;
	}
	return *this;
}

template <typename T>
Pair<T>::Pair(const T& data) {
	key = data;
	count = 1;
}

template <typename T>
size_t Pair<T>::getCount() const {
	return count;
}

template <typename T>
bool operator==(const Pair<T>& p1, const Pair<T>& p2) {
	return p1.key == p2.key;
}

template <typename T>
bool operator!=(const Pair<T>& p1, const Pair<T>& p2) {
	return !p1 == p2;
}

template <typename T>
bool operator<(const Pair<T>& p1, const Pair<T>& p2) {
	return p1.key < p2.key;
}

template <typename T>
bool operator>(const Pair<T>& p1, const Pair<T>& p2) {
	return p1.key > p2.key;
}

template <typename T>
bool operator<=(const Pair<T>& p1, const Pair<T>& p2) {
	return !p1 > p2;
}

template <typename T>
bool operator>=(const Pair<T>& p1, const Pair<T>& p2) {
	return !p1 < p2;
}

template <typename T>
ostream& operator<<(ostream& out, const Pair<T>& p) {
	out << "key: " << p.key << " count: " << p.count;
	return out;
}

template <typename T>
void Pair<T>::increaseCount() {
	count++;
}

template <typename T>
void Pair<T>::decreaseCount() {
	count--;
}

template <typename T>
T& Pair<T>::getKey() {
	return key;
}