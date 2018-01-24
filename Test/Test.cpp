#include "Test.h"

using namespace std;

template<class T>
Test<T>::Test(T* cds, float searchWeight, float insertWeight, float removeWeight, KeyGenerator* keygens, int threadCount)
{
	this->testSubjectPtr = cds;
	this->searchWeight = searchWeight;
	this->insertWeight = insertWeight;
	this->removeWeight = removeWeight;
	this->keygens = keygens;
	this->threadCount = threadCount;
}

template<class T>
void Test<T>::Prepopulate(int count)
{
	int i;
	for(i = 0; i < count; i++)
	{
		testSubjectPtr->insert(keygens[0].key());
	}
}

template<class T>
void Test<T>::Runner(int thread_no)
{
	//Pick Operation;
	hash<thread::id> hasher;
	thread_local static mt19937 gen(time(0) + hasher(this_thread::get_id()));
	thread_local static discrete_distribution<int> dist{searchWeight, insertWeight, removeWeight};
	int operation = dist(gen);
	
	//Generate Key
	int key = keygens[thread_no].key();
	
	//Perform Operation
	switch(operation)
	{
		case 0:testSubjectPtr->search(key);break;
		case 1:testSubjectPtr->insert(key);break;
		case 2:testSubjectPtr->remove(key);break;
	}
}

template<class T>
void Test<T>::LoopRunner(int thread_no, int operationsPerThread)
{
	int i;
	for(i = 0; i < operationsPerThread; i++)
	{
		Runner(thread_no);
	}
}

template<class T>
void Test<T>::Run(int operationsPerThread)
{
	thread* threads = new thread[threadCount];
	int i;
	for(i = 0; i < threadCount; i++)
	{
		threads[i] = thread(&Test::LoopRunner, this, i, operationsPerThread);
	}
	for(i = 0; i < threadCount; i++)
	{
		threads[i].join();
	}
}