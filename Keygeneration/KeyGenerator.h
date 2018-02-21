#ifndef SAHIL_KEY_GENERATOR
#define SAHIL_KEY_GENERATOR

#include <thread>
#include <random>


using namespace std;

class KeyGenerator
{
public:
	virtual int key() =0;
	virtual string distributionType() =0;
	int min, max;
};

class uniformKeyGenerator : public KeyGenerator
{
public:
	mt19937 gen;
	uniform_int_distribution<int> dist;
	
	uniformKeyGenerator();
	uniformKeyGenerator(int min, int max, int threadId);
	int key();
	string distributionType();
};

class zipfKeyGenerator : public KeyGenerator
{
	const int MAX_POSSIBLE = 1000;
public:
	mt19937 gen;
	discrete_distribution<int> dist;
	zipfKeyGenerator();
	zipfKeyGenerator(int min, int max, int threadId);
	int key();
	string distributionType();
};

#endif