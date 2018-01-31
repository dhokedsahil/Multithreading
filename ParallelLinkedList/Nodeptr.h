#ifndef SAHIL_CONCURRENT_LL_NODEPTR
#define SAHIL_CONCURRENT_LL_NODEPTR

#include <utility>
#include <atomic>

class Node;

class Nodeptr
{
	static const int MARK_BIT = 1;
public:
	Node* pointer;
	//Nodeptr& operator=(Nodeptr&&);
	Nodeptr() noexcept;
	Nodeptr(int);
	Nodeptr(Node*);
	Node* getNode();
	int getValue();
	Nodeptr getNext();
	Nodeptr getNextNode();
	void setNext(Node*);
	bool atomicSetNext(Nodeptr currentlyExpected, Nodeptr);
	bool atomicMarkNode();
	bool isMarked();
};

#endif