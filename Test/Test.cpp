#include "Test.h"
#include "../ParallelLinkedList/LinkedList.h"
#include <iostream>
#include <chrono>
#include <fstream>

using namespace std;

template<class T>
Test<T>::Test(T* cds, float searchWeight, float insertWeight, float removeWeight, KeyGenerator** keygens, int threadCount)
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
		testSubjectPtr->insert(keygens[0]->key());
	}
}

template<class T>
void Test<T>::Runner(int thread_no)
{
	//Pick Operation;
	static thread_local hash<thread::id> hasher;
	static thread_local mt19937 gen(time(0) + hasher(this_thread::get_id()));
	static thread_local discrete_distribution<int> dist{searchWeight, insertWeight, removeWeight};
	int operation = dist(gen);
	
	//Generate Key
	int key = keygens[thread_no]->key();
	
	operationsCount[operation]++;
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
	ofstream fout;
	fout.open("results.csv", ios::out|ios::app);
	
	std::chrono::steady_clock::time_point start, end;
	operationsCount[0] = 0;
	operationsCount[1] = 0;
	operationsCount[2] = 0;

	thread* threads = new thread[threadCount];
	int i;

	start = std::chrono::steady_clock::now();
	
	for(i = 0; i < threadCount; i++)
	{
		threads[i] = thread(&Test<T>::LoopRunner, this, i, operationsPerThread);
	}

	for(i = 0; i < threadCount; i++)
	{
		threads[i].join();
	}
		
	end = std::chrono::steady_clock::now();

	fout << threadCount;
	//fout << ',' << threadCount;
	fout << ',' << operationsPerThread;
	fout << ',' << std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count() << "ms";
	fout << ',' << threadCount * operationsPerThread;
	fout << ',' << searchWeight;
	fout << ',' << insertWeight;
	fout << ',' << removeWeight;
	fout << ',' << operationsCount[0];	//Actual Search count
	fout << ',' << operationsCount[1];	//Actual Insert count
	fout << ',' << operationsCount[2];	//Actual Remove count
	fout << std::endl;
	fout.close();
}

template class Test<ConcurrentLinkedList>;