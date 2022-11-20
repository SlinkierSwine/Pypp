#include <stdexcept>
#include <string>

class SyntaxError : public std::runtime_error
{
    public:
        SyntaxError(std::string const& msg) : runtime_error("SyntaxError: " + msg){}
};
