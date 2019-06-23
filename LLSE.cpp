/* 10/29/17
 * Linked List
*/

#include "LLSE.hpp"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <cstring>
using namespace std;

// Constructor
LLSE::LLSE(){
	first = NULL;
	last = NULL;
	size = 0;
	wordcount = 0;
}

// Destructor
// Referene: https://stackoverflow.com/questions/2265967/writing-a-linkedlist-destructor
LLSE::~LLSE(){
	Node* tmp = first;
	while(tmp != NULL) {
	    Node* next = tmp->next;
	    delete tmp;
	    tmp = next;
	}
	size = 0;
	wordcount = 0;
}

void LLSE::printLL(){
	int changeLineWordCount = 0;
	// In the output, the line is changed when one line is filled with 8 words.
	// This is a flag variable to help with this process
	Node* tmp = first;
	while(tmp != NULL) {
		changeLineWordCount++;
		Node* next = tmp->next;
		cout <<tmp->word<<":"<<tmp->count;
		if (changeLineWordCount < 8){
			cout <<", ";
		} else {
			cout <<endl;
			changeLineWordCount = 0;
		}
		tmp = next;
	}
	cout <<endl;
	tmp = NULL;
	delete tmp;
}

void LLSE::addFirst(string x){
	first = new Node(x); //By default, prev and next are both NULL, and it fits in the circumstance of the first node
	last = first;
	size++;
	wordcount++;
}

void LLSE::addAtFirst(string x){
	Node *newFirst = new Node(x);
	first->prev = newFirst;
	newFirst->next = first;
	first = newFirst;
	//first->prev = NULL;
	size++;
	wordcount++;
}

void LLSE::push(string x){
	Node *newTail = new Node(x);
	last->next = newTail;
	newTail->prev = last;
	last = newTail;
	//newTail->next = NULL;
	size++;
	wordcount++;
}

void LLSE::insertUnique(string x){
	if (size == 0){
		addFirst(x);
		return;
	}
	if (strcmp(x.c_str(),first->word.c_str()) < 0) { //http://www.cplusplus.com/reference/cstring/strcmp/ https://stackoverflow.com/questions/35376303/no-matching-function-for-call-to-strcmp
		addAtFirst(x);
		return;
	} else if (strcmp(x.c_str(),first->word.c_str()) == 0) {
		first->count++;
		wordcount++;
		return;
	} else {
		Node *tmp = findInsert(x);
		while (tmp != NULL){
			if(last == tmp){ //calling findInsert(x) here. If it's not the last the word was also inserted. See the code of findInsert().
				push(x);
				return;
			} else {
				//If the returned node is not the last, the returned node address tmp
				//is the node right before where we should insert the new word into the list.
				//Therefore, we have to insert the node at tmp->next
				Node *ogriginalTmpNext = tmp->next;
				Node *insertNode = new Node(x);
				//Change the next of tmp points to the insertNode
				tmp->next = insertNode;
				//Change ogriginalTmpNext->prev points to the insertNode
				ogriginalTmpNext->prev = insertNode;  //DO NOT WRITE tmpPrev = insertNode! tmpPrev !== tmp->prev Althoght Node *tmpPrev = tmp->prev, but these two are two different pointers, have the same address
				//Add(change) the prev and next of the insertNode to the right order
				insertNode->prev = tmp;
				insertNode->next = ogriginalTmpNext;
				size++;
				wordcount++;
				return;
			}
		}
	}
}
// COMPARE STRING FIRST LETTER ALBE
// COMPARE TWO STRINGS SEE IF EXACTLY
Node * LLSE::findInsert(string x){
	Node *tmp = first;
	while (tmp != NULL){
		if (strcmp(x.c_str(), tmp->word.c_str()) <= 0){
			if (strcmp(x.c_str(), tmp->word.c_str()) == 0){ // If two words are identical
				tmp->count++;
				wordcount++;
				return NULL;
			} else {
				return tmp->prev;
			}
		} else {
			if (tmp == last){ //If the iteration reaches at the end, the node cannot go on to NULL but push itself to be the new last, thus return it
				return tmp;
			} else {
				tmp = tmp->next; //If not reaches the end, continue
			}
		}
	}
}

void LLSE::normalizeCounts(){
	Node* tmp = first;
	while(tmp != NULL) {
		tmp->count = tmp->count/wordcount;
		tmp = tmp->next;
	}
	tmp = NULL;
	delete tmp;
}

string LLSE::remFirst(){
	Node* tmp = first->next;
	tmp->prev = NULL;
	wordcount -= first->count;
	string nodeWord = first->word;
	delete first;
	first = tmp;
	size--;
	return nodeWord;
}

string LLSE::pop(){
	Node* tmp = last->prev;
	tmp->next = NULL;
	wordcount -= last->count;
	string nodeWord = last->word;
	delete last;
	last = tmp;
	size--;
	return nodeWord;
}

string LLSE::remNext(Node *n){
	Node *tmp = first;
	while(tmp != n){
		tmp = tmp->next;
	}
	tmp = tmp->next;
	string nodeWord = tmp->word;
	wordcount -= tmp->count;
	Node *tmpNext = tmp->next;
	Node *tmpPrev = tmp->prev;
	tmpPrev->next = tmpNext;
	tmpNext->prev = tmpPrev;
	delete tmp;
	size--;
	return nodeWord;
}

void LLSE::eliminateLowWords(){
	Node *tmp = first;
	string removedWord;
	while(tmp != NULL){
		Node *nextStartingNode = tmp->next; //Since tmp might get eliminated, we have to store its next node in case it's needed
		if (tmp->count < 0.004){
			if (tmp == first){
				removedWord = remFirst();
				cout <<"removing "<<removedWord<<endl;
			} else if (tmp == last){
				removedWord = pop();
				cout <<"removing "<<removedWord<<endl;
			} else {
				removedWord = remNext(tmp->prev);
				cout <<"removing "<<removedWord<<endl;
			}
		}
		tmp = nextStartingNode;
	}
}
