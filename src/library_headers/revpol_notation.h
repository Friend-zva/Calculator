#ifndef _REVPOL_NOTATION_H
#define _REVPOL_NOTATION_H

#include "stack_bigints.h"

int parse_by_revpol_notation(void);

void free_all_revpol(stack_bigints_s **stack_bigints, char *string_input);

#endif
