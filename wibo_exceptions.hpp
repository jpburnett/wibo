#ifndef WIBO_EXCEPTIONS_HPP
#define WIBO_EXCEPTIONS_HPP

#include <exception>

enum class WiboExceptionCode : int {
    WIBO_EXCEPTION_CODE_SUCCESS = 0,
    WIBO_EXCEPTION_CODE_FAILURE = 1
};

struct WiboException : std::exception
{

public:
    WiboException(const char *msg, const char *fn, int line);

    const char *get_src_func();

    int get_src_line();

    int get_ret_code();

    virtual const char *what() const noexcept override;

private:
    const char *message;
    const char *source_function;
    int source_line;
    int return_code;
};

template <typename ExceptionType>
struct WiboTemplatedException : WiboException
{
private:
    static constexpr int ret_code = ExceptionType::ret_code;

public:
    WiboTemplatedException(const char *msg, const char *fn, int line);
};

template <typename ExceptionType>
struct WiboTemplatedException : WiboException
{
private:
    static constexpr int ret_code = static_cast<int>(ExceptionType::WIBO_EXCEPTION_CODE_FAILURE);

public:
    WiboTemplatedException(const char *msg, const char *fn, int line)
        : WiboException(msg, fn, line)
    {
    }
};  

#endif