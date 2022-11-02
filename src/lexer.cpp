#include <algorithm>
#include <ostream>
#include <string>
#include "lexer.hpp"
#include "token.hpp"
#include "types.hpp"


Lexer::Lexer(std::string file_path)
{
    this->file_path = file_path;
};


bool Lexer::isDigit(char ch)
{
    return ((ch - '0' >= 0) && (ch - '0' <= 9));
}


/*
    symbol : one of
      ! % ^ ~ & * ( ) - + = [ ] { } | : ; < > , . / \ ' " @ # ` ?
*/
bool Lexer::isSymbol(char ch)
{
  return std::binary_search(symbols.begin(), symbols.end(), ch);
}


char Lexer::getNextChar()
{
    if (!source.is_open())
    {
        source.open(file_path, std::ios::in);
    }

    char c;
    c = source.get();
    return c;
};


void Lexer::ungetChar()
{
    source.unget();
}


Token Lexer::makeToken(token_type_t token_type)
{
    loc_t loc;
    loc.line = line;
    loc.col = col - current_lexeme.size();

    Token token = Token(token_type, current_lexeme, loc);
    return token;
}


Token Lexer::literal()
{
    char c = getNextChar();
    Token token = makeToken(token_type_t::NONE);
    if (isDigit(c))
    {
        current_lexeme.push_back(c);
        token = integerLiteral();
    }
    return token;
}


Token Lexer::integerLiteral()
{
    char c = getNextChar();
    Token token = makeToken(token_type_t::NUM);
    current_lexeme.push_back(c);
    subIntegerLiteral();
    return token;
}


void Lexer::subIntegerLiteral()
{
    char c = getNextChar();
    if (isDigit(c))
    {
        current_lexeme.push_back(c);
        subIntegerLiteral();
    }
    else
        ungetChar();
}


Token Lexer::operatorToken(char ch)
{
    Token token = makeToken(token_type_t::NONE);
    if (ch == '+')
        token = makeToken(token_type_t::ADD);
    return token;
}


Token Lexer::getNextToken()
{ 
    char c = getNextChar();
    Token token = makeToken(token_type_t::NONE);
    switch (c) {
        case '+':
            token = operatorToken(c);
            break;
        case '0'...'9':
            ungetChar();
            token = literal();
            break;
        case ' ':
        case '\n':
            current_lexeme.clear();
            token = getNextToken();
            break;
        default:
            token = makeToken(token_type_t::END_OF_FILE);
    }
    return token;
}



