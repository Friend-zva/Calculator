#include "stack_math_symbols.h"

#include <stdio.h>
#include <stdlib.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define check_priority(operator) ((operator == '+' || operator == '-')) ? 0 : 1
#define is_operator(symbol) ((symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/'))

struct stack_math_symbols_t {
    char math_symbol;
    struct stack_math_symbols_t *next;
};

int anylyse_stack_math_symbols(stack_math_symbols_s **stack_math_symbols, 
                               stack_bigints_s **stack_bigints, char operator) {
    int value_return;

    while (*stack_math_symbols != NULL) {
        if ((*stack_math_symbols)->math_symbol == '(') {
            break;
        }

        if (check_priority(operator) > check_priority((*stack_math_symbols)->math_symbol)) {
            break;
        }

        char operator = pop_stack_math_symbols(stack_math_symbols);
        if ((value_return = calculate(stack_bigints, operator))) {
            return value_return;
        }
    }

    if (push_stack_math_symbols(stack_math_symbols, operator)) {
        return 5;
    }

    return 0;
}

int clear_stack_math_symbols(stack_math_symbols_s **stack_math_symbols, stack_bigints_s **stack_bigints, 
                                                                        bool permission_opening_parenthesis) {
    int value_return;
    
    if (!is_operator((*stack_math_symbols)->math_symbol)) {
        error("Missing quantity in parentheses\n");
        return 4;
    }

    while (*stack_math_symbols != NULL) {
        char operator = pop_stack_math_symbols(stack_math_symbols);
        if ((value_return = calculate(stack_bigints, operator))) {
            return value_return;
        }

        if (*stack_math_symbols != NULL && (*stack_math_symbols)->math_symbol == '(') {
            if (permission_opening_parenthesis) {
                pop_stack_math_symbols(stack_math_symbols);
                break;
            }
            error("A missing closing parenthesis\n");
            return 4;
        }
    }

    return 0;
}

int push_stack_math_symbols(stack_math_symbols_s **head_stack_math_symbols, char math_symbol) {
    stack_math_symbols_s *new_head_stack_math_symbols = (stack_math_symbols_s *)calloc(1, sizeof(stack_math_symbols_s));
    if (new_head_stack_math_symbols == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    new_head_stack_math_symbols->math_symbol = math_symbol;
    new_head_stack_math_symbols->next = *head_stack_math_symbols;
    *head_stack_math_symbols = new_head_stack_math_symbols;

    return 0;
}

char pop_stack_math_symbols(stack_math_symbols_s **head_stack_math_symbols) {
    stack_math_symbols_s *stack_math_symbols = *head_stack_math_symbols;
    *head_stack_math_symbols = (*head_stack_math_symbols)->next;

    char operator = stack_math_symbols->math_symbol;
    free(stack_math_symbols);
    return operator;
}

char get_stack_math_symbols(stack_math_symbols_s *stack_math_symbols) {
    return stack_math_symbols->math_symbol;
}

void free_stack_math_symbols(stack_math_symbols_s **stack_math_symbols) {
    while (*stack_math_symbols != NULL) {
        pop_stack_math_symbols(stack_math_symbols);
    }
}
