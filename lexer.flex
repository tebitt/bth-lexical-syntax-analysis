%top{
    #include "parser.tab.hh"
    #define YY_DECL yy::parser::symbol_type yylex()
    #include "Node.h"
    int lexical_errors = 0;
}
%option yylineno noyywrap nounput batch noinput stack

%%

"public"    {if(USE_LEX_ONLY) {printf("PUBLIC ");} else {return yy::parser::make_PUBLIC(std::to_string(yylineno));}}
"class"     {if(USE_LEX_ONLY) {printf("CLASS ");} else {return yy::parser::make_CLASS(std::to_string(yylineno));}}
"static"    {if(USE_LEX_ONLY) {printf("STATIC ");} else {return yy::parser::make_STATIC(std::to_string(yylineno));}}
"void"      {if(USE_LEX_ONLY) {printf("VOID ");} else {return yy::parser::make_VOID(std::to_string(yylineno));}}
"main"      {if(USE_LEX_ONLY) {printf("MAIN ");} else {return yy::parser::make_MAIN(std::to_string(yylineno));}}
"String"    {if(USE_LEX_ONLY) {printf("STRING ");} else {return yy::parser::make_STRING(std::to_string(yylineno));}}
"if"        {if(USE_LEX_ONLY) {printf("IF ");} else {return yy::parser::make_IF(std::to_string(yylineno));}}
"else"      {if(USE_LEX_ONLY) {printf("ELSE ");} else {return yy::parser::make_ELSE(std::to_string(yylineno));}}
"while"     {if(USE_LEX_ONLY) {printf("WHILE ");} else {return yy::parser::make_WHILE(std::to_string(yylineno));}}
"System.out.println" {if(USE_LEX_ONLY) {printf("PRINT ");} else {return yy::parser::make_PRINT(std::to_string(yylineno));}}
"not"       {if(USE_LEX_ONLY) {printf("NOT ");} else {return yy::parser::make_NOT(std::to_string(yylineno));}}
"new"       {if(USE_LEX_ONLY) {printf("NEW ");} else {return yy::parser::make_NEW(std::to_string(yylineno));}}
"int"       {if(USE_LEX_ONLY) {printf("INT ");} else {return yy::parser::make_INT(std::to_string(yylineno));}}
"boolean"   {if(USE_LEX_ONLY) {printf("BOOLEAN ");} else {return yy::parser::make_BOOLEAN(std::to_string(yylineno));}}
"true"      {if(USE_LEX_ONLY) {printf("TRUE ");} else {return yy::parser::make_TRUE(std::to_string(yylineno));}}
"false"     {if(USE_LEX_ONLY) {printf("FALSE ");} else {return yy::parser::make_FALSE(std::to_string(yylineno));}}
"this"      {if(USE_LEX_ONLY) {printf("THIS ");} else {return yy::parser::make_THIS(std::to_string(yylineno));}}
"return"    {if(USE_LEX_ONLY) {printf("RETURN ");} else {return yy::parser::make_RETURN(std::to_string(yylineno));}}
"&&"        {if(USE_LEX_ONLY) {printf("AND ");} else {return yy::parser::make_AND(std::to_string(yylineno));}}
"||"        {if(USE_LEX_ONLY) {printf("OR ");} else {return yy::parser::make_OR(std::to_string(yylineno));}}
"<"         {if(USE_LEX_ONLY) {printf("LT ");} else {return yy::parser::make_LT(std::to_string(yylineno));}}
">"         {if(USE_LEX_ONLY) {printf("GT ");} else {return yy::parser::make_GT(std::to_string(yylineno));}}
"=="        {if(USE_LEX_ONLY) {printf("EQ ");} else {return yy::parser::make_EQ(std::to_string(yylineno));}}
"/"         {if(USE_LEX_ONLY) {printf("DIVOP ");} else {return yy::parser::make_DIV(std::to_string(yylineno));}}
"["         {if(USE_LEX_ONLY) {printf("LBRACKET ");} else {return yy::parser::make_LBRACKET(std::to_string(yylineno));}}
"]"         {if(USE_LEX_ONLY) {printf("RBRACKET ");} else {return yy::parser::make_RBRACKET(std::to_string(yylineno));}}
"!"         {if(USE_LEX_ONLY) {printf("NOT ");} else {return yy::parser::make_NOT(std::to_string(yylineno));}}
";"         {if(USE_LEX_ONLY) {printf("SEMICOLON ");} else {return yy::parser::make_SEMICOLON(std::to_string(yylineno));}}
","         {if(USE_LEX_ONLY) {printf("COMMA ");} else {return yy::parser::make_COMMA(std::to_string(yylineno));}}
"="         {if(USE_LEX_ONLY) {printf("ASSIGN ");} else {return yy::parser::make_ASSIGN(std::to_string(yylineno));}}
"+"         {if(USE_LEX_ONLY) {printf("PLUSOP ");} else {return yy::parser::make_PLUS(std::to_string(yylineno));}}
"-"         {if(USE_LEX_ONLY) {printf("SUBOP ");} else {return yy::parser::make_MINUS(std::to_string(yylineno));}}
"*"         {if(USE_LEX_ONLY) {printf("MULTOP ");} else {return yy::parser::make_MULT(std::to_string(yylineno));}}
"("         {if(USE_LEX_ONLY) {printf("LP ");} else {return yy::parser::make_LPAREN(std::to_string(yylineno));}}
")"         {if(USE_LEX_ONLY) {printf("RP ");} else {return yy::parser::make_RPAREN(std::to_string(yylineno));}}
"{"         {if(USE_LEX_ONLY) {printf("LBRACE ");} else {return yy::parser::make_LBRACE(std::to_string(yylineno));}}
"}"         {if(USE_LEX_ONLY) {printf("RBRACE ");} else {return yy::parser::make_RBRACE(std::to_string(yylineno));}}
"length"    {if(USE_LEX_ONLY) {printf("LENGTH ");} else {return yy::parser::make_LENGTH(std::to_string(yylineno));}}
"."         {if(USE_LEX_ONLY) {printf("DOT ");} else {return yy::parser::make_DOT(std::to_string(yylineno));}}

0|[1-9][0-9]*           {if(USE_LEX_ONLY) {printf("INTEGER_LITERAL ");} else {return yy::parser::make_INTEGER_LITERAL(yytext);}}
[a-zA-Z_][a-zA-Z_0-9]*  {if(USE_LEX_ONLY) {printf("IDENTIFIER ");} else {return yy::parser::make_IDENTIFIER(yytext);}}

[ \t\n\r]+              {}
"//"[^\n]*              {}
.                       { if(!lexical_errors) fprintf(stderr, "Lexical errors found! See the logs below: \n"); fprintf(stderr,"\t@error at line %d. Character %s is not recognized\n", yylineno, yytext); lexical_errors = 1;}
<<EOF>>                  {return yy::parser::make_END();}
%%