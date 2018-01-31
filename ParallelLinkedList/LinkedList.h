#ifndef SAHIL_CONCURRENT_LL
#define SAHIL_CONCURRENT_LL

#include <utility>
#include "Nodeptr.h"

class ConcurrentLinkedList
{
public:
	Nodeptr head, tail;
	ConcurrentLinkedList();
	std::pair<Nodeptr, Nodeptr> find(int v);
	std::pair<Nodeptr, Nodeptr> findAndFix(int v);
	Nodeptr search(int key);
	Nodeptr insert(int key);
	Nodeptr remove(int key);
	void print();
};

#endif