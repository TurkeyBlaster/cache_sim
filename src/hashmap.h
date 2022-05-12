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
    uint (*hash_algo)(uint elem);
} HashMap;

HashMap *hashmap_malloc(uint capacity, uint data_size, uint (*hash_algo)(uint elem));
void hashmap_free(HashMap *map);
bool hashmap_insert(HashMap *map, unsigned long elem, uint index, uint hash);
void hashmap_remove(HashMap *map, uint index);
bool hashmap_find(HashMap *map, uint hash, uint *index);

#endif // HASHMAP_H