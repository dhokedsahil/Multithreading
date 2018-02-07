#include "LinkedList.h"
#include "../Test/Test.h"
#include "../Keygeneration/KeyGenerator.h"

int main()
{
    ConcurrentLinkedList cll;
    int noThreads = 10;
    KeyGenerator** keygens = new KeyGenerator*[noThreads];
    for(int i = 0; i < noThreads; i++)
    {
        keygens[i] = new uniformKeyGenerator(5,80);
    }
    Test<ConcurrentLinkedList> test(&cll, 10.0, 100.0, 100.0, keygens, noThreads);
    test.Prepopulate(10);
    cll.print();
    test.Run(1000000);
    cll.print();
    return 0;
}