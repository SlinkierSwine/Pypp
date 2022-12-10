#include "types.hpp"
#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class BaseError : public std::exception
{
    public:
        BaseError(const char *error_name, const char *m, loc_t loc, const std::string& file_path)
        {
            std::stringstream ss;
            ss << error_name << ": " << m << "\n  File: " + file_path + "\n  Line: " + std::to_string(loc.line) + " col: " + std::to_string(loc.col);
            msg = ss.str();
        }

        const char* what() const throw()
        {
            return msg.c_str();
        }

    protected:
        std::string msg;
};


class SyntaxError : public BaseError
{
    public:
        SyntaxError(const char *m, loc_t loc, const std::string &file_path) : BaseError("SyntaxError", m, loc, file_path){};
};


class NameError : public BaseError
{
    public:
        NameError(const char *m, loc_t loc, const std::string &file_path) : BaseError("NameError", m, loc, file_path){};
};
