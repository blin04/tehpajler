#include "CodeGenVisitor.h"

void CodeGenVisitor::setOutputLocation(const std::string &filename) 
{
    output_location = filename;
}

Instruction* CodeGenVisitor::addInstruction(std::string op, std::string dest, std::string src1, std::string src2) 
{
    instructions.push_back(new Instruction(op, dest, src1, src2));
    return instructions.back();
}

void CodeGenVisitor::generateToFile(const std::string &filename) 
{
    std::ofstream out(filename);
    for (Instruction* instr : instructions) {
        out << instr->type << " " << instr->dest << " " << instr->src1 << " " << instr->src2 << std::endl;
    }
    out.close();
}

std::string CodeGenVisitor::getNextRegister() 
{
    return "r" + std::to_string(nextFreeRegister++);
}

void CodeGenVisitor::setRegisterForVariable(const std::string &varName) 
{
    variableToRegister[varName] = nextFreeRegister++;
}

std::string CodeGenVisitor::getRegisterForVariable(const std::string &varName) 
{
    return "r" + std::to_string(variableToRegister[varName]);
}

std::any CodeGenVisitor::visitProg(TEHParser::ProgContext *context) 
{
    for (auto decl : context->decl())
    {
        decl->accept(this);
    }

    for (auto statement : context->statement())
    {
        statement->accept(this);
    }
    generateToFile(output_location);
    for (Instruction* instr : instructions) {
        std::cout << instr->type << " " << instr->dest << " " << instr->src1 << " " << instr->src2 << std::endl;
    }
    return 0;
}

std::any CodeGenVisitor::visitDecl(TEHParser::DeclContext *context) 
{
    auto identifiers = context->IDENT();
    for (size_t i = 1; i < identifiers.size(); ++i)
    {
        setRegisterForVariable(identifiers[i]->getText());
    }
    return 0;
}

// returns string, if it starts with r it's a register, otherwise it's a constant
std::any CodeGenVisitor::visitExpr(TEHParser::ExprContext *context) 
{
    // case for INT 
    if (context->INT() != nullptr)
    {
        std::string temp_reg = getNextRegister();
        addInstruction("SUB", temp_reg, temp_reg, temp_reg); // temp_reg = 0
        addInstruction("ADDI", temp_reg, temp_reg, context->INT()->getText()); // temp_reg = int_value
        return temp_reg;
    }
    // case for IDENT
    if (context->IDENT() != nullptr)
    {
        return getRegisterForVariable(context->IDENT()->getText());
    }


    auto subExpressions = context->expr();

    // case for ( expr )
    if (subExpressions.size() == 1)
    {
        return std::any_cast<std::string>(subExpressions[0]->accept(this));
    }

    if (subExpressions.size() != 2)
    {
        return std::string("invalid");
    }


    std::string left = std::any_cast<std::string>(subExpressions[0]->accept(this));
    std::string right = std::any_cast<std::string>(subExpressions[1]->accept(this));
    std::string resultRegister = getNextRegister();

    if (context->mulOp() != nullptr)
    {
        std::string op = context->mulOp()->MUL() != nullptr ? "MUL" : "DIV";
        addInstruction(op, resultRegister, left, right);
        return resultRegister;
    }

    if (context->addOp() != nullptr)
    {
        std::string op = context->addOp()->ADD() != nullptr ? "ADD" : "SUB";
        addInstruction(op, resultRegister, left, right);
        return resultRegister;
    }

    if (context->relOp() != nullptr && context->relOp()->EQUALS() != nullptr)
    {
        std::string t1 = getNextRegister();
        std::string t2 = getNextRegister();
        std::string t3 = getNextRegister();
        std::string t4 = getNextRegister();

        addInstruction("SUB", t1, left, right); // t1 = left - right
        addInstruction("MUL", t2, t1, t1); // t2 = (left - right)^2
        addInstruction("ADDI", t3, t2, "1"); // t3 = (left - right)^2 + 1
        addInstruction("SUB", t4, t4, t4); // t4 = 0
        addInstruction("ADDI", t4, t4, "1"); // t4 = 1
        addInstruction("DIV", resultRegister, t4, t3); // result = 1 / ((left - right)^2 + 1), will be 1 if left ==
        
        return resultRegister;
    }
}

std::any CodeGenVisitor::visitIfstatement(TEHParser::IfstatementContext *context) 
{
    std::string condition = std::any_cast<std::string>(context->expr()->accept(this));

    Instruction* ifJump = addInstruction("BZ", condition, ""); // jump location to be filled later

    for (auto statement : context->statement())
    {
        statement->accept(this);
    }

    int jumpLocation = instructions.size() + 1;
    ifJump->src1 = std::to_string(jumpLocation);

    if (context->elsestatement() != nullptr)
    {
        std::string tempReg = getNextRegister();
        addInstruction("SUB", tempReg, tempReg, tempReg); // tempReg = 0
        Instruction* elseJump = addInstruction("BZ", tempReg, ""); 
        context->elsestatement()->accept(this);
        elseJump->src1 = std::to_string(instructions.size() + 1);
    }

    return 0;
}

std::any CodeGenVisitor::visitElsestatement(TEHParser::ElsestatementContext *context)
{
    for (auto statement : context->statement())
    {
        statement->accept(this);
    }
    return 0;
}

std::any CodeGenVisitor::visitAssignstatement(TEHParser::AssignstatementContext *context) 
{
    if (context->expr() != nullptr)
    {
        std::string value = std::any_cast<std::string>(context->expr()->accept(this));
        std::string dest = getRegisterForVariable(context->IDENT()->getText());
        addInstruction("ADDI", dest, value, "0");
    }
    return 0;
}

std::any CodeGenVisitor::visitPrintstatement(TEHParser::PrintstatementContext *context) 
{
    if (context->expr() != nullptr)
    {
        std::string value = std::any_cast<std::string>(context->expr()->accept(this));
        addInstruction("OUT", value);
    }
    if (context->STRING() != nullptr)
    {
        std::string output_string = context->STRING()->getText();
        for (int i = 1; i < output_string.size()-1; ++i)
        {
            addInstruction("OUTC", std::to_string((int)output_string[i]));
        }
        addInstruction("OUTC", std::to_string((int)'\n'));
    }
    return 0;
}

std::any CodeGenVisitor::visitAddOp(TEHParser::AddOpContext *context) 
{
    return 0;
}

std::any CodeGenVisitor::visitMulOp(TEHParser::MulOpContext *context) 
{
    return 0;
}

std::any CodeGenVisitor::visitRelOp(TEHParser::RelOpContext *context) 
{
    return 0;
}

std::any CodeGenVisitor::visitStatement(TEHParser::StatementContext *context) 
{
     if (context->ifstatement() != nullptr)
    {
        return context->ifstatement()->accept(this);
    }

    if (context->assignstatement() != nullptr)
    {
        return context->assignstatement()->accept(this);
    }

    if (context->printstatement() != nullptr)
    {
        return context->printstatement()->accept(this);
    }
    return 0;
}
