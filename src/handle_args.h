#ifndef HANDLE_ARGS_H
#define HANDLE_ARGS_H

#include "cache.h"

void parse_args(CacheOptions *cache_options);
void validate_arg(unsigned char* char_member, unsigned const char options[], char limit, char* prompt, char* error_msg);

#endif // HANDLE_ARGS_H
