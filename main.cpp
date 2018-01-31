#include "ParallelLinkedList/LinkedList.h"
#include "ParallelLinkedList/Nodeptr.h"
#include "ParallelLinkedList/Node.h"
#include <iostream>

void LinkedListInteractive()
{
	ConcurrentLinkedList ll;
	bool loop = true;
	int action = -1;
	int key;
	while(loop)
	{
		std::cout << "1. Insert\n2. Remove\n3. Search\n4. Display\n5. Exit\nChoose an action: ";
		std::cin >> action;
		switch(action)
		{
			case 1: std::cout << "Key: "; std::cin >> key; if(ll.insert(key).pointer == NULL) std::cout << "Insert failed\n"; else std::cout << "Insert succeeded\n"; break;
			case 2: std::cout << "Key: "; std::cin >> key; if(ll.remove(key).pointer == NULL) std::cout << "Remove failed\n"; else std::cout << "Remove succeeded\n"; break;
			case 3: std::cout << "Key: "; std::cin >> key; if(ll.search(key).pointer == NULL) std::cout << "Search failed\n"; else std::cout << "Search succeeded\n"; break;
			case 4: ll.print(); std::cout << std::endl; break;
			case 5: loop = false; break;
			default: std::cout << "Invalid action\n\n"; break;
		}
	}
}

int main()
{
	LinkedListInteractive();
	return 0;
}
