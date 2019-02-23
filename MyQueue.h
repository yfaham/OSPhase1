#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <iostream>

using namespace std;

template <typename T>
class MyQueue {

public:
	struct node {
		T data;
		node *next;
	};

	node *head, *tail;

	MyQueue() {
		head = NULL;
		tail = NULL;
		length = 0;
	}

	void enqueue(T newData) {

		if (length == 0) {
			tail = new node;
			tail->next = NULL;
			tail->data = newData;
			head = tail;
		}
		else if (length > 0) {
			tail->next = new node;
			tail = tail->next;
			tail->data = newData;
		}

		length++;
	}

	T* dequeue() {  //potential memory leak
	  if (length == 0) {
			return NULL;
		}
		else if (length == 1) {
			tail = NULL;
		}
		node *temp = head;
		head = head->next;
		temp->next = NULL;
		length--;

		return &(temp->data);
	}

	T* front() {
		return &(head->data);
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
  int length;
};

#endif
