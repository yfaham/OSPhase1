#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <iostream>

using namespace std;

template <typename T>
class MyVector {

public:
        MyVector() {
		head = NULL;
		tail = NULL;
		length = 0;
	}

	bool insert(T &newData, int position) {
	  if (position < 0 || position > length)
	    return false;

	  if (length < 0) {
	    cout << "\nError in MyVector::insert(), length < 0.\n";
	    return false;
	  }
	  else if (length == 0) {
	    tail = new node;
	    head = tail;
	    head->next = NULL;
	    head->data = newData;
	  }
	  else {
	    node *newNode = new node;
	    newNode->data = newData;

	    if (position == 0) {
	      newNode->next = head;
	      head = newNode;
	    }
	    else if (position == length) {
	      tail->next = newNode;
	      tail = newNode;
	    }
	    else {
	      node *prev = head;
	      int i = 0;
	      while (prev->next && i < position) {
		prev = prev->next;
		i++;
	      }
	      newNode->next = prev->next;
	      prev->next = newNode;
	    }
	  }
	  
	  length++;
	  return true;
	}

	void remove_index(int start, int last) {
	  if (last < start || length < 1)
	    return;

	  node *temp = head;
	  int i = 0;

	  if (start == 0) {
	    while (i <= last && temp) {
	      head = head->next;
	      delete temp;
	      length--;
	      i++;
	      temp = head;
	    }

	    if (head == NULL)
	      tail = NULL;

	    return;
	  }

	  node *prev = NULL;

	  while (i < start && temp) {
	    prev = temp;
	    temp = temp->next;
	    i++;
	  }

	  while (i <= last && temp) {
	    prev->next = temp->next;
	    delete temp;
	    length--;
	    i++;
	    temp = prev->next;
	  }

	  if (temp == NULL)
	    tail = prev;
	}

	void remove_size(int start, int size) {
	  if (size < 1 || length < 1)
	    return;

	  node *temp = head;
	  int i = 0;

	  if (start == 0) {
	    while (i < size && temp) {
	      head = head->next;
	      delete temp;
	      length--;
	      i++;
	      temp = head;
	    }

	    if (head == NULL)
	      tail = NULL;

	    return;
	  }

	  node *prev = NULL;

	  while (i < start && temp) {
	    prev = temp;
	    temp = temp->next;
	    i++;
	  }

	  i = 0;

	  while (i < size && temp) {
	    prev->next = temp->next;
	    delete temp;
	    length--;
	    i++;
	    temp = prev->next;
	  }

	  if (temp == NULL)
	    tail = prev;
	}

	void remove(int position) {
	  if (position < 0 || length < 1)
	    return;

	  node *temp = head;

	  if (position == 0) {
	    head = head->next;
	    delete temp;
	    length--;

	    if (head == NULL)
	      tail = NULL;

	    return;
	  }

	  int i = 0;
	  node *prev = NULL;

	  while (i < position && temp) {
	    prev = temp;
	    temp = temp->next;
	    i++;
	  }

	  prev->next = temp->next;
	  delete temp;
	  length--;

	  if (temp == tail)
	    tail = prev;
	}

	T* at(int position) {
	  node* temp = head;
	  int i = 0;
	  while (i < position && temp) {
	    temp = temp->next;
	    i++;
	  }
	  return temp;
	}

	int getLength() {
	  return length;
	}

	bool isEmpty() {
	  return length == 0;
	}

	void clear() {
	  remove_size(0, length);
	}

	~MyVector() {
	  clear();
	}

private:
	struct node {
	  T data;
	  node *next;
	};
	node *head, *tail;
	
  int length = 0;

};

#endif
