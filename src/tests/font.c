#include <stdio.h>
#include <string.h>
#define DEBUGVV

#include "../cheetah.h"
CHEETAH_EXPORT Font *newFont(const char *filename);

int main(int argc, char **argv)
{
	if(newFont(argv[1]))
		return 0;
	return -1;
}

