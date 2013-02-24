/*******************************************************************************

Copyright (c) 2012-2013 Pavel Roschin (aka RPG) <rpg89@post.ru>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:  The above
copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.

*******************************************************************************/

#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>

/**
 * Set of some xorshift RNG's. They are all fast as hell, period differs only.
 * http://en.wikipedia.org/wiki/Xorshift
 **/

static uint32_t _xor_seed;

static inline uint32_t hash_uint32(uint32_t hash)
{
	hash += ~(hash << 15);
	hash ^=   hash >> 10;
	hash +=   hash << 3;
	hash ^=   hash >> 6;
	hash += ~(hash << 11);
	hash ^=   hash >> 16;
	return hash;
}

static inline void random_seed(uint32_t seed)
{
	_xor_seed = seed;
}

static inline void random_hash_seed(uint32_t seed)
{
	_xor_seed = hash_uint32(seed);
}

static inline uint32_t random_get_seed()
{
	return _xor_seed;
}

/**
 * Period: 2^128-1. Fastest: 646M rn/sec @ Intel Pentium 4 3.0 GHz
 **/
static inline uint32_t rand128()
{
	static uint32_t y = 362436069;
	static uint32_t z = 521288629;
	static uint32_t w = 88675123;
	uint32_t t;

	t = _xor_seed ^ (_xor_seed << 11);
	_xor_seed = y; y = z; z = w;
	return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
}

/**
 * Period: 2^192-2^32. Fast enough: 555M rn/sec @ Intel Pentium 4 3.0 GHz
 **/
static inline uint32_t rand192()
{
	static uint32_t y = 362436069, z = 521288629, w = 88675123, v = 5783321, d = 6615241;
	uint32_t t;
	t = (_xor_seed ^ (_xor_seed >> 2));
	_xor_seed = y;	y = z;	z = w;	w = v;
	v = (v ^ (v << 4)) ^ (t ^ (t << 1));
	return (d += 362437) + v;
}

/**
 * Small period, needs 64bit integer... Without seed!
 * It does not pass all tests, it slow, so I kept it just as example.
 **/
static inline uint32_t rand32()
{
	static unsigned long y = 123456789;
	y ^= (y << 13);
	y = (y >> 17);
	return (y ^= (y << 5));
}

/**
 * Return a float from 0.0 to 0.999...
 **/
static inline float randf(uint32_t random_integer)
{
	union {
		uint32_t	i;
		float		f;
	} u;
	u.i = 0x3F800000 | (random_integer >> 9);
	return u.f - 1.f;
}
/**
 * Return a float from -1.0 to 0.999...
 **/
static inline float randf2(uint32_t random_integer)
{
	union {
		uint32_t	i;
		float		f;
	} u;
	u.i = 0x40000000 | (random_integer >> 9);
	return u.f - 3.f;
}

#endif /*__RANDOM_H__*/
