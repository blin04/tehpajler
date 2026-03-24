#pragma once

#include <string>
#include <sstream>

class Instruction 
{
public:
    std::string type;
    std::string dest;
    std::string src1;
    std::string src2;

    Instruction(std::string line);
    Instruction(std::string op, std::string dest, std::string src1 = "", std::string src2 = "");
};