#include "list_bigint.h"

#include <stdio.h>
#include <stdlib.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define DEGREE 9

struct list_bigint_t {
    uint32_t digit_bn;
    struct list_bigint_t *next;
    struct list_bigint_t *prev;
};

int push_list(list_bigint_s **head_list_bigint, uint32_t digit_bn) {
    list_bigint_s *new_head_list_bigint = (list_bigint_s *)calloc(1, sizeof(list_bigint_s));
    if (new_head_list_bigint == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    new_head_list_bigint->digit_bn = digit_bn;

    if (*head_list_bigint == NULL) {
        *head_list_bigint = new_head_list_bigint;
        return 0;
    }

    new_head_list_bigint->next = *head_list_bigint;
    (*head_list_bigint)->prev = new_head_list_bigint;
    *head_list_bigint = new_head_list_bigint;

    return 0;
}

int push_back_list(list_bigint_s **back_list_bigint, uint32_t digit_bn) {
    list_bigint_s *new_back_list_bigint = (list_bigint_s *)calloc(1, sizeof(list_bigint_s));
    if (new_back_list_bigint == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    new_back_list_bigint->digit_bn = digit_bn;

    if (*back_list_bigint == NULL) {
        *back_list_bigint = new_back_list_bigint;
        return 0;
    }

    (*back_list_bigint)->next = new_back_list_bigint;
    new_back_list_bigint->prev = *back_list_bigint;
    *back_list_bigint = new_back_list_bigint;

    return 0;
}

uint32_t pop_list(list_bigint_s **head_list_bigint) {
    list_bigint_s *list_bigint = *head_list_bigint;
    *head_list_bigint = (*head_list_bigint)->next;
    if (*head_list_bigint != NULL) {
        (*head_list_bigint)->prev = NULL;
    }

    uint32_t digit_bn = list_bigint->digit_bn;
    free(list_bigint);
    return digit_bn;
}

uint32_t pop_back_list(list_bigint_s **back_list_bigint) {
    list_bigint_s *list_bigint = *back_list_bigint;
    *back_list_bigint = (*back_list_bigint)->prev;
    if (*back_list_bigint != NULL) {
        (*back_list_bigint)->next = NULL;
    }

    uint32_t digit_bn = list_bigint->digit_bn;
    free(list_bigint);
    return digit_bn;
}

void go_next_list_bigint(list_bigint_s **list_bigint) {
    *list_bigint = (*list_bigint)->next;
}

void go_prev_list_bigint(list_bigint_s **list_bigint) {
    *list_bigint = (*list_bigint)->prev;
}

uint32_t get_digit_bn(list_bigint_s *list_bigint) {
    return list_bigint->digit_bn;
}

void change_digit_bn(list_bigint_s *list_bigint, uint32_t digit_bn) {
    list_bigint->digit_bn = digit_bn;
}

void print_part_bigint(list_bigint_s *back_list_bigint) {
    list_bigint_s *list_bigint = back_list_bigint;
    printf("%u", get_digit_bn(list_bigint));
    go_prev_list_bigint(&list_bigint);

    while (list_bigint != NULL) {
        printf("%09u", get_digit_bn(list_bigint));
        go_prev_list_bigint(&list_bigint);
    }
    
    printf("\n");
}
