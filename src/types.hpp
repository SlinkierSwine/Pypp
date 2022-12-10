#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

#define TYPE_ENUM(NAME)\
    NAME(NONE),\
    NAME(END_OF_FILE),\
    NAME(INT),\
    NAME(STR),\
    NAME(IDENT),\
    NAME(EQ),\
    NAME(PLUS),\
    NAME(PRINT),\
    NAME(VAR),\
    NAME(MINUS),\
    NAME(NEWLINE)\

#define MAKE_ENUM(VAR) VAR
typedef enum {
    TYPE_ENUM(MAKE_ENUM)
} token_type_t;

#define MAKE_ENUM_NAMES(VAR) #VAR
const char* const TokenTypeNames[] {
    TYPE_ENUM(MAKE_ENUM_NAMES)
};

typedef std::string lexeme_t ;

typedef struct 
{
    int col;
    int line;
}loc_t;

typedef struct
{
    lexeme_t lexeme;
    std::string value;
    token_type_t type;
}var_t;

#endif
