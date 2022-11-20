#ifndef TYPES_HPP
#define TYPES_HPP

#include <string>

typedef enum {
    NONE,
    END_OF_FILE,
    INT,
    ADD,
} token_type_t;

typedef std::string lexeme_t ;

typedef struct 
{
    int col;
    int line;
}loc_t;

#endif
