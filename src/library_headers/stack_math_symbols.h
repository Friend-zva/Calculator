#ifndef _STACK_MATH_SYMBOLS_H
#define _STACK_MATH_SYMBOLS_H

#include "stack_bigints.h"

typedef struct stack_math_symbols_t stack_math_symbols_s;

int anylyse_stack_math_symbols(stack_math_symbols_s **stack_math_symbols, 
                               stack_bigints_s **stack_bigints, char operator);

int clear_stack_math_symbols(stack_math_symbols_s **stack_math_symbols, stack_bigints_s **stack_bigints, 
                                                                        bool permission_opening_parenthesis);

int push_stack_math_symbols(stack_math_symbols_s **stack_math_symbols, char math_symbol);

char pop_stack_math_symbols(stack_math_symbols_s **stack_math_symbols);

char get_stack_math_symbols(stack_math_symbols_s *stack_math_symbols);

void free_stack_math_symbols(stack_math_symbols_s **stack_math_symbols);

#endif
