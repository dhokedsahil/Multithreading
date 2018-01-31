#include "Node.h"
#include "Nodeptr.h"
#include <cstddef>

using namespace std;

Node::Node() : Node(0) {}

Node::Node(int v)
{
	this->value = v;
	this->next.store(Nodeptr((Node*)NULL));
}