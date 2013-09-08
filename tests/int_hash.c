#include <stdio.h>
#include <time.h>
typedef unsigned char bool;

// #define HASH_DEBUG

#include "../src/cmacros.h"
#include "../src/chashtable.h"
#include "../src/crandom.h"


#ifndef TRUE
#define TRUE	(bool) 1
#endif
#ifndef FALSE
#define FALSE	(bool) 0
#endif

static inline uint32_t uint32_cmp(uint32_t a, uint32_t b)
{
	return a == b;
}

static inline uint32_t hash_reduced_instructions(uint32_t key)
{
	return key ^ (key << 7) ^ (key >> 3);
}

static inline uint32_t hash_key(uint32_t key)
{
	return key;
}

// HASH_TEMPLATE(uint32hash, uint32_t, uint32_t, fontHashFunc, uint32_cmp)
HASH_TEMPLATE(uint32hash_std, uint32_t, uint32_t, hash_uint32, uint32_cmp)
HASH_TEMPLATE(uint32hash_reduced, uint32_t, uint32_t, hash_reduced_instructions, uint32_cmp)
HASH_TEMPLATE(uint32hash_key, uint32_t, uint32_t, hash_key, uint32_cmp)


static inline uint32_t data_linear(uint32_t key)
{
	return key;
}

static inline uint32_t data_random(uint32_t key)
{
	return hash_uint32(key) >> 6;
}

#define TEST(f, data, description)                                             \
	v = 0;                                                                 \
	uint32hash_##f *hash##f##data = uint32hash_##f##_new();                \
	for (i = 0; i < 10000000; i++)                                         \
	{                                                                      \
		uint32hash_##f##_set(hash##f##data, data(i), 1);               \
	}                                                                      \
	/* testing only search speed due to randomized nature of data */       \
	begin = clock();                                                       \
	for (i = 0; i < 20000000; i++)                                         \
	{                                                                      \
		v += uint32hash_##f##_get(hash##f##data, i);                   \
	}                                                                      \
	printf("%u\n", v);                                                     \
	end = clock();                                                         \
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;                   \
	printf("%-60s %.3g s\n", description, time_spent);

int main(int argc, char **argv)
{
	int i;
	clock_t begin, end;
	double time_spent;
	volatile uint32_t v;
	TEST(std, data_linear, "Standart hash func, linear data")
	TEST(reduced, data_linear, "Reduced instructions hash func, linear data")
	random_hash_seed(0);
	TEST(std, data_random, "Standart hash func, random data")
	random_hash_seed(0);
	TEST(reduced, data_random, "Reduced instructions hash func, random data")
	TEST(key, data_random, "Simple key = hash func, random data")
}
