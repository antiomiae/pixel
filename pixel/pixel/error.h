#ifndef PIXEL_ERROR_H
#define PIXEL_ERROR_H

#include <exception>
#include <string>
#include <pixel/util/util.h>

#define pixel_error(str) pixel::util::_error(__LINE__, __FILE__, str)
#define error_if(expr, str) pixel::util::_error_if(__LINE__, __FILE__, expr, str);
#define error_unless(expr, str) pixel::util::_error_if(__LINE__, __FILE__, !(expr), str);

namespace pixel
{

/** Base exception class. Requires error string for construction.
 */
class BaseError : std::exception
{
private:
    std::string msg_;
public:
    BaseError(std::string m)
        : msg_(std::move(m))
    { }


    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
};

/** Exception thrown when argument to method or function
 *  is not acceptable.
 */
class ArgumentError : BaseError
{
    using BaseError::BaseError;
};


inline void argument_error_if(bool expr, const std::string& msg)
{
    if (expr) {
        throw ArgumentError(msg);
    }
}


inline void argument_error_unless(bool expr, const std::string& msg)
{
    if (!expr) {
        throw ArgumentError(msg);
    }
}

}

#endif //PIXEL_ERROR_H
