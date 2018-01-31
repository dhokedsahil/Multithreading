#ifndef SAHIL_KEY_GENERATOR
#define SAHIL_KEY_GENERATOR

#include <thread>
#include <random>

using namespace std;

class KeyGenerator
{
public:
	virtual int key() =0;
};

class uniformKeyGenerator : public KeyGenerator
{
public:
	int min, max;
	mt19937 gen;
	uniform_int_distribution<int> dist;
	
	uniformKeyGenerator();
	uniformKeyGenerator(int min, int max);
	int key();
};

class zipfKeyGenerator : public KeyGenerator
{
	const int MAX_POSSIBLE = 1000;
public:
	int max;
	mt19937 gen;
	discrete_distribution<int> dist;
	zipfKeyGenerator();
	zipfKeyGenerator(int max);
	int key();
};

#endif