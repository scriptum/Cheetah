#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include "cheetah.h"

void myError(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "error: ");
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

bool isPointer(void * ptr) {
	return ptr != NULL;
}

unsigned char * loadfile(const char * filename, unsigned int * length) {
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
	if (length)
		*length = size;
	fclose(f);
	return result;
}

#define filetime(var) int file ## var ## time(const char * filename) {\
	struct stat buf;\
	int result = stat( filename, &buf );\
	if( result != 0 )\
		myError("can't get information about file %s", filename);\
	else\
		return buf.st_ ## var ## time;\
}

filetime(m)
filetime(a)
filetime(c)

#ifdef false
int fileatime(const char * filename) {
int filemtime(const char * filename) {
int filectime(const char * filename) {
#endif

DIR *openDir(const char *name) {
	return opendir (name);
}

struct dirent *readDir (DIR *dirp) {
	return readdir(dirp);
}

int closeDir(DIR *dirp) {
	return closedir(dirp);
}

bool isDir(const char *name) {
	DIR *dir = opendir(name);
	if(dir)
	{
		closedir(dir);
		return 1;
	}
	return 0;
}

//~ const char * fileExt(const char * name) {
	//~ int i, pos = 0;
	//~ char ch;
	//~ for (i = 0; ; i++)
	//~ {
		//~ ch = name[i];
		//~ if(ch)
		//~ {
			//~ if(ch == '.') pos = i;
		//~ }
		//~ else
			//~ break;
	//~ }
	//~ return (name + pos + 1);
//~ }