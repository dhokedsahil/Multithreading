#include "BST.h"
#include <limits.h>
#include "SeekRecord.h"

//Last bit is flagged
//Second Last bit is tagged

bool isTagged(Node* nodePtr)
{
	return int(nodePtr) & 2;
}

bool isFlagged(Node* nodePtr)
{
	return int(nodePtr) & 1;
}

BST::BST()
{
	root = new Node(INT_MAX);
	root->left = new Node(INT_MAX - 1);
	root->right = new Node(INT_MAX);

	((Node*)(root->left))->left = new Node(INT_MAX - 2);
	((Node*)(root->left))->right = new Node(INT_MAX - 1);
}


BST::~BST()
{
}

SeekRecord BST::seek(int key)
{
	SeekRecord seekRecord;
	seekRecord.ancestor = root;
	seekRecord.successor = root->left;
	seekRecord.parent = root->left;
	seekRecord.leaf = seekRecord.parent->left;

	Node* parentField = seekRecord.parent->left;
	Node* currentField = seekRecord.leaf->left;
	Node* current = currentField;

	while (current != NULL)
	{
		if (!(isTagged(parentField)))
		{
			seekRecord.ancestor = seekRecord.parent;
			seekRecord.successor = seekRecord.leaf;
		}
		seekRecord.parent = seekRecord.leaf;
		seekRecord.leaf = current;

		parentField = currentField;
		if (key < current->key)
		{
			currentField = current->left;
		}
		else
		{
			currentField = current->right;
		}
		current = currentField;
	}
	return seekRecord;
}

bool BST::search(int key)
{
	SeekRecord seekRecord = seek(key);
	if (seekRecord.leaf->key == key)
	{
		return true;
	}
	else
	{
		return false;
	}
}