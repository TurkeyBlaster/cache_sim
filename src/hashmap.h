#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdlib.h>

typedef struct HashMap
{
    void *map;
    size_t cell_size;
    size_t capacity;
    size_t size;
} HashMap;

HashMap *hashmap_malloc(size_t capacity, size_t data_size);
void hashmap_free(HashMap *map);
int insert(HashMap *map, void *elem);
int remove(HashMap *map, void *elem);
size_t find(HashMap *map, void *elem);
int hash()

#endif // HASHMAP_H