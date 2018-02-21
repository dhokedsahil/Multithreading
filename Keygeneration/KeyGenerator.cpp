#include <iostream>
#include <thread>
#include <random>
#include "KeyGenerator.h"

using namespace std;

uniformKeyGenerator::uniformKeyGenerator():uniformKeyGenerator(1,1000, 0){}

//TODO
uniformKeyGenerator::uniformKeyGenerator(int min, int max, int threadId)
{
	this->min = min;
	this->max = max;
	std::hash<int> hasher;
	gen = mt19937(time(0) + hasher(threadId));
	dist = uniform_int_distribution<int>(min, max);
}

int uniformKeyGenerator::key()
{
	return dist(gen);
}

string uniformKeyGenerator::distributionType(){return "uniform";}

zipfKeyGenerator::zipfKeyGenerator():zipfKeyGenerator(1, 1000, 0){}

//TODO
zipfKeyGenerator::zipfKeyGenerator(int min, int max, int threadId)
{
	this->min = (min < 1) ? 1 : min;
	this->max = (max < MAX_POSSIBLE) ? max : MAX_POSSIBLE;
	std::hash<int> hasher;
	gen = mt19937(time(0) + hasher(threadId));
	dist = discrete_distribution<int>(this->max, this->min - 1, (double)(this->max) - this->min + 1, [](double i){return 1.0/(i + 0.5);});
}

int zipfKeyGenerator::key()
{
	return dist(gen);
}

string zipfKeyGenerator::distributionType(){return "zipf";}