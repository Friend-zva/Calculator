#include "stack_bigints.h"

#include <stdio.h>
#include <stdlib.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))

struct stack_bigints_t {
    bigint_s *bigint;
    struct stack_bigints_t *next;
};

int calculate(stack_bigints_s **stack_bigints, char operator) {
    if (*stack_bigints == NULL || (*stack_bigints)->next == NULL) {
        error("A missing operand\n");
        return 4;
    }

    int value_return;

    bigint_s *bigint_two = pop_stack_bigints(stack_bigints);
    bigint_s *bigint_one = pop_stack_bigints(stack_bigints);
    if ((value_return = do_operation(&bigint_one, &bigint_two, operator))) {
        return value_return;
    }

    if (push_stack_bigints(stack_bigints, bigint_one)) {
        return 5;
    }

    return 0;
}

int push_stack_bigints(stack_bigints_s **head_stack_bigints, bigint_s *bigint) {
    stack_bigints_s *new_head_stack_bigints = (stack_bigints_s *)calloc(1, sizeof(stack_bigints_s));
    if (new_head_stack_bigints == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    new_head_stack_bigints->bigint = bigint;
    new_head_stack_bigints->next = *head_stack_bigints;
    *head_stack_bigints = new_head_stack_bigints;

    return 0;
}

bigint_s *pop_stack_bigints(stack_bigints_s **head_stack_bigints) {
    stack_bigints_s *stack_bigint = *head_stack_bigints;
    *head_stack_bigints = (*head_stack_bigints)->next;

    bigint_s *bigint = stack_bigint->bigint;
    free(stack_bigint);
    return bigint;
}

int print_result(stack_bigints_s **stack_bigints) {
    if ((*stack_bigints)->next != NULL) {
        error("A missing operator\n");
        return 4;
    }

    bigint_s *bigint = pop_stack_bigints(stack_bigints);
    print_bigint(bigint);
    free_bigint(bigint);

    return 0;
}

void free_stack_bigints(stack_bigints_s **stack_bigints) {
    while (*stack_bigints != NULL) {
        free_bigint(pop_stack_bigints(stack_bigints));
    }
}
