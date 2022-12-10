#include <algorithm>
#include <iostream>
#include <ostream>
#include <string>
#include "lexer.hpp"
#include "types.hpp"
#include "exceptions.cpp"


Lexer::Lexer(std::string file_path)
{
    this->file_path = file_path;
}


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


bool Lexer::isNonDigit(char ch)
{
  return (ch == '_'
          || (ch >= 'a' && ch <= 'z')
          || (ch >= 'A' && ch <= 'Z'));
}


char Lexer::getNextChar()
{
    if (!source.is_open())
    {
        source.open(file_path, std::ios::in);
    }

    char c;
    c = source.get();
    line_changed = false;
    if (c == '\n')
    {
        prev_col = col;
        col = 1;
        line++;
        line_changed = true;
    }
    else
        col++;
    return c;
}


void Lexer::ungetChar()
{
    source.unget();
    if (line_changed)
    {
        col = prev_col;
        line--;
    }
    else
        col--;
}


loc_t Lexer::getCurrentTokenLoc()
{
    loc_t loc = getCurrentLoc();
    loc.col -= current_lexeme.size();
    return loc;
}


loc_t Lexer::getCurrentLoc()
{
    loc_t loc;
    loc.line = line;
    loc.col = col;
    return loc;
}


Token Lexer::makeToken(token_type_t token_type)
{
    loc_t loc = getCurrentTokenLoc();
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

    else if (c == '"') 
    {
        token = stringLiteral();
    }

    current_lexeme.clear();
    return token; }


Token Lexer::integerLiteral()
{
    subIntegerLiteral();
    Token token = makeToken(token_type_t::INT);
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


Token Lexer::stringLiteral()
{
    subStringLiteral();
    Token token = makeToken(token_type_t::STR);
    return token;
}


void Lexer::subStringLiteral()
{
    char c = getNextChar();
    if (c == '\n')
    {
        ungetChar();
        loc_t loc = getCurrentLoc();
        throw SyntaxError("No closing quotes", loc, file_path);
    }
    if (c != '"')
    {
        current_lexeme.push_back(c);
        subStringLiteral();
    }
}


Token Lexer::identificator()
{
    subIdentificator();
    Token token = makeToken(token_type_t::IDENT);

    std::unordered_map<std::string, token_type_t>::iterator find_it = keywords.find(current_lexeme);
    if (find_it != keywords.end())
    {
        token.token_type = find_it->second;
    }

    current_lexeme.clear();

    return token;
}


void Lexer::subIdentificator()
{
    char c = getNextChar();
    if (isNonDigit(c) || isDigit(c))
    {
        current_lexeme.push_back(c);
        subIdentificator();
    }
    else
        ungetChar();
}


Token Lexer::operatorToken()
{
    char c = getNextChar();
    Token token = makeToken(token_type_t::NONE);
    if (c == '+')
        token = makeToken(token_type_t::PLUS);
    else if (c == '=')
        token = makeToken(token_type_t::EQ);
    else if (c == '-')
        token = makeToken(token_type_t::MINUS);
    return token;
}


Token Lexer::getNextToken()
{ 
    char c = getNextChar();
    Token token = makeToken(token_type_t::NONE);
    switch (c) {
        case '=':
        case '+':
        case '-':
            ungetChar();
            token = operatorToken();
            break;
        case '"':
        case '0'...'9':
            ungetChar();
            token = literal();
            break;
        case 'a'...'z':
        case 'A'...'Z':
            ungetChar();
            token = identificator();
            break;
        case '\n':
            current_lexeme.clear();
            current_lexeme.push_back('\n');
            token = makeToken(token_type_t::NEWLINE);
            current_lexeme.clear();
            break;
        case ' ':
        case '\t':
            current_lexeme.clear();
            token = getNextToken();
            break;
        case EOF:
            token = makeToken(token_type_t::END_OF_FILE);
            break;
        default:
            std::string msg = "Unexpected character: \"";
            msg += c;
            msg += "\"";
            throw SyntaxError(msg.c_str(), token.loc, file_path);
    }
    return token;
}



