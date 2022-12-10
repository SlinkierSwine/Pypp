#include "emitter.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "types.hpp"
#include <algorithm>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <sys/types.h>
#include <unistd.h>


bool compile(std::string filename, std::string output_filename)
{
    Lexer lexer = Lexer(filename);
    Emitter emitter = Emitter(output_filename + ".asm");
    Parser parser = Parser(lexer, emitter);
    parser.program();
    emitter.writeFile();
    return true;
}


void assemble(std::string output_filename)
{
    std::string assembler = "/usr/bin/nasm";
    std::string option1 = "-f elf";
    int status;
    std::string output = output_filename + ".asm";
    char *ps_argv[3];
    ps_argv[0] = const_cast<char*>("nasm");
    ps_argv[1] = const_cast<char*>(option1.c_str());
    ps_argv[2] = const_cast<char*>(output.c_str());
    ps_argv[3] = 0;
    pid_t pid = fork();

    switch(pid){
        case -1:
            std::cout <<"fork() failed\n";
            return;
        case 0:
            execvp(assembler.c_str(), ps_argv);
            return;
    }
}


void link(std::string output_filename)
{
    std::string linker = "/usr/bin/ld";
    std::string option1 = "-melf_i386";
    std::string option2 = "-o";
    int status;
    std::string object_filename = output_filename + ".o";
    char *ps_argv[6];
    ps_argv[0] = const_cast<char*>("ld");
    ps_argv[1] = const_cast<char*>(option1.c_str());
    ps_argv[2] = const_cast<char*>(object_filename.c_str());
    ps_argv[3] = const_cast<char*>(option2.c_str());
    ps_argv[4] = const_cast<char*>(output_filename.c_str());
    ps_argv[5] = 0;
    pid_t pid = fork();

    switch(pid){
        case -1:
            std::cout <<"fork() failed\n";
            return;
        case 0:
            execvp(linker.c_str(), ps_argv);
            return;
    }
}


int main(int argc, char *argv[]) 
{
    try 
    {
        if (argc > 4)
            throw std::invalid_argument("Too many arguments: expected 1, got " +
                                  std::to_string(argc - 1));
        else if (argc < 2) {
            throw std::invalid_argument("Not enough arguments: expected 1, got " +
                                  std::to_string(argc - 1));
        }
        std::string filename = argv[1];
        std::string output_filename = "";
        if (argv[2])
        {
            if (std::string(argv[2]) == "-f")
                output_filename = argv[3];
        }
        else
            output_filename = "output";
        compile(filename, output_filename);
        assemble(output_filename);
        link(output_filename);
    }
    catch (std::exception &err) 
    {
    std::cerr << err.what() << "\n";
    }
  return 0;
}
