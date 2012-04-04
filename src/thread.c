#include "cheetah.h"

inline queue newQueue()
{
	node q = malloc(sizeof(node_t));
	q->next = q->prev = 0;
	return q;
}

inline void enqueue(queue q, QDATA n)
{
	//~ printf("%s\n", n.image->name);
	//~ SDL_mutexP(resQueueMutex);
	node nd = malloc(sizeof(node_t));
	nd->val = n;
	if (!QHEAD(q)) QHEAD(q) = nd;
	nd->prev = QTAIL(q);
	if (nd->prev) nd->prev->next = nd;
	QTAIL(q) = nd;
	nd->next = 0;
	//~ printf("%d\n", QEMPTY(resLoaderQueue));
	//~ SDL_mutexV(resQueueMutex);
}
 
inline int dequeue(queue q, QDATA *val)
{
	//~ SDL_mutexP(resQueueMutex);
	node tmp = QHEAD(q);
	if (!tmp) return 0;
	*val = tmp->val;
	//~ printf("%s\n", tmp->val.image->name);
	QHEAD(q) = tmp->next;
	if (QTAIL(q) == tmp) QTAIL(q) = 0;
	free(tmp);
	//~ SDL_mutexV(resQueueMutex);
	return 1;
}

int resLoaderThread(void *unused)
{
	Resource r;
	//~ SDL_Event e;
	unsigned char *img;
	//~ unsigned char *myBuf;
	//~ bool empty;
	int width, height;
	while(1){
		//~ SDL_mutexP(resQueueMutex);
		//~ empty = QEMPTY(resLoaderQueue)
		//~ SDL_mutexV(resQueueMutex);
		SDL_Delay(10);
		//~ if(QEMPTY(resLoaderQueue)) printf(": %d\n", QEMPTY(resLoaderQueue));
		if(!resShared&&!QEMPTY(resLoaderQueue))
		{
			//~ printf("Queue: %d\n", QEMPTY(resLoaderQueue));
			//~ SDL_mutexP(resQueueMutex);
			dequeue(resLoaderQueue, &r);
			img = loadImageData(r.image->name, &width, &height, &r.image->channels);
			r.image->w = (float)width;
			r.image->h = (float)height;
			r.data = img;
			//~ printf("%s\n", r.image->name);
			resShared = &r;
			//~ SDL_Delay(5);
			//~ new(e, SDL_Event, 1);
			//~ e.type = SDL_USEREVENT;
			//~ e.user.code = 0;
			//~ e.user.data1 = (void*)&r;
			//~ SDL_PushEvent(&e);
		}
	}
}
