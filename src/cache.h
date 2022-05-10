#ifndef CACHE_STRUCT_H
#define CACHE_STRUCT_H

#include "linkedlist.h"
#include "hashmap.h"

extern char *memory; // To be declared in main

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
    128,
    256};
const unsigned char BLOCK_SIZES[] = {
    2,
    4,
    8};
const unsigned char ASSOCIATIVITIES[] = {
    1,
    2,
    4};

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
    short offset_bits;
} Cache;

bool read(Cache *cache, CacheOptions *cache_ops, short address);
bool write(Cache *cache, CacheOptions *cache_ops, short address, char data);
void insert(Set *set, CacheOptions *cache_ops, short address, uint index, uint hash);
void evict(Set *set, CacheOptions *cache_ops);
int flush(Cache *cache, CacheOptions *cache_ops, short address);

#endif // CACHE_STRUCT_H
