#ifndef SAHIL_CONCURRENT_LL_NODE
#define SAHIL_CONCURRENT_LL_NODE

#include <atomic>
#include "Nodeptr.h"

class Node
{
public:
	int value;
	std::atomic<Nodeptr> next;
	Node();
	Node(int);
};

#endif