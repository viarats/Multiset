#pragma once

#include "Color.h"
#include "Queue.h"
#include <iostream>

using namespace std;

template <typename T>
struct RBNode {
	T data;
	RBNode* parent;
	RBNode* left;
	RBNode* right;
	Color color;

	RBNode(const T& data) {
		this->data = data;
		parent = NULL;
		left = NULL;
		right = NULL;
		color = RED;
	}
};

template <typename T>
class RBTree {
public:
	RBTree();
	~RBTree();
	RBTree(const RBTree& other);
	RBTree<T>& operator=(const RBTree& other);

	class Iterator {
		
	private:

		friend class RBTree<T>;
		const RBTree<T>* instance;
		RBNode<T>* target;  

	public:

		Iterator(RBTree<T>* instance) {
			this->instance = instance;
			this->target = instance->root;
			if (target) {
				while (target->left) {
					target = target->left;	
				}
			}
		}

		Iterator(Iterator& other) {
			target = other.target;
		}

		void moveForward() {
			if (target->right) {
				target = target->right;
				while (target->left) {
					target = target->left;
				}
			}
			else {
				while (target->parent && target != target->parent->left) {
					target = target->parent;
				}
				target = target->parent;
			}
		}

		const T& operator*() {
			return target->data;
		}

		T& getValue() {
			return target->data;
		}

		RBNode<T>* getTarget() {
			return target;
		}
	};

	void insert(const T& data);
	void remove(const T& data);

	bool search(const T& data);

	T& getElement(const T& data);
	size_t getSize();

	void print();

private:

	RBNode<T>* root;
	size_t size;

	Color getColor(RBNode<T>*& n);
	void setColor(RBNode<T>*& n, Color color);

	RBNode<T>* insert(RBNode<T>*& root, RBNode<T>*& n);

	void remove(RBNode<T>*& n);
	RBNode<T>* removeHelper(RBNode<T>*& root, const T& data);

	void fixAfterInsertion(RBNode<T>*& n);
	void fixColorsAfterInsertion(RBNode<T>*& uncle, RBNode<T>*& parent, RBNode<T>*& grandparent);

	void rotateRight(RBNode<T>*& n);
	void rotateLeft(RBNode<T>*& n);

	void swapColors(Color& c1, Color& c2);
	RBNode<T>* findMinValue(RBNode<T>*& n);

	bool search(const T& data, RBNode<T>*& root);
	T& getElement(const T& data, RBNode<T>*& root);

	void print(RBNode<T>*& root, int level = 0);
	
	void copy(RBNode<T>* other);
	void destroy(RBNode<T>*& root);
};

template <typename T>
RBTree<T>::RBTree() {
	root = NULL;
	size = 0;
}

template <typename T>
RBTree<T>::~RBTree() {
	destroy(root);
}

template <typename T>
void RBTree<T>::destroy(RBNode<T>*& root) {
	if (root) {
		destroy(root->left);
		destroy(root->right);

		delete root;
		root = NULL;
	}
}

template <typename T>
RBTree<T>::RBTree(const RBTree<T>& other) {
	copy(other.root);
}

template <typename T>
RBTree<T>& RBTree<T>::operator=(const RBTree<T>& other) {
	if (this != &other) {
		destroy(root);
		copy(other.root);
	}
	return *this;
}

template <typename T>
void RBTree<T>::copy(RBNode<T>* root) {
	if (!root) {
		return;
	}

	Queue<RBNode<T>*> q;
	q.enqueue(root);


	while (!q.isEmpty()) {
		insert(q.front()->data);
		
		if (q.front()->left) {
			q.enqueue(q.front()->left);
		}
		if (q.front()->right) {
			q.enqueue(q.front()->right);
		}

		q.dequeue();
	}
}

template <typename T>
size_t RBTree<T>::getSize() {
	return size;
}

template <typename T>
void RBTree<T>::rotateLeft(RBNode<T>*& n) {
	RBNode<T>* rightChild = n->right;
	n->right = rightChild->left;

	if (n->right) {
		n->right->parent = n;
	}

	rightChild->parent = n->parent;

	if (!rightChild->parent) {
		root = rightChild;
	}

	else if (n->parent->left == n) {
		n->parent->left = rightChild;
	}

	else {
		n->parent->right = rightChild;
	}

	rightChild->left = n;
	n->parent = rightChild;
}

template <typename T>
void RBTree<T>::rotateRight(RBNode<T>*& n) {
	RBNode<T>* leftChild = n->left;
	n->left = leftChild->right;

	if (n->left) {
		n->left->parent = n;
	}

	leftChild->parent = n->parent;

	if (!leftChild->parent) {
		root = leftChild;
	}

	else if (n->parent->right == n) {
		n->parent->right = leftChild;
	}

	else {
		n->parent->left = leftChild;
	}

	leftChild->right = n;
	n->parent = leftChild;
}

template <typename T>
void RBTree<T>::setColor(RBNode<T>*& n, Color color) {
	if (n) {
		n->color = color;
	}
}

template <typename T>
Color RBTree<T>::getColor(RBNode<T>*& n) {
	if (!n) {
		return BLACK;
	}
	return n->color;
}

template <typename T>
void RBTree<T>::swapColors(Color& c1, Color& c2) {
	Color temp = c1;
	c1 = c2;
	c2 = temp;
}

template <typename T>
void RBTree<T>::insert(const T& data) {
	RBNode<T>* n = new RBNode<T>(data);
	root = insert(root, n);
	fixAfterInsertion(n);
	size++;
}

template <typename T>
RBNode<T>* RBTree<T>::insert(RBNode<T>*& root, RBNode<T>*& n) {
	if (!root) {
		return n;
	}
	if (n->data < root->data) {
		root->left = insert(root->left, n);
		root->left->parent = root;
	}
	else  if (n->data > root->data){
		root->right = insert(root->right, n);
		root->right->parent = root;
	}
	return root;
}

template <typename T>
void RBTree<T>::fixAfterInsertion(RBNode<T>*& n) {
	RBNode<T>* parent = NULL;
	RBNode<T>* grandparent = NULL;

	while (n != root && getColor(n) == RED && getColor(n->parent) == RED) {
		parent = n->parent;
		grandparent = parent->parent;
		if (parent == grandparent->left) {
			RBNode<T>* uncle = grandparent->right;

			if (getColor(uncle) == RED) {
				fixColorsAfterInsertion(uncle, parent, grandparent);
				n = grandparent;
			}

			else {
				if (n == parent->right) {
					rotateLeft(parent);
					n = parent;
					parent = n->parent;
				}
				rotateRight(grandparent);
				swapColors(parent->color, grandparent->color);
				n = parent;
			}
		}
		else {
			RBNode<T>* uncle = grandparent->left;
			if (getColor(uncle) == RED) {
				fixColorsAfterInsertion(uncle, parent, grandparent);
				n = grandparent;
			}

			else {
				if (n == parent->left) {
					rotateRight(parent);
					n = parent;
					parent = n->parent;
				}
				rotateLeft(grandparent);
				swapColors(parent->color, grandparent->color);
				n = parent;
			}
		}
	}
	setColor(root, BLACK);
}

template <typename T>
void RBTree<T>::fixColorsAfterInsertion(RBNode<T>*& uncle, RBNode<T>*& parent, RBNode<T>*& grandparent) {
	setColor(uncle, BLACK);
	setColor(parent, BLACK);
	setColor(grandparent, RED);
}

template <typename T>
void RBTree<T>::remove(const T& data) {
	RBNode<T>* toRemove = removeHelper(root, data);
	remove(toRemove);
	size--;
}

template <typename T>
RBNode<T>* RBTree<T>::removeHelper(RBNode<T>*& root, const T& data) {
	if (root == NULL) {
		return root;
	}

	if (data < root->data) {
		return removeHelper(root->left, data);
	}

	if (data > root->data) {
		return removeHelper(root->right, data);
	}

	if (root->left == NULL || root->right == NULL) {
		return root;
	}

	RBNode<T> *temp = findMinValue(root->right);
	root->data = temp->data;
	return removeHelper(root->right, temp->data);
}

template <typename T>
void RBTree<T>::remove(RBNode<T>*& n) {
	if (!n) {
		return;
	}

	if (n == root) {
		if (n->left)
			root = n->left;
		else if (n->right)
			root = n->right;
		else
			root = NULL;
		return;
	}

	if (getColor(n) == RED || getColor(n->left) == RED || getColor(n->right) == RED) {
		RBNode<T>* child = n->left != NULL ? n->left : n->right;

		if (n == n->parent->left) {
			n->parent->left = child;
			if (child) {
				child->parent = n->parent;
			}
			setColor(child, BLACK);
			delete n;
		}
		else {
			n->parent->right = child;
			if (child)
				child->parent = n->parent;
			setColor(child, BLACK);
			delete n;
		}
	}
	else {
		RBNode<T>* sibling = NULL;
		RBNode<T>* parent = NULL;
		RBNode<T>* p = n;
		setColor(p, DOUBLE_BLACK);

		while (p != root && getColor(p) == DOUBLE_BLACK) {
			parent = p->parent;
			if (p == parent->left) {
				sibling = parent->right;
				if (getColor(sibling) == RED) {                                // case 2
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateLeft(parent);
				}
				else {
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {               // case 3 and 4
						setColor(sibling, RED);
						if (getColor(parent) == RED) {             // case 4
							setColor(parent, BLACK);
						}
						else {
							setColor(parent, DOUBLE_BLACK);       // case 3
						}
						p = parent;                               // end of case 4
					}
					else {
						if (getColor(sibling->right) == BLACK) {           // case 5 -> case 6
							setColor(sibling->left, BLACK);
							setColor(sibling, RED);
							rotateRight(sibling);
							sibling = parent->right;
						}
						setColor(sibling, parent->color);                // case 6
						setColor(parent, BLACK);
						setColor(sibling->right, BLACK);
						rotateLeft(parent);
						break;
					}
				}
			}
			else {
				sibling = parent->left;
				if (getColor(sibling) == RED) {                           // case 2
					setColor(sibling, BLACK);
					setColor(parent, RED);
					rotateRight(parent);
				}
				else {
					if (getColor(sibling->left) == BLACK && getColor(sibling->right) == BLACK) {        // cases 3 and 4
						setColor(sibling, RED);
						if (getColor(parent) == RED) {                   // case 4
							setColor(parent, BLACK);
						}
						else {
							setColor(parent, DOUBLE_BLACK);             // case 3
						}
						p = parent;                                      // end of case 4
					}
					else {
						if (getColor(sibling->left) == BLACK) {          // case 5 -> case 6
							setColor(sibling->right, BLACK);
							setColor(sibling, RED);
							rotateLeft(sibling);
							sibling = parent->left;
						}
						setColor(sibling, parent->color);                // case 6
						setColor(parent, BLACK);
						setColor(sibling->left, BLACK);
						rotateRight(parent);
						break;
					}
				}
			}
		}
		if (n == n->parent->left) {
			n->parent->left = NULL;
		}
		else {
			n->parent->right = NULL;
		}
		delete n;

		setColor(root, BLACK);                                 // case 1
	}
}

template <typename T>
RBNode<T>* RBTree<T>::findMinValue(RBNode<T>*& n) {
	RBNode<T>* min = n;
	while (min->left) {
		min = min->left;
	}
	return min;
}

template <typename T>
void RBTree<T>::print() {
	print(root);
}

template <typename T>
void RBTree<T>::print(RBNode<T>*& root, int level) {
	if (root) {
		for (int i = 0; i < level; i++) {
			cout << '\t';
		}
		cout << root->data << endl;

		print(root->left, level + 1);
		print(root->right, level +1);
	}
}

template <typename T>
bool RBTree<T>::search(const T& data) {
	return search(data, root);
}

template <typename T>
bool RBTree<T>::search(const T& data, RBNode<T>*& root) {
	if (root) {
		if (data < root->data) {
			return search(data, root->left);
		}
		else if (data == root->data) {
			return true;
		}
		else {
			return search(data, root->right);
		}
	}
	return false;
}

template <typename T>
T& RBTree<T>::getElement(const T& data) {
	return getElement(data, root);
}

template <typename T>
T& RBTree<T>::getElement(const T& data, RBNode<T>*& root) {
	if (data < root->data) {
		return getElement(data, root->left);
	}
	else if (data == root->data) {
		return root->data;
	}
	else {
		return getElement(data, root->right);
	}
}