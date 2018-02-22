#include "Test.h"
#include "../ParallelLinkedList/LinkedList.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <map>

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
void Test<T>::RunFromConfig(T* cds, string configFileName)
{	
	int i, j;
	ifstream fin;
	map<string, int> configs;
	string config_name;
	char equals;
	int config_value;
	fin.open("config", ios::in);
	while(fin)
	{
		fin >> config_name >> equals >> config_value;
		if(equals == '=')
		{
			configs[config_name] = config_value;
		}
	}
	fin.close();

	int noThreads = configs["max_threads"];
	KeyGenerator** keygens = new KeyGenerator*[noThreads];
    for(i = 0; i < noThreads; i++)
    {
		if(configs["keyspace"] == 0)
		{
			keygens[i] = new uniformKeyGenerator(configs["min_key"], configs["max_key"], i);
		}
		else if(configs["keyspace"] == 1)
		{
			keygens[i] = new zipfKeyGenerator(configs["min_key"], configs["max_key"], i);
		}
    }

	Test<T> test(cds, configs["search_weight"], configs["insert_weight"], configs["remove_weight"], keygens, configs["min_threads"]);

	for(i = configs["min_threads"]; i <= configs["max_threads"]; i+= configs["thread_increment"])
	{
		for(j= configs["operations_per_thread_min"]; j <= configs["operations_per_thread_max"]; j*= configs["operations_per_thread_increment_factor"])
		{
			test.Prepopulate(configs["prepopulate"]);
			test.threadCount = i;
			cout << "Beginning run for " << i << " threads with " << j << " operations per thread" << endl;
			test.Run(j);
			cout << "Finished run for " << i << " threads with " << j << " operations per thread\n" << endl;
			cds->removeAll();
		}
	}
	
}

template<class T>
void Test<T>::Prepopulate(int count)
{
	bool success = false;
	int i;
	for(i = 0; i < count; i++)
	{
		while(!success)
		{
			testSubjectPtr->insert(keygens[0]->key(), success);
		}
		success = false;
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
	bool success;
	//Perform Operation
	switch(operation)
	{
		case 0:testSubjectPtr->search(key, success);break;
		case 1:testSubjectPtr->insert(key, success);break;
		case 2:testSubjectPtr->remove(key, success);break;
	}
	if(success)
	{
		operationsSuccessCount[operation]++;
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
	std::chrono::steady_clock::time_point start, end;
	operationsCount[0] = 0;
	operationsCount[1] = 0;
	operationsCount[2] = 0;

	operationsSuccessCount[0] = 0;
	operationsSuccessCount[1] = 0;
	operationsSuccessCount[2] = 0;

	int before_size = testSubjectPtr->size();
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
	std::chrono::milliseconds time_ms = chrono::duration_cast<std::chrono::milliseconds>(end-start);
	double latency_ms = chrono::duration_cast<std::chrono::microseconds>(end-start).count() / (double)operationsPerThread;//latency(time per operation) in microseconds
	testSubjectPtr->CleanUp();
	int after_size = testSubjectPtr->size();

	ofstream fout;
	fout.open("results.csv", ios::out|ios::app);
	fout << threadCount;
	//fout << ',' << threadCount;
	fout << ',' << operationsPerThread;
	fout << ',' << time_ms.count();// in milliseconds
	fout << ',' << threadCount * operationsPerThread;
	fout << ',' << threadCount * operationsPerThread/ (double)time_ms.count();//operations per second
	fout << ',' << latency_ms;
	fout << ',' << searchWeight;
	fout << ',' << insertWeight;
	fout << ',' << removeWeight;
	fout << ',' << operationsCount[0];	//Actual Search count
	fout << ',' << operationsCount[1];	//Actual Insert count
	fout << ',' << operationsCount[2];	//Actual Remove count
	fout << ',' << before_size;
	fout << ',' << after_size;
	fout << ',' << operationsSuccessCount[0];	//Successful Search count
	fout << ',' << operationsSuccessCount[1];	//Successful Insert count
	fout << ',' << operationsSuccessCount[2];	//Successful Remove count
	fout << ',' << keygens[0]->min;
	fout << ',' << keygens[0]->max;
	fout << ',' << keygens[0]->distributionType();
	fout << std::endl;
	fout.close();
}

template class Test<ConcurrentLinkedList>;