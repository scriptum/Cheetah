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

/**
 * Hash arrays for C.
 * */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "cdebug.h"

#ifndef HASH_START_SIZE
#define HASH_START_SIZE 128
#endif

#ifndef HASH_REHASH_RATIO
#define HASH_REHASH_RATIO 3 / 4
#endif

#ifndef HASH_PROBING
#define HASH_PROBING (unsigned)(i + (probes))
#endif

#define OFFSETOF(type, field) (&(((type *) 0)->field))

#ifndef unlikely
#define unlikely(a) (a)
#endif

typedef struct HashInfo {
    void         (*key_free)(void*);          /* Function to free each key */
    void         (*value_free)(void*);        /* Function to free each value */
    unsigned       node_size;                 /* Node size */
    unsigned       key_offset;                /* Key offset inside node structure */
    unsigned       val_offset;                /* value offset inside node structure */
} HashInfo;

typedef struct Hash {
    void          *nodes;                     /* Nodes array */
    unsigned       size;                      /* Hash size (nodes array size) */
    unsigned       items;                     /* Number of items that hash stores */
    HashInfo       info;
} Hash;

static inline unsigned roundpot(unsigned v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	return v + 1;
}

static inline void *hashGetNode(Hash *hash, unsigned i)
{
    return (char *)hash->nodes + (i * hash->info.node_size);
}

static inline void *hashGetKey(Hash *hash, unsigned i)
{
    char *ptr = ((char *)hash->nodes + (i * hash->info.node_size + hash->info.key_offset));
    return *((void**)ptr);
}

static inline void *hashGetValue(Hash *hash, unsigned i)
{
    char *ptr = ((char *)hash->nodes + (i * hash->info.node_size + hash->info.val_offset));
    return *((void**)ptr);
}

static void hashForeach(Hash *hash, void (*func)(void*, void*), void *data)
{
    unsigned i;
    for(i = 0; i <= hash->size; i++)
        func(hashGetNode(hash, i), data);
}

static void hashForeachKey(Hash *hash, void (*func)(void*, void*), void *data)
{
    unsigned i;
    for(i = 0; i <= hash->size; i++)
        func(hashGetKey(hash, i), data);
}

static void hashForeachValue(Hash *hash, void (*func)(void*, void*), void *data)
{
    unsigned i;
    for(i = 0; i <= hash->size; i++)
        func(hashGetValue(hash, i), data);
}

static void hashDestroy(Hash *hash)
{
    if(unlikely(NULL == hash))
        return;
    if(hash->info.key_free)
    {
        unsigned i;
        for(i = 0; i <= hash->size; i++)
            hash->info.key_free(hashGetKey(hash, i));
    }
    if(hash->info.value_free)
    {
        unsigned i;
        for(i = 0; i <= hash->size; i++)
            hash->info.value_free(hashGetValue(hash, i));
    }
    free(hash->nodes);
    free(hash);
}

static Hash *hashNewSizeInternal(unsigned size, HashInfo info)
{
    Hash *hash = (Hash*)calloc(1, sizeof(Hash));
    if(unlikely(NULL == hash))
        goto error;
    if(unlikely(size == 0))
        goto error;
    size = roundpot(size);
    hash->size  = size - 1;
    hash->info = info;
    hash->nodes = calloc(size, info.node_size);
    if(unlikely(NULL == hash->nodes))
        goto error;
    dbg("Hash: created new of size %u, node size: %u", size, info.node_size);
    return hash;
error:
    dbg("Hash: error: cannot allocate memory");
    hashDestroy(hash);
    return NULL;
}

static bool hashRehashInternal(Hash *hash, void (*func)(void*, void*))
{
    unsigned i;
    if(unlikely(NULL == hash))
        return false;
    Hash *newhash = hashNewSizeInternal((hash->size + 1) * 2, hash->info);
    if(unlikely(NULL == newhash))
        return false;
    hashForeach(hash, func, newhash);
    free(hash->nodes);
    hash->nodes = newhash->nodes;
    hash->size = newhash->size;
    free(newhash);
    return true;
}

static inline unsigned hashLength(Hash *hash)
{
    if(unlikely(NULL == hash))
        return 0;
    return hash->items;
}

static inline unsigned hashSize(Hash *hash)
{
    if(unlikely(NULL == hash))
        return 0;
    return hash->size + 1;
}

#define HASH_TEMPLATE_FULL(hName, keyType, valType, hashFunc, cmpFunc, freeKey, freeVal)\
                                                                               \
typedef struct __attribute__((packed)) hName##Node {                           \
    keyType  key;                                                              \
    valType  value;                                                            \
    unsigned exists;                                                           \
} hName##Node;                                                                 \
                                                                               \
static Hash *hName##NewSize(unsigned size) {                                   \
    HashInfo info;                                                             \
    info.node_size = sizeof(hName##Node);                                      \
    dbg("%s: node size: %u", #hName, info.node_size);                          \
    info.key_offset = offsetof(hName##Node, key);                              \
    dbg("%s: key offset: %u", #hName, info.key_offset);                        \
    info.val_offset = offsetof(hName##Node, value);                            \
    dbg("%s: value offset: %u", #hName, info.val_offset);                      \
    info.key_free = freeKey;                                                   \
    info.value_free = freeVal;                                                 \
    return hashNewSizeInternal(size, info);                                    \
}                                                                              \
                                                                               \
static Hash *hName##New(void) {                                                \
    return hName##NewSize(HASH_START_SIZE);                                    \
}                                                                              \
                                                                               \
static inline bool hName##Set(Hash *hash, keyType key, valType value);         \
                                                                               \
static bool hName##Rehash(Hash *hash) {                                        \
    unsigned i;                                                                \
    if(unlikely(NULL == hash))                                                 \
        return false;                                                          \
    Hash *newhash = hName##NewSize((hash->size + 1) * 2);                      \
    if(unlikely(NULL == newhash))                                              \
        return false;                                                          \
    for(i = 0; i <= hash->size; i++) {                                         \
        hName##Node n = ((hName##Node*)hash->nodes)[i];                        \
        if(n.exists)                                                           \
            hName##Set(newhash, n.key, n.value);                               \
    }                                                                          \
    free(hash->nodes);                                                         \
    hash->nodes = newhash->nodes;                                              \
    hash->size = newhash->size;                                                \
    free(newhash);                                                             \
    return true;                                                               \
}                                                                              \
                                                                               \
static inline hName##Node *hName##GetNode(Hash *hash, keyType key) {           \
    unsigned hashv = hashFunc(key);                                            \
    unsigned i = hashv & hash->size;                                           \
    unsigned probes = 0;                                                       \
    hName##Node n = ((hName##Node*)hash->nodes)[i];                            \
    while(n.exists && !(n.exists == (hashv | 1) && cmpFunc(n.key, key))) {     \
        probes++;                                                              \
        i = (HASH_PROBING) & hash->size;                                       \
        n = ((hName##Node*)hash->nodes)[i];                                    \
    }                                                                          \
    return &(((hName##Node*)hash->nodes)[i]);                                  \
}                                                                              \
                                                                               \
static inline valType hName##Get(Hash *hash, keyType key) {                    \
    hName##Node *n = hName##GetNode(hash, key);                                \
    return n->value;                                                           \
}                                                                              \
                                                                               \
static void hName##SetCallback(void *n, void *data) {                          \
    hName##Node old_node = *((hName##Node *)n);                                \
    if(old_node.exists)                                                        \
        *hName##GetNode((Hash*)data, old_node.key) = old_node;                 \
}                                                                              \
                                                                               \
static inline bool hName##Set(Hash *hash, keyType key, valType value) {        \
    hName##Node *n = hName##GetNode(hash, key);                                \
    n->key   = key;                                                            \
    n->value = value;                                                          \
    if(!n->exists) hash->items++;                                              \
    n->exists  = hashFunc(key) | 1;                                            \
    /* rehash if space is limited */                                           \
    if(hash->items > hash->size * HASH_REHASH_RATIO) {                         \
        dbg("%s: free space is limited, needs rehash", #hName);                \
        return hashRehashInternal(hash, hName##SetCallback);                   \
    }                                                                          \
    return true;                                                               \
}

#define HASH_TEMPLATE(hName, keyType, valType, hashFunc, cmpFunc) HASH_TEMPLATE_FULL(hName, keyType, valType, hashFunc, cmpFunc, NULL, NULL)

#define HASH_TEMPLATE_FREE_KEY(hName, keyType, valType, hashFunc, cmpFunc) HASH_TEMPLATE_FULL(hName, keyType, valType, hashFunc, cmpFunc, free, NULL)

#define HASH_TEMPLATE_FREE_VALUE(hName, keyType, valType, hashFunc, cmpFunc) HASH_TEMPLATE_FULL(hName, keyType, valType, hashFunc, cmpFunc, NULL, free)

#define HASH_TEMPLATE_FREE_KEY_VALUE(hName, keyType, valType, hashFunc, cmpFunc) HASH_TEMPLATE_FULL(hName, keyType, valType, hashFunc, cmpFunc, free, free)


#endif /* HASHTABLE_H_ */
