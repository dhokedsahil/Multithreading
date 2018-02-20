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
	Nodeptr search(int key, bool &success);
	Nodeptr insert(int key, bool &success);
	Nodeptr remove(int key, bool &success);
	void print();
	int size();
	void CleanUp();
	void removeAll();
};

#endif