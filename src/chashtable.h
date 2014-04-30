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

#include <stdbool.h>
#include "cdebug.h"

#ifndef HASH_START_SIZE
#define HASH_START_SIZE 128
#endif

#ifndef HASH_REHASH_RATIO
#define HASH_REHASH_RATIO 2 / 3
#endif

#ifndef HASH_PROBING
#define HASH_PROBING (unsigned)(i + (probes))
#endif

typedef struct Hash {
    void          *nodes;                     /* Nodes array */
    void         (*node_free)(void*, void*);  /* Function to free each node */
    unsigned       size;                      /* Hash size (nodes array size) */
    unsigned       items;                     /* Number of items that hash stores */
    unsigned       node_size;                 /* Node size */
} Hash;

static void hashForeach(Hash *hash, void (*func)(void*, void*), void *data)
{
    unsigned i;
    for(i = 0; i <= hash->size; i++)
        func(hash->nodes + i * hash->node_size, data);
}

static void hashDestroy(Hash *hash)
{
    if(NULL == hash) return;
    if(hash->node_free)
        hashForeach(hash, hash->node_free, NULL);
    free(hash->nodes);
    free(hash);
}

static Hash *hashNewSize(unsigned size, unsigned node_size)
{
    Hash *hash = (Hash*)calloc(1, sizeof(Hash));
    if(NULL == hash)
        goto error;
    hash->size  = size - 1;
    hash->node_size = node_size;
    hash->nodes = calloc(size, node_size);
    if(NULL == hash->nodes)
        goto error;
    dbg("Hash: created new of size %u, node size: %u", size, node_size);
    return hash;
error:
    dbg("Hash: error: cannot allocate memory");
    hashDestroy(hash);
    return NULL;
}

static Hash *hashNew(unsigned node_size)
{
    return hashNewSize(HASH_START_SIZE, node_size);
}

static inline unsigned hashLength(Hash *hash) {
    if(unlikely(NULL == hash))
        return 0;
    return hash->items;
}

static inline unsigned hashSize(Hash *hash) {
    if(unlikely(NULL == hash))
        return 0;
    return hash->size + 1;
}

#define HASH_TEMPLATE(hName, keyType, valType, hashFunc, cmpFunc)              \
                                                                               \
typedef struct __attribute__((packed)) hName##Node {                           \
    keyType         key;                                                       \
    valType         value;                                                     \
    bool            exists;                                                    \
} hName##Node;                                                                 \
                                                                               \
static Hash *hName##New(void) {                                                \
    return hashNew(sizeof(hName##Node));                                       \
}                                                                              \
                                                                               \
static Hash *hName##NewSize(unsigned size) {                                   \
    return hashNewSize(size, sizeof(hName##Node));                             \
}                                                                              \
                                                                               \
static inline bool hName##Set(Hash *hash, keyType key, valType value);         \
                                                                               \
static bool hName##Rehash(Hash *hash) {                                        \
    unsigned i;                                                                \
    if(NULL == hash)                                                           \
        return FALSE;                                                          \
    Hash *newhash = hashNewSize((hash->size + 1) * 2, sizeof(hName##Node));    \
    if(NULL == newhash)                                                        \
        return FALSE;                                                          \
    for(i = 0; i <= hash->size; i++) {                                         \
        hName##Node n = ((hName##Node*)hash->nodes)[i];                        \
        if(n.exists)                                                           \
            hName##Set(newhash, n.key, n.value);                               \
    }                                                                          \
    free(hash->nodes);                                                         \
    hash->nodes = newhash->nodes;                                              \
    hash->size = newhash->size;                                                \
    free(newhash);                                                             \
    return TRUE;                                                               \
}                                                                              \
                                                                               \
static inline hName##Node *hName##GetNode(Hash *hash, keyType key) {           \
    unsigned i = hashFunc(key) & hash->size;                                   \
    unsigned probes = 0;                                                       \
    while(((hName##Node*)hash->nodes)[i].exists &&                             \
           !cmpFunc(((hName##Node*)hash->nodes)[i].key, key)) {                \
        probes++;                                                              \
        i = (HASH_PROBING) & hash->size;                                       \
    }                                                                          \
    return &(((hName##Node*)hash->nodes)[i]);                                  \
}                                                                              \
                                                                               \
static inline valType hName##Get(Hash *hash, keyType key) {                    \
    hName##Node *n = hName##GetNode(hash, key);                                \
    return n->value;                                                           \
}                                                                              \
                                                                               \
static inline bool hName##Set(Hash *hash, keyType key, valType value) {        \
    hName##Node *n = hName##GetNode(hash, key);                                \
    n->key   = key;                                                            \
    n->value = value;                                                          \
    n->exists  = TRUE;                                                         \
    hash->items++;                                                             \
    /* rehash if space is limited */                                           \
    if(hash->items > hash->size * HASH_REHASH_RATIO) {                         \
        dbg("%s: free space is limited, needs rehash", #hName);                \
        return hName##Rehash(hash);                                            \
    }                                                                          \
    return TRUE;                                                               \
}


#endif /* HASHTABLE_H_ */
