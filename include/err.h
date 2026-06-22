#ifndef ERR_H
#define ERR_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    INVALID_MEMORY_ACCESS,
    INVALID_READ_ADDRESS 
} err_code;

static const char* error_sentence = "[ERR] Aborting the program:";

static void report_and_abort(err_code error)
{
    switch (error) {
        case INVALID_MEMORY_ACCESS:
            printf("%s INVALID_MEMORY_ACCESS (%d)\n\n", error_sentence, INVALID_MEMORY_ACCESS);
            abort();
            break;
        case INVALID_READ_ADDRESS:
            printf("%s INVALID_READ_ADDRESS (%d)\n\n", error_sentence, INVALID_READ_ADDRESS);
            abort();
            break;
        default:
            break;
    }
}

#endif