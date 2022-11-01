#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <fstream>
#include "token.hpp"

class Lexer
{
    private:
        std::fstream source;
        std::string file_path;

        unsigned int line = 0;
        unsigned int col = 0;

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
        Lexer(std::string file_path);

        Token getNextToken();
        lexeme_t current_lexeme;
};

#endif
