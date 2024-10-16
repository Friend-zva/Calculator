#include "infix_notation.h"
#include "bigint.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COUNT_DIGITS_NUMBER 256 // I think that's enough for input number

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define is_digit(symbol) ('0' <= symbol && symbol <= '9')
#define is_operator(symbol) ((symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/'))

int parse_by_infix_notation(void) {
    stack_bigints_s *stack_bigints = NULL;
    stack_math_symbols_s *stack_math_symbols = NULL;

    char *string_input = (char *)calloc(MAX_COUNT_DIGITS_NUMBER, sizeof(char));
    if (string_input == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    size_t max_len_string_read = MAX_COUNT_DIGITS_NUMBER;
    size_t len_string_read = 0u;
    char symbol_read;

    bool last_symbol_read_is_operator = false;
    size_t position_input = 0u;
    int value_return;

    while ((symbol_read = getchar()) != EOF && symbol_read != '\n') {
        position_input++;

        if (len_string_read == max_len_string_read) {
            max_len_string_read *= 2;
            string_input = (char *)realloc(string_input, max_len_string_read);
            if (string_input == NULL) {
                free_stack_bigints(&stack_bigints);
                free_stack_math_symbols(&stack_math_symbols);
                error("Memory allocation error\n");
                return 5;
            }
        }

        if (symbol_read == ' ') {
            if (!len_string_read) {
                continue;
            }

            if (last_symbol_read_is_operator) {
                if ((value_return = anylyse_stack_math_symbols(&stack_math_symbols, &stack_bigints, 
                                                                string_input[len_string_read - 1]))) {
                    free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                    return value_return;
                }
            } else {
                if (string_input[0] == '-') {
                    free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                    error("Missing parentheses for a negative operand\n");
                    return 4;
                }

                bigint_s *bigint = NULL;
                if (create_bigint(&bigint, string_input, len_string_read)) {
                    free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                    return 5;
                }

                if (push_stack_bigints(&stack_bigints, bigint)) {
                    free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                    return 5;
                }
            }

            len_string_read = 0u;
        } else if (symbol_read == ')' && !last_symbol_read_is_operator) {
            if (len_string_read) {
                bigint_s *bigint = NULL;
                if (create_bigint(&bigint, string_input, len_string_read)) {
                    free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                    return 5;
                }
                len_string_read = 0u;

                if (push_stack_bigints(&stack_bigints, bigint)) {
                    free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                    return 5;
                }

                if (string_input[0] == '-') {
                    if (is_operator(get_stack_math_symbols(stack_math_symbols))) {
                        free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                        error("A missing opening parenthesis for a negative operand\n");
                        return 4;
                    }
                    pop_stack_math_symbols(&stack_math_symbols);
                    continue;
                }
            }

            if (stack_math_symbols == NULL) {
                free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                error("Missing quantity in parentheses\n");
                return 4;
            }

            if ((value_return = clear_stack_math_symbols(&stack_math_symbols, &stack_bigints, true))) {
                free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                return value_return;
            }
        } else if (symbol_read == '(') {
            if (push_stack_math_symbols(&stack_math_symbols, symbol_read)) {
                free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
                return 5;
            }
            last_symbol_read_is_operator = false;
        } else if (is_digit(symbol_read)) {
            string_input[len_string_read++] = symbol_read;
            last_symbol_read_is_operator = false;
        } else if (is_operator(symbol_read) && !len_string_read && !last_symbol_read_is_operator) {
            string_input[len_string_read++] = symbol_read;
            last_symbol_read_is_operator = true;
        } else {
            free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
            error("An undefined character at position %zu\n", position_input);
            return 4;
        }
    }
    if (len_string_read) {
        if (last_symbol_read_is_operator) {
            free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
            error("A missing operand\n");
            return 4;
        }

        bigint_s *bigint = NULL;
        if (create_bigint(&bigint, string_input, len_string_read)) {
            free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
            return 5;
        }

        if (push_stack_bigints(&stack_bigints, bigint)) {
            free_all_infix(&stack_bigints, &stack_math_symbols, string_input);
            return 5;
        }
    }
    free(string_input);

    if (stack_math_symbols != NULL) {
        if ((value_return = clear_stack_math_symbols(&stack_math_symbols, &stack_bigints, false))) {
            free_stack_bigints(&stack_bigints);
            free_stack_math_symbols(&stack_math_symbols);
            return value_return;
        }
    }

    if (stack_math_symbols != NULL) {
        free_stack_bigints(&stack_bigints);
        free_stack_math_symbols(&stack_math_symbols);
        error("A missing closing parenthesis\n");
        return 4;
    }

    if (stack_bigints != NULL) {
        if (print_result(&stack_bigints)) {
            free_stack_bigints(&stack_bigints);
            return 4;
        }
    }

    return 0;
}

void free_all_infix(stack_bigints_s **stack_bigints, stack_math_symbols_s **stack_math_symbols, 
                                                                          char *string_input) {
    free_stack_bigints(stack_bigints);
    free_stack_math_symbols(stack_math_symbols);
    free(string_input);
}
