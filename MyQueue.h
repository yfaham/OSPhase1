#ifndef MYQUEUE_H
#define MYQUEUE_H
#include "MyNode.h"
#include <iostream>

using namespace std;

template <typename T>
class MyQueue {

public:
	MyQueue() {
		head = NULL;
		tail = NULL;
		length = 0;
	}

	friend ostream &operator<< (ostream &o, const MyQueue<T> &q) {
		o << (q.head)->getData();
		return o;
	};

	void enqueue(T &newData) {

		if (length == 0) {
			tail = new MyNode<T>(newData);
			head = tail;
		}
		else if (length > 0) {
			tail->setNext(new MyNode<T>(newData));
			tail = tail->getNext();
		}

		length++;
	}

	MyNode<T>* dequeue() {  //potential memory leak
	  if (length == 0) {
			return NULL;
		}
		else if (length == 1) {
			tail = NULL;
		}
		MyNode<T> *temp = head;
		head = head->getNext();
		temp->setNext(NULL);
		length--;

		return temp;
	}

	MyNode<T>* front() {
		return head;
	}

	int getLength() {
		return length;
	}

	bool isEmpty() {
		return length == 0;
	}

	void clear() {
		while (length > 0)
			dequeue();
	}

	~MyQueue() {
		clear();
	}

private:
  MyNode<T> *head, *tail;
  int length;

};

#endif
