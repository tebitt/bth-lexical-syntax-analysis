%skeleton "lalr1.cc" 
%defines
%define parse.error verbose
%define api.value.type variant
%define api.token.constructor

%code requires{
  #include <string>
  #include "Node.h"
  #define USE_LEX_ONLY false //change this macro to true if you want to isolate the lexer from the parser.
}

%code{
  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;
}

// definition of set of tokens. All tokens are of type string
%token <std::string> THIS NOT CLASS PUBLIC STATIC VOID MAIN STRING IF ELSE WHILE RETURN NEW TRUE FALSE PRINT INT BOOLEAN IDENTIFIER INTEGER_LITERAL LBRACE RBRACE LPAREN RPAREN LBRACKET RBRACKET SEMICOLON COMMA DOT ASSIGN PLUS MINUS MULT DIV AND OR LT GT EQ LENGTH
%token END 0 "end of file"

// Operator precedence (if needed)
%left OR
%left AND
%nonassoc LT GT EQ
%left PLUS MINUS
%left MULT DIV
%right NOT


// Type definitions for non-terminals
%type <Node *> Goal MainClass ClassDeclaration VarDeclaration MethodDeclaration Type Statement Expression Identifier ClassDeclarationList StatementList VarDeclarationList MethodDeclarationList TypeIdentList ExpressionList VarDeclarationOrStatementList

%%

Goal: MainClass END { 
    root = new Node("Goal", "", yylineno); 
    std::cout << "Goal w/o Class Declaration" << std::endl;
};
    | MainClass ClassDeclarationList END{ 
    root = new Node("Goal", "", yylineno); 
    root->children.push_back($2);
    std::cout << "Goal w/ Class Declaration" << std::endl;
};
    

MainClass: PUBLIC CLASS Identifier LBRACE PUBLIC STATIC VOID MAIN LPAREN STRING LBRACKET RBRACKET Identifier RPAREN LBRACE StatementList RBRACE RBRACE {
    $$ = new Node("MainClass", "", yylineno);
    $$->children.push_back($3); 
    $$->children.push_back($13); 
    $$->children.push_back($16); 
    std::cout << "Main Class" << std::endl;
};

ClassDeclaration: CLASS Identifier LBRACE VarDeclarationList MethodDeclarationList RBRACE {
    $$ = new Node("ClassDeclaration", "", yylineno);
    $$->children.push_back($2); 
    $$->children.push_back($4); 
    $$->children.push_back($5); 
    std::cout << "Class Declaration" << std::endl;
};
    | CLASS Identifier LBRACE MethodDeclarationList RBRACE {
    $$ = new Node("ClassDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($4);
    std::cout << "Class Declaration w/o VarDeclaration" << std::endl;
};
    | CLASS Identifier LBRACE VarDeclarationList RBRACE {
    $$ = new Node("ClassDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($4);
    std::cout << "Class Declaration w/o MethodDeclaration" << std::endl;
};
    | CLASS Identifier LBRACE RBRACE {
    $$ = new Node("ClassDeclaration", "", yylineno);
    $$->children.push_back($2);
    std::cout << "Class Declaration w/o VarDeclaration & MethodDeclaration" << std::endl;
};

VarDeclaration: Type Identifier SEMICOLON {
    $$ = new Node("VarDeclaration", "", yylineno);
    $$->children.push_back($1); 
    $$->children.push_back($2); 
    std::cout << "Var Declaration" << std::endl;
};

MethodDeclaration: PUBLIC Type Identifier LPAREN TypeIdentList RPAREN LBRACE VarDeclarationOrStatementList RETURN Expression SEMICOLON RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2); 
    $$->children.push_back($3);
    $$->children.push_back($5); 
    $$->children.push_back($8);
    $$->children.push_back(new Node("Return", $9, yylineno));
    $$->children.push_back($10); 
    std::cout << "Method Declaration" << std::endl;
}; 
    | PUBLIC Type Identifier LPAREN TypeIdentList RPAREN LBRACE RETURN Expression SEMICOLON RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($3);
    $$->children.push_back($5);
    $$->children.push_back(new Node("Return", $8, yylineno));
    $$->children.push_back($9);
    std::cout << "Method Declaration w/o VarOrState Declaration" << std::endl;
};
    | PUBLIC Type Identifier LPAREN RPAREN LBRACE VarDeclarationOrStatementList RETURN Expression SEMICOLON RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($3);
    $$->children.push_back($7);
    $$->children.push_back(new Node("Return", $8, yylineno));
    $$->children.push_back($9);
    std::cout << "Method Declaration w/o TypeIdent" << std::endl;
};
    | PUBLIC Type Identifier LPAREN RPAREN LBRACE RETURN Expression SEMICOLON RBRACE {
    $$ = new Node("MethodDeclaration", "", yylineno);
    $$->children.push_back($2);
    $$->children.push_back($3);
    $$->children.push_back(new Node("Return", $7, yylineno));
    $$->children.push_back($8);
    std::cout << "Method Declaration w/o TypeIdent & VarOrState Declaration" << std::endl;
};

Type: INT LBRACKET RBRACKET { $$ = new Node("ArrayType", "int", yylineno); }
    | BOOLEAN { $$ = new Node("BooleanType", "", yylineno); }
    | INT { $$ = new Node("IntegerType", "", yylineno); }
    | Identifier { $$ = $1; };

Statement: LBRACE RBRACE { std::cout << "Empty Block" << std::endl; }
    | LBRACE StatementList RBRACE { $$ = new Node("Block", "", yylineno); $$->children.push_back($2); }
    | IF LPAREN Expression RPAREN Statement ELSE Statement { $$ = new Node("IfElse", "", yylineno); $$->children.push_back($3); $$->children.push_back($5); $$->children.push_back($7); std::cout << "If else" << std::endl;}
    | IF LPAREN Expression RPAREN Statement { $$ = new Node("If", "", yylineno); $$->children.push_back($3); $$->children.push_back($5); std::cout << "If" << std::endl;}
    | WHILE LPAREN Expression RPAREN Statement { $$ = new Node("While", "", yylineno); $$->children.push_back($3); $$->children.push_back($5); std::cout << "While" << std::endl;}
    | PRINT LPAREN Expression RPAREN SEMICOLON { $$ = new Node("Print", "", yylineno); $$->children.push_back($3); std::cout << "Print" << std::endl;}
    | Identifier ASSIGN Expression SEMICOLON { $$ = new Node("Assign", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Assign" << std::endl;}
    | Identifier LBRACKET Expression RBRACKET ASSIGN Expression SEMICOLON { $$ = new Node("ArrayAssign", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); $$->children.push_back($6); std::cout << "Array Assign" << std::endl;};

Expression: Expression PLUS Expression { $$ = new Node("Plus", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Plus" << std::endl;}
    | Expression MINUS Expression { $$ = new Node("Minus", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Minus" << std::endl;}
    | Expression MULT Expression { $$ = new Node("Multiply", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Multiply" << std::endl;}
    | Expression DIV Expression { $$ = new Node("Divide", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Divide" << std::endl;}
    | Expression AND Expression { $$ = new Node("And", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "And" << std::endl;}
    | Expression OR Expression { $$ = new Node("Or", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Or" << std::endl;}
    | Expression LT Expression { $$ = new Node("LessThan", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Less Than" << std::endl;}
    | Expression GT Expression { $$ = new Node("GreaterThan", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Greater Than" << std::endl;}
    | Expression EQ Expression { $$ = new Node("Equal", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Equal" << std::endl;}
    | Expression LBRACKET Expression RBRACKET { $$ = new Node("ArrayLookup", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Array Lookup" << std::endl;}
    | Expression DOT LENGTH { $$ = new Node("ArrayLength", "", yylineno); $$->children.push_back($1); std::cout << "Array Length" << std::endl;}
    | Expression DOT Identifier LPAREN ExpressionList RPAREN { $$ = new Node("Call", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); $$->children.push_back($5); std::cout << "Call" << std::endl;}
    | Expression DOT Identifier LPAREN RPAREN { $$ = new Node("Call", "", yylineno); $$->children.push_back($1); $$->children.push_back($3); std::cout << "Call w/o Expression" << std::endl;}
    | INTEGER_LITERAL { $$ = new Node("IntegerLiteral", $1, yylineno); std::cout << "Integer Literal" << std::endl;}
    | TRUE { $$ = new Node("BooleanLiteral", "true", yylineno); std::cout << "True" << std::endl;}
    | FALSE { $$ = new Node("BooleanLiteral", "false", yylineno); std::cout << "False" << std::endl;}
    | Identifier { $$ = $1; std::cout << "Identifier" << std::endl;}
    | THIS { $$ = new Node("This", "", yylineno); std::cout << "This" << std::endl;}
    | NEW INT LBRACKET Expression RBRACKET { $$ = new Node("NewArray", "", yylineno); $$->children.push_back($4); std::cout << "New Array" << std::endl;}
    | NEW Identifier LPAREN RPAREN { $$ = new Node("NewIdentifier", "", yylineno); $$->children.push_back($2); std::cout << "New Object" << std::endl;}
    | NOT Expression { $$ = new Node("Not", "", yylineno); $$->children.push_back($2); std::cout << "Not" << std::endl;}
    | LPAREN Expression RPAREN { $$ = $2; std::cout << "Parenthesis" << std::endl; };

ClassDeclarationList: ClassDeclaration{
    $$ = new Node("ClassDeclarationList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "ClassDeclarationList" << std::endl;
};
    | ClassDeclarationList ClassDeclaration{
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "ClassDeclarationList Recursion" << std::endl;
};

StatementList: Statement{ 
    $$ = new Node("StatementList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "StatementList" << std::endl;
};
    | StatementList Statement{
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "StatementList Recursion" << std::endl;
};

VarDeclarationList: VarDeclaration {
    $$ = new Node("VarDeclarationList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "VarDeclarationList" << std::endl;
};
    | VarDeclarationList VarDeclaration {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "VarDeclarationList Recursion" << std::endl;
};

MethodDeclarationList: MethodDeclaration{
    $$ = new Node("MethodDeclarationList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "MethodDeclarationList" << std::endl;
};
    | MethodDeclarationList MethodDeclaration{
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "MethodDeclarationList Recursion" << std::endl;
};

TypeIdentList: Type Identifier {
    $$ = new Node("TypeIdentList", "", yylineno);
    $$->children.push_back($1);
    $$->children.push_back($2);
    std::cout << "TypeIdentList" << std::endl;
};
    | TypeIdentList COMMA Type Identifier {
    $$ = $1;
    $$->children.push_back($3);
    $$->children.push_back($4);
    std::cout << "TypeIdentList Recursion" << std::endl;
};

VarDeclarationOrStatementList: VarDeclaration {
    $$ = new Node("VarDeclarationOrStatementList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "VarOrState Variable" << std::endl;
};
    | Statement {
    $$ = new Node("VarDeclarationOrStatementList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "VarOrState Statement" << std::endl;
};
    | VarDeclarationOrStatementList Statement {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "VarOrState Statement Recursion" << std::endl;
};
    | VarDeclarationOrStatementList VarDeclaration {
    $$ = $1;
    $$->children.push_back($2);
    std::cout << "VarOrState Variable Recursion" << std::endl;
};

ExpressionList: Expression {
    $$ = new Node("ExpressionList", "", yylineno);
    $$->children.push_back($1);
    std::cout << "Expression List" << std::endl; 
};
    | ExpressionList COMMA Expression {
    $$ = $1;
    $$->children.push_back($3);
    std::cout << "Expression List Recursion" << std::endl;
};

Identifier: IDENTIFIER { $$ = new Node("Identifier ", $1, yylineno); std::cout << "Identifier " << $1 << std::endl; };