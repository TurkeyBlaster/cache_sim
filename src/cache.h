#ifndef CACHE_STRUCT_H
#define CACHE_STRUCT_H

#include "linkedlist.h"
#include "hashmap.h"

extern char *memory; // To be declared in main

extern const unsigned char ADDRESS_WIDTHS[];
extern const unsigned char CACHE_SIZES[];
extern const unsigned char BLOCK_SIZES[];
extern const unsigned char ASSOCIATIVITIES[];

typedef struct CacheOptions
{
    unsigned char address_width;
    unsigned char cache_size;
    unsigned char block_size;
    unsigned char associativity;
    unsigned char write_back;
    unsigned char write_allocate;
    unsigned char replacement;
} CacheOptions;

// typedef struct Line
// {
//     void *node;
// } Line;

typedef struct Set
{
    HashMap *lines;
    DoublyLinkedList *order;
} Set;

typedef struct Cache
{
    Set *cache;
    char *data;
    short offset_mask;
    short index_mask;
} Cache;

void build_cache(Cache *cache, CacheOptions *cache_ops, uint (*hash_algo)(uint elem));
void delete_cache(Cache *cache, CacheOptions *cache_ops);
bool read(Cache *cache, CacheOptions *cache_ops, unsigned short address);
bool write(Cache *cache, CacheOptions *cache_ops, unsigned short address, char data);
void insert(Set *set, CacheOptions *cache_ops, unsigned short address, uint index, uint hash);
void evict(Set *set, CacheOptions *cache_ops, uint set_index, char *data);
int flush(Cache *cache, CacheOptions *cache_ops, unsigned short address);

#endif // CACHE_STRUCT_H
