#ifndef _BIGINT_H
#define _BIGINT_H

#include "list_bigint.h"

#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct bigint_t bigint_s;

int do_operation(bigint_s **bigint_one, bigint_s **bigint_two, char operator);

int create_bigint(bigint_s **bigint, char *str, size_t len_string);

int add_bigints(bigint_s *bigint_addend_one, bigint_s *bigint_addend_two);

void subtract_bigints(bigint_s *bigint_minuend, bigint_s *bigint_subtrahend);

int multiply_bigints(bigint_s **bigint_multiplicand, bigint_s *bigint_multiplier);

int divide_bigints(bigint_s **bigint_dividend, bigint_s *bigint_divisor);

int multiply_bigint_by_digit_bn(bigint_s *bigint_multiplicand, uint32_t digit_bn_multiplier);

int divide_bigint_by_digit_bn(bigint_s **bigint_dividend, uint32_t digit_bn_divisor);

int copy_bigint(bigint_s **bigint_copy, bigint_s *bigint_base);

int modify_bigint(bigint_s *bigint, size_t count_new_digit_bn);

void choose_longer_bigint(bigint_s **bigint_one, bigint_s **bigint_two, bool necessity_accuracy);

int search_bigger_bigint(list_bigint_s *back_list_bigint_one, list_bigint_s *back_list_bigint_two);

int check_bigger_bigint(bigint_s *bigint_one, size_t count_digit_bn_bigint_one, bigint_s *bigint_two);

void check_significant_digits_bn(bigint_s *bigint);

void print_bigint(bigint_s *bigint);

void free_bigint(bigint_s *bigint);

#endif
