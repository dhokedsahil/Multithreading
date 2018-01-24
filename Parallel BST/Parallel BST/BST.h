#pragma once
#include "Node.h"
#include "SeekRecord.h"
class BST
{
public:
	Node* root;
	BST();
	~BST();
	SeekRecord seek(int);
	bool search(int);
};

