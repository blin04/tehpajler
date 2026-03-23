lexer grammar TEHLexer;

START: 'start';
PRINT: 'print';
IF: 'if';
ELSE: 'else';

ADD: '+';
SUB: '-';
MUL: '*';
DIV: '/';
LPAREN: '(';
RPAREN: ')';
LBRACE: '{';
RBRACE: '}';
SEMI: ';';
COMMA: ',';
EQUALS: '==';
ASSIGN: '=';

fragment NUM: [0-9];
fragment LETTER: [a-zA-Z];

BLANK: [ \t\r\n]+ -> skip;
INT: NUM+;
STRING: '"' ~["\r\n]* '"';
IDENT: LETTER [a-zA-Z0-9]*;
