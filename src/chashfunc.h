/*******************************************************************************

Copyright (c) 2013 Pavel Roschin (aka RPG) <rpg89@post.ru>

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


/*
 * Hash functions collection
 * */

#ifndef HASHFUNC_H_
#define HASHFUNC_H_

#include <stdint.h>

static inline uint32_t jenkins_one_at_a_time_hash(const char *key)
{
	uint32_t hash, i;
	const unsigned char *s = (unsigned char *)key;
	for(hash = i = 0; s[i]; ++i)
	{
		hash += s[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}


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

#endif /* HASHFUNC_H_ */
