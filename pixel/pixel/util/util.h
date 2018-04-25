//
//

#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include "symbol_map.h"

void _log_gl_errors(const int line, const char *file);

#define log_gl_errors() _log_gl_errors(__LINE__, __FILE__)

namespace pixel::util {
    bool file_exists(const std::string&);
    void _error(const int line, const char *file, const std::string &);
    void _error_if(const int line, const char *file, bool expr, const std::string &msg);
}

#endif //UTIL_UTIL_H
