#pragma once
#include <iostream>

template <typename T>
class List;

// Узел списка
template <typename T>
class ListNode
{
	friend class List<T>;

public:
	ListNode(T);

private:
	T data;
	ListNode* nextPtr;
};

template <typename T>
ListNode<T>::ListNode(T dataIn)
{
	data = dataIn;
	nextPtr = 0;
}


// Односвязный список
template <typename T>
class List {
public:
	List();
	~List();
	void insertNewNode(T); 
	void removeNode(T);
	void print(); 
	int search(T);

private:
	ListNode<T>* startPtr; 
	ListNode<T>* endPtr; 
	bool isEmpty(); 
	void insertBegin(T); 
	void insertEnd(T);
};

template <typename T>
List<T>::List(){
	startPtr = NULL;
	endPtr = NULL;
}

template <typename T>
List<T>::~List(){
	if (!isEmpty()) {
		ListNode<T>* currentPtr = startPtr;
		ListNode<T>* tempPtr;

		while (currentPtr != 0) {
			tempPtr = currentPtr;
			currentPtr = currentPtr->nextPtr;
			delete tempPtr;
		}
	}
}

template <typename T>
bool List<T>::isEmpty(){
	if (startPtr == NULL && endPtr == NULL) 
		return 1;
	else
		return 0;
}

template <typename T>
void List<T>::insertBegin(T dataIn){
	if (isEmpty()) {
		ListNode<T>* newPtr = new ListNode<T>(dataIn); 
		startPtr = newPtr;
		endPtr = newPtr;
	}
	else {
		ListNode<T>* newPtr = new ListNode<T>(dataIn);
		newPtr->nextPtr = startPtr; 
		startPtr = newPtr; 
	}
}

template <typename T>
void List<T>::insertEnd(T dataIn){
	if (isEmpty()){
		ListNode<T>* newPtr = new ListNode<T>(dataIn);
		startPtr = newPtr;
		endPtr = newPtr;
	}
	else {
		ListNode<T>* newPtr = new ListNode<T>(dataIn);
		endPtr->nextPtr = newPtr;
		endPtr = newPtr; 
	}
}

template <typename T>
void List<T>::insertNewNode(T dataIn) {
	if (isEmpty()) {
		insertBegin(dataIn);
	}
	else {
		if (dataIn < startPtr->data) {
			insertBegin(dataIn);
		}
		else if (dataIn >= endPtr->data) {
			insertEnd(dataIn);
		}
		else {
			ListNode<T>* currentPtr = startPtr;
			ListNode<T>* newPtr = new ListNode<T>(dataIn); 
			while (currentPtr != endPtr) {
				if ((newPtr->data < currentPtr->nextPtr->data) && (newPtr->data >= currentPtr->data)) {
					ListNode<T>* next = currentPtr->nextPtr;
					currentPtr->nextPtr = newPtr; 
					newPtr->nextPtr = next; 
					break;
				}
				currentPtr = currentPtr->nextPtr; 
			}
		}
	}
}

template <typename T>
void List<T>::removeNode(T data) {
	if (isEmpty()) {
		std::cout << "The list is empty. Cannot remove element." << std::endl;
		return;
	}

	if (startPtr->data == data) {
		ListNode<T>* tempPtr = startPtr;
		startPtr = startPtr->nextPtr;
		if (startPtr == NULL) {
			endPtr = NULL;
		}
		delete tempPtr;
		return;
	}

	if (endPtr->data == data) {
		ListNode<T>* tempPtr = endPtr;
		endPtr = endPtr->nextPtr;
		delete tempPtr;
		return;
	}

	ListNode<T>* previousPtr = NULL;
	ListNode<T>* currentPtr = startPtr;

	while (currentPtr != NULL && currentPtr->data != data) {
		previousPtr = currentPtr;
		currentPtr = currentPtr->nextPtr;
	}

	if (currentPtr == NULL) {
		std::cout << "Element not found in the list." << std::endl;
		return;
	}

	ListNode<T>* tempPtr = currentPtr;
	previousPtr->nextPtr = currentPtr->nextPtr;
	delete tempPtr;
}

template <typename T>
void List<T>::print(){
	if (isEmpty()){
		std::cout << "The list is empty" << std::endl;

	}
	else{
		ListNode<T>* currentPtr = startPtr;

		std::cout << "The contents of the list is: ";
		while (currentPtr != NULL) {
			std::cout << currentPtr->data << ' ';
			currentPtr = currentPtr->nextPtr; 
		}
		std::cout << std::endl;
	}
}

template <typename T>
int List<T>::search(T key) {
	int index = 0;
	ListNode<T>* currentPtr = startPtr;

	while (currentPtr != NULL) {
		if (currentPtr->data == key) {
			return index;
		}
		currentPtr = currentPtr->nextPtr;
		index++;
	}
	return -1;
}