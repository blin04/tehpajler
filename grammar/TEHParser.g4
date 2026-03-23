parser grammar TEHParser;

options { tokenVocab=TEHLexer; }

prog: (decl)* START (statement)* EOF;

decl: IDENT IDENT (COMMA IDENT)* SEMI;

ifstatement: IF LPAREN expr RPAREN LBRACE (statement)* RBRACE (ELSE LBRACE (statement)* RBRACE)?;

assignstatement: IDENT ASSIGN expr SEMI;

printstatement: PRINT LPAREN (STRING | INT | expr)? RPAREN SEMI;

addOp: ADD | SUB;
mulOp: MUL | DIV;
relOp: EQUALS;

expr: expr mulOp expr | expr addOp expr | expr relOp expr
    | INT
    | IDENT
    | LPAREN expr RPAREN
    ;

statement: ifstatement | assignstatement | printstatement;
