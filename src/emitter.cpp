#include "emitter.hpp"
#include "types.hpp"
#include <fstream>
#include <iostream>
#include <string>

Emitter::Emitter(std::string output_filename)
{
    this->out = output_filename;
}


std::string Emitter::getCode()
{
    std::string code = text + main + bss + data;
    return code;
}


void Emitter::emitData(std::string line, int tabsize)
{
    std::string tabs(tabsize, '\t');
    data.append(tabs + line + '\n');
}


void Emitter::emitBss(std::string line, int tabsize)
{
    std::string tabs(tabsize, '\t');
    bss.append(tabs + line + '\n');
}


void Emitter::emitLine(std::string line, int tabsize)
{
    std::string tabs(tabsize, '\t');
    main.append(tabs + line + '\n');
}


void Emitter::emitPrint(std::string name)
{
    emitLine("mov ecx, " + name);
    emitLine("mov ebx, 1");
    emitLine("mov eax, 4");
    emitLine("int 0x80");
}


void Emitter::emitPrintNewLine()
{
    emitLine("mov edx, 1");
    emitPrint("?_nl");
}


void Emitter::emitPrintString(std::string string)
{
    std::string name = "?_" + std::to_string(print_cout);
    print_cout++;
    emitData(name + ": db \"" + string + "\"");
    emitLine("mov edx, " + std::to_string(string.length()));
    emitPrint(name);
    emitPrintNewLine();
}


void Emitter::emitPrintVar(lexeme_t lexeme)
{
    emitLine("mov eax, [" + lexeme + "]");
    emitLine("push eax");
    emitLine("call _iprint");
    emitLine("pop eax");
    emitLine("mov [" + lexeme + "], eax");
    emitPrintNewLine();
}


void Emitter::emitIntVar(var_t var)
{
    emitBss(var.lexeme + ": resb 4");
    emitUpdateVar(var);
}


void Emitter::emitUpdateVar(var_t var)
{
    emitLine("mov word [" + var.lexeme + "], " + var.value);
}


void Emitter::writeFile()
{
    emitLine("mov eax, 1");
    emitLine("int 0x80");

    std::ofstream output;
    std::string code = getCode();
    output.open(out);
    output << code;
    output.close();
}
