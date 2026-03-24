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
    string declaredType = identifiers[0]->getText();

    if (declaredType != "bool" && declaredType != "int")
    {
        hasErrors = true;
        errors.push_back("Error: Type '" + declaredType + "' is not supported. Allowed types are bool and int.");
        return 0;
    }

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
            variableTypes[varName] = declaredType;
        }
    }
    return 0;
}

any SemanticVisitor::visitIfstatement(TEHParser::IfstatementContext *context) 
{
    string conditionType = any_cast<string>(context->expr()->accept(this));
    if (conditionType != "bool" && conditionType != "invalid")
    {
        hasErrors = true;
        errors.push_back("Error: If condition must be of type bool.");
    }

    for (auto statement : context->statement())
    {
        statement->accept(this);
    }

    if (context->elsestatement() != nullptr)
    {
        context->elsestatement()->accept(this);
    }

    return 0;
}

any SemanticVisitor::visitElsestatement(TEHParser::ElsestatementContext *context)
{
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

    string exprType = "invalid";
    if (context->expr() != nullptr)
    {
        exprType = any_cast<string>(context->expr()->accept(this));
    }

    auto typeIt = variableTypes.find(varName);
    if (typeIt != variableTypes.end() && exprType != "invalid" && typeIt->second != exprType)
    {
        hasErrors = true;
        errors.push_back("Error: Cannot assign expression of type '" + exprType +
                         "' to variable '" + varName + "' of type '" + typeIt->second + "'.");
    }
    return 0;
}

any SemanticVisitor::visitPrintstatement(TEHParser::PrintstatementContext *context) 
{
    if (context->STRING() != nullptr)
    {
        return 0;
    }

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
    if (context->INT() != nullptr)
    {
        return string("int");
    }

    if (context->IDENT() != nullptr)
    {
        string varName = context->IDENT()->getText();

        if (declIdentifiers.find(varName) == declIdentifiers.end())
        {
            hasErrors = true;
            errors.push_back("Error: Variable '" + varName + "' is not declared.");
            return string("invalid");
        }

        auto typeIt = variableTypes.find(varName);
        if (typeIt == variableTypes.end())
        {
            hasErrors = true;
            errors.push_back("Error: Variable '" + varName + "' has no known type.");
            return string("invalid");
        }

        return typeIt->second;
    }

    auto subExpressions = context->expr();

    if (subExpressions.size() == 1)
    {
        return any_cast<string>(subExpressions[0]->accept(this));
    }

    if (subExpressions.size() != 2)
    {
        return string("invalid");
    }

    string leftType = any_cast<string>(subExpressions[0]->accept(this));
    string rightType = any_cast<string>(subExpressions[1]->accept(this));

    if (leftType == "invalid" || rightType == "invalid")
    {
        return string("invalid");
    }

    if (context->addOp() != nullptr || context->mulOp() != nullptr)
    {
        if (leftType != "int" || rightType != "int")
        {
            hasErrors = true;
            errors.push_back("Error: Arithmetic expressions require integer operands.");
            return string("invalid");
        }

        return string("int");
    }

    if (context->relOp() != nullptr)
    {
        bool validComparison =
            (leftType == "bool" && rightType == "bool") ||
            (leftType == "bool" && rightType == "int") ||
            (leftType == "int" && rightType == "bool") ||
            (leftType == "int" && rightType == "int");

        if (!validComparison)
        {
            hasErrors = true;
            errors.push_back("Error: Comparison expressions can only use bool and int operands.");
            return string("invalid");
        }

        return string("bool");
    }

    return string("invalid");
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

    if (context->inputstatement() != nullptr)
    {
        return context->inputstatement()->accept(this);
    }

    return 0;
}

any SemanticVisitor::visitInputstatement(TEHParser::InputstatementContext *context) 
{
    string varName = context->IDENT()->getText();

    if (declIdentifiers.find(varName) == declIdentifiers.end())
    {
        hasErrors = true;
        errors.push_back("Error: Variable '" + varName + "' is not declared.");
        return 0;
    }

    return 0;
}
