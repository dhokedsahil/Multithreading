#include "LinkedList.h"
#include "../Test/Test.h"
#include "../Keygeneration/KeyGenerator.h"

int main()
{
    ConcurrentLinkedList cll;
    
    Test<ConcurrentLinkedList>::RunFromConfig(&cll, "config");
    
    return 0;
}