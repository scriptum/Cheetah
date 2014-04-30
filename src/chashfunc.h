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

#include <stdbool.h>
#include <stdint.h>

static inline uint32_t hash_string_jenkins(const char *key)
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

static inline uint32_t hash_string(const char *c)
{
	//c = (const char*)__builtin_assume_aligned(c, 32);
	uint32_t h = (uint32_t) * c;
	while(c && *c)
	{
		h = ((h << 5) + h) + (uint32_t)(*c++);
	}
	return h;
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

static inline int cmp_string(const char *a, const char *b)
{
	/* Assume that string at least has 1 character and '\0'  */
	if((*(unsigned short *)a != *(unsigned short *)b))
		return 0;
	return strcmp(a, b) == 0;
}

#endif /* HASHFUNC_H_ */
