#pragma once

template <typename T>
class Queue {
public:
	Queue();
	~Queue();
	Queue(Queue& other);
	Queue& operator=(const Queue& other);

	const T& front() const;
	void enqueue(const T& element);
	void dequeue();

	bool isEmpty() const;

private:
	
	struct Node {
		T data;
		Node* next;

		Node(const T& data, Node* n = NULL) {
			this->data = data;
			this->next = n;
		}
	};

	Node* first;
	Node* last;

	void copy(Queue& other);
	void destroy();
};

template <typename T>
Queue<T>::Queue() {
	first = NULL;
	last = NULL;
}

template <typename T>
Queue<T>::~Queue() {
	destroy();
}

template <typename T>
Queue<T>::Queue(Queue& other) {
	copy(other);
}

template <typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
	if (this != &other) {
		destroy();
		copy(other);
	}
	return *this;
}

template <typename T>
void Queue<T>::copy(Queue& other) {
	if (other.isEmpty()) {
		return;
	}
	while (!other.isEmpty()) {
		enqueue(other.front());
		other.dequeue();
	}
}

template <typename T>
void Queue<T>::destroy() {
	while (first) {
		Node* toDelete = first;
		first = first->next;
		delete toDelete;
	}
}

template <typename T>
const T& Queue<T>::front() const {
	return first->data;
}

template <typename T>
void Queue<T>::enqueue(const T& element) {
	Node* n = new Node(element);
	if (isEmpty()) {
		first = last = n;
	}
	else {
		last->next = n;
		last = n;
	}
}

template <typename T>
void Queue<T>::dequeue() {
	if (isEmpty()) {
		return;
	}

	Node* toDelete = first;
	first = first->next;
	delete toDelete;
}

template <typename T>
bool Queue<T>::isEmpty() const {
	return !first;
}