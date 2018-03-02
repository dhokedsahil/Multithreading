#include "LinkedList.h"
#include "../Test/Test.h"
#include "../Keygeneration/KeyGenerator.h"
#include <map>
#include <iostream>
#include <string>
#include <cstdlib>

int main(int argc, char* argv[])
{
    if(argc % 2 == 0)
    {
        std::cerr << "Incorrect usage" << std::endl;
        return 1;
    }
    map<string, int> configs = Test<ConcurrentLinkedList>::getConfigs("config");
    int i;
    for(i = 1; i < argc; i+= 2)
    {
        configs[argv[i]] = atoi(argv[i+1]);
    }
    ConcurrentLinkedList cll;
    Test<ConcurrentLinkedList>::Run(&cll, configs);
    
    return 0;
}