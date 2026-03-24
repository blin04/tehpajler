#include "Instruction.h"

Instruction::Instruction(std::string op, std::string dest, std::string src1, std::string src2) 
    : type(op), dest(dest), src1(src1), src2(src2) {}