#include "token.hpp"
#include "types.hpp"

Token::Token(token_type_t token_type, lexeme_t lexeme, loc_t loc)
{
    this->token_type = token_type;
    this->lexeme = lexeme;
    this->loc = loc;
}


Token::Token()
{
    this->token_type = token_type_t::NONE;
    this->lexeme = "";
    this->loc = loc_t{0, 0};
}
