/*******************************************************************************

Copyright (c) 2012-2014 Pavel Roschin (aka RPG) <rpg89@post.ru>

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
#include <errno.h>

#include "cheetah.h"
#include "cmacros.h"
#include "test.h"

CHEETAH_EXPORT void myError(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fputs("error: ", stderr);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);
}

CHEETAH_EXPORT bool isPointer(void *ptr)
{
	return ptr != NULL;
}

/**
 * Loads whole file into C-string. Note that returned value  null-terminated.
 * @param filename file to read
 * @param length resulting string length
 * @return newly allocated string, caller should free
 * */
CHEETAH_EXPORT unsigned char *loadfile(const char *filename, long *length)
{
	unsigned char *result = NULL;
	long size = 0;
	FILE *f = NULL;
	errno = 0;
	f = fopen(filename, "rb");
	ERROR_IF_NULL(f);
	if(fseek(f, 0, SEEK_END) != 0)
	{
		goto error;
	}
	size = ftell(f);
	if(size < 0)
	{
		goto error;
	}
	if(fseek(f, 0, SEEK_SET) != 0)
	{
		goto error;
	}
	new(result, unsigned char, size + 1);
	if(size != (long)fread(result, sizeof(char), (size_t)size, f))
	{
		goto error;
	}
	if(length)
	{
		*length = size;
	}
	result[size] = '\0';
	fclose(f);
	return result;
error:
	myError("Cannot load file %s (%s)", filename, strerror(errno));
	if(f)
	{
		fclose(f);
	}
	delete(result);
	return NULL;
}

CHEETAH_EXPORT bool fileExists(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if(NULL == file)
	{
		return FALSE;
	}
	else
	{
		fclose(file);
		return TRUE;
	}
}

#define filetime(var) long int file ## var ## time(const char * filename) {    \
    static struct stat buf;                                                    \
    long int result = (long int)stat(filename, &buf);                          \
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

#if 0
CHEETAH_EXPORT long int fileatime(const char *filename)
CHEETAH_EXPORT long int filemtime(const char * filename)
CHEETAH_EXPORT long int filectime(const char * filename)
#endif

CHEETAH_EXPORT DIR *openDir(const char * name)
{
	return opendir(name);
}

CHEETAH_EXPORT struct dirent *readDir(DIR * dirp)
{
	return readdir(dirp);
}

CHEETAH_EXPORT int closeDir(DIR * dirp)
{
	return closedir(dirp);
}

CHEETAH_EXPORT bool isDir(const char * name)
{
	DIR *dir = opendir(name);
	if(dir)
	{
		closedir(dir);
		return TRUE;
	}
	return FALSE;
}

CHEETAH_EXPORT bool mkDir(const char * path)
{
#ifdef _WIN32
	if(mkdir(path) == 0)
	{
		return TRUE;
	}
#else
	if(mkdir(path, 0755) == 0)
	{
		return TRUE;
	}
#endif
	return FALSE;
}

CHEETAH_EXPORT const char *getDirentName(struct dirent * de)
{
	return de->d_name;
}

