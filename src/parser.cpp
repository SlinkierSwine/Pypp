#include "parser.hpp"
#include "emitter.hpp"
#include "exceptions.cpp"
#include "types.hpp"
#include <cstdlib>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>

Parser::Parser(Lexer &lexer, Emitter &emitter) : lex(lexer), emitter(emitter){};


void Parser::match(token_type_t type)
{
    if (currentToken.token_type != type)
    {
        std::string msg = "Expected " + std::string(TokenTypeNames[type]) + ", got \"" + currentToken.lexeme + "\"";
        throw SyntaxError(msg.c_str(), currentToken.loc, lex.file_path);
    }
    nextToken();
}


var_t Parser::updateVar(lexeme_t lexeme, std::string value)
{
    std::map<lexeme_t, var_t>::iterator it = vars.find(lexeme);
    it->second.value = value;
    return it->second;
}


var_t Parser::addVar(lexeme_t lexeme, std::string value, token_type_t type)
{
    var_t variable;
    variable.lexeme = lexeme;
    variable.type = type;
    variable.value = value;
    vars.insert(std::make_pair(lexeme, variable));
    return variable;
}


bool Parser::checkTokenType(token_type_t type)
{
    return currentToken.token_type == type;
}


bool Parser::checkVar(lexeme_t lexeme)
{
    std::map<lexeme_t, var_t>::iterator it = vars.find(lexeme);
    return it != vars.end();
}


void Parser::checkVarNameError(lexeme_t lexeme)
{
    if (not checkVar(lexeme))
    {
        std::string msg = "Name \"" + lexeme + "\" wasn't found";
        throw NameError(msg.c_str(), currentToken.loc, lex.file_path);
    }
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
    if (checkTokenType(token_type_t::VAR))
    {
        nextToken();
        std::string lexeme = currentToken.lexeme;
        nextToken();
        match(token_type_t::EQ);
        if (checkTokenType(token_type_t::INT))
        {
            if (checkVar(lexeme))
            {
                std::string msg = "Redefinition of \"" + lexeme + "\"";
                throw NameError(msg.c_str(), currentToken.loc, lex.file_path);
            }
            else
            {
                var_t var = addVar(lexeme, currentToken.lexeme, currentToken.token_type);
                emitter.emitIntVar(var);
            }
        }
        else
        {
            std::string msg = "Expected INT, got \"" + currentToken.lexeme + "\"";
            throw SyntaxError(msg.c_str(), currentToken.loc, lex.file_path);
        }
    }

    else if (checkTokenType(token_type_t::PRINT))
    {
        nextToken();
        if (checkTokenType(token_type_t::INT) || checkTokenType(token_type_t::STR))
        {
            emitter.emitPrintString(currentToken.lexeme);
        }
        else if (checkTokenType(token_type_t::IDENT)) 
        {
            checkVarNameError(currentToken.lexeme);
            emitter.emitPrintVar(currentToken.lexeme);
        }
    }
    
    else if (checkTokenType(token_type_t::IDENT))
    {
        checkVarNameError(currentToken.lexeme);
        lexeme_t lexeme = currentToken.lexeme;
        nextToken();
        match(token_type_t::EQ);
        emitter.emitLine("mov eax, 0");
        expression();
        emitter.emitLine("mov [" + lexeme + "], eax");
    }

    nextToken();
}


void Parser::expression()
{
    operand(true);
    nextToken();
    if (checkTokenType(token_type_t::PLUS) || checkTokenType(token_type_t::MINUS))
    {
        while (not checkTokenType(token_type_t::NEWLINE))
        {
            term();
            emitter.emitLine("mov ebx, 0");
        }
    }
    else
        emitter.emitLine("add eax, ebx");
}


void Parser::term()
{
    operand(false);
    nextToken();
    emitter.emitLine("add ebx, ecx");
    emitter.emitLine("add eax, ebx");
}


void Parser::operand(bool is_left)
{
    bool is_negative = false;
    std::string reg;
    if (is_left)
        reg = "ebx";
    else
        reg = "ecx";

    if (checkTokenType(token_type_t::MINUS))
    {
        is_negative = true;
        nextToken();
    }

    else if (not is_left)
        match(token_type_t::PLUS);

    if (checkTokenType(token_type_t::IDENT))
    {
        checkVarNameError(currentToken.lexeme);
        emitter.emitLine("mov " + reg + ", [" + currentToken.lexeme + "]");       
        if (is_negative)
            emitter.emitLine("neg " + reg);   
    }

    else if (checkTokenType(token_type_t::INT))
    {
        if (is_negative)
            emitter.emitLine("mov " + reg + ", -" + currentToken.lexeme);
        else
            emitter.emitLine("mov " + reg + ", " + currentToken.lexeme);
    }

    else
    {
        std::string msg = "Expected INT or IDENT, got " + std::string(TokenTypeNames[currentToken.token_type]);
        throw SyntaxError(msg.c_str(), currentToken.loc, lex.file_path);
    }
}
