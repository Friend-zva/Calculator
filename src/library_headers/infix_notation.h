#ifndef _INFIX_NOTATION_H
#define _INFIX_NOTATION_H

#include "stack_math_symbols.h"
#include "stack_bigints.h"

int parse_by_infix_notation(void);

void free_all_infix(stack_bigints_s **stack_bigints, stack_math_symbols_s **stack_math_symbols, 
                                                                          char *string_input);

#endif
