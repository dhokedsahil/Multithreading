#pragma once
#include <atomic>

using namespace std;

class Node
{
public:
	atomic<Node*> left;
	atomic<Node*> right;
	int key;
	Node(int key);
	Node();
	~Node();
};

