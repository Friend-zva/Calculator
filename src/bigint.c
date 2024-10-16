#include "bigint.h"
#include "auxiliary_functions.h"

#include <stdlib.h>

#define LEN_UNIT 1
#define DEGREE 9
#define MODULE 1000000000 // 10 ^ DEGREE
#define MAX_COUNT_DIGITS_NUMBER 256 // I think that's enough for input bigint

#define error(...) (fprintf(stderr, __VA_ARGS__))
#define is_negative(symbol) (symbol == '-') ? true : false

struct bigint_t {
    bool sign_bigint; // false is equal to positive integer, true is equal to negative integer
    size_t count_digits_bn;
    list_bigint_s *head_list_bigint;
    list_bigint_s *back_list_bigint;
};

int do_operation(bigint_s **bigint_one, bigint_s **bigint_two, char operator) {
    switch (operator) {
        case '+':
            if ((*bigint_one)->sign_bigint == (*bigint_two)->sign_bigint) {
                choose_longer_bigint(bigint_one, bigint_two, false);
                if (add_bigints(*bigint_one, *bigint_two)) {
                    free_bigint(*bigint_one);
                    free_bigint(*bigint_two);
                    return 5;
                }

                break;
            }

            choose_longer_bigint(bigint_one, bigint_two, true);
            subtract_bigints(*bigint_one, *bigint_two);

            break;
        case '-':
            (*bigint_two)->sign_bigint = !(*bigint_two)->sign_bigint;

            if ((*bigint_one)->sign_bigint != (*bigint_two)->sign_bigint) {
                choose_longer_bigint(bigint_one, bigint_two, true);
                subtract_bigints(*bigint_one, *bigint_two);

                break;
            }

            choose_longer_bigint(bigint_one, bigint_two, false);
            if (add_bigints(*bigint_one, *bigint_two)) {
                free_bigint(*bigint_one);
                free_bigint(*bigint_two);
                return 5;
            }

            break;
        case '*':
            choose_longer_bigint(bigint_one, bigint_two, false);
            if (multiply_bigints(bigint_one, *bigint_two)) {
                free_bigint(*bigint_one);
                free_bigint(*bigint_two);
                return 5;
            }

            break;
        case '/':
            if (!get_digit_bn((*bigint_two)->back_list_bigint)) {
                free_bigint(*bigint_one);
                free_bigint(*bigint_two);
                error("Attempt to divide by zero\n");
                return 4;
            }

            if (divide_bigints(bigint_one, *bigint_two)) {
                free_bigint(*bigint_one);
                free_bigint(*bigint_two);
                return 5;
            }

            break;
    }
    free_bigint(*bigint_two);

    if ((*bigint_one)->count_digits_bn == 1 && !get_digit_bn((*bigint_one)->back_list_bigint)) {
        (*bigint_one)->sign_bigint = false;
    }

    return 0;
}

int create_bigint(bigint_s **bigint, char *string_read, size_t len_string_read) {
    *bigint = (bigint_s *)calloc(1, sizeof(bigint_s));
    if (*bigint == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    (*bigint)->sign_bigint = is_negative(string_read[0]);

    char *string_digit_bn = (char *)calloc(DEGREE, sizeof(char));
    if (string_digit_bn == NULL) {
        free(*bigint);
        error("Memory allocation error\n");
        return 5;
    }
    size_t len_string_digit_bn = 0u;

    for (int index = (len_string_read - 1); index >= (int)(*bigint)->sign_bigint; index--) {
        string_digit_bn[len_string_digit_bn++] = string_read[index];

        if (len_string_digit_bn == DEGREE) {
            if (push_back_list(&(*bigint)->back_list_bigint, reverse_string_to_uint32(string_digit_bn, 
                                                                                  len_string_digit_bn))) {
                free_bigint(*bigint);
                free(string_digit_bn);
                return 5;
            }
            if ((*bigint)->head_list_bigint == NULL) {
                (*bigint)->head_list_bigint = (*bigint)->back_list_bigint;
            }
            ((*bigint)->count_digits_bn)++;

            reset_string(string_digit_bn, len_string_digit_bn);
            len_string_digit_bn = 0u;
        }
    }
    if (len_string_digit_bn) {
        if (push_back_list(&(*bigint)->back_list_bigint, reverse_string_to_uint32(string_digit_bn, 
                                                                              len_string_digit_bn))) {
            free_bigint(*bigint);
            free(string_digit_bn);
            return 5;
        }
        if ((*bigint)->head_list_bigint == NULL) {
            (*bigint)->head_list_bigint = (*bigint)->back_list_bigint;
        }
        ((*bigint)->count_digits_bn)++;
    }
    free(string_digit_bn);

    check_significant_digits_bn(*bigint);
    return 0;
}

int add_bigints(bigint_s *bigint_addend_one, bigint_s *bigint_addend_two) {
    list_bigint_s *list_bigint_addend_one = bigint_addend_one->head_list_bigint;
    list_bigint_s *list_bigint_addend_two = bigint_addend_two->head_list_bigint;
    uint32_t digit_bn_correction = 0u;

    while (list_bigint_addend_one != NULL) {
        uint32_t digit_bn_sum = get_digit_bn(list_bigint_addend_one) + digit_bn_correction;

        if (list_bigint_addend_two != NULL) {
            digit_bn_sum += get_digit_bn(list_bigint_addend_two);
            go_next_list_bigint(&list_bigint_addend_two);
        }

        change_digit_bn(list_bigint_addend_one, digit_bn_sum % MODULE);
        go_next_list_bigint(&list_bigint_addend_one);

        digit_bn_correction = digit_bn_sum / MODULE;
    }
    if (digit_bn_correction) {
        if (push_back_list(&bigint_addend_one->back_list_bigint, digit_bn_correction)) {
            return 5;
        }
        (bigint_addend_one->count_digits_bn)++;
    }

    return 0;
}

void subtract_bigints(bigint_s *bigint_minuend, bigint_s *bigint_subtrahend) {
    list_bigint_s *list_bigint_minuend = bigint_minuend->head_list_bigint;
    list_bigint_s *list_bigint_subtrahend = bigint_subtrahend->head_list_bigint;
    uint32_t digit_bn_correction = 0u;

    while (list_bigint_minuend != NULL) {
        uint32_t digit_bn_remainder = get_digit_bn(list_bigint_minuend);

        if (digit_bn_remainder < digit_bn_correction) {
            digit_bn_remainder += MODULE - digit_bn_correction;
            digit_bn_correction = 1;
        } else {
            digit_bn_remainder -= digit_bn_correction;
            digit_bn_correction = 0u;
        }

        if (list_bigint_subtrahend != NULL) {
            uint32_t digit_bn_subtrahend = get_digit_bn(list_bigint_subtrahend);
            go_next_list_bigint(&list_bigint_subtrahend);

            if (digit_bn_remainder < digit_bn_subtrahend) {
                digit_bn_remainder += MODULE - digit_bn_subtrahend;
                digit_bn_correction = 1;
            } else {
                digit_bn_remainder -= digit_bn_subtrahend;
            }
        }

        change_digit_bn(list_bigint_minuend, digit_bn_remainder);
        go_next_list_bigint(&list_bigint_minuend);
    }

    check_significant_digits_bn(bigint_minuend);
}

int multiply_bigints(bigint_s **bigint_multiplicand, bigint_s *bigint_multiplier) {
    bigint_s *bigint_product = (bigint_s *)calloc(1, sizeof(bigint_s));
    if (bigint_product == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    bigint_product->sign_bigint = (*bigint_multiplicand)->sign_bigint ^ bigint_multiplier->sign_bigint;

    list_bigint_s *first_significant_list_bigint_product = NULL;

    while (bigint_multiplier->head_list_bigint != NULL) {
        uint64_t digit_bn_multiplier = (uint64_t)pop_list(&bigint_multiplier->head_list_bigint);
        list_bigint_s *list_bigint_multiplicand = (*bigint_multiplicand)->head_list_bigint;
        list_bigint_s *list_bigint_product = first_significant_list_bigint_product;
        uint64_t digit_bn_correction = 0u;

        while (list_bigint_multiplicand != NULL) {
            uint64_t digit_bn_product = (uint64_t)get_digit_bn(list_bigint_multiplicand) * digit_bn_multiplier +
                                                                                           digit_bn_correction;

            if (list_bigint_product == NULL) {
                if (push_back_list(&bigint_product->back_list_bigint, digit_bn_product % MODULE)) {
                    free_bigint(bigint_product);
                    return 5;
                }
                if (bigint_product->head_list_bigint == NULL) {
                    bigint_product->head_list_bigint = bigint_product->back_list_bigint;
                    first_significant_list_bigint_product = bigint_product->head_list_bigint;
                }
                (bigint_product->count_digits_bn)++;
            } else {
                digit_bn_product = get_digit_bn(list_bigint_product) + digit_bn_product;
                change_digit_bn(list_bigint_product, (uint32_t)(digit_bn_product % MODULE));
                go_next_list_bigint(&list_bigint_product);
            }

            go_next_list_bigint(&list_bigint_multiplicand);

            digit_bn_correction = digit_bn_product / MODULE;
        }
        while (digit_bn_correction) {
            if (list_bigint_product == NULL) {
                if (push_back_list(&bigint_product->back_list_bigint, (uint32_t)digit_bn_correction)) {
                    free_bigint(bigint_product);
                    return 5;
                }
                (bigint_product->count_digits_bn)++;
                
                digit_bn_correction = 0u;
            } else {
                uint32_t digit_bn_product = get_digit_bn(list_bigint_product) + (uint32_t)digit_bn_correction;
                change_digit_bn(list_bigint_product, digit_bn_product % MODULE);
                go_next_list_bigint(&list_bigint_product);

                digit_bn_correction = digit_bn_product / MODULE;
            }
        }

        go_next_list_bigint(&first_significant_list_bigint_product);
    }
    free_bigint(*bigint_multiplicand);

    *bigint_multiplicand = bigint_product;
    check_significant_digits_bn(*bigint_multiplicand);
    return 0;
}

int divide_bigints(bigint_s **bigint_dividend, bigint_s *bigint_divisor) {
    char *string_digits_bn_quotient = (char *)calloc(MAX_COUNT_DIGITS_NUMBER, sizeof(char));
    if (string_digits_bn_quotient == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    size_t max_len_string_digits_bn_quotient = MAX_COUNT_DIGITS_NUMBER;
    size_t len_string_digits_bn_quotient = 0u;

    bigint_s *bigint_divisor_copy = NULL;
    int difference_count_digits_bn = (*bigint_dividend)->count_digits_bn - bigint_divisor->count_digits_bn;

    if (difference_count_digits_bn >= 0) {
        if (copy_bigint(&bigint_divisor_copy, bigint_divisor)) {
            free(string_digits_bn_quotient);
            return 5;
        }
        if (modify_bigint(bigint_divisor_copy, (*bigint_dividend)->count_digits_bn - bigint_divisor->count_digits_bn)) {
            free_bigint(bigint_divisor_copy);
            free(string_digits_bn_quotient);
            return 5;
        }
    } else {
        (*bigint_dividend)->count_digits_bn = 0u;
        len_string_digits_bn_quotient = 1;
        string_digits_bn_quotient[len_string_digits_bn_quotient - 1] = '0';
    }

    while (difference_count_digits_bn >= 0) {
        size_t last_count_digit_bn_dividend = (*bigint_dividend)->count_digits_bn;
        size_t count_anylysed_digit_bn_dividend = last_count_digit_bn_dividend - difference_count_digits_bn;
        uint32_t digit_bn_quotient = 0u;
        
        bigint_s *bigint_divisor_multiplied = NULL;
        if (copy_bigint(&bigint_divisor_multiplied, bigint_divisor_copy)) {
            free_bigint(bigint_divisor_copy);
            free(string_digits_bn_quotient);
            return 5;
        }

        if (check_bigger_bigint(*bigint_dividend, count_anylysed_digit_bn_dividend, bigint_divisor_copy)) {
            bigint_s *bigint_difference = NULL;
            if (copy_bigint(&bigint_difference, *bigint_dividend)) {
                free_bigint(bigint_divisor_multiplied);
                free_bigint(bigint_divisor_copy);
                free(string_digits_bn_quotient);
                return 5;
            }

            if (divide_bigint_by_digit_bn(&bigint_difference, get_digit_bn(bigint_divisor_copy->back_list_bigint))) {
                free_bigint(bigint_difference);
                free_bigint(bigint_divisor_multiplied);
                free_bigint(bigint_divisor_copy);
                free(string_digits_bn_quotient);
                return 5;
            }
            uint32_t digit_bn_difference = get_digit_bn(bigint_difference->back_list_bigint);
            free_bigint(bigint_difference);

            if (multiply_bigint_by_digit_bn(bigint_divisor_multiplied, digit_bn_difference)) {
                free_bigint(bigint_divisor_multiplied);
                free_bigint(bigint_divisor_copy);
                free(string_digits_bn_quotient);
                return 5;
            }

            while (!check_bigger_bigint(*bigint_dividend, count_anylysed_digit_bn_dividend, bigint_divisor_multiplied)) {
                if (last_count_digit_bn_dividend != (*bigint_dividend)->count_digits_bn) {
                    count_anylysed_digit_bn_dividend = last_count_digit_bn_dividend - (*bigint_dividend)->count_digits_bn;
                    last_count_digit_bn_dividend = (*bigint_dividend)->count_digits_bn;
                }

                subtract_bigints(bigint_divisor_multiplied, bigint_divisor_copy);
                digit_bn_difference--;
            }

            subtract_bigints(*bigint_dividend, bigint_divisor_multiplied);

            digit_bn_quotient = digit_bn_difference;
        }
        free_bigint(bigint_divisor_multiplied);

        len_string_digits_bn_quotient += DEGREE;
        if (len_string_digits_bn_quotient >= max_len_string_digits_bn_quotient) {
            max_len_string_digits_bn_quotient *= 2;
            string_digits_bn_quotient = (char *)realloc(string_digits_bn_quotient, max_len_string_digits_bn_quotient);
            if (string_digits_bn_quotient == NULL) {
                free_bigint(bigint_divisor_copy);
                free(string_digits_bn_quotient);
                error("Memory allocation error\n");
                return 5;
            }
        }
        paste_digit_bn_in_string(string_digits_bn_quotient, len_string_digits_bn_quotient, digit_bn_quotient);

        pop_list(&bigint_divisor_copy->head_list_bigint);
        count_anylysed_digit_bn_dividend++;

        difference_count_digits_bn--;
    }

    if (bigint_divisor_copy != NULL) {
        free_bigint(bigint_divisor_copy);
    }

    bigint_s *bigint_quotient = NULL;
    if (create_bigint(&bigint_quotient, string_digits_bn_quotient, len_string_digits_bn_quotient)) {
        free(string_digits_bn_quotient);
        error("Memory allocation error\n");
        return 5;
    }
    bigint_quotient->sign_bigint = (*bigint_dividend)->sign_bigint ^ bigint_divisor->sign_bigint;
    free(string_digits_bn_quotient);

    if (bigint_quotient->sign_bigint && get_digit_bn((*bigint_dividend)->back_list_bigint)) {
        if (get_digit_bn(bigint_quotient->head_list_bigint) || (bigint_quotient->count_digits_bn > 1)) {
            bigint_s *bigint_addend = NULL;
            char string_digit_bn_addend[LEN_UNIT] = {'1'};
            if (create_bigint(&bigint_addend, string_digit_bn_addend, LEN_UNIT)) {
                free_bigint(bigint_quotient);
                return 5;
            }

            if (add_bigints(bigint_quotient, bigint_addend)) {
                free_bigint(bigint_quotient);
                free_bigint(bigint_addend);
                return 5;
            }

            free_bigint(bigint_addend);
        } else {
            uint32_t digit_bn_quotient = get_digit_bn(bigint_quotient->head_list_bigint);
            change_digit_bn(bigint_quotient->head_list_bigint, digit_bn_quotient + 1);
        }
    }
    free_bigint(*bigint_dividend);

    *bigint_dividend = bigint_quotient;
    check_significant_digits_bn(*bigint_dividend);
    return 0;
}

int multiply_bigint_by_digit_bn(bigint_s *bigint_multiplicand, uint32_t digit_bn_multiplier) {
    list_bigint_s *list_bigint_multiplicand = bigint_multiplicand->head_list_bigint;
    uint64_t digit_bn_correction = 0u;

    while (list_bigint_multiplicand != NULL) {
        uint64_t digit_bn_product = (uint64_t)get_digit_bn(list_bigint_multiplicand) * digit_bn_multiplier + 
                                                                                       digit_bn_correction;
        change_digit_bn(list_bigint_multiplicand, (uint32_t)(digit_bn_product % MODULE));
        go_next_list_bigint(&list_bigint_multiplicand);

        digit_bn_correction = digit_bn_product / MODULE;
    }
    if (digit_bn_correction) {
        if (push_back_list(&bigint_multiplicand->back_list_bigint, (uint32_t)digit_bn_correction)) { 
            return 5;
        }
        (bigint_multiplicand->count_digits_bn)++;
    }

    return 0;
}

int divide_bigint_by_digit_bn(bigint_s **bigint_dividend, uint32_t digit_bn_divisor) {
    bigint_s *bigint_quotient = (bigint_s *)calloc(1, sizeof(bigint_s));
    if (bigint_quotient == NULL) {
        error("Memory allocation error\n");
        return 5;
    }
    bigint_quotient->sign_bigint = (*bigint_dividend)->sign_bigint;

    list_bigint_s *list_bigint_dividend = (*bigint_dividend)->back_list_bigint;
    uint64_t digit_bn_correction = 0u;

    while (list_bigint_dividend != NULL) {
        uint64_t digit_bn_quotient =  (uint64_t)(digit_bn_correction + get_digit_bn(list_bigint_dividend)) / 
                                                                                            digit_bn_divisor;

        if (push_list(&bigint_quotient->head_list_bigint, digit_bn_quotient % MODULE)) {
            free_bigint(bigint_quotient);
            return 5;
        }
        if (bigint_quotient->back_list_bigint == NULL) {
            bigint_quotient->back_list_bigint = bigint_quotient->head_list_bigint;
        }
        (bigint_quotient->count_digits_bn)++;

        if (!digit_bn_quotient) {
            digit_bn_quotient = (uint64_t)get_digit_bn(list_bigint_dividend) + digit_bn_correction;
        }

        go_prev_list_bigint(&list_bigint_dividend);

        digit_bn_correction = digit_bn_quotient * MODULE;
    }
    free_bigint(*bigint_dividend);

    *bigint_dividend = bigint_quotient;
    check_significant_digits_bn(*bigint_dividend);
    return 0;
}

int copy_bigint(bigint_s **bigint_copy, bigint_s *bigint_base) {
    bigint_s *bigint = (bigint_s *)calloc(1, sizeof(bigint_s));
    if (bigint == NULL) {
        return 5;
    }
    bigint->sign_bigint = bigint_base->sign_bigint;
    bigint->count_digits_bn = bigint_base->count_digits_bn;

    list_bigint_s *list_bigint = bigint_base->head_list_bigint;

    while (list_bigint != NULL) {
        if (push_back_list(&bigint->back_list_bigint, get_digit_bn(list_bigint))) {
            return 5;
        }
        if (bigint->head_list_bigint == NULL) {
            bigint->head_list_bigint = bigint->back_list_bigint;
        }

        go_next_list_bigint(&list_bigint);
    }

    *bigint_copy = bigint;
    return 0;
}

int modify_bigint(bigint_s *bigint, size_t count_new_digit_bn) {
    for (size_t count = 0u; count < count_new_digit_bn; count++) {
        if (push_list(&bigint->head_list_bigint, 0u)) {
            return 5;
        }
    }

    return 0;
}

void choose_longer_bigint(bigint_s **bigint_one, bigint_s **bigint_two, bool necessity_accuracy) {
    if ((*bigint_one)->count_digits_bn < (*bigint_two)->count_digits_bn) {
        bigint_s *bigint_replace = *bigint_one;
        *bigint_one = *bigint_two;
        *bigint_two = bigint_replace;
    } else if (necessity_accuracy) {
        if ((*bigint_one)->count_digits_bn == (*bigint_two)->count_digits_bn) {
            if (search_bigger_bigint((*bigint_one)->back_list_bigint, (*bigint_two)->back_list_bigint)) {
                bigint_s *bigint_replace = *bigint_one;
                *bigint_one = *bigint_two;
                *bigint_two = bigint_replace;
            }
        }
    }
}

int search_bigger_bigint(list_bigint_s *back_list_bigint_one, list_bigint_s *back_list_bigint_two) {
    while (back_list_bigint_two != NULL) {
        if (get_digit_bn(back_list_bigint_one) > get_digit_bn(back_list_bigint_two)) {
            return 0;
        } else if (get_digit_bn(back_list_bigint_one) < get_digit_bn(back_list_bigint_two)) {
            return 1;
        }

        go_prev_list_bigint(&back_list_bigint_one);
        go_prev_list_bigint(&back_list_bigint_two);
    }

    return 0;
}

int check_bigger_bigint(bigint_s *bigint_one, size_t count_analysed_digit_bn_bigint_one, bigint_s *bigint_two) {
    list_bigint_s *list_bigint_one = bigint_one->back_list_bigint;
    list_bigint_s *list_bigint_two = bigint_two->back_list_bigint;
    
    if (count_analysed_digit_bn_bigint_one > bigint_two->count_digits_bn) {
        return 1;
    }
    if (count_analysed_digit_bn_bigint_one < bigint_two->count_digits_bn) {
        return 0;
    }

    while (count_analysed_digit_bn_bigint_one) {
        if (get_digit_bn(list_bigint_one) > get_digit_bn(list_bigint_two)) {
            return 1;
        } else if (get_digit_bn(list_bigint_one) < get_digit_bn(list_bigint_two)) {
            return 0;
        }

        go_prev_list_bigint(&list_bigint_one);
        go_prev_list_bigint(&list_bigint_two);
        count_analysed_digit_bn_bigint_one--;
    }

    return 1;
}

void check_significant_digits_bn(bigint_s *bigint) {
    while (bigint->count_digits_bn > 1 && !get_digit_bn(bigint->back_list_bigint)) {  
        pop_back_list(&bigint->back_list_bigint);
        (bigint->count_digits_bn)--;
    }
}

void print_bigint(bigint_s *bigint) {
    if (bigint->sign_bigint) {
        printf("-");
    }

    print_part_bigint(bigint->back_list_bigint);
}

void free_bigint(bigint_s *bigint) {
    while (bigint->head_list_bigint != NULL) {
        pop_list(&bigint->head_list_bigint);
    }

    free(bigint);
}
