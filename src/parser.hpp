#ifndef PARSER_HPP
#define PARSER_HPP

#include <map>
#include <string>
#include "emitter.hpp"
#include "lexer.hpp"
#include "token.hpp"
#include "types.hpp"

class Parser
{
    private:
        Lexer& lex;
        Emitter& emitter;
        Token currentToken = Token();
        std::map<lexeme_t, var_t> vars;
        
        var_t addOrUpdateVars(lexeme_t lexeme, std::string value, token_type_t type);
        var_t addVar(lexeme_t lexeme, std::string value, token_type_t type);
        var_t updateVar(lexeme_t lexeme, std::string value);
        void match(token_type_t type);
        bool checkTokenType(token_type_t type);
        void checkVarNameError(lexeme_t lexeme);
        bool checkVar(lexeme_t lexeme);
        void nextToken();
        bool isEOF(Token token);
        void statement();
        void expression();
        void term();
        void operand(bool is_left);
        
    public:
        Parser(Lexer &lexer, Emitter &emitter);
        void program();
};

#endif

