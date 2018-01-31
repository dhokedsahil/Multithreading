#include "Node.h"

Node::Node(int key)
{
	this->key = key;
	left = NULL;
	right = NULL;
}

Node::Node():Node(0)
{
}


Node::~Node()
{
}
