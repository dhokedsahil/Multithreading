#pragma once
#include "Node.h"
class SeekRecord
{
public:
	Node* ancestor;
	Node* successor;
	Node* parent;
	Node* leaf;

	SeekRecord();
	~SeekRecord();
};

