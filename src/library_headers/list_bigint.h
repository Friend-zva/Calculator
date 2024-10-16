#ifndef _LIST_BIGINT_H
#define _LIST_BIGINT_H

#include <stdio.h>
#include <inttypes.h>

typedef struct list_bigint_t list_bigint_s;

int push_back_list(list_bigint_s **back_list_bigint, uint32_t digit_bn);

int push_list(list_bigint_s **head_list_bigint, uint32_t digit_bn);

uint32_t pop_list(list_bigint_s **head_list_bigint);

uint32_t pop_back_list(list_bigint_s **back_list_bigint);

void go_next_list_bigint(list_bigint_s **list_bigint);

void go_prev_list_bigint(list_bigint_s **list_bigint);

uint32_t get_digit_bn(list_bigint_s *list_bigint);

void change_digit_bn(list_bigint_s *list_bigint, uint32_t digit_bn);

void print_part_bigint(list_bigint_s *back_list_bigint);

#endif
