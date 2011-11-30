#include "cheetah.h"

void myError(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

unsigned char * loadfile(const char * filename, unsigned int * length)
{
	unsigned char * result;
	unsigned int size = 0;
	FILE *f = fopen(filename, "rb");
	if (!f) {
		myError("can't load file %s", filename);
		return 0;
	}
	fseek(f, 0, SEEK_END);
	size = ftell(f);
	fseek(f, 0, SEEK_SET);
	result = (char *)malloc(size);
	if (size != fread(result, sizeof(char), size, f)) {
		free(result);
		myError("can't load file %s", filename);
		return 0;
	}
	*length = size;
	fclose(f);
	return result;
}