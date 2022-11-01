#include "lexer.hpp"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>


int compile(std::string filename){
    Lexer lexer = Lexer(filename);
    lexer.getNextToken();
    std::cout << lexer.current_lexeme;
    return 0;
}


int main(int argc, char *argv[]) {
    try 
    {
        if (argc > 2)
            throw std::invalid_argument("Too many arguments: expected 1, got " +
                                  std::to_string(argc - 1));
        else if (argc < 2) {
            throw std::invalid_argument("Not enough arguments: expected 1, got " +
                                  std::to_string(argc - 1));
        }
        compile(argv[1]);
    }
    catch (std::exception &err) 
    {
    std::cerr << "Error: " << err.what() << "\n";
    }
  return 0;
}
