#include "lexer.hpp"
#include <string>

Lexer::Lexer(std::string file_path)
{
    this->file_path = file_path;
};

char Lexer::getNextChar()
{
    if (!source.is_open())
    {
        source.open(file_path, std::ios::in);
    }
    source.read(buffer, 1);
};
