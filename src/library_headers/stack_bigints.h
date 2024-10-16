#ifndef _STACK_BIGINTS_H
#define _STACK_BIGINTS_H

#include "bigint.h"

typedef struct stack_bigints_t stack_bigints_s;

int calculate(stack_bigints_s **stack_bigints, char operator);

int push_stack_bigints(stack_bigints_s **head_stack_bigints, bigint_s *bigint);

bigint_s *pop_stack_bigints(stack_bigints_s **head_stack_bigints);

int print_result(stack_bigints_s **stack_bigints);

void free_stack_bigints(stack_bigints_s **stack_bigints);

#endif
