#include "TEHParserVisitor.h"

class CodeGenVisitor : public TEHParserVisitor 
{
    public:
        virtual std::any visitProg(TEHParser::ProgContext *context) override;

        virtual std::any visitDecl(TEHParser::DeclContext *context) override;

        virtual std::any visitIfstatement(TEHParser::IfstatementContext *context) override;

        virtual std::any visitAssignstatement(TEHParser::AssignstatementContext *context) override;

        virtual std::any visitPrintstatement(TEHParser::PrintstatementContext *context) override;

        virtual std::any visitAddOp(TEHParser::AddOpContext *context) override;

        virtual std::any visitMulOp(TEHParser::MulOpContext *context) override;

        virtual std::any visitRelOp(TEHParser::RelOpContext *context) override;

        virtual std::any visitExpr(TEHParser::ExprContext *context) override;

        virtual std::any visitStatement(TEHParser::StatementContext *context) override;

};
