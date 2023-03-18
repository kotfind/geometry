#pragma once

#include <stdexcept>
#include <string>

class IOError : public std::runtime_error {
    public:
        IOError(const std::string& s) : std::runtime_error(s) {}
};
