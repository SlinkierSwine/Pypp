#include "parser.hpp"
#include "exceptions.cpp"
#include "types.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

Parser::Parser(Lexer &lexer) : lex(lexer){};


void Parser::throwSyntaxError(std::string expected, std::string got, loc_t loc)
{
    std::string msg = "Expected " + expected + ", got " + got +
        "\nFile: " + lex.file_path +
        "\nLine " + std::to_string(loc.line) + ", col " + std::to_string(loc.col);

    throw SyntaxError(msg);
};


void Parser::match(token_type_t type)
{
    if (currentToken.token_type != type)
        throwSyntaxError(std::to_string(type), currentToken.lexeme, currentToken.loc);
    nextToken();
}


bool Parser::checkTokenType(token_type_t type)
{
    return currentToken.token_type == type;
}


void Parser::nextToken()
{
    this->currentToken = lex.getNextToken();
}


bool Parser::isEOF(Token token)
{
    return token.token_type == token_type_t::END_OF_FILE;
}


void Parser::program()
{
    nextToken();
    while (not isEOF(currentToken))
        statement();
}


void Parser::statement()
{
    if (checkTokenType(token_type_t::INT))
    {
        std::cout << "NUMBER! " << currentToken.lexeme << '\n';
        nextToken();
    }
}
