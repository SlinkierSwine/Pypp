#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <fstream>
#include "token.hpp"

class Lexer
{
    private:
        std::fstream source;

        unsigned int line = 1;
        unsigned int col = 1;

        std::vector<char> symbols = {' ', '\t', '\n', '!',
            '%' , '^' , '~' , '&' ,
            '*' , '(' , ')' , '-' ,
            '+' , '=' , '[' , ']' ,
            '{' , '}' , '|' , ':' ,
            ';' , '<' , '>' , ',' ,
            '.' , '/' , '\\' , '\'' ,
            '"' , '@' , '`' , '?'};

        bool isDigit(char ch);
        bool isSymbol(char ch);

        char getNextChar();
        void ungetChar();
        Token makeToken(token_type_t token_type);

        Token literal();
        Token integerLiteral();
        void subIntegerLiteral();
        Token operatorToken(char ch);

    public:
        std::string file_path;
        lexeme_t current_lexeme;

        Lexer(std::string file_path);
        Token getNextToken();
};

#endif
