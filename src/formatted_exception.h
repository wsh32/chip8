/**
 * @file formatted_exception.h
 * @brief Extension of std::exception to allow dynamic exception messages. See
 * http://www.andrewewhite.net/wordpress/2007/07/30/formatted-exception-cpp/
 */

#ifndef FORMATTED_EXCEPTION_H
#define FORMATTED_EXCEPTION_H

#include <exception>
#include <stdio.h>
#include <stdarg.h>

#define MAX_ERR_LEN 256

/**
 * @class FormattedException
 * @brief Allows printf-like dynamic exception messages
 */
class FormattedException : public std::exception {
public:
    FormattedException(const char * format, ...);
    virtual const char * what() const throw() {return errMsg;};
    virtual ~FormattedException() throw() {};

private:
    char errMsg[MAX_ERR_LEN];
};

#endif

