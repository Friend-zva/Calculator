#include "revpol_notation.h"
#include "bigint.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COUNT_DIGITS_NUMBER 256 // I think that's enough for input bigint

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define is_digit(symbol) ('0' <= symbol && symbol <= '9')
#define is_operator(symbol) ((symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/'))

int parse_by_revpol_notation(void) {
    stack_bigints_s *stack_bigints = NULL;

    char *string_input = (char *)calloc(MAX_COUNT_DIGITS_NUMBER, sizeof(char));
    if (string_input == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    size_t max_len_string_read = MAX_COUNT_DIGITS_NUMBER;
    size_t len_string_read = 0u;
    char symbol_read;

    size_t position_input = 0u;
    int value_return;

    while ((symbol_read = getchar()) != EOF && symbol_read != '\n') {
        position_input++;

        if (len_string_read == max_len_string_read) {
            max_len_string_read *= 2;
            string_input = (char *)realloc(string_input, max_len_string_read);
            if (string_input == NULL) {
                free_stack_bigints(&stack_bigints);
                error("Memory allocation error\n");
                return 5;
            }
        }

        if (symbol_read == ' ') {
            if (!len_string_read) {
                continue;
            }

            if (len_string_read == 1 && is_operator(string_input[len_string_read - 1])) {
                if ((value_return = calculate(&stack_bigints, string_input[len_string_read - 1]))) {
                    free_all_revpol(&stack_bigints, string_input);
                    return value_return;
                }
            } else {
                bigint_s *bigint = NULL;
                if (create_bigint(&bigint, string_input, len_string_read)) {
                    free_all_revpol(&stack_bigints, string_input);
                    return 5;
                }

                if (push_stack_bigints(&stack_bigints, bigint)) {
                    free_all_revpol(&stack_bigints, string_input);
                    return 5;
                }
            }

            len_string_read = 0u;
        } else if (is_digit(symbol_read) || (is_operator(symbol_read) && !len_string_read)) {
            string_input[len_string_read++] = symbol_read;
        } else {
            free_all_revpol(&stack_bigints, string_input);
            error("An undefined character at position %zu\n", position_input);
            return 4;
        }
    }
    if (len_string_read) {
        if (len_string_read == 1 && is_operator(string_input[len_string_read - 1])) {
            if ((value_return = calculate(&stack_bigints, string_input[len_string_read - 1]))) {
                free_all_revpol(&stack_bigints, string_input);
                return value_return;
            }
        } else {
            bigint_s *bigint = NULL;
            if (create_bigint(&bigint, string_input, len_string_read)) {
                free_all_revpol(&stack_bigints, string_input);
                return 5;
            }

            if (push_stack_bigints(&stack_bigints, bigint)) {
                free_all_revpol(&stack_bigints, string_input);
                return 5;
            }
        }
    }
    free(string_input);

    if (stack_bigints != NULL) {
        if (print_result(&stack_bigints)) {
            free_stack_bigints(&stack_bigints);
            return 4;
        }
    }

    return 0;
}

void free_all_revpol(stack_bigints_s **stack_bigints, char *string_input) {
    free_stack_bigints(stack_bigints);
    free(string_input);
}
