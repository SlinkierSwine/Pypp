#ifndef LEXER_HPP
#define LEXER_HPP

#include <fstream>

class Lexer
{
    private:
        std::fstream source;
        char buffer[1];

    public:
        std::string file_path;

        Lexer(std::string file_path);

        char getNextChar();
};

#endif
