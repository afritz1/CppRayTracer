#ifndef UTILITY_H
#define UTILITY_H

typedef long long llong;
typedef unsigned long long ullong;
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char uchar;

//#define DEBUG

// Axis enum, used with the "split coordinate" calculation in the BVH build.
enum class Axis { X = 0, Y = 1, Z = 2 };

class Utility
{
public:
	static const double PI;
	static const double EPSILON;

	Utility() = delete;
	Utility(const Utility&) = delete;
	~Utility() = delete;

	static double rand0To1();
	static double fastRand0To1();
};

#endif