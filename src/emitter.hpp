#ifndef EMITTER_HPP
#define EMITTER_HPP

#include "types.hpp"
#include <string>

class Emitter
{
    private:
        std::string out;
        std::string main = "\t_start:\n";
        std::string data = "section .data\n\t?_nl: db 0xA\n";
        std::string text = "section .text\n\
\tglobal _start\n\
\t_cprint:\n\
\t\tpushad\n\
\t\tmov [?_c], eax\n\
\t\tmov eax, 0x04\n\
\t\tmov ebx, 1\n\
\t\tmov ecx, ?_c\n\
\t\tmov edx, 1\n\
\t\tint 0x80\n\
\t\tpopad\n\
\t\tret\n\
\t_dprint:\n\
\t\tpushad\n\
\t\tadd eax, '0'\n\
\t\tmov [?_c], eax\n\
\t\tmov eax, 0x04\n\
\t\tmov ebx, 1\n\
\t\tmov ecx, ?_c\n\
\t\tmov edx, 1\n\
\t\tint 0x80\n\
\t\tpopad\n\
\t\tret\n\
\t_iprint:\n\
\t\tpushad\n\
\t\tcmp eax, 0\n\
\t\tjge Pos\n\
\t\tpush eax\n\
\t\tmov eax, '-'\n\
\t\tcall _cprint\n\
\t\tpop eax\n\
\t\tneg eax\n\
\tPos:\n\
\t\tmov ebx, 10\n\
\t\tmov ecx, 1\n\
\tCycle1:\n\
\t\tmov edx, 0\n\
\t\tdiv ebx\n\
\t\tpush edx\n\
\t\tcmp eax, 0\n\
\t\tjz EndLoop1\n\
\t\tinc ecx\n\
\t\tjmp Cycle1\n\
\tEndLoop1:\n\
\tCycle2:\n\
\t\tpop eax\n\
\t\tcall _dprint\n\
\t\tdec ecx\n\
\t\tjz EndLoop2\n\
\t\tjmp Cycle2\n\
\tEndLoop2:   \n\
\t\tpopad\n\
\t\tret\n";

        std::string bss  = "section .bss\n\t?_output_buffer: resb 4\n\t?_c: resb 1\n";
        int print_cout = 1;

        std::string getCode();

    public:
        Emitter(std::string output_filename);

        void emitLine(std::string line, int tabsize=2);
        void emitData(std::string line, int tabsize=1);
        void emitBss(std::string line, int tabsize=1);

        void emitPrint(std::string name);
        void emitPrintNewLine();
        void emitPrintString(std::string string);
        void emitPrintVar(lexeme_t lexeme);

        void emitIntVar(var_t variable);
        void emitUpdateVar(var_t variable);

        void writeFile();
};

#endif
