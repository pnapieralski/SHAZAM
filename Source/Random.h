/*
[]~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ random.h ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[]
[]                                                                            []
[]                       Random Number Generator Class                        []
[]                                                                            []
[]                             James L. Richards                              []
[]                       Last Update: August 25, 2008                         []
[]____________________________________________________________________________[]
*/

#pragma once

#include <climits>
#include <ctime>

#ifndef _RANDOM
#define _RANDOM

class Random
{
public:
	Random();
	// Initializes the generator.

	double RandomNum();
	// Returns a random number between 0.0 and 1.0.

	int RandomInt();
	// Returns a random integer between 0 and RAND_MAX.

	int RandomInt(int n);
	// Precondition: n > 0;
	// Returns a random integer between 0 and n - 1.

	int RandomInt(int min, int max);
	// Precondition: max >= min;
	// Returns a random integer between min and max.

	int RandomMax();
	// Returns the largest random integer that the
	// generator can produce.

private:
	void Seed();
};

Random::Random() { Seed(); }

double Random::RandomNum()
{ return rand() / double(RAND_MAX); }

int Random::RandomInt()
{ return rand(); }

int Random::RandomInt(int n)
{ return rand() % n; }

int Random::RandomInt(int min, int max)
{ return min + int(RandomNum() * (max - min + 1)); }

int Random::RandomMax()
{ return RAND_MAX; }

void Random::Seed()
{
	time_t seconds;

	time(&seconds);
	srand(seconds);
	rand();
}

#endif