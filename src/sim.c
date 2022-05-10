#include "cache.h"
#include "handle_args.h"
#include <stdbool.h>
#include <stdio.h>

int main(int argc, char const *argv[])
{
    if (argc == 1 && argv[0] == "sim")
    {
        Cache *cache;
    beginning:
        if (!fflush(stdin))
        {
            return 1;
        }
        CacheOptions cache_ops;
        
        while (true)
        {
            parse_args(&cache_ops);
            memory = (char *)malloc(0x1 << cache_ops.address_width);
            cache = build_cache(cache, &cache_ops);
            while (true)
            {
                
            }
            free(memory);
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
