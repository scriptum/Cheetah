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

#ifndef LIST_H_
#define LIST_H_

typedef struct listSingle
{
	void *data;
	struct listSingle *next;
} listSingle;

typedef struct listDouble
{
	void *data;
	struct listDouble *next;
	struct listDouble *prev;
} listDouble;

typedef struct listSingleHead
{
	void *data;
	struct listSingle *next;
	struct listSingle *prev;
} listSingleHead;

static inline void listPush(listDouble *head, listDouble *item)
{
	if(NULL == head)
	{
		return;
	}
	if(NULL == item)
	{
		return;
	}
	if(NULL == head->next)
	{
		head->next = item;
		item->prev = head;
	}
	else
	{
		head->prev->next = item;
		item->prev = head->prev;
	}
	head->prev = item;
	item->next = NULL;
}

static inline listDouble *listPop(listDouble *head)
{
	if(NULL == head || NULL == head->next)
	{
		return NULL;
	}
	listDouble *ret = head->prev;
	ret->prev->next = NULL;
	head->prev = head->prev->prev;
	return ret;
}

#endif /* LIST_H_ */
