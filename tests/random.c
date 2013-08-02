#include <time.h>
#include <stdlib.h>
#include <stdio.h>

/* cheetah's RNG */
#include "../src/random.h"

/* Simplest poor RNG */
static int fastrand_seed;

static inline int fastrand() {
	fastrand_seed = (214013 * fastrand_seed + 2531011);
	return (fastrand_seed >> 16) & 0x7FFF;
}

/* Generate 1000M numbers */
#define TEST(description, f) begin = clock();\
for (i = 0; i < 100000000; i++) {x=f; x=f; x=f; x=f; x=f; x=f; x=f; x=f; x=f; x=f;}\
end = clock();\
time_spent = (double)(end - begin) / CLOCKS_PER_SEC;\
printf("%-40s %.3g s\n", description, time_spent);

int main() {
	clock_t begin, end;
	double time_spent;
	int i;
	volatile uint32_t x;
	TEST("Stdlib's rand()", rand())
	TEST("Stupid simple fastrand()", fastrand())
	TEST("Xorshift 128", rand128())
	TEST("Xorshift 192", rand192())
}
