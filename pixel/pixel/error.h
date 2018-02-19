#ifndef PIXEL_ERROR_H
#define PIXEL_ERROR_H

#include <exception>
#include <string>

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
