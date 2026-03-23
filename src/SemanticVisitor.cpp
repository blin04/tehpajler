#include "SemanticVisitor.h"
#include <iostream>

using namespace std;

any SemanticVisitor::visitProg(TEHParser::ProgContext *context) 
{
    for (auto decl : context->decl())
    {
        decl->accept(this);
    }

    for (auto statement : context->statement())
    {
        statement->accept(this);
    }
    
    cout << "Semantic analysis completed with " << errors.size() << " errors!" << endl;
    
    if (hasErrors)
    {
        for (const auto& error : errors)
        {
            cout << error << endl;
        }
    }
    return hasErrors;
}

any SemanticVisitor::visitDecl(TEHParser::DeclContext *context) 
{
    auto identifiers = context->IDENT();

    for (size_t i = 1; i < identifiers.size(); ++i)
    {
        string varName = identifiers[i]->getText();
        if (declIdentifiers.find(varName) != declIdentifiers.end())
        {
            hasErrors = true;
            errors.push_back("Error: Variable '" + varName + "' is already declared.");
        }
        else
        {
            declIdentifiers.insert(varName);
        }
    }
    return 0;
}

any SemanticVisitor::visitIfstatement(TEHParser::IfstatementContext *context) 
{
    context->expr()->accept(this);

    for (auto statement : context->statement())
    {
        statement->accept(this);
    }
    return 0;
}

any SemanticVisitor::visitAssignstatement(TEHParser::AssignstatementContext *context) 
{
    string varName = context->IDENT()->getText();

    if (declIdentifiers.find(varName) == declIdentifiers.end())
    {
        hasErrors = true;
        errors.push_back("Error: Variable '" + varName + "' is not declared.");
    }

    context->expr()->accept(this);
    return 0;
}

any SemanticVisitor::visitPrintstatement(TEHParser::PrintstatementContext *context) 
{
    if (context->expr() != nullptr)
    {
        context->expr()->accept(this);
    }
    return 0;
}

any SemanticVisitor::visitAddOp(TEHParser::AddOpContext *context) 
{
    return 0;
}

any SemanticVisitor::visitMulOp(TEHParser::MulOpContext *context) 
{
    return 0;
}

any SemanticVisitor::visitRelOp(TEHParser::RelOpContext *context) 
{
    return 0;
}

any SemanticVisitor::visitExpr(TEHParser::ExprContext *context) 
{
    if (context->IDENT() != nullptr)
    {
        string varName = context->IDENT()->getText();
        if (declIdentifiers.find(varName) == declIdentifiers.end())
        {
            hasErrors = true;
            errors.push_back("Error: Variable '" + varName + "' is not declared.");
        }
        return 0;
    }

    for (auto expr : context->expr())
    {
        expr->accept(this);
    }
    return 0;
}

any SemanticVisitor::visitStatement(TEHParser::StatementContext *context) 
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
