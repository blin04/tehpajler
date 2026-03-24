#include <iostream>
#include <string>

#include "antlr4-runtime.h"
#include "CodeGenVisitor.h"
#include "SemanticVisitor.h"
#include "TEHLexer.h"
#include "TEHParser.h"
#include "cpu.h"

int main(int argc, char* argv[])
{
  if (argc != 3)
  {
    std::cerr << "Usage: ./tehpajler <source_file> <output_file>\n";
    return 1;
  }

  std::cout << "Debug? (y/n): ";
  char debug_input;
  std::cin >> debug_input;
  bool debug = (debug_input == 'y' || debug_input == 'Y');

  std::ifstream inputFile(argv[1]);
  if (!inputFile.is_open())
  {
    std::cerr << "Could not open file: " << argv[1] << '\n';
    return 1;
  }

  if (debug)
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

  if (debug && parser.getNumberOfSyntaxErrors() > 0) {
    std::cout << "Syntax errors detected: " << parser.getNumberOfSyntaxErrors() << '\n';
    return 1;
  }

  if (debug)
    std::cout << tree->toStringTree(&parser) << '\n';

  SemanticVisitor visitor;
  tree->accept(&visitor);

  if (debug && visitor.hasErrors) {
    std::cout << "Semantic errors detected!\n";
    return 1;
  }

  CodeGenVisitor codeGenVisitor(argv[2], debug);
  tree->accept(&codeGenVisitor);

  CPU cpu(argv[2]);
  cpu.execute();
  
  return 0;
}
