#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "token.hpp"
#include "types.hpp"

class Parser
{
    private:
        Lexer& lex;
        Token currentToken = Token();
        
        void throwSyntaxError(std::string expected, std::string got, loc_t loc);
        void match(token_type_t type);
        bool checkTokenType(token_type_t type);
        void nextToken();
        bool isEOF(Token token);
        void statement();
        void expression();
        void term();
        void primary();
        
    public:
        Parser(Lexer &lexer);
        void program();
};

#endif

