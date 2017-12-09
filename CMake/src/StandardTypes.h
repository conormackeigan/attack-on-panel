#pragma once

#include <cstdlib>
#include <iostream>

#define USING_SFML			1
#define STRCODE				unsigned int
#define	NoName				0xf06a6692
#define UNIQUEID			int

#define	MATH_HALF_PI		(1.5707963267948966192313216916398f)
#define	MATH_PI				(3.1415926535897932384626433832795f)
#define MATH_2PI			(6.283185307179586476925286766559f)
#define	MATH_180_OVER_PI	(57.295779513082320876798154814105f)
#define	MATH_PI_OVER_180	(0.01745329251994329576923690768489f)

#define	RAD_TO_DEG(rad)		(rad*MATH_180_OVER_PI)
#define	DEG_TO_RAD(deg)		(deg*MATH_PI_OVER_180)

#define e					(2.71828182845904523536f)

#define DEBUG_PRINT
#ifdef DEBUG_PRINT
#define LOG(out) std::cout << out << std::endl;
#else
#define LOG(out)
#endif

enum Flag
{
	none   = 0x0,
	flag1  = 0x1,
	flag2  = 0x2,
	flag3  = 0x4,
	flag4  = 0x8,
	flag5  = 0x10,
	flag6  = 0x20,
	flag7  = 0x40,
	flag8  = 0x80,
	flag9  = 0x100,
	flag10 = 0x200,
	flag11 = 0x400,
	flag12 = 0x800,
	flag14 = 0x1000,
	flag15 = 0x2000,
	flag16 = 0x4000
};

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets a random float from 0.0 and 1.0. </summary>
///-------------------------------------------------------------------------------------------------
inline float randFloat()
{
	return static_cast<float>((rand()) / (RAND_MAX + 1.0f));
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets a random value clamped between -1.0 and 1.0. </summary>
///-------------------------------------------------------------------------------------------------
inline float randFloatClamped()
{
	return randFloat() - randFloat();
}

inline int randRangeInt(int min, int max)
{
	return rand() % (max - min) + min;
}

///-------------------------------------------------------------------------------------------------
/// <summary>	Clamps a value between a minimum and maximum. </summary>
///-------------------------------------------------------------------------------------------------
#define clamp(val, t0, t1)					val < t0 ? t0 : val > t1 ? t1 : val

///-------------------------------------------------------------------------------------------------
/// <summary>	Gets the sign of a value. 0 is its own sign. </summary>
///-------------------------------------------------------------------------------------------------
#define sign(val)			val < 0 ? -1 : val > 0 ? 1 : 0

///-------------------------------------------------------------------------------------------------
/// <summary>	Normalizes a value between 0 and 1. </summary>
///-------------------------------------------------------------------------------------------------
#define normalize(val, t0, t1)				(val - t0) / (t1 - t0)

///-------------------------------------------------------------------------------------------------
/// <summary>	Normalizes a value between -1 and 1. </summary>
///-------------------------------------------------------------------------------------------------
#define normalize_negative(val, t0, t1)		2 * (val - t0) / (t1 - t0) - 1

///-------------------------------------------------------------------------------------------------
/// <summary>	Generates a hash code using FVN-1. </summary>
///-------------------------------------------------------------------------------------------------
inline STRCODE getHashCode(const char* str)
{
	const unsigned int fnv_prime = 0x811C9DC5;
	unsigned int hash = 0;
	char end = '\0';

	for (const char* ref = str; *ref != end; ref++)
	{
		hash *= fnv_prime;
		hash ^= *ref;
	}

	return hash;
}


#define THROW_RUNTIME_ERROR(condition, message)	\
	if (condition)								\
	{											\
		throw std::runtime_error(message);		\
	}


#ifndef NDEBUG
///-------------------------------------------------------------------------------------------------
/// <summary>	A macro that defines assert. </summary>
///
/// <remarks>	William Barry, 05/02/2013. </remarks>
/// <remarks>	Modified by Conor MacKeigan, 09/21/2017. </remarks>
///
/// <param name="condition">	The condition. </param>
/// <param name="message">  	The message. </param>
///-------------------------------------------------------------------------------------------------
#define ASSERT(condition, message) \
    do { \
        if (! (condition)) { \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
			abort(); \
        } \
    } while (false)
#else
#   define ASSERT(condition, message) do { } while (false)
#endif

///-------------------------------------------------------------------------------------------------
/// <summary>	A macro that defines static assert. </summary>
///-------------------------------------------------------------------------------------------------
#ifndef NDEBUG
#	define STATIC_ASSERT(condition, message) static_assert(condition, message)
#else
#	define STATIC_ASSERT(condition, message) do { } while (false)
#endif