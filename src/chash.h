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

#define HASH_START_SIZE 128

#define HASH_PROBING (index + (probes))

#define HASH_EACH(hash, CODE) {                                                \
    int __i;                                                                   \
    typeof(hash->nodes) hashnode;                                              \
    for(__i = 0; __i <= hash->size; __i++) {                                   \
        hashnode = &(hash->nodes[__i]);                                        \
        if(hashnode->busy) { CODE }                                            \
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
    bool            busy;                                                      \
} hashName##Node;                                                              \
                                                                               \
typedef struct {                                                               \
    unsigned        size;                                                      \
    unsigned        items;                                                     \
    hashName##Node *nodes;                                                     \
} hashName;                                                                    \
                                                                               \
void hashName##_destroy(hashName *hash) {                                      \
    if(hash->nodes)    free(hash->nodes);                                      \
    if(hash)           free(hash);                                             \
}                                                                              \
                                                                               \
hashName *hashName##_new_size(unsigned size) {                                 \
    hashName *hash = calloc(1, sizeof(hashName));                              \
    if(NULL == hash) goto error;                                               \
    hash->size     = size - 1;                                                 \
    hash->nodes    = calloc(hash->size + 1, sizeof(hashName##Node));           \
    return hash;                                                               \
error:                                                                         \
    hashName##_destroy(hash);                                                  \
    return NULL;                                                               \
}                                                                              \
                                                                               \
hashName *hashName##_new() {                                                   \
    return hashName##_new_size(HASH_START_SIZE);                               \
}                                                                              \
                                                                               \
static inline bool hashName##_set(hashName *hash, keyType key, valType value); \
                                                                               \
bool hashName##_rehash(hashName *hash) {                                       \
    hashName *newhash = hashName##_new_size((hash->size + 1) * 2);             \
    if(NULL == newhash) return FALSE;                                          \
    HASH_EACH(hash, hashName##_set(newhash,hashnode->key,hashnode->value);)    \
    free(hash->nodes);                                                         \
    hash->nodes = newhash->nodes;                                              \
    hash->size = newhash->size;                                                \
    free(newhash);                                                             \
    return TRUE;                                                               \
}                                                                              \
                                                                               \
static inline valType hashName##_get(hashName *hash, keyType key) {            \
    _HASH_INDEX(hashFunc, _HASH_NODE.busy && !cmpFunc(_HASH_NODE.key,key))     \
    return _HASH_NODE.value;                                                   \
}                                                                              \
                                                                               \
static inline bool hashName##_set(hashName *hash, keyType key, valType value) {\
    _HASH_INDEX(hashFunc, _HASH_NODE.busy)                                     \
    _HASH_NODE.key   = key;                                                    \
    _HASH_NODE.value = value;                                                  \
    _HASH_NODE.busy  = TRUE;                                                   \
    hash->items++;                                                             \
    /* rehash if space is limited */                                           \
    if(hash->items > hash->size * 2 / 3)                                       \
        return hashName##_rehash(hash);                                        \
    /* rehash if too much collisions */                                        \
    if(probes > (hash->size >> 4) && hash->items > (hash->size / 3))           \
        return hashName##_rehash(hash);                                        \
    return TRUE;                                                               \
}                                                                              \
                                                                               \
static inline unsigned hashName##_length(hashName *hash) {                     \
    return hash->items;                                                        \
}                                                                              \
                                                                               \
static inline unsigned hashName##_size(hashName *hash) {                       \
    return hash->size + 1;                                                     \
}
