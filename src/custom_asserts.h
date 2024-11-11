#ifndef CUSTOM_ASSERTS_H_
#define CUSTOM_ASSERTS_H_

#include "../Custom-asserts/Color/color_printf.h"

enum TYPE_OF_ERROR
{
    SUCCESS         = 0,
    POINTER_IS_NULL = 1,
    DOUBLE_IS_NAN   = 2,
    FILE_OPEN_ERROR = 3,
    FILE_READ_ERROR = 4,
    STAT_ERROR      = 5,
    CALLOC_ERROR    = 6,
    VALUE_ERROR     = 7,
    INPUT_ERROR     = 8,
    PROGRAM_ERROR   = 9
};

#define warning(condition, message) do                                                     \
{                                                                                          \
    if(!(condition))                                                                      \
    {                                                                                      \
        color_printf(RED_TEXT, BOLD, "ERROR %s : %s, file: %s, function: %s, line: %d\n",  \
        #condition, #message, __FILE__, __FUNCTION__, __LINE__);                           \
        exit(message);                                                                     \
    }                                                                                      \
} while(0)

#ifndef NDEBUG

    #define check_expression(condition, message) do                                            \
    {                                                                                          \
        if(!(condition))                                                                      \
        {                                                                                      \
            color_printf(YELLOW_TEXT, BOLD, "ERROR %s : %s, file: %s, function: %s, line: %d\n",\
                        #condition, #message, __FILE__, __FUNCTION__, __LINE__);               \
            printf("\n");                                                                      \
            return message;                                                                    \
        }                                                                                      \
    } while(0)

#else

    #define check_expression(condition, message)

#endif
#endif
