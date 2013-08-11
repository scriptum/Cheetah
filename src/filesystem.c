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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "cheetah.h"
#include "macros.h"
#include "test.h"

void myError(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	fputs("error: ", stderr);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

bool isPointer(void * ptr) {
	return ptr != NULL;
}


unsigned char *loadfile(const char *filename, unsigned int *length) {
	unsigned char *result = NULL;
	size_t size = 0;
	FILE *file = fopen(filename, "rb");
	ERROR_IF_NULL(file);
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	new(result, unsigned char, size);
	if (size != fread(result, sizeof(unsigned char), size, file)) {
		goto error;
	}
	if (length)
		*length = size;
	fclose(file);
	return result;
error:
	delete(result);
	myError("can't load file %s", filename);
	return NULL;
}

bool fileExists(const char * filename) {
	FILE *file = fopen(filename, "rb");
	if(NULL == file)
		return FALSE;
	else
	{
		fclose(file);
		return TRUE;
	}
}

#define filetime(var) int file ## var ## time(const char * filename) {         \
    static struct stat buf;                                                    \
    int result = stat(filename, &buf);                                         \
    if( result != 0 ) {                                                        \
        myError("can't get information about file %s", filename);              \
        return -1;                                                             \
    }                                                                          \
    else                                                                       \
        return buf.st_ ## var ## time;                                         \
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
	return opendir(name);
}

struct dirent *readDir(DIR *dirp) {
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
		return TRUE;
	}
	return FALSE;
}

bool mkDir(const char * path) {
#ifdef _WIN32
	if(mkdir(path) == 0) return TRUE;
#else
	if(mkdir(path, 0755) == 0) return TRUE;
#endif
	return FALSE;
}

char *getDirentName(struct dirent * de) {
	return de->d_name;
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
