/**
 * @file formatted_exception.cpp
 * @brief Implements the printf formatted exceptions
 */

#include "formatted_exception.h"

FormattedException::FormattedException(const char * format, ...) : std::exception() {
    va_list args;
    va_start(args, format);

    vsnprintf(errMsg, MAX_ERR_LEN, format, args);
    va_end(args);
}

