#include "cache.h"

static inline void hit_handler(Set *set, CacheOptions *cache_ops, uint index)
{
    if (!(cache_ops->replacement & 0x2)) // LRU, LFU
    {
        Node *node = (Node *)(set->lines->map) + index * sizeof(Node *);
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

Cache *build_cache(Cache *cache, CacheOptions *cache_ops)
{
    cache = (Cache *)malloc(sizeof(Cache));
    unsigned char num_sets = cache_ops->cache_size / (cache_ops->block_size * cache_ops->associativity);
    cache->cache = (Set *)malloc(sizeof(Set) * num_sets);
    for (unsigned char i = 0; i < num_sets; ++i)
    {
        cache->cache[i].lines = hashmap_malloc(
            cache_ops->associativity,
            cache_ops->replacement == 3 ? sizeof(Node *) : sizeof(unsigned short));
        if (cache_ops->replacement == 3)
        {
            cache->cache[i].order = (DoublyLinkedList *)malloc(sizeof(DoublyLinkedList));
            cache->cache[i].order->head = cache->cache[i].order->tail = 0;
        }
        else
        {
            cache->cache[i].order = 0;
        }
    }
    cache->data = malloc(sizeof(char) * cache_ops->cache_size);
    cache->offset_mask = 0x1 << (cache_ops->block_size + 1) - 1;
    cache->index_mask = 0x1 << (num_sets + 1) - 1 - cache->offset_mask;
}

void delete_cache(Cache *cache, CacheOptions *cache_ops)
{
    unsigned char num_sets = cache_ops->cache_size / (cache_ops->block_size * cache_ops->associativity);
    for (unsigned char i = 0; i < num_sets; ++i)
    {
        hashmap_free(cache->cache[i].lines);
        if (cache->cache[i].order)
        {
            list_free(cache->cache[i].order);
        }
    }
    free(cache->cache);
    free(cache->data);
    free(cache);
}

bool read(Cache *cache, CacheOptions *cache_ops, short address)
{
    short offset = address & cache->offset_mask;
    address &= ~cache->offset_mask;
    uint set_index = (address & cache->index_mask) >> cache_ops->block_size;
    Set *set = &cache->cache[set_index];
    uint index;
    uint hash = set->lines->hash_algo(address);
    if (hashmap_find(set->lines, hash, &index))
    {
        hit_handler(set, cache_ops, index);
        return true;
    }
    if (set->lines->size >= cache_ops->associativity)
    {
        evict(set, cache_ops, set_index, cache->data);
    }
    insert(set, cache_ops, address, index, hash);
    return false;
}

void insert(Set *set, CacheOptions *cache_ops, short address, uint index, uint hash)
{
    if (!(cache_ops->replacement & 0x1)) // LRU, FIFO
    {
        Node *node = insert_tail(set->order, address);
        hashmap_insert(set->lines, node, index, hash);
    }
    else if (cache_ops->replacement == 1)
    {
        Node *node = insert_head(set->order, address);
    }
    else
    {
        hashmap_insert(set->lines, &address, index, hash);
    }
}

void evict(Set *set, CacheOptions *cache_ops, uint set_index, char *data)
{
    unsigned short address;
    uint index;
    if (cache_ops->replacement != 3) // LRU, LFU, FIFO
    {
        Node *line_to_evict = set->order->head;
        address = line_to_evict->address;
        uint hash = set->lines->hash_algo(address);
        hashmap_find(set->lines, hash, index);
    }
    else // Random
    {
        index = rand() % set->lines->capacity;
    }
    if (set->lines->cell_attrs[index].flag & 0x4) // If dirty bit
    {
        // Write back the whole block to memory
        for (size_t i = 0; i < cache_ops->block_size; ++i)
        {
            memory[address + i] = data[set_index * cache_ops->block_size + i];
        }
    }
    hashmap_remove(set->lines, index);
}

bool write(Cache *cache, CacheOptions *cache_ops, short address, char data)
{
    short offset = address & cache->offset_mask;
    address &= ~cache->offset_mask;
    uint set_index = (address & cache->index_mask) >> cache_ops->block_size;
    Set *set = &cache->cache[set_index];
    uint hash = set->lines->hash_algo(address);
    uint index;
    if (hashmap_find(set->lines, hash, &index))
    {
        hit_handler(set, cache_ops, index);
        cache->data[set_index * cache_ops->block_size + offset] = data;
        if (cache_ops->write_back)
        {
            set->lines->cell_attrs[index].flag |= 0x4; // Toggle dirty bit on
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