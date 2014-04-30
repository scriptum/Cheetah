#include <stdio.h>
#include <string.h>

#include "../chashfunc.h"
#include "../chashtable.h"

HASH_TEMPLATE_FREE_KEY(strHash, char*, unsigned, hash_string, cmp_string)

//__attribute__((hot))
static inline void form(char *s, int i)
{
	char buf[11];
	int j = 10;
	buf[10] = '\0';
	do {
		buf[--j] = i % 10 + '0';
	} while(i /= 10);
	int k = 0, l = j;
	while(buf[l])
	{
		s[k] = s[k + 10 - j + 1] = buf[l];
		k++;
		l++;
	}
	s[k] = 'x';
	s[k + 10 - j + 1] = 0;
}


int main()
{
	const int max = 1000000;
	const int repeat = 10;
	Hash *hash = strHashNewSize(max);
	int i;
	long long res = 0;
	char buf[] = "1000000000x1000000000";
	for(i = 0; i < max; i++) {
		form(buf, i);
		strHashSet(hash, strdup(buf), i);
	}
	int j;
	for(j = 0; j < repeat; j++)
		for(i = 0; i < max; i++) {
			form(buf, i);
			res += strHashGet(hash, buf);
		}
	if(res != (long long)max * ((long long)max - 1L) * 5L) {
		printf("TEST FAILED");
		return -1;
	}
	//hashDestroy(hash);
	return 0;

}
