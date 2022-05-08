#ifndef CACHE_STRUCT_H
#define CACHE_STRUCT_H

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

typedef struct Cache
{
    unsigned char address_width;
    unsigned char cache_size;
    unsigned char block_size;
    unsigned char associativity;
    unsigned char write_back;
    unsigned char write_allocate;
    unsigned char replacement;
} Cache;

#endif // CACHE_STRUCT_H
