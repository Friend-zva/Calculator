#include "auxiliary_functions.h"

#define DEGREE 9

void reset_string(char *string, size_t len_string) {
    for (size_t index = 0u; index < len_string; index++) {
        string[index] = 0;
    }
}

void paste_digit_bn_in_string(char *string, size_t len_string, uint32_t digit_bn) {
    for (size_t index = len_string ; index > (len_string - DEGREE); index--) {
        int digit = digit_bn % 10;
        string[index - 1] = digit + '0';
        digit_bn /= 10;
    }
}

uint32_t reverse_string_to_uint32(char *string, size_t len_string) {
    uint32_t digit_bn = 0u;

    for (size_t index = 0u; index < len_string; index++) {
        digit_bn += (string[index] - '0') * (uint32_t)power(10, index);
    }

    return digit_bn;
}

uint32_t power(int base, int degree) {
    uint32_t number = 1;

    for (int i = 0; i < degree; i++) {
        number *= base;
    }

    return number;
}
