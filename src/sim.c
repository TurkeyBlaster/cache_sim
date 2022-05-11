#include "cache.h"
#include "handle_args.h"
#include <stdbool.h>
#include <stdio.h>

static unsigned short address;
static unsigned char data;
static bool hit;
static bool exit;
static bool valid_hex(short *number, unsigned char num_digits)
{
    bool success = false;
    if (getchar() == ' ' && getchar() == '0' && getchar() == 'x')
    {
        char x;
        bool x_is_numeric;
        for (unsigned char i = 0; i < num_digits; ++i)
        {
            x = getchar();
            x_is_numeric = x >= '0' && x <= '9';
            if (x_is_numeric || x >= 'A' && x <= 'F')
            {
                *number += (x - ('0' ? x_is_numeric : 'A')) << ((num_digits - i) * 4);
            }
            else
            {
                goto fail;
            }
        }
        return true;
    }
fail:
    return false;
}

int main(int argc, char const *argv[])
{
    if (argc == 1 && argv[0] == "sim")
    {
        Cache *cache = 0;
    beginning:
        if (!fflush(stdin))
        {
            return 1;
        }
        CacheOptions cache_ops;
        while (true)
        {
            parse_args(&cache_ops);
            unsigned short address_max = 0x1 << (cache_ops.address_width + 1);
            memory = (char *)malloc(address_max + 1);
            cache = build_cache(cache, &cache_ops);
            exit = false;
            while (exit)
            {
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
                        address < address_max &&
                        getchar() == ' ' &&
                        valid_hex(&data, 2))
                    {
                        char delim = getchar();
                        if (delim != '\r' && delim != '\n')
                        {
                            goto fail;
                        }
                        hit = write(cache, &cache_ops, address, data);
                    }
                    else
                    {
                        goto fail;
                    }
                    break;
                case 'R':
                    if (getchar() == ' ' &&
                        valid_hex(&address, 4) &&
                        address < address_max)
                    {
                        char delim = getchar();
                        if (delim != '\r' && delim != '\n')
                        {
                            goto fail;
                        }
                        hit = read(cache, &cache_ops, address);
                    }
                    else
                    {
                        goto fail;
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
                            goto fail;
                        }
                    }
                case 'Q':
                    exit = true;
                    break;
                fail:
                default:
                    printf("\nInvalid argument");
                    break;
                }
                fflush(stdin);
            }
            free(memory);
            delete_cache(cache, &cache_ops);
        repeat:
            while (true)
            {
                printf("Would you like to exit? (Y or N)");
                switch (getc(stdin))
                {
                case 'Y':
                    goto exit;
                    break;
                case 'N':
                    goto beginning;
                    break;
                default:
                    printf("That is an invalid option");
                    break;
                }
            }
        }
    }
exit:
    return 0;
}
