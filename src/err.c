#include <err.h>

static const char *error_sentence = "[ERR] Aborting the program:";

void report_and_abort(err_code error)
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

        case ELF_OPEN_FAIL:
            printf("%s ELF_OPEN_FAIL (%d)\n\n", error_sentence, ELF_OPEN_FAIL);
            abort();
            break;

        case ELF_HEADER_READ_FAIL:
            printf("%s ELF_HEADER_READ_FAIL (%d)\n\n", error_sentence, ELF_HEADER_READ_FAIL);
            abort();
            break;

        case PROGRAM_HEADER_READ_FAIL:
            printf("%s PROGRAM_HEADER_READ_FAIL (%d)\n\n", error_sentence, PROGRAM_HEADER_READ_FAIL);
            abort();
            break;

        case NOT_ELF_FILE:
            printf("%s NOT_ELF_FILE (%d)\n\n", error_sentence, NOT_ELF_FILE);
            abort();
            break;

        case WRONG_ENDIAN_OR_SYSTEM:
            printf("%s WRONG_ENDIAN_OR_SYSTEM (%d)\n\n", error_sentence, WRONG_ENDIAN_OR_SYSTEM);
            abort();
            break;

        case WRONG_ARCHITECTURE_OR_NOT_EXEC:
            printf("%s WRONG_ARCHITECTURE_OR_NOT_EXEC (%d)\n\n", error_sentence, WRONG_ARCHITECTURE_OR_NOT_EXEC);
            abort();
            break;

        case ELF_IS_CORRUPTED:
            printf("%s ELF_IS_CORRUPTED (%d)\n\n", error_sentence, ELF_IS_CORRUPTED);
            abort();
            break;

        case MEMORY_BOUNDS_HIT:
            printf("%s MEMORY_BOUNDS_HIT (%d)\n\n", error_sentence, MEMORY_BOUNDS_HIT);
            abort();
            break;

        case LOAD_ELF_LSEEK_FAIL:
            printf("%s LOAD_ELF_LSEEK_FAIL (%d)\n\n", error_sentence, LOAD_ELF_LSEEK_FAIL);
            abort();
            break;

        case LOAD_SEGMENT_FAIL:
            printf("%s LOAD_SEGMENT_FAIL (%d)\n\n", error_sentence, LOAD_SEGMENT_FAIL);
            abort();
            break;
        case ELF_CLOSE_FAIL:
            printf("%s ELF_CLOSE_FAIL (%d)\n\n", error_sentence, ELF_CLOSE_FAIL);
            abort();
            break;
        default:
            break;
    }
}