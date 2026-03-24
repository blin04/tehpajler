#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>

#include "Instruction.h"
#include "cpu.h"

CPU::CPU(std::string source) {
    this->source.open(source, std::ios::in);
    for (std::string line; std::getline(this->source, line);) {
        program.push_back(Instruction(line));
    }
}

CPU::~CPU() { this->source.close(); }

void CPU::execute()  {
    int pc = 0;
    while (pc < program.size()) {
        if (program[pc].type == "ADD")
            variables[program[pc].dest] = variables[program[pc].src1] + variables[program[pc].src2];
        else if (program[pc].type == "ADDI")
            variables[program[pc].dest] = variables[program[pc].src1] + std::stoi(program[pc].src2);
        else if (program[pc].type == "SUB")
            variables[program[pc].dest] = variables[program[pc].src1] - variables[program[pc].src2];
        else if (program[pc].type == "SUBI")
            variables[program[pc].dest] = variables[program[pc].src1] - std::stoi(program[pc].src2);
        else if (program[pc].type == "MUL")
            variables[program[pc].dest] = variables[program[pc].src1] * variables[program[pc].src2]; 
        else if (program[pc].type == "MULI")
            variables[program[pc].dest] = variables[program[pc].src1] * std::stoi(program[pc].src2);
        else if (program[pc].type == "DIV")
            variables[program[pc].dest] = variables[program[pc].src1] / variables[program[pc].src2]; 
        else if (program[pc].type == "DIVI")
            variables[program[pc].dest] = variables[program[pc].src1] / std::stoi(program[pc].src2);
        else if (program[pc].type == "BZ") {
            if (variables[program[pc].src1] == 0) {
                pc = std::stoi(program[pc].dest) - 1;
                continue;
            }
        }
        else if (program[pc].type == "BP") {
            if (variables[program[pc].src1] > 0) {
                pc = std::stoi(program[pc].dest) - 1;
                continue;
            }
        }
        else if (program[pc].type == "BN") {
            if (variables[program[pc].src1] < 0) {
                pc = std::stoi(program[pc].dest) - 1;
                continue;
            }
        }
        else if (program[pc].type == "IN")
            std::cin >> variables[program[pc].dest];   
        else if (program[pc].type == "OUT")
            std::cout << variables[program[pc].dest] << std::endl;
        else if (program[pc].type == "OUTC")
            std::cout << (char)(std::stoi(program[pc].dest));
        else {
            // error
            std::cout << "error: unknown instruction " << program[pc].type << std::endl;
            return;
        }
        pc++;
    }
}