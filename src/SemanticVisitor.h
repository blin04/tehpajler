#include "TEHParserVisitor.h"

#include <vector>
#include <string>
#include <set>
#include <unordered_map>

class SemanticVisitor : public TEHParserVisitor 
{
    public:
        virtual std::any visitProg(TEHParser::ProgContext *context) override;

        virtual std::any visitDecl(TEHParser::DeclContext *context) override;

        virtual std::any visitIfstatement(TEHParser::IfstatementContext *context) override;

        virtual std::any visitElsestatement(TEHParser::ElsestatementContext *context) override;

        virtual std::any visitAssignstatement(TEHParser::AssignstatementContext *context) override;

        virtual std::any visitPrintstatement(TEHParser::PrintstatementContext *context) override;

        virtual std::any visitAddOp(TEHParser::AddOpContext *context) override;

        virtual std::any visitMulOp(TEHParser::MulOpContext *context) override;

        virtual std::any visitRelOp(TEHParser::RelOpContext *context) override;

        virtual std::any visitExpr(TEHParser::ExprContext *context) override;

        virtual std::any visitStatement(TEHParser::StatementContext *context) override;

        virtual std::any visitInputstatement(TEHParser::InputstatementContext *context) override;

        bool hasErrors = false;
    private:
        std::vector<std::string> errors;
        std::set<std::string> declIdentifiers; 
        std::unordered_map<std::string, std::string> variableTypes;
};
