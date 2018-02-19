CXX = g++
CPPFLAGS = -std=c++11 -g -pthread

ParallelLinkedList/Tester: ParallelLinkedList/LinkedList.o ParallelLinkedList/Node.o ParallelLinkedList/Nodeptr.o Test/Test.o Keygeneration/KeyGenerator.o

main: ParallelLinkedList/LinkedList.o ParallelLinkedList/Node.o ParallelLinkedList/Nodeptr.o

ParallelLinkedList/LinkedList.o : ParallelLinkedList/LinkedList.cpp ParallelLinkedList/LinkedList.h ParallelLinkedList/Nodeptr.o ParallelLinkedList/Node.o