#include "cache.h"

bool read(Cache *cache, CacheOptions *cache_ops, short address)
{
    short offset = address & cache->offset_mask;
    address &= ~cache->offset_mask;
    uint index = (address & cache->index_mask) >> cache->offset_bits;
    Set *set = &cache->cache[index];
    uint hash = set->lines->hash_algo(address);
    if (hashmap_find(set->lines, hash, &index))
    {
        return true;
    }
    if (set->lines->size >= cache_ops->associativity)
    {
        evict(set, cache_ops);
    }
    insert(set, cache_ops, address, index, hash);
    return false;
}

void insert(Set *set, CacheOptions *cache_ops, short address, uint index, uint hash)
{
    if (!cache_ops->replacement) // LRU
    {
        Node *node = insert_tail(set->order, address);
        hashmap_insert(set->lines, node, index, hash);
    }
    else
    {
        hashmap_insert(set->lines, &address, index, hash);
    }
}

void evict(Set *set, CacheOptions *cache_ops)
{
    short address;
    switch (cache_ops->replacement)
    {
    case 0:
        Node *line_to_evict = set->order->head;
        uint index;
        uint hash = set->lines->hash_algo(line_to_evict->address);
        hashmap_find(set->lines, hash, index);
        // hashmap_remove(set->lines, );
        break;
    
    case 1:

    case 2:
    
    default:
        break;
    }
}

bool write(Cache *cache, CacheOptions *cache_ops, short address, char data)
{
    short offset = address & cache->offset_mask;
    address &= ~cache->offset_mask;
    uint set_index = (address & cache->index_mask) >> cache->offset_bits;
    Set *set = &cache->cache[set_index];
    uint hash = set->lines->hash_algo(address);
    uint index;
    if(hashmap_find(set->lines, hash, &index))
    {
        cache->data[set_index * cache_ops->block_size + offset] = data;
        if (cache_ops->write_back)
        {
            set->lines->cell_attrs[index].flag |= 0x2; // Toggle dirty bit on
        }
        else
        {
            memory[address] = data;
        }
    }
    else
    {
        memory[address] = data;
        if (cache_ops->write_allocate)
        {
            insert(set, cache_ops, address, index, hash);
        }
    }
}