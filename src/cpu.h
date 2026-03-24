#pragma once

#include <fstream>
#include <vector>
#include <map>

#include "Instruction.h"

class CPU {
public:
    CPU (std::string source);
    ~CPU();
    void execute();
private:
    std::vector<Instruction> program;
    std::ifstream source;
    std::map<std::string, int> variables;
};