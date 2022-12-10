#ifndef LEXER_HPP
#define LEXER_HPP

#include <vector>
#include <fstream>
#include <unordered_map>
#include "token.hpp"
#include "types.hpp"

class Lexer
{
    private:
        std::fstream source;

        int line = 1;
        int col = 1;
        int prev_col = 1;
        bool line_changed = false;

        std::vector<char> symbols = {' ', '\t', '\n', '!',
            '%' , '^' , '~' , '&' ,
            '*' , '(' , ')' , '-' ,
            '+' , '=' , '[' , ']' ,
            '{' , '}' , '|' , ':' ,
            ';' , '<' , '>' , ',' ,
            '.' , '/' , '\\' , '\'' ,
            '"' , '@' , '`' , '?'};
        std::unordered_map<std::string, token_type_t> keywords = {
            {"print", token_type_t::PRINT},
            {"var", token_type_t::VAR},
        };

        bool isDigit(char ch);
        bool isSymbol(char ch);
        bool isNonDigit(char ch);

        char getNextChar();
        void ungetChar();
        loc_t getCurrentLoc();
        loc_t getCurrentTokenLoc();
        Token makeToken(token_type_t token_type);

        Token literal();

        Token integerLiteral();
        void subIntegerLiteral();
        
        Token stringLiteral();
        void subStringLiteral();

        Token operatorToken();

        Token identificator();
        void subIdentificator();

    public:
        std::string file_path;
        lexeme_t current_lexeme;

        Lexer(std::string file_path);
        Token getNextToken();
};

#endif
