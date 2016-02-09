#include <cstdlib>
#include <ctime>
#include <random>

#include "Utility.h"

const double Utility::PI = 3.1415926535897932;
const double Utility::EPSILON = 1.0e-6;

uint xorshf96(void)
{
	/* Faster than rand() and still reliable. */
	/* http://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c */

	static __declspec(thread) uint x = 123456789, y = 362436069, z = 521288629;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;

	uint t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;
	return z;
}

double Utility::rand0To1()
{
	static std::default_random_engine generator =
		std::default_random_engine(static_cast<uint>(time(nullptr)));
	static std::uniform_real_distribution<double> distribution =
		std::uniform_real_distribution<double>(0.0, std::nextafter(1.0, DBL_MAX));
	return distribution(generator);
}

double Utility::fastRand0To1()
{
	return static_cast<double>(xorshf96()) / static_cast<double>(0xFFFFFFFF);
}