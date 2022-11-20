#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "types.hpp"
class Token
{
    private:
        
    public:
        token_type_t token_type;
        lexeme_t lexeme;
        loc_t loc;

        Token(token_type_t token_type, lexeme_t lexeme, loc_t loc);
        Token();
};

#endif
