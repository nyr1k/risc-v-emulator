#ifndef ERR_H
#define ERR_H

#include <stdlib.h>
#include <stdio.h>

typedef enum {
    INVALID_MEMORY_ACCESS,
    INVALID_READ_ADDRESS,
    INVALID_WRITE_ADDRESS,
    INVALID_INSTRUCTION,
    INVALID_LOAD_ALIGNMENT,
    INVALID_STORE_ALIGNMENT,
    INVALID_INSTRUCTION_ALIGNMENT
} err_code;

static const char *error_sentence = "[ERR] Aborting the program:";

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

        case INVALID_WRITE_ADDRESS:
            printf("%s INVALID_WRITE_ADDRESS (%d)\n\n", error_sentence, INVALID_WRITE_ADDRESS);
            abort();
            break;

        case INVALID_INSTRUCTION:
            printf("%s INVALID_INSTRUCTION (%d)\n\n", error_sentence, INVALID_INSTRUCTION);
            abort();
            break;

        case INVALID_LOAD_ALIGNMENT:
            printf("%s INVALID_LOAD_ALIGNMENT (%d)\n\n", error_sentence, INVALID_LOAD_ALIGNMENT);
            abort();
            break;

        case INVALID_STORE_ALIGNMENT:
            printf("%s INVALID_STORE_ALIGNMENT (%d)\n\n", error_sentence, INVALID_STORE_ALIGNMENT);
            abort();
            break;

        case INVALID_INSTRUCTION_ALIGNMENT:
            printf("%s INVALID_INSTRUCTION_ALIGNMENT (%d)\n\n", error_sentence, INVALID_INSTRUCTION_ALIGNMENT);
            abort();
            break;

        default:
            break;
    }
}

#endif