#include "cache.h"
#include "handle_args.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

char *memory;
static unsigned short address;
static unsigned short data;
static bool hit;
static double hits = 0;
static double misses = 0;
static bool exit_bool;
// Taken from https://stackoverflow.com/questions/664014/what-integer-hash-function-are-good-that-accepts-an-integer-hash-key
// answer by Thomas Mueller
uint hash_int(uint x)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}
static bool valid_hex(unsigned short *number, unsigned char num_digits)
{
    if (getchar() == '0' && getchar() == 'x')
    {
        char x;
        bool x_is_numeric;
        for (unsigned char i = 0; i < num_digits; ++i)
        {
            x = getchar();
            x_is_numeric = x >= '0' && x <= '9';
            if (x_is_numeric || (x >= 'A' && x <= 'F'))
            {
                x -= x_is_numeric ? '0' : '7'; // 'A' => ascii val. 65, 7 => ascii val. 55
                x <<= (num_digits - i - 1) * 4;
                *number += x;
            }
            else
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
static inline void print_op_results(bool hit, double *hits, double *misses)
{
    if (hit)
    {
        ++(*hits);
        printf("Hit\n");
    }
    else
    {
        ++(*misses);
        printf("Missed\n");
    }
    printf("Miss Rate: %lf\n", (*hits > 0. && *misses > 0.) ? *hits / (*hits + *misses) : 0.);
}

int main(int argc, char const *argv[])
{
    if (argc == 1 && strcmp(argv[0], "sim"))
    {
        Cache *cache = 0;
        CacheOptions cache_ops;
    beginning:
        while (true)
        {
            // DEBUG: predefined cache options
            cache_ops.address_width = 8;
            cache_ops.cache_size = 32;
            cache_ops.block_size = 8;
            cache_ops.associativity = 2;
            cache_ops.replacement = 0;
            cache_ops.write_back = 1;
            cache_ops.write_allocate = 1;
            // parse_args(&cache_ops);
            unsigned short address_limit = 0x1 << (cache_ops.address_width);
            memory = (char *)malloc(address_limit);
            cache = build_cache(&cache_ops, hash_int);
            exit_bool = false;
            while (!exit_bool)
            {
                address = data = 0;
                printf("Enter a command\n"
                       " W <addr.> <0xDD> : write data 0xDD to cache at address <addr.>\n"
                       " R <addr.> : read from cache at address <addr.>\n"
                       " P C|M : print the cache (C) or relevant bytes in memory (M)\n"
                       " Q : quit\n"
                       " : ");
                switch (getchar())
                {
                case 'W':
                    if (getchar() == ' ' &&
                        valid_hex(&address, 4) &&
                        address < address_limit &&
                        getchar() == ' ' &&
                        valid_hex(&data, 2) &&
                        flush_istream(stdin))
                    {
                        hit = write(cache, &cache_ops, address, (unsigned char)data);
                        print_op_results(hit, &hits, &misses);
                    }
                    else
                    {
                        goto invalid;
                    }
                    break;
                case 'R':
                    if (getchar() == ' ' &&
                        valid_hex(&address, 4) &&
                        address < address_limit &&
                        flush_istream(stdin))
                    {
                        hit = read(cache, &cache_ops, address);
                        print_op_results(hit, &hits, &misses);
                    }
                    else
                    {
                        goto invalid;
                    }
                case 'P':
                    if (getchar() == ' ')
                    {
                        switch (getchar())
                        {
                        case 'C':

                            break;

                        case 'M':

                            break;

                        default:
                            goto invalid;
                        }
                    }
                    break;
                case 'Q':
                    exit_bool = true;
                    break;
                invalid:
                default:
                    printf("Invalid argument\n");
                    flush_istream(stdin);
                    break;
                }
            }
            free(memory);
            delete_cache(cache, &cache_ops);
            while (true)
            {
                printf("Would you like to exit? (Y or N)");
                switch (getc(stdin))
                {
                case 'Y':
                    if (flush_istream(stdin))
                    {
                        goto exit;
                    }
                    break;
                case 'N':
                    if (flush_istream(stdin))
                    {
                        goto beginning;
                    }
                    break;
                default:
                    break;
                }
                printf("That is an invalid option");
            }
        }
    }
exit:
    return 0;
}
