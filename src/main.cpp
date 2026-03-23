#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "TEHLexer.h"
#include "TEHParser.h"
#include "SemanticVisitor.h"


int main() 
{
  std::string input = "start";

  antlr4::ANTLRInputStream inputStream(input);
  TEHLexer lexer(&inputStream);
  antlr4::CommonTokenStream tokens(&lexer);
  TEHParser parser(&tokens);

  parser.setBuildParseTree(true);

  // Replace prog() if you change the grammar's start rule.
  antlr4::tree::ParseTree* tree = parser.prog();

  if (parser.getNumberOfSyntaxErrors() > 0) 
  {
    printf("Syntax errors detected: %d\n", parser.getNumberOfSyntaxErrors());
    return 1;
  }

  std::cout << tree->toStringTree(&parser) << '\n';
  SemanticVisitor visitor;

  tree->accept(&visitor);
  
  return 0;
}
