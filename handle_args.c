#include "handle_args.h"
#include <stdio.h>

void parse_args(Cache *cache)
{
    validate_arg(&cache->address_width, ADDRESS_WIDTHS, 4,
                 "Address width:\n"
                 " 0 = 4 bits\n"
                 " 1 = 6 bits\n"
                 " 2 = 8 bits\n"
                 " 3 = 10 bits\n"
                 " 4 = 12 bits\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 4");
    validate_arg(&cache->cache_size, CACHE_SIZES, 5,
                 "Cache size:\n"
                 " 0 = 8 bytes\n"
                 " 1 = 16 bytes\n"
                 " 2 = 32 bytes\n"
                 " 3 = 64 bytes\n"
                 " 4 = 128 bytes\n"
                 " 5 = 256 bytes\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 5");
    validate_arg(&cache->block_size, BLOCK_SIZES, 2,
                 "Block size:\n"
                 " 0 = 2 bytes\n"
                 " 1 = 4 bytes\n"
                 " 2 = 8 bytes\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 2");
    validate_arg(&cache->associativity, ASSOCIATIVITIES, 2,
                 "Associativity:\n"
                 " 0 = 1\n"
                 " 1 = 2\n"
                 " 2 = 4\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 2");
    validate_arg(&cache->write_back, NULL, 1,
                 "Write back?\n"
                 " 0 = false\n"
                 " 1 = true\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 1");
    validate_arg(&cache->write_allocate, NULL, 1,
                 "Write allocate?\n"
                 " 0 = false\n"
                 " 1 = true\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 1");
    validate_arg(&cache->replacement, NULL, 2,
                 "Replacement:\n"
                 " 0 = LRU\n"
                 " 1 = Random\n"
                 " 2 = FIFO\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 2");
}

void validate_arg(char *char_member, const char options[], char limit, char *prompt, char *error_msg)
{
    char delimiter;
    while (1)
    {
        printf(prompt);
        *char_member = fgetc(stdin) - '0';
        delimiter = fgetc(stdin);
        if (*char_member > limit && delimiter != '\n' && delimiter != '\r')
        {
            printf(error_msg);
        }
        else
        {
            if (options != NULL)
            {
                *char_member = options[*char_member];
            }
            return;
        }
    }
}
