source ~/.zshrc 
cd grammar && antlr4 TEHLexer.g4 TEHParser.g4 -o ../generated -Dlanguage=Cpp -visitor 
