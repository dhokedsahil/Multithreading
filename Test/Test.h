#ifndef SAHIL_TEST
#define SAHIL_TEST

#include "../Key generation/KeyGenerator.h"

class ConcurrentDS
{
public:
	virtual int search(int key) =0;
	virtual int insert(int key) =0;
	virtual int remove(int key) =0;	
};

class Test
{
public:
	ConcurrentDS* testSubjectPtr;
	float searchWeight;
	float insertWeight; 
	float removeWeight;
	int threadCount;
	KeyGenerator* keygens;//An array of key generators
	Test(ConcurrentDS*, float searchWeight, float insertWeight, float removeWeight, KeyGenerator*, int threadCount);
	void Prepopulate(int count);
	void Runner(int thread_no);
	void LoopRunner(int thread_no, int operationsPerThread);
	void Run();
};

#endif