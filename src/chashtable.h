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

/* #define HASH_DEBUG */

#ifdef HASH_DEBUGV
	#define dprintf_chash(...) printf(__VA_ARGS__)
	#define dprintf_chashv(...) printf(__VA_ARGS__)
#elif defined HASH_DEBUG
	#define dprintf_chash(...) printf(__VA_ARGS__)
	#define dprintf_chashv(...)
#else
	#define dprintf_chash(...)
	#define dprintf_chashv(...)
#endif

#ifndef HASH_START_SIZE
#define HASH_START_SIZE 128
#endif

#ifndef HASH_REHASH_RATIO
#define HASH_REHASH_RATIO 2 / 3
#endif

#ifndef HASH_PROBING
#define HASH_PROBING (unsigned)(index + (probes))
#endif

#define HASH_EACH(hash, CODE) {                                                \
    unsigned __i;                                                              \
    typeof(hash->nodes) hashnode;                                              \
    for(__i = 0; __i <= hash->size; __i++)                                     \
    {                                                                          \
        hashnode = &(hash->nodes[__i]);                                        \
        if(hashnode->exists) { CODE }                                          \
    }                                                                          \
}

#define _HASH_NODE hash->nodes[index]

#define _HASH_INDEX(hashFunc, condition)                                       \
unsigned index = hashFunc(key) & hash->size;                                   \
unsigned probes = 0;                                                           \
while (condition) {                                                            \
    probes++;                                                                  \
    index = (HASH_PROBING) & hash->size;                                       \
}

#define HASH_TEMPLATE(hashName, keyType, valType, hashFunc, cmpFunc)           \
                                                                               \
typedef struct hashName##Node {                                                \
    keyType         key;                                                       \
    valType         value;                                                     \
    bool            exists;                                                    \
} hashName##Node;                                                              \
                                                                               \
typedef struct {                                                               \
    unsigned          size;                                                    \
    unsigned          items;                                                   \
    hashName##Node   *nodes;                                                   \
} hashName;                                                                    \
                                                                               \
void hashName##_destroy(hashName *hash) {                                      \
    if(hash) free(hash->nodes);                                                \
    free(hash);                                                                \
}                                                                              \
                                                                               \
hashName *hashName##_new_size(unsigned size) {                                 \
    hashName *hash = calloc(1, sizeof(hashName));                              \
    if(NULL == hash)                                                           \
        goto error;                                                            \
    hash->size     = size - 1;                                                 \
    hash->nodes    = calloc(hash->size + 1, sizeof(hashName##Node));           \
    dprintf_chash("%s: created new of size %u\n", #hashName, size);            \
    return hash;                                                               \
error:                                                                         \
    dprintf_chash("%s: error: cannot allocate memory\n", #hashName);           \
    hashName##_destroy(hash);                                                  \
    return NULL;                                                               \
}                                                                              \
                                                                               \
hashName *hashName##_new(void) {                                               \
    return hashName##_new_size(HASH_START_SIZE);                               \
}                                                                              \
                                                                               \
static inline bool hashName##_set(hashName *hash, keyType key, valType value); \
                                                                               \
bool hashName##_rehash(hashName *hash) {                                       \
    if(NULL == hash)                                                           \
        return FALSE;                                                          \
    hashName *newhash = hashName##_new_size((hash->size + 1) * 2);             \
    if(NULL == newhash)                                                        \
        return FALSE;                                                          \
    HASH_EACH(hash, hashName##_set(newhash, hashnode->key, hashnode->value);)  \
    free(hash->nodes);                                                         \
    hash->nodes = newhash->nodes;                                              \
    hash->size = newhash->size;                                                \
    free(newhash);                                                             \
    return TRUE;                                                               \
}                                                                              \
                                                                               \
static inline valType hashName##_get(hashName *hash, keyType key) {            \
    _HASH_INDEX(hashFunc, _HASH_NODE.exists && !cmpFunc(_HASH_NODE.key, key))  \
    return _HASH_NODE.value;                                                   \
}                                                                              \
                                                                               \
static inline valType *hashName##_getptr(hashName *hash, keyType key) {        \
    _HASH_INDEX(hashFunc, _HASH_NODE.exists && !cmpFunc(_HASH_NODE.key, key))  \
    return _HASH_NODE.exists ? &(_HASH_NODE.value) : NULL;                     \
}                                                                              \
                                                                               \
static inline bool hashName##_set(hashName *hash, keyType key, valType value) {\
    _HASH_INDEX(hashFunc, _HASH_NODE.exists)                                   \
    _HASH_NODE.key   = key;                                                    \
    dprintf_chashv("%s: insert key using %4d probes\n",                        \
                  #hashName, probes);                                          \
    _HASH_NODE.value = value;                                                  \
    _HASH_NODE.exists  = TRUE;                                                 \
    hash->items++;                                                             \
    /* rehash if space is limited */                                           \
    if(hash->items > hash->size * HASH_REHASH_RATIO) {                         \
        dprintf_chash("%s: free space is limited, needs rehash\n", #hashName); \
        return hashName##_rehash(hash);                                        \
    }                                                                          \
    /* rehash if too much collisions */                                        \
    if(probes > (hash->size >> 4) && hash->items > (hash->size / 3)) {         \
        dprintf_chash("%s: too much collisions, needs rehash\n", #hashName);   \
        return hashName##_rehash(hash);                                        \
    }                                                                          \
    return TRUE;                                                               \
}                                                                              \
                                                                               \
static inline unsigned hashName##_length(hashName *hash) {                     \
    if(NULL == hash)                                                           \
        return 0;                                                              \
    return hash->items;                                                        \
}                                                                              \
                                                                               \
static inline unsigned hashName##_size(hashName *hash) {                       \
    if(NULL == hash)                                                           \
        return 0;                                                              \
    return hash->size + 1;                                                     \
}

#endif /* HASHTABLE_H_ */
