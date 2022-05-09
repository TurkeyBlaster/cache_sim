#ifndef CACHE_STRUCT_H
#define CACHE_STRUCT_H

#include "linkedlist.h"
#include <libs/hashmap.c/hashmap.h>

typedef struct hashmap HashMap;

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

typedef struct Line
{
    unsigned char valid;
    unsigned char dirty;
    Node* node;
} Line;

typedef struct Set
{
    HashMap* lines;
    DoublyLinkedList* order;
} Set;

int read(Set *cache, CacheOptions *cache_ops, short address, HashMap* hm, DoublyLinkedList* ll);
int write(Set *cache, CacheOptions *cache_ops, short address, HashMap* hm, DoublyLinkedList* ll);
int evict(Set *cache, CacheOptions *cache_ops, short address, HashMap* hm, DoublyLinkedList* ll);
int flush(Set *cache, CacheOptions *cache_ops, short address, HashMap* hm, DoublyLinkedList* ll);

#endif // CACHE_STRUCT_H
