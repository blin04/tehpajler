#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "CodeGenVisitor.h"
#include "SemanticVisitor.h"
#include "TEHLexer.h"
#include "TEHParser.h"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: ./basic_interpreter <test_file>\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Could not open file: " << argv[1] << '\n';
    return 1;
  }
  std::cout << "Processing file: " << argv[1] << '\n';

  std::stringstream buffer;
  buffer << inputFile.rdbuf();
  std::string input = buffer.str();

  antlr4::ANTLRInputStream inputStream(input);
  TEHLexer lexer(&inputStream);
  antlr4::CommonTokenStream tokens(&lexer);
  TEHParser parser(&tokens);

  parser.setBuildParseTree(true);

  // Replace prog() if you change the grammar's start rule.
  antlr4::tree::ParseTree* tree = parser.prog();

  if (parser.getNumberOfSyntaxErrors() > 0) {
    std::cout << "Syntax errors detected: " << parser.getNumberOfSyntaxErrors() << '\n';
    return 1;
  }

  std::cout << tree->toStringTree(&parser) << '\n';
  SemanticVisitor visitor;

  tree->accept(&visitor);

  if (visitor.hasErrors) {
    std::cout << "Semantic errors detected!\n";
    return 1;
  }

  CodeGenVisitor codeGenVisitor;
  tree->accept(&codeGenVisitor);
  
  return 0;
}
