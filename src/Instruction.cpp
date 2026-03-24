#include "Instruction.h"

Instruction::Instruction(std::string line) : type(""), dest(""), src1(""), src2("") {
    std::stringstream ss(line);
    ss >> type;
    if (type == "IN" || type == "OUT" || type == "OUTC") ss >> dest;
    else if (type == "BZ" || type == "BP" || type == "BN") ss >> src1 >> dest;
    else ss >> dest >> src1 >> src2; 
}

Instruction::Instruction(std::string op, std::string dest, std::string src1, std::string src2) 
    : type(op), dest(dest), src1(src1), src2(src2) {}