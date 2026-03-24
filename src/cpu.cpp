#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>


class Instruction {
public:
    std::string type;
    std::string dest;
    std::string src1;
    std::string src2;

    Instruction(std::string line) : type(""), dest(""), src1(""), src2("") {
        std::stringstream ss(line);
        ss >> type;
        if (type == "IN" || type == "OUT" || type == "OUTC") ss >> dest;
        else if (type == "BZ" || type == "BP" || type == "BN") ss >> src1 >> dest;
        else ss >> dest >> src1 >> src2; 
    }
};

int main() 
{
    std::ifstream source("../build/program.out", std::ios::in);    

    std::vector<Instruction> program;
    for (std::string line; std::getline(source, line);) {
        program.push_back(Instruction(line));
    }

    std::map<std::string, int> variables;

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
            break;
        }
        pc++;
    }
    return 0;
}