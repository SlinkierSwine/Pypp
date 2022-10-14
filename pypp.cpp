#include <exception>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include "lexer.hpp"

int main(int argc, char *argv[]) {
    try
    {
        if (argc > 2)
            throw std::invalid_argument("Too many arguments: expected 1, got " + std::to_string(argc - 1));
        else if (argc < 2) {
            throw std::invalid_argument("Not enough arguments: expected 1, got " + std::to_string(argc - 1));
        }

        Lexer l = Lexer(argv[1]);
        std::cout << l.file_path;
    }
    catch (std::exception& err)
    {
        std::cerr << "Error: " << err.what() << "\n";
    }
    return 0;
}
