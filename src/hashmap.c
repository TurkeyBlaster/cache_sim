#include "hashmap.h"

HashMap *hashmap_malloc(size_t capacity, size_t data_size)
{
    HashMap *map = (HashMap *)malloc(sizeof(char *) + 3 * sizeof(size_t));
    map->map = malloc(capacity*data_size);
    map->cell_size = 0;
    map->capacity = capacity;
    map->size = 0;
    return map;
}

void hashmap_free(HashMap *map)
{
    free(map->map);
    free(map);
}

int insert(HashMap *map, void *elem)
{
    
}