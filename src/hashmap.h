#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct CellAttrs
{
    uint hash;
    char flag;
} CellAttrs;

typedef struct HashMap
{
    void *map;
    CellAttrs *cell_attrs;
    uint cell_size;
    uint capacity;
    uint size;
    uint (*hash_algo)(const void *elem);
} HashMap;

HashMap *hashmap_malloc(uint capacity, uint data_size);
void hashmap_free(HashMap *map);
bool insert(HashMap *map, void *elem, bool probe);
bool remove(HashMap *map, void *elem);
bool find(HashMap *map, uint hash, uint *index);

// Taken from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
// answer by Thomas Mueller
uint hash_int(uint x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

#endif // HASHMAP_H