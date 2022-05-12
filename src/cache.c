#include "cache.h"
#include "linkedlist.h"
#include "hashmap.h"
#include <string.h>
#include <stdio.h>

const unsigned char ADDRESS_WIDTHS[] = {
    4,
    6,
    8,
    10,
    12};
const unsigned char CACHE_SIZES[] = {
    8,
    16,
    32,
    64,
    128};
const unsigned char BLOCK_SIZES[] = {
    2,
    4,
    8};
const unsigned char ASSOCIATIVITIES[] = {
    1,
    2,
    4};

static inline void hit_handler(Set *set, CacheOptions *cache_ops, uint index)
{
    if (!(cache_ops->replacement & 0x2)) // LRU, LFU
    {
        Node *node = (Node *)(((long *)set->lines->map)[index]);
        if (cache_ops->replacement) // LFU
        {
            ++(node->count);
            increase_order(set->order, node);
        }
        else // LRU
        {
            move_to_end(set->order, node);
        }
    }
}

Cache *build_cache(CacheOptions *cache_ops, uint (*hash_algo)(uint elem))
{
    Cache *cache = (Cache *)malloc(sizeof(Cache));
    cache->num_sets = cache_ops->cache_size / (cache_ops->block_size * cache_ops->associativity);
    cache->sets = (Set *)malloc(sizeof(Set) * cache->num_sets);
    for (unsigned char i = 0; i < cache->num_sets; ++i)
    {
        cache->sets[i].data = (char *)malloc(
            sizeof(char) * cache_ops->block_size * cache_ops->associativity);
        cache->sets[i].lines = hashmap_malloc(
            cache_ops->associativity,
            cache_ops->replacement != 3 ? sizeof(Node *) : sizeof(unsigned short),
            hash_algo);
        if (cache_ops->replacement != 3)
        {
            cache->sets[i].order = list_malloc();
        }
        else
        {
            cache->sets[i].order = 0;
        }
    }
    unsigned char _size = cache_ops->block_size;
    cache->offset_mask = 0x1;
    cache->offset_size = 0;
    while (_size >>= 1)
    {
        cache->offset_mask <<= 1;
        ++cache->offset_size;
    }
    --cache->offset_mask;
    _size = cache->num_sets;
    cache->index_mask = 0x1;
    cache->index_size = 0;
    while (_size >>= 1)
    {
        cache->index_mask <<= 1;
        ++cache->index_size;
    }
    --cache->index_mask;
    return cache;
}

void delete_cache(Cache *cache, CacheOptions *cache_ops)
{
    unsigned char num_sets =
        cache_ops->cache_size / (cache_ops->block_size * cache_ops->associativity);
    for (unsigned char i = 0; i < num_sets; ++i)
    {
        free(cache->sets[i].data);
        hashmap_free(cache->sets[i].lines);
        if (cache->sets[i].order)
        {
            list_free(cache->sets[i].order);
        }
    }
    free(cache->sets);
    free(cache);
}

bool read(Cache *cache, CacheOptions *cache_ops, unsigned short address)
{
    unsigned short masked_address = address & (~cache->offset_mask);
    uint set_index = (masked_address >> cache->offset_size) & cache->index_mask;
    Set *set = &cache->sets[set_index];
    uint index;
    uint hash = set->lines->hash_algo((uint)masked_address);
    if (hashmap_find(set->lines, hash, &index))
    {
        hit_handler(set, cache_ops, index);
        return true;
    }
    insert(set, cache_ops, address, index, hash);
    return false;
}

void insert(Set *set, CacheOptions *cache_ops, unsigned short address, uint index, uint hash)
{
    if (set->lines->size >= cache_ops->associativity)
    {
        evict(set, cache_ops);
    }
    if (!(cache_ops->replacement & 0x1)) // LRU, FIFO
    {
        Node *node = insert_tail(set->order, address);
        hashmap_insert(set->lines, (unsigned long)node, index, hash);
    }
    else if (cache_ops->replacement == 1) // LFU
    {
        Node *node = insert_head(set->order, address);
        hashmap_insert(set->lines, (unsigned long)node, index, hash);
    }
    else
    {
        hashmap_insert(set->lines, (unsigned long)address, index, hash);
    }
    memcpy(&set->data[cache_ops->block_size * index], &memory[address], cache_ops->block_size);
}

void evict(Set *set, CacheOptions *cache_ops)
{
    unsigned short address;
    uint index;
    if (cache_ops->replacement != 3) // LRU, LFU, FIFO
    {
        Node *line_to_evict = set->order->head;
        address = line_to_evict->address;
        uint hash = set->lines->hash_algo((uint)address);
        hashmap_find(set->lines, hash, &index);
    }
    else // Random
    {
        index = rand() % set->lines->capacity;
    }
    if (set->lines->cell_attrs[index].flag & 0x4) // If dirty bit
    {
        // Write back the whole block to memory
        memcpy(&memory[address], &set->data[cache_ops->block_size * index], cache_ops->block_size);
    }
    hashmap_remove(set->lines, index);
}

bool write(Cache *cache, CacheOptions *cache_ops, unsigned short address, char data)
{
    unsigned short offset = address & cache->offset_mask;
    unsigned short masked_address = address & (~cache->offset_mask);
    uint set_index = (masked_address >> cache->offset_size) & cache->index_mask;
    Set *set = &cache->sets[set_index];
    uint hash = set->lines->hash_algo((uint)masked_address);
    uint index;
    if (hashmap_find(set->lines, hash, &index))
    {
        hit_handler(set, cache_ops, index);
        set->data[offset] = data;
        if (cache_ops->write_back)
        {
            set->lines->cell_attrs[index].flag |= 0x4; // Toggle dirty bit on
        }
        else
        {
            memory[address] = data;
        }
        return true;
    }
    else
    {
        memory[address] = data;
        if (cache_ops->write_allocate)
        {
            insert(set, cache_ops, masked_address, index, hash);
        }
        return false;
    }
}

void print_cache(Cache *cache, CacheOptions *cache_ops)
{
    Set *set;
    HashMap *line;
    bool valid;
    for (unsigned int i = 0; i < (unsigned int)cache->num_sets; ++i)
    {
        set = &cache->sets[i];
        printf("Set %u:\n", i);
        printf("I\tV\tD\tT\tData\n");
        for (unsigned int j = 0; j < (unsigned int)cache_ops->associativity; ++j)
        {
            line = &set->lines[j];
            valid = line->cell_attrs->flag & 0x2;
            printf("%d\t%d\t%d\t", j, valid);
            if (valid)
            {
                unsigned char tag;
                if (cache_ops->replacement != 3)
                {
                    tag = ((Node **)set->lines->map)[j]->address;
                }
                else
                {
                    tag = ((short *)set->lines->map)[j];
                }
                tag >>= (cache->offset_size + cache->index_size);
                printf("%d\t%d\t", line->cell_attrs->flag >> 2, tag);
                for (unsigned k = 0; k < cache_ops->block_size; ++k)
                {
                   printf("%#02X ", set->data[j * cache_ops->block_size + k]);
                }
            }
            printf("\n");
        }
    }
}

void print_memory(Cache *cache, CacheOptions *cache_ops)
{
    Set *set;
    HashMap *line;
    bool valid;
    for (unsigned int i = 0; i < (unsigned int)cache->num_sets; ++i)
    {
        set = &cache->sets[i];
        printf("Set %u:\n", i);
        for (unsigned int j = 0; j < (unsigned int)cache_ops->associativity; ++j)
        {
            line = &set->lines[j];
            valid = line->cell_attrs->flag & 0x2;
            if (valid)
            {
                unsigned char address;
                if (cache_ops->replacement != 3)
                {
                    address = ((Node **)set->lines->map)[j]->address;
                }
                else
                {
                    address = ((short *)set->lines->map)[j];
                }
                printf("%#04X:\t", address);
                for (unsigned k = 0; k < cache_ops->block_size; ++k)
                {
                   printf("%#02X ", memory[address + k]);
                }
            }
            printf("\n");
        }
    }
}