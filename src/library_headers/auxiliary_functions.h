#ifndef _AUXILIARY_FUNCTIONS_H
#define _AUXILIARY_FUNCTIONS_H

#include <stdio.h>
#include <inttypes.h>

void reset_string(char *string, size_t len_string);

void paste_digit_bn_in_string(char *string, size_t len_string, uint32_t digit_bn);

uint32_t reverse_string_to_uint32(char *string, size_t len_string);

uint32_t power(int base, int degree);

#endif
