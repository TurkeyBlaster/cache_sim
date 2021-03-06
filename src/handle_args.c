#include "handle_args.h"
#include <stdio.h>
#include <stdbool.h>


bool flush_istream(FILE *is)
{
    char c;
    bool end_was_eol = true;
    while((c = fgetc(is)) != '\n' && c != '\r' && c != EOF)
    {
        end_was_eol = false;
    }
    return end_was_eol;
}

void parse_args(CacheOptions *cache_options)
{
    validate_arg(&cache_options->address_width, ADDRESS_WIDTHS, 4,
                 "Address width:\n"
                 " 0 = 4 bits\n"
                 " 1 = 6 bits\n"
                 " 2 = 8 bits\n"
                 " 3 = 12 bits\n"
                 " 4 = 16 bits\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 4");
    validate_arg(&cache_options->cache_size, CACHE_SIZES, 5,
                 "Cache size:\n"
                 " 0 = 8 bytes\n"
                 " 1 = 16 bytes\n"
                 " 2 = 32 bytes\n"
                 " 3 = 64 bytes\n"
                 " 4 = 128 bytes\n"
                 " 5 = 256 bytes\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 5");
    validate_arg(&cache_options->block_size, BLOCK_SIZES, 2,
                 "Block size:\n"
                 " 0 = 2 bytes\n"
                 " 1 = 4 bytes\n"
                 " 2 = 8 bytes\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 2");
    validate_arg(&cache_options->associativity, ASSOCIATIVITIES, 2,
                 "Associativity:\n"
                 " 0 = 1\n"
                 " 1 = 2\n"
                 " 2 = 4\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 2");
    validate_arg(&cache_options->write_back, NULL, 1,
                 "Write Hit:\n"
                 " 0 = Write-through\n"
                 " 1 = Write-back\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 1");
    validate_arg(&cache_options->write_allocate, NULL, 1,
                 "Write Miss:\n"
                 " 0 = No Write-allocate\n"
                 " 1 = Write-allocate\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 1");
    validate_arg(&cache_options->replacement, NULL, 2,
                 "Replacement:\n"
                 " 0 = LRU\n"
                 " 1 = LFU\n"
                 " 2 = FIFO\n"
                 " 3 = Random\n"
                 "Enter choice: ",
                 "Invalid choice, must be an integer between 0 and 2");
}

void validate_arg(unsigned char *char_member, const unsigned char options[], char limit, char *prompt, char *error_msg)
{
    while (1)
    {
        printf("%s", prompt);
        *char_member = fgetc(stdin) - '0';
        if (!flush_istream(stdin) || *char_member > limit)
        {
            printf("%s\n", error_msg);
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
