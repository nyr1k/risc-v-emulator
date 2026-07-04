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


void report_and_abort(err_code error);

#endif