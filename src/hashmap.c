#include "hashmap.h"
#include <string.h>

HashMap *hashmap_malloc(uint capacity, uint data_size)
{
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->map = malloc(capacity*data_size);
    map->cell_attrs = calloc(capacity, sizeof(CellAttrs));
    map->cell_size = data_size;
    map->capacity = capacity;
    map->size = 0;
    return map;
}

void hashmap_free(HashMap *map)
{
    free(map->map);
    free(map);
}

bool insert(HashMap *map, void *elem, bool probe)
{
    if (map->size >= map->capacity)
    {
        return false;
    }
    
    uint hash = map->hash_algo((const) elem);
    uint index = hash % map->capacity;
    bool first_empty_set = false;
    uint first_empty = index;
    if (probe)
    {
        for (uint i = 0; i < map->capacity; ++i)
        {
            if (map->cell_attrs[index].flag)
            {
                if (map->cell_attrs[index].flag == 1 && !first_empty_set)
                {
                    first_empty = index;
                }
                if (map->cell_attrs[index].hash == hash)
                {
                    if (map->cell_attrs[index].flag == 1)
                    {
                        map->cell_attrs[index].flag = 2;
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                ++index;
                index %= map->capacity;
            }
            else
            {
                goto insert;
            }
        }
        if (first_empty_set)
        {
            index = first_empty;
            goto insert;
        }
        return false;
    }
insert:
    memcpy((char *)map->map + map->cell_size * index, (char *)elem, map->cell_size);
    map->cell_attrs[index].flag = 2;
    map->cell_attrs[index].hash = hash;
    ++map->size;
    return true;
}

bool remove(HashMap *map, void *elem)
{
    if (map->size == 0)
    {
        return false;
    }
    uint hash = map->hash_algo((const) elem);
    uint index = hash % map->capacity;
    for (uint i = 0; i < map->capacity; ++i)
    {
        if (map->cell_attrs[index].flag)
        {
            if (map->cell_attrs[index].hash == hash)
            {
                if (map->cell_attrs[index].flag == 2)
                {
                    map->cell_attrs[index].flag = 1;
                    --map->size;
                    if (map->size == 0)
                    {
                        memset((char *)map->cell_attrs, 0, map->capacity*sizeof(CellAttrs));
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }
            ++index;
            index %= map->capacity;
        }
    }
    return false;
}

// uint find(HashMap *map, uint hash, bool *fail)
// {}