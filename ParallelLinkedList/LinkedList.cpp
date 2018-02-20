#include "LinkedList.h"
#include "Node.h"
#include <iostream>
#include <limits>
#include <cstddef>

using namespace std;

ConcurrentLinkedList::ConcurrentLinkedList()
{
	this->head = Nodeptr(numeric_limits<int>::min());
	this->tail = Nodeptr(numeric_limits<int>::max());
	this->head.getNode()->next.store(tail);
}

pair<Nodeptr, Nodeptr> ConcurrentLinkedList::find(int v)
{
	Nodeptr current = this->head;
	Nodeptr next = current.getNext();
	while(next.getValue() < v)
	{
		current = next;
		next = current.getNext();
	}
	return make_pair(current, next);
}

pair<Nodeptr, Nodeptr> ConcurrentLinkedList::findAndFix(int v)
{
	Nodeptr current;
	Nodeptr next;
	do
	{
		current = this->head;
		next = current.getNext();
		//Look for value and fix along the way
		while(next.getValue() < v)
		{
			if(next.isMarked())
			{
				if(current.isMarked())
				{
					//Restart
					current = this->head;
					next = current.getNext();
				}
				else
				{
					//If current was not marked, the pointer is next Nodeptr does not have mark bit
					//If current gets marked, the atomicSetNext will fail
					current.atomicSetNext(next, next.getNextNode());
					
					//TODO: delete next before changing it
					next = current.getNext();
				}
			}
			else
			{
				current = next;
				next = current.getNext();
			}
		}
		//Restart if current node was marked
	}while(current.isMarked());
	return make_pair(current, next);
}

Nodeptr ConcurrentLinkedList::search(int key, bool &success)
{
	success = false;
	if(this->head.getValue() == key || this->tail.getValue() == key)
	{
		//Cannot look for head or tail
		return Nodeptr((Node*)NULL);
	}
	
	pair<Nodeptr, Nodeptr> findResult = find(key);
	if(findResult.second.getValue() == key && !findResult.second.isMarked())
	{
		success = true;
		return findResult.second;
	}
	return Nodeptr((Node*)NULL);
}

Nodeptr ConcurrentLinkedList::insert(int key, bool &success)
{
	success = false;
	if(this->head.getValue() == key || this->tail.getValue() == key)
	{
		//Cannot insert head or tail
		return Nodeptr((Node*)NULL);
	}
	//Constructor of Nodeptr allocates memory for Node
	Nodeptr newNodePtr = Nodeptr(key);
	pair<Nodeptr, Nodeptr> findResult;
	do
	{
		findResult = findAndFix(key);
		if(findResult.second.getValue() == key)
		{
			delete newNodePtr.pointer;
			return Nodeptr((Node*)NULL);
		}
		//Insert between first and second
		newNodePtr.setNext(findResult.second.getNode());
	}while(!findResult.first.atomicSetNext(findResult.second, newNodePtr));
	success = true;
	return newNodePtr;
}

Nodeptr ConcurrentLinkedList::remove(int key, bool &success)
{
	success = false;
	if(this->head.getValue() == key || this->tail.getValue() == key)
	{
		//Cannot remove head or tail
		return Nodeptr((Node*)NULL);
	}
	
	pair<Nodeptr, Nodeptr> findResult;
	findResult = findAndFix(key);
	if(findResult.second.getValue() != key)
	{
		return Nodeptr((Node*)NULL);
	}
	if(!findResult.second.atomicMarkNode())
	{
		//Given node is already marked
		return Nodeptr((Node*)NULL);
	}
	findResult.first.atomicSetNext(findResult.second, findResult.second.getNextNode());
	
	//TODO: delete the findResult.second
	success = true;
	return findResult.second;
}

void ConcurrentLinkedList::print()
{
	std::cout << "\nH" << " --> ";
	Nodeptr current = this->head.getNext();
	while(current.getValue() < tail.getValue())
	{
		std::cout << current.getValue() << " --> ";
		current = current.getNext();
	}
	std::cout << "T --> End\n" << std::endl;
}

int ConcurrentLinkedList::size()
{
	int length = 0;
	Nodeptr current = this->head.getNext();
	while(current.getValue() < tail.getValue())
	{
		current = current.getNext();
		length++;
	}
	return length;
}

//Removes all  marked nodes
void ConcurrentLinkedList::CleanUp()
{
	findAndFix(tail.getValue());
}

//deletes all nodes
//synchronous routine
void ConcurrentLinkedList::removeAll()
{
	Nodeptr current = this->head.getNext();
	Nodeptr next = current.getNext();
	while(current.getValue() < tail.getValue())
	{
		delete current.pointer;
		current = next;
		next = current.getNext();
	}
	this->head.setNext(current.pointer);
}