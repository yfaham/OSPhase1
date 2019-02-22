#ifndef MYNODE_H
#define MYNODE_H

#include <cstddef>

using namespace std;

template <typename T>
class MyNode {

public:
	MyNode() {
		next = NULL;
	}

	MyNode(T &newData) {
		data = newData;
		next = NULL;
	}

	T getData() {
		return data;
	}

	void setData(T &newData) {
		data = newData;
	}

	MyNode<T>* getNext() {
		return next;
	}

	void setNext(MyNode<T>* newNext) {
		next = newNext;
	}

	~MyNode() {

	}

private:
  T data;
  MyNode *next;
};

#endif
