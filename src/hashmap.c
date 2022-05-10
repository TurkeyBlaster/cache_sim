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
    free(map->cell_attrs);
    free(map);
    map = 0;
}

bool hashmap_insert(HashMap *map, void *elem, uint index, uint hash)
{
    if (map->size >= map->capacity)
    {
        return false;
    }
    if (!index)
    {
        uint hash = map->hash_algo((const) elem);
        uint index = hash % map->capacity;
        bool first_empty_set = false;
        uint first_empty = index;
        hashmap_find(map, hash, &index);
        return false;
    }
    memcpy((char *)map->map + map->cell_size * index, (char *)elem, map->cell_size);
    map->cell_attrs[index].flag = 2;
    map->cell_attrs[index].hash = hash;
    ++map->size;
    return true;
}

// bool hashmap_remove_elem(HashMap *map, void *elem)
// {
//     if (map->size == 0)
//     {
//         return false;
//     }
//     return hashmap_remove_hash(map, map->hash_algo((const) elem));
// }

// bool hashmap_remove_hash(HashMap *map, uint hash)
// {
//     uint index = hash % map->capacity;
//     for (uint i = 0; i < map->capacity; ++i)
//     {
//         if (map->cell_attrs[index].flag)
//         {
//             if (map->cell_attrs[index].hash == hash)
//             {
//                 if (map->cell_attrs[index].flag == 2)
//                 {
//                     map->cell_attrs[index].flag = 1;
//                     --map->size;
//                     if (map->size == 0)
//                     {
//                         memset((char *)map->cell_attrs, 0, map->capacity*sizeof(CellAttrs));
//                     }
//                     return true;
//                 }
//                 else
//                 {
//                     return false;
//                 }
//             }
//             ++index;
//             index %= map->capacity;
//         }
//     }
//     return false;
// }

bool hashmap_remove(HashMap *map, uint index)
{
    map->cell_attrs[index].flag = 1;
}

bool hashmap_find(HashMap *map, uint hash, uint *index)
{
    uint *index = hash % map->capacity;
    bool first_empty_set = false;
    uint first_empty = *index;
    for (uint i = 0; i < map->capacity; ++i)
    {
        if (map->cell_attrs[*index].flag)
        {
            if (map->cell_attrs[*index].flag == 1 && !first_empty_set)
            {
                first_empty_set = true;
                first_empty = index;
            }
            if (map->cell_attrs[*index].hash == hash)
            {
                if (map->cell_attrs[*index].flag == 1) // TODO: MIGHT need to modify to match updated flags
                {
                    return false;
                }
                else
                {
                    return true;
                }
            }
            ++(*index);
            *index %= map->capacity;
        }
        else
        {
            return false;
        }
    }
    if (first_empty_set)
    {
        *index = first_empty;
    }
    return false;
}