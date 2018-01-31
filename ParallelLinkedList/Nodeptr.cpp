#include "Nodeptr.h"
#include "Node.h"
#include <cstddef>

using namespace std;

Nodeptr::Nodeptr() noexcept
{
	this->pointer = NULL;
}

Nodeptr::Nodeptr(int v)
{
	this->pointer = new Node(v);
}

Nodeptr::Nodeptr(Node* node)
{
	this->pointer = node;
}

Node* Nodeptr::getNode()
{
	return (Node*)(((uintptr_t)(this->pointer)) & (~MARK_BIT));
}

int Nodeptr::getValue()
{
	return this->getNode()->value;
}

Nodeptr Nodeptr::getNext()
{
	return this->getNode()->next.load();
}

Nodeptr Nodeptr::getNextNode()
{
	return Nodeptr(this->getNext().getNode());
}

void Nodeptr::setNext(Node* nextNode)
{
	this->getNode()->next.store(Nodeptr(nextNode));
}

bool Nodeptr::atomicSetNext(Nodeptr expected, Nodeptr desired)
{
	return this->getNode()->next.compare_exchange_weak(expected, desired);
}

bool Nodeptr::atomicMarkNode()
{
	//next is a copy of the next pointer and not the original next pointer
	Nodeptr next = getNext();
	Nodeptr markedNext;
	do
	{
		if(this->isMarked())
		{
			return false;
		}
		markedNext = Nodeptr((Node*)(((uintptr_t)(next.pointer)) | MARK_BIT));
	//comare_exchange_weak automatically sets the value of next
	}while(!getNode()->next.compare_exchange_weak(next, markedNext));
	return true;
}

//Checks if the node this pointer points to is marked
bool Nodeptr::isMarked()
{
	return (bool)(((uintptr_t)(getNext().pointer)) & MARK_BIT);
}