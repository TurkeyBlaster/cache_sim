#ifndef HANDLE_ARGS_H
#define HANDLE_ARGS_H

#include "cache.h"

// #define IS_POWER_OF_TWO(X) !(X & (X - 1))
// #define HANDLE_ARGS(PROMPT_MSG, CACHE, NAME, LIMIT, OPTIONS, ERROR_MSG)        \
//     {                                                                          \
//         char delimiter;                                                        \
//         while (1)                                                              \
//         {                                                                      \
//             printf(PROMPT_MSG);                                                \
//             CACHE->NAME = fgetc(stdin) - '0';                                  \
//             delimiter = fgetc(stdin);                                          \
//             if (CACHE->NAME > LIMIT && delimiter != '\n' && delimiter != '\r') \
//             {                                                                  \
//                 printf(ERROR_MSG);                                             \
//             }                                                                  \
//             else                                                               \
//             {                                                                  \
//                 CACHE->NAME = OPTIONS[CACHE->NAME];                            \
//                 break;                                                         \
//             }                                                                  \
//         }                                                                      \
//     }

void parse_args(Cache *cache);
void validate_arg(char* char_member, const char options[], char limit, char* prompt, char* error_msg);

#endif // HANDLE_ARGS_H
