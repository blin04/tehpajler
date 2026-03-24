#include "TEHParserVisitor.h"
#include "Instruction.h"

class CodeGenVisitor : public TEHParserVisitor 
{
    public:
        virtual std::any visitProg(TEHParser::ProgContext *context) override;

        // gives each variable a unique register, no return value
        virtual std::any visitDecl(TEHParser::DeclContext *context) override;

        // only adds instructions, no return value
        virtual std::any visitIfstatement(TEHParser::IfstatementContext *context) override;

        // TODO: handle else block when branch codegen is implemented
        virtual std::any visitElsestatement(TEHParser::ElsestatementContext *context) override;

        // only adds instructions, no return value
        virtual std::any visitAssignstatement(TEHParser::AssignstatementContext *context) override;

        // only adds instructions, no return value
        virtual std::any visitPrintstatement(TEHParser::PrintstatementContext *context) override;

        // nothing
        virtual std::any visitAddOp(TEHParser::AddOpContext *context) override;

        // nothing
        virtual std::any visitMulOp(TEHParser::MulOpContext *context) override;

        // nothing
        virtual std::any visitRelOp(TEHParser::RelOpContext *context) override;

        // adds instructions and returns the name of the register where the result is stored, or constant value
        virtual std::any visitExpr(TEHParser::ExprContext *context) override;

        // only adds instructions, no return value
        virtual std::any visitStatement(TEHParser::StatementContext *context) override;

    private:
        std::map<std::string, int> variableToRegister;
        std::vector<Instruction*> instructions;
        int nextFreeRegister = 0;
        std::string output_location = "program.out";

        void setOutputLocation(const std::string &filename);

        Instruction* addInstruction(std::string op, std::string dest, std::string src1 = "", std::string src2 = "");

        void generateToFile(const std::string &filename);

        std::string getNextRegister();

        void setRegisterForVariable(const std::string &varName);

        std::string getRegisterForVariable(const std::string &varName);
};
