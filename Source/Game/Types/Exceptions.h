#pragma once
#include <stdexcept>

class SystemInitException : public std::runtime_error
{
public:
    SystemInitException(const char* error_message)
        : std::runtime_error(error_message)
    {}
};
