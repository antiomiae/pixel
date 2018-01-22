//
//

#ifndef UTIL_UTIL_H
#define UTIL_UTIL_H

#include "symbol_map.h"

void _logGlErrors(const int line, const char *file);

#define logGlErrors() _logGlErrors(__LINE__, __FILE__)
#define error(str) pixel::util::_error(__LINE__, __FILE__, str)
#define error_if(expr, str) pixel::util::_error_if(__LINE__, __FILE__, expr, str);
#define error_unless(expr, str) pixel::util::_error_if(__LINE__, __FILE__, !(expr), str);

namespace pixel::util {
    bool file_exists(const std::string&);
    void _error(const int line, const char *file, const std::string &);
    void _error_if(const int line, const char *file, bool expr, const std::string &msg);
}

#endif //UTIL_UTIL_H
