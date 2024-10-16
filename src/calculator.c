#include "revpol_notation.h"
#include "infix_notation.h"

#include <stdio.h>
#include <string.h>

#define error(...) (fprintf(stderr, __VA_ARGS__))

int main(int argc, char **argv) { 
    if (argc == 1) {
        error("Cannot read command line arguments\n");
        return 1;
    }
    if (argc > 2) {
        error("Invalid command line arguments\n");
        return 1;
    }

    if (!strcmp(argv[1], "--revpol")) {
        return parse_by_revpol_notation();
    } else if (!strcmp(argv[1], "--infix")) {
        return parse_by_infix_notation();
    }
    
    error("Invalid command line arguments\n");
    return 1;
}
