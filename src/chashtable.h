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

#define HASH_FOREACH(i, h, n)                                                  \
    unsigned i;                                                                \
    typeof((h)->nodes) n;                                                      \
    for(i = 0; n = &((h)->nodes[i]), i <= (h)->size; i++)                      \
        if(n->exists)

#define HASH_TEMPLATE(hName, keyType, valType, hashFunc, cmpFunc)              \
                                                                               \
typedef struct __attribute__((packed)) hName##Node {                           \
    keyType         key;                                                       \
    valType         value;                                                     \
    bool            exists;                                                    \
} hName##Node;                                                                 \
                                                                               \
typedef struct {                                                               \
    hName##Node   *nodes;                                                      \
    unsigned       size;                                                       \
    unsigned       items;                                                      \
} hName;                                                                       \
                                                                               \
void hName##_destroy(hName *hash) {                                            \
    if(hash) free(hash->nodes);                                                \
    free(hash);                                                                \
}                                                                              \
                                                                               \
hName *hName##_new_size(unsigned size) {                                       \
    hName *hash = (hName*)calloc(1, sizeof(hName));                            \
    if(NULL == hash)                                                           \
        goto error;                                                            \
    hash->size  = size - 1;                                                    \
    hash->nodes = (hName##Node*)calloc(hash->size + 1, sizeof(hName##Node));   \
    dbg("%s: created new of size %u", #hName, size);                           \
    return hash;                                                               \
error:                                                                         \
    dbg("%s: error: cannot allocate memory", #hName);                          \
    hName##_destroy(hash);                                                     \
    return NULL;                                                               \
}                                                                              \
                                                                               \
hName *hName##_new(void) {                                                     \
    return hName##_new_size(HASH_START_SIZE);                                  \
}                                                                              \
                                                                               \
static inline bool hName##_set(hName *hash, keyType key, valType value);       \
                                                                               \
bool hName##_rehash(hName *hash) {                                             \
    if(NULL == hash)                                                           \
        return FALSE;                                                          \
    hName *newhash = hName##_new_size((hash->size + 1) * 2);                   \
    if(NULL == newhash)                                                        \
        return FALSE;                                                          \
    HASH_FOREACH(i, (hName*)hash, n)                                           \
        hName##_set(newhash, n->key, n->value);                                \
    free(hash->nodes);                                                         \
    hash->nodes = newhash->nodes;                                              \
    hash->size = newhash->size;                                                \
    free(newhash);                                                             \
    return TRUE;                                                               \
}                                                                              \
                                                                               \
static inline hName##Node *hName##_getnode(hName *hash, keyType key) {         \
    unsigned i = hashFunc(key) & hash->size;                                   \
    unsigned probes = 0;                                                       \
    while(hash->nodes[i].exists && !cmpFunc(hash->nodes[i].key, key)) {        \
        probes++;                                                              \
        i = (HASH_PROBING) & hash->size;                                       \
    }                                                                          \
    return &hash->nodes[i];                                                    \
}                                                                              \
                                                                               \
static inline valType hName##_get(hName *hash, keyType key) {                  \
    hName##Node *n = hName##_getnode(hash, key);                               \
    return n->value;                                                           \
}                                                                              \
                                                                               \
static inline bool hName##_set(hName *hash, keyType key, valType value) {      \
    hName##Node *n = hName##_getnode(hash, key);                               \
    n->key   = key;                                                            \
    n->value = value;                                                          \
    n->exists  = TRUE;                                                         \
    hash->items++;                                                             \
    /* rehash if space is limited */                                           \
    if(hash->items > hash->size * HASH_REHASH_RATIO) {                         \
        dbg("%s: free space is limited, needs rehash", #hName);                \
        return hName##_rehash(hash);                                           \
    }                                                                          \
    /* rehash if too much collisions */                                        \
    /*if(probes > (hash->size >> 4) && hash->items > (hash->size / 3)) {       \
        dbg("%s: too much collisions, needs rehash", #hName);                  \
        return hName##_rehash(hash);                                           \
    }*/                                                                        \
    return TRUE;                                                               \
}                                                                              \
                                                                               \
static inline unsigned hName##_length(hName *hash) {                           \
    if(NULL == hash)                                                           \
        return 0;                                                              \
    return hash->items;                                                        \
}                                                                              \
                                                                               \
static inline unsigned hName##_size(hName *hash) {                             \
    if(NULL == hash)                                                           \
        return 0;                                                              \
    return hash->size + 1;                                                     \
}


#endif /* HASHTABLE_H_ */
