#include "wibo_exceptions.hpp"

using std::exception;

WiboException::WiboException(const char *msg, const char *fn, int line)
    : message(msg), source_function(fn), source_line(line)
{
    return_code = get_ret_code();
}

const char *WiboException::get_src_func()
{
    return source_function;
}

int WiboException::get_src_line() { return source_line; }

int WiboException::get_ret_code() { return return_code; }

const char *WiboException::what() const noexcept
{
    return message;
}
